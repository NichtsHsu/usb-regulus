/*! C++ class wrapper of libusb_device

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

namespace usb {
    /**
     * @brief The UsbSpeed enum
     * Low Speed: 1.5 Mbps
     * Full Speed: 12 Mbps
     * High Speed: 480 Mbps
     * SuperSpeed: 5 Gbps
     * SuperSpeed Plus: 10 Gbps
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
         * @brief controlTransfer
         * Send a request and get data via control transfer
         * @param bmRequestType
         * Bitmap shows characteristics of request
         * @note
         * D4..0    Recipient
         *          0 = Device
         *          1 = Interface
         *          2 = Endpoint
         *          3 = Other
         *          4..30 = Reserved
         *          31 = Vendor Specific
         * D6..5    Type
         *          0 = Standard
         *          1 = Class
         *          2 = Vendor
         *          3 = Reserved
         * D7       Data transfer direction
         *          0 = Host-to-device
         *          1 = Device-to-host
         * @param bRequest
         * Specific request, please refer to USB 3.2 Spec chapter 9.4.
         * @param wValue
         * Word-sized field that varies according to request.
         * @param wIndex
         * Word-sized field that varies according to request,
         * typically used to pass an index or offset
         * @param buffer
         * The buffer who carries data.
         * @note
         * Buffers must be pre-allocated because its raw pointers will be used in transfers.
         * @param timeout
         * Timeout to wait.
         * @return
         */
        int controlTransfer(uint8_t bmRequestType,
                            uint8_t bRequest,
                            uint16_t wValue,
                            uint16_t wIndex,
                            QByteArray &buffer,
                            unsigned int timeout);

        /**
         * @brief claimInterface
         * A wrapper function of UsbInterface::claim,
         * but using bInterfaceNumber.
         * @param bInterfaceNumber
         * The number of interface need to be claimed
         * @return error code
         */
        int claimInterface(uint8_t bInterfaceNumber);

        /**
         * @brief releaseInterface
         * A wrapper function of UsbInterface::release,
         * but using bInterfaceNumber.
         * @param bInterfaceNumber
         * The number of interface need to be released
         */
        void releaseInterface(uint8_t bInterfaceNumber);

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

#ifndef USBDEVICEDESCRIPTOR_H
#include "usbdevicedescriptor.h"
#endif
#ifndef USBCONFIGURATIONDESCRIPTOR_H
#include "usbconfigurationdescriptor.h"
#endif
#ifndef USBBOSDESCRIPTOR_H
#include "usbbosdescriptor.h"
#endif

#endif // USBDEVICE_H
