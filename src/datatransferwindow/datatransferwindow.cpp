#include "datatransferwindow.h"
#include "ui_datatransferwindow.h"

DataTransferWindow::DataTransferWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataTransferWindow),
    _device(nullptr),
    _interface(nullptr),
    _isControlTransfer(false)
{
    ui->setupUi(this);
    setStyleSheet(Tools::getQStyleSheet());
}

DataTransferWindow::~DataTransferWindow()
{
    if (_interface && !_isControlTransfer)
        _interface->release();
    delete ui;
}

void DataTransferWindow::setInterface(usb::UsbInterface *interface)
{
    if (_interface && !_isControlTransfer)
        _interface->release();
    qDeleteAll(ui->scrollAreaWidgetContents->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    _interface = interface;
    _currentAltsetting = _interface->currentAltsetting();
    _device = interface->configurationDescriptor()->device();

    // Make sure all IN endpoints are above OUT endpoints
    for (uint8_t i = 0; i < _interface->currentInterfaceDescriptor()->bNumEndpoints(); ++i)
    {
        usb::UsbEndpointDescriptor *epDesc = _interface->currentInterfaceDescriptor()->endpoint(i);
        switch (epDesc->direction())
        {
            case usb::EndpointDirection::IN:
            {
                EndpointInWidget *widget = new EndpointInWidget;
                ui->scrollAreaWidgetContents->layout()->addWidget(widget);
                widget->setEndpoint(epDesc);
            }
            break;
            default:
            break;
        }
    }

    for (uint8_t i = 0; i < _interface->currentInterfaceDescriptor()->bNumEndpoints(); ++i)
    {
        usb::UsbEndpointDescriptor *epDesc = _interface->currentInterfaceDescriptor()->endpoint(i);
        switch (epDesc->direction())
        {
            case usb::EndpointDirection::OUT:
            {
                EndpointOutWidget *widget = new EndpointOutWidget;
                ui->scrollAreaWidgetContents->layout()->addWidget(widget);
                widget->setEndpoint(epDesc);
            }
            break;
            default:
            break;
        }
    }

    _isControlTransfer = false;
}

void DataTransferWindow::setDevice(usb::UsbDevice *device)
{
    if (_interface)
        _interface->release();
    qDeleteAll(ui->scrollAreaWidgetContents->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    ControlTransferWidget *widget = new ControlTransferWidget;
    ui->scrollAreaWidgetContents->layout()->addWidget(widget);
    widget->setDevice(device);
    _isControlTransfer = true;
}

void DataTransferWindow::showEvent(QShowEvent *event)
{
    if (_interface && !_isControlTransfer)
    {
        // Re-init if altsetting is changed
        if (_interface->currentAltsetting() == _currentAltsetting)
            setInterface(_interface);
        if (_interface->claim() < 0)
        {
            LOGD(tr("Hide because claiming interface failed."));
            _interface = nullptr;
            _device = nullptr;
            event->ignore();
            hide();
            return;
        }

        LOGD(tr("claim the interface \"%1\" of device \"%2\".")
             .arg(_interface->displayName())
             .arg(_interface->configurationDescriptor()->device()->displayName()));
    }

    event->accept();
}

void DataTransferWindow::closeEvent(QCloseEvent *event)
{
    /* Wait for read/write stop */
    if (!_isControlTransfer)
        QTimer::singleShot(500, this, [this] () {
            if (_interface)
                _interface->release();
        });
    event->accept();
}

void DataTransferWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    event->accept();
}
