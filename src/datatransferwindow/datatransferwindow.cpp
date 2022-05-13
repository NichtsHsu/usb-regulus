#include "datatransferwindow.h"
#include "ui_datatransferwindow.h"

DataTransferWindow::DataTransferWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataTransferWindow),
    _interface(nullptr)
{
    ui->setupUi(this);
}

DataTransferWindow::~DataTransferWindow()
{
    if (_interface)
        _interface->release();
    delete ui;
}

void DataTransferWindow::setInterface(usb::UsbInterface *interface)
{
    if (_interface)
        _interface->release();
    qDeleteAll(ui->scrollAreaWidgetContents->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    _interface = interface;
    _device = interface->configDescriptor()->device();
    if (_interface->claim() < 0)
    {
        LOGD(tr("Hide because claiming interface failed."));
        _interface = nullptr;
        _device = nullptr;
        hide();
        return;
    }

    LOGD(tr("claim the interface \"%1\" of device \"%2\".")
         .arg(_interface->displayName())
         .arg(_interface->configDescriptor()->device()->displayName()));

    // Make sure all IN endpoints are above OUT endpoints
    for (uint8_t i = 0; i < _interface->currentInterfaceDescriptor()->bNumEndpoints(); ++i)
    {
        usb::UsbEndpointDescriptor *epDesc = _interface->currentInterfaceDescriptor()->endpoint(i);
        switch (epDesc->direction())
        {
            case usb::EndpointDirection::EP_IN:
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
            case usb::EndpointDirection::EP_OUT:
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
}

void DataTransferWindow::showEvent(QShowEvent *event)
{
    if (_interface)
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
         .arg(_interface->configDescriptor()->device()->displayName()));

    event->accept();
}

void DataTransferWindow::closeEvent(QCloseEvent *event)
{
    if (_interface)
        _interface->release();
    event->accept();
}

void DataTransferWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    event->accept();
}
