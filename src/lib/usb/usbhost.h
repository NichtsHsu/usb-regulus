/*! A global-single-instance mode USB devices manager

 * Copyright (C) 2022-2023 Nichts Hsu

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

#ifdef Q_OS_UNIX
#include <usbutils/names.h>
#endif

#include <log/logger.h>

#include "usbdevice.h"

namespace usb {
    /** Qt Does not support mark Q_OBJECT for nested classes,
     * nor classes in a unnamed namespace,
     * we have to place these classes under the __private namespace.
     */
    namespace __private {
        /**
         * @brief The UsbEventHandler class
         * The event handle functions will block current thread,
         * we have to create a new thread for event handling.
         */
        class UsbEventHandler : public QObject
        {
            Q_OBJECT
        public:
            UsbEventHandler(QObject *parent = nullptr);
        signals:
            void finished();
        public slots:
            void run();
            void stop();
        private:
            bool _stopFlag;
            QMutex _stopFlagMutex;
        };

        /**
         * @brief The UsbDeviceRescanWorker class
         * Create a new thread to rescan USB devices.
         */
        class UsbDeviceRescanWorker : public QObject
        {
            Q_OBJECT
        public:
            UsbDeviceRescanWorker(QObject *parent = nullptr);
            /**
             * @brief setUpdateMode
             * The update mode is an alternative solution when the hotplug
             * callback function cannot be registered.
             * We use a rescan-compare-update process to simulate the hotplug.
             */
            void setUpdateMode(bool updateMode);
        signals:
            void finished();
        public slots:
            void run();
        private:
            bool _updateMode;
        };
    }

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
     *
     * You are expected to call deleteLater() before exit program
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
         * emit when device attached.
         * @param device
         * The attached device.
         * @param index
         * The attached device index in the device list.
         * @note
         * The index should not be used to access the device list,
         * because maybe more than one device attach at the same time.
         * @see UsbHost::usbDevices
         */
        void deviceAttached(usb::UsbDevice *device, int index);

        /**
         * @brief deviceDetached
         * emit when device detached.
         * @param device
         * The detached device.
         * @param index
         * The attached device index in the device list.
         * @note
         * The index should not be used to access the device list.
         * @see UsbHost::usbDevices
         */
        void deviceDetached(usb::UsbDevice *device, int index);

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

        /**
         * @brief __addDevice
         * the wrapper of hotplug callback function
         */
        void __addDevice(libusb_device *device);

        /**
         * @brief __removeDevice
         * the wrapper of hotplug callback function
         */
        void __removeDevice(libusb_device *device);
        void __removeDevice(const UsbDevice &device);

        /**
         * @brief __hotplug_callback
         * The real hotplug callback function to register for libusb.
         * Internally calling the UsbHost::__addDevice() and UsbHost::__removeDevice().
         */
        static int LIBUSB_CALL __hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data);

        static std::atomic<UsbHost *> _instance;

        friend __private::UsbEventHandler;
        friend __private::UsbDeviceRescanWorker;
        QVector<UsbDevice *> _usbDevices;
        libusb_context *_context;
        libusb_hotplug_callback_handle _hotplugCbHandle;
        bool _initialized, _hasHotplug;
        bool _protectMouse, _protectKeyboard;
        __private::UsbEventHandler *_libusbEventHandler;
        __private::UsbDeviceRescanWorker *_rescaner;
        QThread *_libusbEventThread, *_rescanThread;
        QTimer *_rescanTimer;
    };
}

#endif // USBHOST_H
