/*! A global-single-instance mode USB devices manager

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

#ifndef USBHOST_H
#define USBHOST_H

#include <libusb.h>
#include <QObject>
#include <QVector>
#include <algorithm>
#include <QTimer>
#include <QThread>
#include <atomic>
#include <mutex>

#ifdef Q_OS_UNIX
#include <usbutils/names.h>
#endif

#include <log/logger.h>

#include "usbdevice.h"

namespace usb {

    /**
     * @brief The UsbHost class
     * The global USB manager, scan and hold the USB devices,
     * register the hotplug callback and translate the hotplug
     * event as Qt's signals.
     *
     * It implements global single-insance mode,
     * when you call UsbHost::instance() for the first time,
     * the single instance will be initialized.
     *
     * Connect the deviceAttached and the deviceDetached signals
     * to your slots so that you can konw when hotplug is occurred.
     */
    class UsbHost : public QObject
    {
        Q_OBJECT
    public:

        /**
         * @brief instance
         * @return the global single instance
         */
        static UsbHost *instance();

        /**
         * @brief usbDevices
         * @return the list of USB devices.
         */
        const QVector<UsbDevice *> &usbDevices() const;

        /**
         * @brief initialized
         * @return if libusb is initialized successfully
         */
        bool initialized() const;

        /**
         * @brief hasHotplug
         * @return if libusb support hotplug in your platform
         */
        bool hasHotplug() const;

        /**
         * @brief context
         * @return libusb context
         */
        libusb_context *context() const;

        /**
         * @brief __addDevice
         * the wrapper of hotplug callback function
         * @warning DO NOT use this function
         */
        void __addDevice(libusb_device *device);

        /**
         * @brief __removeDevice
         * the wrapper of hotplug callback function
         * @warning DO NOT use this function
         */
        void __removeDevice(libusb_device *device);

        /**
         * @brief protectMouse
         * If an interface is a mouse HID interface,
         * it will be refused to be claimed if protectMouse is true.
         */
        bool protectMouse() const;

        /**
         * @brief protectKeyboard
         * If an interface is a keyboard HID interface,
         * it will be refused to be claimed if protectKeyboard is true.
         */
        bool protectKeyboard() const;


    signals:
        /**
         * @brief deviceListRefreshed
         * when device list was completely refreshed, this signal will be emited.
         * @see UsbHost::rescan()
         */
        void deviceListRefreshed();

        /**
         * @brief deviceAttached
         * emit when device attached
         * @param index
         * the attached device index in the device list
         * @see UsbHost::usbDevices
         */
        void deviceAttached(int index);

        /**
         * @brief deviceDetached
         * emit when device detached
         * @param index
         * the attached device index in the device list
         * @warning note that this device is already removed from device list,
         * this index cannot be used to access the detached device.
         * @see UsbHost::usbDevices
         */
        void deviceDetached(int index);

    public slots:
        /**
         * @brief rescan
         * Completely re-generate the device list and emit deviceListRefreshed signal
         */
        void rescan();

        /**
         * @brief setProtectMouse
         * @see protectMouse
         */
        void setProtectMouse(bool protect);

        /**
         * @brief setProtectKeyboard
         * @see protectKeyboard
         */
        void setProtectKeyboard(bool protect);

    private slots:
        /**
         * @brief __refreshHotplutEvent
         * The hotplug event handle of libusb need be triggered periodically manually,
         * therefore we create a new thread running the timer and periodically
         * trigger the hotplug event handle.
         */
        void __refreshHotplutEvent();

    private:
        explicit UsbHost(QObject *parent = nullptr);
        explicit UsbHost(const UsbHost &) = delete;
        UsbHost &operator=(const UsbHost &) = delete;
        ~UsbHost();

        /**
         * @brief __init
         * Initialize the libusb and register hotplug event
         */
        void __init();

        /**
         * @brief __sortDeviceList
         * Sort the device list by bus number and port number
         */
        inline void __sortDeviceList();

        /**
         * @brief __indexOfDevice
         * @param device
         * @return the index of device in device list
         */
        inline int __indexOfDevice(const UsbDevice &device) const;
        inline int __indexOfDevice(libusb_device *device);

        static std::atomic<UsbHost *> _instance;

        QVector<UsbDevice *> _usbDevices;
        libusb_context *_context;
        libusb_hotplug_callback_handle _hotplugCbHandle;
        bool _initialized, _hasHotplug;
        bool _protectMouse, _protectKeyboard;
        QTimer *_hotplugCbTimer;
        QThread *_hotplugCbThread;
    };
}

#endif // USBHOST_H
