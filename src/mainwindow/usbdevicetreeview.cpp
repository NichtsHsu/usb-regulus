#include "usbdevicetreeview.h"

UsbDeviceTreeView::UsbDeviceTreeView(QWidget *parent)
    : QTreeView(parent)
{
    _menuDevice = new QMenu(this);
    _actionReset = new QAction(tr("Reset"));
    connect(_actionReset, &QAction::triggered,
            this, &UsbDeviceTreeView::__resetDevice);
    _menuDevice->addAction(_actionReset);
    _actionControlTransfer = new QAction(tr("Control Transfer"));
    connect(_actionControlTransfer, &QAction::triggered,
            this, &UsbDeviceTreeView::__openControlTransferWindow);
    _menuDevice->addAction(_actionControlTransfer);
    _menuInterface = new QMenu(this);
    _actionDataTransfer = new QAction(tr("Data Trasfer"));
    connect(_actionDataTransfer, &QAction::triggered,
            this, &UsbDeviceTreeView::__openDataTransferWindow);
    _menuInterface->addAction(_actionDataTransfer);

    _model = new QStandardItemModel(this);
    setModel(_model);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &UsbDeviceTreeView::customContextMenuRequested,
            this, &UsbDeviceTreeView::__customMenu);
    header()->hide();
}

void UsbDeviceTreeView::refresh(const QVector<usb::UsbDevice *> &devices)
{
    _model->clear();
    foreach(usb::UsbDevice *const &device, devices)
    {
        UsbDeviceItem *item = new UsbDeviceItem;
        _model->appendRow(item);
        item->setUsbDevice(device);
        item->setEditable(false);
    }
}

void UsbDeviceTreeView::insert(int index, usb::UsbDevice *device)
{
    _menuInterface->hide();
    UsbDeviceItem *item  = new UsbDeviceItem;
    _model->insertRow(index, item);
    item->setUsbDevice(device);
}

void UsbDeviceTreeView::remove(int index)
{
    _menuInterface->hide();
    UsbDeviceItem *deviceItem = dynamic_cast<UsbDeviceItem *>(_model->item(index, 0));
    for (int i = 0; i < deviceItem->rowCount(); ++i)
    {
        UsbInterfaceItem *interfaceItem = dynamic_cast<UsbInterfaceItem *>(deviceItem->child(i));
        if (_dataTransferWindows.contains(interfaceItem->interface()))
        {
            LOGD(tr("Remove a data transfer window which based on an interface of a detached device."));
            _dataTransferWindows[interfaceItem->interface()]->hide();
            delete _dataTransferWindows[interfaceItem->interface()];
            _dataTransferWindows.remove(interfaceItem->interface());
        }
    }
    _model->removeRow(index);
}

void UsbDeviceTreeView::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        __updateTranslations();
    event->accept();
}

void UsbDeviceTreeView::closeAllDataTransferWindow()
{
    foreach (const auto &window, _dataTransferWindows)
        window->close();
}

void UsbDeviceTreeView::__customMenu(const QPoint &point)
{
    QModelIndex index = indexAt(point);
    if (index.parent() == QModelIndex())
    {
        _menuDevice->exec(viewport()->mapToGlobal(point));
    }
    else
    {
        _menuInterface->exec(viewport()->mapToGlobal(point));
    }
}

void UsbDeviceTreeView::__openDataTransferWindow()
{
    UsbInterfaceItem *item = dynamic_cast<UsbInterfaceItem *>(
                _model->itemFromIndex(currentIndex()));
    if (_dataTransferWindows.contains(item->interface()))
    {
        _dataTransferWindows[item->interface()]->show();
    }
    else
    {
        DataTransferWindow *window = new DataTransferWindow;
        window->setInterface(item->interface());
        _dataTransferWindows.insert(item->interface(), window);
        window->show();
    }
}

void UsbDeviceTreeView::__openControlTransferWindow()
{
    UsbDeviceItem *item = dynamic_cast<UsbDeviceItem *>(
                _model->itemFromIndex(currentIndex()));
    if (_dataTransferWindows.contains(item->device()))
    {
        _dataTransferWindows[item->device()]->show();
    }
    else
    {
        DataTransferWindow *window = new DataTransferWindow;
        window->setDevice(item->device());
        _dataTransferWindows.insert(item->device(), window);
        window->show();
    }
}

void UsbDeviceTreeView::__resetDevice()
{
    UsbDeviceItem *deviceItem = dynamic_cast<UsbDeviceItem *>(
                _model->itemFromIndex(currentIndex()));
    if (_dataTransferWindows.contains(deviceItem->device()))
    {
        _dataTransferWindows[deviceItem->device()]->close();
        delete _dataTransferWindows[deviceItem->device()];
        _dataTransferWindows.remove(deviceItem->device());
    }
    for (int i = 0; i < deviceItem->rowCount(); ++i)
    {
        UsbInterfaceItem *interfaceItem = dynamic_cast<UsbInterfaceItem *>(
                    deviceItem->child(i));
        if (_dataTransferWindows.contains(interfaceItem->interface()))
        {
            _dataTransferWindows[interfaceItem->interface()]->close();
            delete _dataTransferWindows[interfaceItem->interface()];
            _dataTransferWindows.remove(interfaceItem->interface());
        }
    }
    usb::UsbDevice::reset(deviceItem->device());
}

void UsbDeviceTreeView::__updateTranslations()
{
    _actionReset->setText(tr("Reset"));
    _actionControlTransfer->setText(tr("Control Transfer"));
    _actionDataTransfer->setText(tr("Data Trasfer"));
}

UsbDeviceItem::UsbDeviceItem(const QString &text) : QStandardItem(text)
{

}

UsbDeviceItem::UsbDeviceItem(const QIcon &icon, const QString &text) : QStandardItem(icon, text)
{

}

UsbDeviceItem::UsbDeviceItem(int rows, int columns) : QStandardItem(rows, columns)
{

}

void UsbDeviceItem::setUsbDevice(usb::UsbDevice *usbDevice)
{
    _device = usbDevice;

    setText(_device->displayName());
    setToolTip(_device->displayName());

    /* Clear children at first */
    removeRows(0, rowCount());
    for (int i = 0; i < usbDevice->configurationDescriptor()->bNumInterfaces(); ++i)
    {
        usb::UsbInterface *interface = usbDevice->configurationDescriptor()->interface(i);
        UsbInterfaceItem *item  = new UsbInterfaceItem;
        appendRow(item);
        item->setUsbInterface(interface);
        item->setEditable(false);
    }
}

usb::UsbDevice *UsbDeviceItem::device() const
{
    return _device;
}

UsbInterfaceItem::UsbInterfaceItem(const QString &text) : QStandardItem(text)
{

}

UsbInterfaceItem::UsbInterfaceItem(const QIcon &icon, const QString &text) : QStandardItem(icon, text)
{

}

UsbInterfaceItem::UsbInterfaceItem(int rows, int columns) : QStandardItem(rows, columns)
{

}

void UsbInterfaceItem::setUsbInterface(usb::UsbInterface *usbInterface)
{
    _interface = usbInterface;

    setText(_interface->displayName());
    setToolTip(_interface->displayName());
}

usb::UsbInterface *UsbInterfaceItem::interface() const
{
    return _interface;
}
