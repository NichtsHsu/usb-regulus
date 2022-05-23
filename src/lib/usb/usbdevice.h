﻿/*! C++ class wrapper of libusb_device

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

#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <libusb.h>
#include <QObject>
#include <QString>

#include "log/logger.h"

namespace usb {
    class UsbDeviceDescriptor;
    class UsbConfigurationDescriptor;
    class UsbBosDescriptor;
}
#ifndef USBDEVICEDESCRIPTOR_H
#include "usbdevicedescriptor.h"
#endif
#ifndef USBCONFIGURATIONDESCRIPTOR_H
#include "usbconfigurationdescriptor.h"
#endif
#ifndef USBBOSDESCRIPTOR_H
#include "usbbosdescriptor.h"
#endif

namespace usb {
    /**
     * @brief The UsbSpeed enum
     * Low Speed: 1.5 Mbps
     * Full Speed: 12 Mbps
     * High Speed: 480 Mbps
     * Super Speed: 5 Gbps
     * Super Speed Plus: 10 Gbps
     * @note
     * USB 3.2 Gen 1x2 and USB 3.2 Gen 2x2 use dual channel for data transferring,
     * therefore their speed is doubled
     * @todo
     * How can we check dual channel?
     */
    enum class UsbSpeed {
        UNKNOWN = LIBUSB_SPEED_UNKNOWN,
        LOW = LIBUSB_SPEED_LOW,
        FULL = LIBUSB_SPEED_FULL,
        HIGH = LIBUSB_SPEED_HIGH,
        SUPER = LIBUSB_SPEED_SUPER,
        SUPER_PLUS = LIBUSB_SPEED_SUPER_PLUS,
    };

    /**
     * @brief usbSpeedToString
     * @param speed
     * USB connect speed
     * @return the string-form connection speed
     */
    QString usbSpeedToString(UsbSpeed speed);

    /**
     * @brief The UsbDevice class
     * C++ class wrapper of libusb_device
     * @see libusb_device
     */
    class UsbDevice : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbDevice(libusb_device *device, QObject *parent = nullptr);
        ~UsbDevice();

        /**
         * @brief deviceDescriptor
         * @return device descriptor of this USB device
         */
        UsbDeviceDescriptor *deviceDescriptor() const;

        /**
         * @brief configurationDescriptor
         * @return configuration descriptor of this USB device
         */
        UsbConfigurationDescriptor *configurationDescriptor() const;

        /**
         * @brief handle
         * @return the handle of device
         * @warning know what you're doing before access the handle
         */
        libusb_device_handle *handle() const;

        /**
         * @brief device
         * @return the associated libusb_device of this wrapper
         */
        libusb_device *device() const;

        /**
         * @brief bosDescriptor
         * @return the BOS descriptor
         */
        UsbBosDescriptor *bosDescriptor() const;

        /**
         * @brief bus
         * @return bus number of this USB device
         * @note
         * Normally each UDC(USB Device Controller) has two bus,
         * one for USB 2.0 and one for USB 3.0.
         */
        uint8_t bus() const;

        /**
         * @brief port
         * @return port number of this USB device
         * @see UsbDevice::bus()
         */
        uint8_t port() const;

        /**
         * @brief speed
         * @return currently connection speed
         */
        UsbSpeed speed() const;

        /**
         * @brief valid
         * @return if is a valid device
         */
        bool valid() const;

        /**
         * @brief displayName
         * @return the name displayed on device tree view.
         */
        const QString &displayName() const;

        /**
         * @brief getStringDescriptor
         * @param index
         * the string descriptor index normally is shown as "iXxx",
         * such as "iInterface", "iProduct" and so on.
         * @return the string descriptor
         */
        QString getStringDescriptor(size_t index) const;

        /**
         * @brief infomationToHtml
         * @return HTML form device informations
         */
        QString infomationToHtml() const;

        /**
         * @brief reset
         * Reset a device may cause this device disconnect and reconnect,
         * then the UsbDevice will be deallocated.
         * For safety, we declare the reset function as a static function,
         * instead of a member function of UsbDevice.
         */
        static void reset(const UsbDevice &device);
        static void reset(const UsbDevice *device);

        /**
         * @brief operator ==
         * @return if idVendor, idProduct, bus and port are equal.
         */
        bool operator==(const UsbDevice &) const;
        bool operator==(libusb_device *);

    signals:

    private:
        libusb_device *_device;
        UsbDeviceDescriptor *_deviceDescriptor;
        UsbConfigurationDescriptor *_configurationDescriptor;
        UsbBosDescriptor *_bosDescriptor;
        libusb_device_handle *_handle;
        uint8_t _bus, _port, _address;
        UsbSpeed _speed;
        QString _displayName;

        bool _valid, _open;
    };
}

#endif // USBDEVICE_H
