/*! The tree view on the left side of the main window to show USB devices.

 * Copyright (C) 2022 Nichts Hsu

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef USBDEVICETREEVIEW_H
#define USBDEVICETREEVIEW_H

#include "log/logger.h"
#include "usb/usbdevice.h"

#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QVector>
#include <QTimer>
#include <QHeaderView>
#include <QMenu>
#include <QInputDialog>
#include <datatransferwindow/datatransferwindow.h>

/**
 * @brief The UsbDeviceItem class
 * Each UsbDeviceItem binds and should bind a USB device, all GUI operation are handled
 * by UsbDeviceItem and send to its associated USB device.
 */
class UsbDeviceItem : public QStandardItem
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

    void updateTranslation();

private:
    usb::UsbDevice *_device;
};

/**
 * @brief The UsbInterfaceItem class
 * Each UsbDeviceItem binds and should bind a USB interface.
 * The UsbDeviceItem normally will be created by UsbDeviceItem.
 */
class UsbInterfaceItem : public QStandardItem
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

    void changeEvent(QEvent *event) override;

signals:
    void currentIndexChanged(const QModelIndex &index, bool rightClick = false);

public slots:
    void closeAllDataTransferWindow();

private slots:
    void __customMenu(const QPoint &point);
    void __openDataTransferWindow();
    void __openControlTransferWindow();
    void __resetDevice();
    void __customSetAltsetting();
    void __setAltsetting(const QModelIndex &index, int altsetting);

private:
    inline void __updateTranslations();

    QStandardItemModel *_model;
    QMenu *_menuDevice;
    QAction *_actionReset;
    QAction *_actionControlTransfer;
    QMenu *_menuInterface;
    QAction *_actionDataTransfer;
    QMenu *_actionSetAltsetting;
    QAction *_actionSetAltsettingFirst;
    QAction *_actionSetAltsettingPrevious;
    QAction *_actionSetAltsettingNext;
    QAction *_actionSetAltsettingCustom;
    QMap<QObject *, DataTransferWindow *> _dataTransferWindows;
};

#endif // USBDEVICETREEVIEW_H
