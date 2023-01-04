#include "controltransferwidget.h"
#include "ui_controltransferwidget.h"
#include "usb/__usbmacro.h"
#include <QStyleOption>
#include <QPainter>

const QMap<QString, uint8_t> ControlTransferWidget::bRequestCodeMap ={
    { "GET_STATUS", 0 },
    { "CLEAR_FEATURE", 1 },
    { "SET_FEATURE", 3 },
    { "SET_ADDRESS", 5 },
    { "GET_DESCRIPTOR", 6 },
    { "SET_DESCRIPTOR", 7 },
    { "GET_CONFIGURATION", 8 },
    { "SET_CONFIGURATION", 9 },
    { "GET_INTERFACE", 10 },
    { "SET_INTERFACE", 11 },
    { "SYNCH_FRAME", 12 },
    { "SET_ENCRYPTION", 13 },
    { "GET_ENCRYPTION", 14 },
    { "SET_HANDSHAKE", 15 },
    { "GET_HANDSHAKE", 16 },
    { "SET_CONNECTION", 17 },
    { "SET_SECURITY_DATA", 18 },
    { "GET_SECURITY_DATA", 19 },
    { "SET_WUSB_DATA", 20 },
    { "LOOPBACK_DATA_WRITE", 21 },
    { "LOOPBACK_DATA_READ", 22 },
    { "SET_INTERFACE_DS", 23 },
    { "SET_SEL", 48 },
    { "SET_ISOCH_DELAY", 49 },
};

/* Device/Interface/Endpoint */
const QStringList ControlTransferWidget::standardBRequestTable[3][2] = {
    {
        {
            "CLEAR_FEATURE",
            "SET_FEATURE",
            "SET_ADDRESS",
            "SET_DESCRIPTOR",
            "SET_CONFIGURATION",
            "SET_SEL",
            "SET_ISOCH_DELAY",
        },
        {
            "GET_STATUS",
            "GET_DESCRIPTOR",
            "GET_CONFIGURATION",
        },
    },
    {
        {
            "CLEAR_FEATURE",
            "SET_FEATURE",
            "SET_INTERFACE",
        },
        {

            "GET_STATUS",
            "GET_INTERFACE",
        },
    },
    {
        {
            "CLEAR_FEATURE",
            "SET_FEATURE",
        },
        {
            "GET_STATUS",
            "SYNCH_FRAME",
        },
    },
};

#define BMREQUEST_TYPE_STANDARD 0
#define BMREQUEST_TYPE_CLASS 1
#define BMREQUEST_TYPE_VENDOR 2
#define BMREQUEST_RECIPIENT_DEVICE 0
#define BMREQUEST_TYPE_CLASS 1
#define BMREQUEST_TYPE_VENDOR 2

ControlTransferWidget::ControlTransferWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlTransferWidget),
    _transfer(false)
{
    ui->setupUi(this);

    _hexEditSend = new QHexEdit;
    _hexEditSend->setOverwriteMode(false);
    ui->verticalLayoutSend->replaceWidget(ui->hexEditPlaceHolderSend, _hexEditSend);
    ui->hexEditPlaceHolderSend->deleteLater();
    ui->hexEditPlaceHolderSend = nullptr;

    _hexEditReceive = new QHexEdit;
    _hexEditReceive->setReadOnly(true);
    ui->verticalLayoutReceive->replaceWidget(ui->hexEditPlaceHolderReceive, _hexEditReceive);
    ui->hexEditPlaceHolderReceive->deleteLater();
    ui->hexEditPlaceHolderReceive = nullptr;

    _comboxBRequestModel = new QStringListModel;
    _comboxBRequestModel->setStringList(standardBRequestTable[0][0]);
    ui->comboBoxBRequest->setModel(_comboxBRequestModel);

    _handler = new usb::UsbControlTransferHandler;
    _handler->moveToThread(&_workerThread);
    _workerThread.start();

    connect(_handler, &usb::UsbControlTransferHandler::finishedTransfer,
            this, &ControlTransferWidget::__readData);
    connect(_handler, &usb::UsbControlTransferHandler::safelyStopped,
            this, &ControlTransferWidget::__resetButtons);
    connect(_handler, &usb::UsbControlTransferHandler::transferFailed,
            this, [] (int ret) {
        QMessageBox::critical(nullptr,
                              tr("Error"),
                              tr("Failed to transfer data, error: %1.")
                              .arg(usb_error_name(ret)));
    });
    connect(this, &ControlTransferWidget::__sendTriggered,
            _handler, &usb::UsbControlTransferHandler::startTransfer);
    connect(_hexEditSend, &QHexEdit::overwriteModeChanged,
            this, &ControlTransferWidget::__setOverwriteMode);
    connect(ui->checkBoxBmRequestRawHex, &QCheckBox::toggled,
            this, &ControlTransferWidget::__bmRequestUseRawHex);
    connect(ui->checkBoxBRequestRawHex, &QCheckBox::toggled,
            this, &ControlTransferWidget::__bRequestUseRawHex);
    connect(ui->comboBoxDirection, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlTransferWidget::__updateBmRequest);
    connect(ui->comboBoxDirection, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlTransferWidget::__updateBRequestList);
    connect(ui->comboBoxType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlTransferWidget::__updateBmRequest);
    connect(ui->comboBoxType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlTransferWidget::__updateBRequestList);
    connect(ui->comboBoxRecipient, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlTransferWidget::__updateBmRequest);
    connect(ui->comboBoxRecipient, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlTransferWidget::__updateBRequestList);
    connect(ui->comboBoxBRequest, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlTransferWidget::__updateBRequest);
    connect(ui->pushButtonSend, &QPushButton::released,
            this, &ControlTransferWidget::__buttonSendReleased);
    connect(ui->pushButtonClearAll, &QPushButton::released,
            this, &ControlTransferWidget::__clearAll);
    connect(ui->pushButtonSetBufferSize, &QPushButton::released,
            this, &ControlTransferWidget::__buttonSetBufferSizeReleased);
}

ControlTransferWidget::~ControlTransferWidget()
{
    _handler->disconnect(this);
    disconnect(_handler);
    _workerThread.quit();
    _workerThread.wait();
    delete ui;
}

void ControlTransferWidget::setDevice(usb::UsbDevice *device)
{
    _device = device;
    _handler->init(_device);
}

void ControlTransferWidget::showEvent(QShowEvent *event)
{
    __clearAll();
    QWidget::showEvent(event);
}

void ControlTransferWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ControlTransferWidget::__bmRequestUseRawHex(bool useRawHex)
{
    if (useRawHex)
    {
        ui->lineEditBmRequestRawHex->setReadOnly(false);
        ui->comboBoxDirection->setEnabled(false);
        ui->comboBoxType->setEnabled(false);
        ui->comboBoxRecipient->setEnabled(false);
        ui->checkBoxBRequestRawHex->setChecked(true);
    }
    else
    {
        /* Must be valid, so ignore this parameter */
        bool _;
        ui->lineEditBmRequestRawHex->setReadOnly(true);
        ui->comboBoxDirection->setEnabled(true);
        ui->comboBoxType->setEnabled(true);
        ui->comboBoxRecipient->setEnabled(true);
        ui->lineEditBmRequestRawHex->setText(QString("%1").arg(__calcBmRequest(_), 2, 16, QChar('0')));
    }
}

void ControlTransferWidget::__bRequestUseRawHex(bool useRawHex)
{
    if (useRawHex)
    {
        ui->lineEditBRequestRawHex->setReadOnly(false);
        ui->comboBoxBRequest->setEnabled(false);
    }
    else
    {
        if (
                /** To avoid unhandled bmRequest type,
                 * we limit that bRequest must use raw hex
                 * when bmRequest is using raw hex.
                 */
                ui->checkBoxBmRequestRawHex->isChecked() ||
                /* Force use raw hex when type is NOT standard  */
                ui->comboBoxType->currentIndex() != 0
                )
        {
            ui->checkBoxBRequestRawHex->blockSignals(true);
            ui->checkBoxBRequestRawHex->setChecked(true);
            ui->checkBoxBRequestRawHex->blockSignals(false);
            return;
        }

        /* Must be valid, so ignore this parameter */
        bool _;
        ui->lineEditBRequestRawHex->setReadOnly(true);
        ui->comboBoxBRequest->setEnabled(true);
        ui->lineEditBRequestRawHex->setText(QString("%1").arg(__calcBRequest(_), 2, 16, QChar('0')));
    }
}

void ControlTransferWidget::__updateBRequestList()
{
    const uint8_t direction = ui->comboBoxDirection->currentIndex();
    const uint8_t type = ui->comboBoxType->currentIndex();
    const uint8_t recipient = ui->comboBoxRecipient->currentIndex();
    if (recipient < 3 && type == 0)
    {
        _comboxBRequestModel->setStringList(standardBRequestTable[recipient][direction]);
        ui->comboBoxBRequest->setCurrentIndex(0);
    }
    else
    {
        if (!ui->checkBoxBRequestRawHex->isChecked())
            ui->checkBoxBRequestRawHex->setChecked(true);
        _comboxBRequestModel->setStringList(QStringList());
        ui->lineEditBRequestRawHex->setText("00");
    }
}

void ControlTransferWidget::__updateBmRequest()
{
    /* Must be valid, so ignore this parameter */
    bool _;
    ui->lineEditBmRequestRawHex->setText(QString("%1").arg(__calcBmRequest(_), 2, 16, QChar('0')));
}

void ControlTransferWidget::__updateBRequest()
{
    /* Must be valid, so ignore this parameter */
    bool _;
    ui->lineEditBRequestRawHex->setText(QString("%1").arg(__calcBRequest(_), 2, 16, QChar('0')));
}

void ControlTransferWidget::__setOverwriteMode(bool overwrite)
{
    if (overwrite)
        ui->labelInsertOverwrite->setText(tr("Mode: Overwrite"));
    else
        ui->labelInsertOverwrite->setText(tr("Mode: Insert"));
}

void ControlTransferWidget::__clearAll()
{
    ui->comboBoxDirection->setCurrentIndex(0);
    ui->comboBoxType->setCurrentIndex(0);
    ui->comboBoxRecipient->setCurrentIndex(0);
    ui->comboBoxBRequest->setCurrentIndex(0);
    ui->checkBoxBmRequestRawHex->setChecked(false);
    ui->checkBoxBRequestRawHex->setChecked(false);
    ui->lineEditWIndexRawHex->setText("0000");
    ui->lineEditWValueRawHex->setText("0000");
    QByteArray data;
    _sendBuffer.setData(data);
    _hexEditSend->setData(_sendBuffer);
    _receiveBuffer.setData(data);
    _hexEditReceive->setData(_receiveBuffer);
}

void ControlTransferWidget::__buttonSendReleased()
{
    if (_transfer)
    {
        ui->pushButtonSend->setEnabled(false);
        _handler->stopTransfer();
    }
    else
    {
        bool isValid;
        _handler->setBmRequest(__calcBmRequest(isValid));
        if (!isValid)
        {
            LOGE(tr("The value of bmRequest is invalid: %1.")
                 .arg(ui->lineEditBmRequestRawHex->text()));
            QMessageBox::critical(nullptr, "Error",
                                  tr("The value of bmRequest is invalid: %1.")
                                  .arg(ui->lineEditBmRequestRawHex->text()));
            return;
        }
        _handler->setBRequest(__calcBRequest(isValid));
        if (!isValid)
        {
            LOGE(tr("The value of bRequest is invalid: %1.")
                 .arg(ui->lineEditBRequestRawHex->text()));
            QMessageBox::critical(nullptr, "Error",
                                  tr("The value of bRequest is invalid: %1.")
                                  .arg(ui->lineEditBRequestRawHex->text()));
            return;
        }
        _handler->setWIndex(ui->lineEditWIndexRawHex->text().toUInt(&isValid, 16));
        if (!isValid)
        {
            LOGE(tr("The value of wIndex is invalid: %1.")
                 .arg(ui->lineEditWIndexRawHex->text()));
            QMessageBox::critical(nullptr, "Error",
                                  tr("The value of wIndex is invalid: %1.")
                                  .arg(ui->lineEditWIndexRawHex->text()));
            return;
        }
        _handler->setWValue(ui->lineEditWValueRawHex->text().toUInt(&isValid, 16));
        if (!isValid)
        {
            LOGE(tr("The value of wValue is invalid: %1.")
                 .arg(ui->lineEditWValueRawHex->text()));
            QMessageBox::critical(nullptr, "Error",
                                  tr("The value of wValue is invalid: %1.")
                                  .arg(ui->lineEditWValueRawHex->text()));
            return;
        }
        if (!(_handler->bmRequest() & 0x80))
            _handler->setData(_hexEditSend->data());

        _transfer = true;
        ui->pushButtonClearAll->setEnabled(false);
        ui->pushButtonSetBufferSize->setEnabled(false);
        ui->pushButtonSend->setText(tr("Stop"));
        emit __sendTriggered();
    }
}

void ControlTransferWidget::__buttonSetBufferSizeReleased()
{
    bool ok;
    uint16_t value = QInputDialog::getInt(this,
                                          tr("Set Receive Buffer Size"),
                                          tr("Set Receive buffer size to:"),
                                          _handler->readBufferSize(),
                                          0,
                                          0xFFFF,
                                          1,
                                          &ok);
    if (ok)
        _handler->setReadBufferSize(value);
    raise();
}

void ControlTransferWidget::__resetButtons()
{
    ui->pushButtonSend->setEnabled(true);
    ui->pushButtonClearAll->setEnabled(true);
    ui->pushButtonSetBufferSize->setEnabled(true);
    ui->pushButtonSend->setText(tr("Send"));
    _transfer = false;
}

void ControlTransferWidget::__readData()
{
    if (_handler->bmRequest() & 0x80)
    {
        _receiveBuffer.setData(_handler->data());
        _hexEditReceive->setData(_receiveBuffer);
    }
}

uint8_t ControlTransferWidget::__calcBmRequest(bool &isValid) const
{
    if (ui->checkBoxBmRequestRawHex->isChecked())
    {
        if (ui->lineEditBmRequestRawHex->text().length() > 2)
        {
            isValid = false;
            return 0;
        }

        const uint8_t bmRequest = ui->lineEditBmRequestRawHex->text().toUInt(&isValid, 16);
        if (!isValid)
        {
            isValid = false;
            return 0;
        }
        else
        {
            isValid = true;
            return bmRequest;
        }
    }
    else
    {
        isValid = true;

        const uint8_t direction = ui->comboBoxDirection->currentIndex();
        const uint8_t type = ui->comboBoxType->currentIndex();
        const uint8_t recipient = ui->comboBoxRecipient->currentIndex();
        const uint8_t bmRequest = (direction << 7) | (type << 5) | recipient;
        return bmRequest;
    }
}

uint8_t ControlTransferWidget::__calcBRequest(bool &isValid) const
{
    if (ui->checkBoxBRequestRawHex->isChecked())
    {
        if (ui->lineEditBRequestRawHex->text().length() > 2)
        {
            isValid = false;
            return 0;
        }

        const uint8_t bRequest = ui->lineEditBRequestRawHex->text().toUInt(&isValid, 16);
        if (!isValid)
        {
            isValid = false;
            return 0;
        }
        else
        {
            isValid = true;
            return bRequest;
        }
    }
    else
    {
        isValid = true;
        return bRequestCodeMap[ui->comboBoxBRequest->currentText()];
    }
}
