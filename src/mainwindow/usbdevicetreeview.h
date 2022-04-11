#ifndef USBDEVICETREEVIEW_H
#define USBDEVICETREEVIEW_H

#include "log/logger.h"
#include "usb/usbdevice.h"

#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QVector>
#include <QHeaderView>

/**
 * @brief The UsbDeviceItem class
 * Each UsbDeviceItem binds and should bind a USB device, all GUI operation are handled
 * by UsbDeviceItem and send to its associated USB device.
 */
class UsbDeviceItem: public QStandardItem
{
public:
    UsbDeviceItem() = default;
    explicit UsbDeviceItem(const QString &text);
    UsbDeviceItem(const QIcon &icon, const QString &text);
    explicit UsbDeviceItem(int rows, int columns = 1);
    virtual ~UsbDeviceItem() = default;

    /**
     * @brief setUsbDevice
     * Set the associated USB device, use its display name as the label.
     * Then traverse all interfaces of this USB device and create the
     * corresponding UsbInterfaceItems as children.
     * @param usbDevice
     * The asscociated USB device
     */
    void setUsbDevice(usb::UsbDevice *usbDevice);

    /**
     * @brief device
     * @return the asscociated USB device
     */
    usb::UsbDevice *device() const;

private:
    usb::UsbDevice *_device;
};

/**
 * @brief The UsbInterfaceItem class
 * Each UsbDeviceItem binds and should bind a USB interface.
 * The UsbDeviceItem normally will be created by UsbDeviceItem.
 */
class UsbInterfaceItem: public QStandardItem
{
public:
    UsbInterfaceItem() = default;
    explicit UsbInterfaceItem(const QString &text);
    UsbInterfaceItem(const QIcon &icon, const QString &text);
    explicit UsbInterfaceItem(int rows, int columns = 1);
    virtual ~UsbInterfaceItem() = default;

    /**
     * @brief setUsbInterface
     * Set the associated USB interface
     * @param usbInterface
     * The associated USB interface
     */
    void setUsbInterface(usb::UsbInterface *usbInterface);

    /**
     * @brief interface
     * @return the associated USB interface
     */
    usb::UsbInterface *interface() const;

private:
    usb::UsbInterface *_interface;
};

/**
 * @brief The UsbDeviceTreeView class
 * A Tree View who show the USB device list.
 */
class UsbDeviceTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit UsbDeviceTreeView(QWidget *parent = nullptr);

    /**
     * @brief refresh
     * Completely refresh the USB device list.
     * @param devices
     * USB device list to show
     */
    void refresh(const QVector<usb::UsbDevice *> &devices);
    /**
     * @brief insert
     * Insert a USB device at specified index
     * @param index
     * The index indicated to the position where the new device insert to
     * @param device
     * The device to be insert
     */
    void insert(int index, usb::UsbDevice *device);
    /**
     * @brief remove
     * Remove a USB device with specified index
     * @param index
     */
    void remove(int index);
signals:

public slots:

private:
    QStandardItemModel *_model;
};

#endif // USBDEVICETREEVIEW_H
