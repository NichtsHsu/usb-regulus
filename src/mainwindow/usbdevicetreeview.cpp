#include "usbdevicetreeview.h"

UsbDeviceTreeView::UsbDeviceTreeView(QWidget *parent)
    : QTreeView(parent)
{
    _model = new QStandardItemModel(this);
    setModel(_model);
    header()->hide();
}

void UsbDeviceTreeView::refresh(const QVector<usb::UsbDevice *> &devices)
{
    _model->clear();
    foreach(usb::UsbDevice *const &device, devices)
    {
        UsbDeviceItem *item  = new UsbDeviceItem(device->displayName());
        item->setToolTip(device->displayName());
        _model->appendRow(item);
        item->setUsbDevice(device);
    }
}

void UsbDeviceTreeView::insert(int index, usb::UsbDevice *device)
{
    UsbDeviceItem *item  = new UsbDeviceItem(device->displayName());
    item->setToolTip(device->displayName());
    _model->insertRow(index, item);
    item->setUsbDevice(device);
}

void UsbDeviceTreeView::remove(int index)
{
    _model->removeRow(index);
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

    removeRows(0, rowCount());
    for (int i = 0; i < usbDevice->configurationDescriptor()->bNumInterfaces(); ++i)
    {
        usb::UsbInterface *interface = usbDevice->configurationDescriptor()->interface(i);
        UsbInterfaceItem *item  = new UsbInterfaceItem(interface->displayName());
        item->setToolTip(interface->displayName());
        appendRow(item);
        item->setUsbInterface(interface);
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
}

usb::UsbInterface *UsbInterfaceItem::interface() const
{
    return _interface;
}
