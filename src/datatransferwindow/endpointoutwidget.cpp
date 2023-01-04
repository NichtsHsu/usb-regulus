#include "endpointoutwidget.h"
#include "ui_endpointoutwidget.h"
#include "usb/__usbmacro.h"
#include <QStyleOption>
#include <QPainter>

EndpointOutWidget::EndpointOutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndpointOutWidget),
    _writeOnce(false), _keepWrite(false),
    _resetButtonAfterSafelyStop(nullptr),
    _endpointDescriptor(nullptr)
{
    ui->setupUi(this);

    _hexEdit = new QHexEdit;
    layout()->replaceWidget(ui->hexEditPlaceHolder, _hexEdit);
    ui->hexEditPlaceHolder->deleteLater();
    ui->hexEditPlaceHolder = nullptr;
    __setOverwriteMode(_hexEdit->overwriteMode());
    _writer = new usb::UsbEndpointWriter;
    _workerThread = new QThread;
    _writer->moveToThread(_workerThread);
    _workerThread->start();

    connect(_hexEdit, &QHexEdit::overwriteModeChanged,
            this, &EndpointOutWidget::__setOverwriteMode);
    connect(_writer, &usb::UsbEndpointWriter::writeSucceed,
            this, [this] (size_t count) {
        ui->labelWriteCount->setText(tr("Write Count: %1").arg(count));
    });
    connect(_writer, &usb::UsbEndpointWriter::writeFailed,
            this, [this] (int ret) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Failed to write data, error: %1.")
                              .arg(usb_error_name(ret)));
    });
    connect(_writer, &usb::UsbEndpointWriter::safelyStopped,
            this, &EndpointOutWidget::__resetButton);
    connect(this, &EndpointOutWidget::__writeOnceTriggered,
            _writer, &usb::UsbEndpointWriter::writeOnce);
    connect(this, &EndpointOutWidget::__keepWriteTriggered,
            _writer, &usb::UsbEndpointWriter::keepWrite);
    connect(ui->pushButtonWriteOnce, &QPushButton::released,
            this, &EndpointOutWidget::__buttonWriteOnceReleased);
    connect(ui->pushButtonKeepWrite, &QPushButton::released,
            this, &EndpointOutWidget::__buttonKeepWriteReleased);
    connect(ui->pushButtonClearCount, &QPushButton::released,
            this, &EndpointOutWidget::__buttonClearCounterReleased);
    connect(ui->pushButtonResetData, &QPushButton::released,
            this, &EndpointOutWidget::__buttonResetDataReleased);
}

EndpointOutWidget::~EndpointOutWidget()
{
    _writer->disconnect(this);
    disconnect(_writer);
    if (!_writeOnce && !_keepWrite)
    {
        _writer->stopWrite();
        _workerThread->quit();
        _writer->deleteLater();
        _workerThread->deleteLater();
    }
    else
    {
        connect(_writer, &usb::UsbEndpointWriter::safelyStopped,
                _writer, &usb::UsbEndpointWriter::deleteLater);
        connect(_writer, &usb::UsbEndpointWriter::safelyStopped,
                _workerThread, &QThread::quit);
        connect(_workerThread, &QThread::finished,
                _workerThread, &QThread::deleteLater);
        _writer->stopWrite();
    }
    delete ui;
}

void EndpointOutWidget::setEndpoint(usb::UsbEndpointDescriptor *epDesc)
{
    _endpointDescriptor = epDesc;
    ui->labelEndpointName->setText(_endpointDescriptor->endpointAddressInfo());
    QByteArray data;
    data.fill('\0', _endpointDescriptor->wMaxPacketSize());
    _buffer.setData(data);
    _hexEdit->setData(_buffer);
    _writer->init(_endpointDescriptor);
}

void EndpointOutWidget::hideEvent(QHideEvent *event)
{
    _writer->stopWrite();
    QWidget::hideEvent(event);
}

void EndpointOutWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        __setOverwriteMode(_hexEdit->overwriteMode());
        ui->labelWriteCount->setText(tr("Write Count: %1").arg(_writer->wroteTimes()));
    }
    QWidget::changeEvent(event);
}

void EndpointOutWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void EndpointOutWidget::__buttonWriteOnceReleased()
{
    if (!_writeOnce && !_keepWrite)
    {
        _writeOnce = true;
        ui->pushButtonKeepWrite->setEnabled(false);
        ui->pushButtonWriteOnce->setText(tr("Stop Write"));
        ui->pushButtonResetData->setEnabled(false);
        _hexEdit->setReadOnly(true);
        _writer->setData(_hexEdit->data());
        _resetButtonAfterSafelyStop = [this] () {
            _writeOnce = false;
            ui->pushButtonWriteOnce->setText(tr("Write Once"));
        };
        emit __writeOnceTriggered();
    }
    else if (_writeOnce)
    {
        ui->pushButtonWriteOnce->setEnabled(false);
        _writer->stopWrite();
    }
}

void EndpointOutWidget::__buttonKeepWriteReleased()
{
    if (!_writeOnce && !_keepWrite)
    {
        _keepWrite = true;
        ui->pushButtonWriteOnce->setEnabled(false);
        ui->pushButtonKeepWrite->setText(tr("Stop Write"));
        ui->pushButtonResetData->setEnabled(false);
        _hexEdit->setReadOnly(true);
        _writer->setData(_hexEdit->data());
        _resetButtonAfterSafelyStop = [this] () {
            _keepWrite = false;
            ui->pushButtonKeepWrite->setText(tr("Keep Write"));
        };
        emit __keepWriteTriggered();
    }
    else if (_keepWrite)
    {
        ui->pushButtonKeepWrite->setEnabled(false);
        _writer->stopWrite();
    }
}

void EndpointOutWidget::__buttonClearCounterReleased()
{
    _writer->clearWroteTimes();
    ui->labelWriteCount->setText(tr("Write Count: %1").arg(0));
}

void EndpointOutWidget::__buttonResetDataReleased()
{
    QByteArray data;
    data.fill('\0', _endpointDescriptor->wMaxPacketSize());
    _buffer.setData(data);
    _hexEdit->setData(_buffer);
}

void EndpointOutWidget::__resetButton()
{
    ui->pushButtonWriteOnce->setEnabled(true);
    ui->pushButtonKeepWrite->setEnabled(true);
    ui->pushButtonResetData->setEnabled(true);
    _hexEdit->setReadOnly(false);
    if (_resetButtonAfterSafelyStop)
        _resetButtonAfterSafelyStop();
    _resetButtonAfterSafelyStop = nullptr;
}

void EndpointOutWidget::__setOverwriteMode(bool overwrite)
{
    if (overwrite)
        ui->labelInsertOverwrite->setText(tr("Mode: Overwrite"));
    else
        ui->labelInsertOverwrite->setText(tr("Mode: Insert"));
}
