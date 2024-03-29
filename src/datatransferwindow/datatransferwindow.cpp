#include "datatransferwindow.h"
#include "ui_datatransferwindow.h"
#include <QStyleOption>
#include <QPainter>

DataTransferWindow::DataTransferWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataTransferWindow),
    _device(nullptr),
    _interface(nullptr),
    _isControlTransfer(false)
{
    ui->setupUi(this);
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
                QFrame *line = new QFrame;
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                ui->scrollAreaWidgetContents->layout()->addWidget(line);
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
        if (_interface->currentAltsetting() != _currentAltsetting)
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

    QWidget::showEvent(event);
}

void DataTransferWindow::closeEvent(QCloseEvent *event)
{
    /* Wait for read/write stop */
    if (!_isControlTransfer)
        QTimer::singleShot(500, this, [this] () {
            if (_interface)
                _interface->release();
        });
    QWidget::closeEvent(event);
}

void DataTransferWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    QWidget::changeEvent(event);
}

void DataTransferWindow::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
