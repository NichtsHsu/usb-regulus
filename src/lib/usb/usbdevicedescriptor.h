/*! C++ class wrapper of libusb_device_descriptor

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

#ifndef USBDEVICEDESCRIPTOR_H
#define USBDEVICEDESCRIPTOR_H

#include <libusb.h>
#include <QObject>

#ifdef Q_OS_UNIX
#include <usbutils/names.h>
#include <usbutils/sysfs.h>
#endif

namespace usb{
    class UsbDevice;
}
#ifndef USBDEVICE_H
#include "usbdevice.h"
#endif

namespace usb{
    /**
     * @brief parseBcdUSB
     * Parse bcdUSB's value to string.
     * @param bcdUSB
     * @return string description of bcdUSB
     * @note
     * The bcdUSB only indicates which version of the USB specification
     * the device implements, not the actual connection speed
     * @see UsbDevice::speed() to get the real connection speed
     * @see usbSpeedToString()
     */
    QString parseBcdUSB(uint16_t bcdUSB);

    /**
     * @brief The UsbDeviceDescriptor class
     * C++ class wrapper of libusb_device_descriptor
     */
    class UsbDeviceDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbDeviceDescriptor(const libusb_device_descriptor *desc, UsbDevice *parent = nullptr);

        /**
         * @brief bLength
         * @return size of this descriptor (in bytes)
         */
        uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_DEVICE in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief bcdUSB
         * @return USB specification release number in binary-coded decimal
         */
        uint16_t bcdUSB() const;

        /**
         * @brief bDeviceClass
         * @return USB-IF class code for the device
         * @see libusb_class_code
         */
        uint8_t bDeviceClass() const;

        /**
         * @brief bDeviceSubClass
         * @return USB-IF subclass code for the device
         */
        uint8_t bDeviceSubClass() const;

        /**
         * @brief bDeviceProtocol
         * @return USB-IF protocol code for the device
         */
        uint8_t bDeviceProtocol() const;

        /**
         * @brief bMaxPacketSize0
         * @return maximum packet size for endpoint 0
         */
        uint8_t bMaxPacketSize0() const;

        /**
         * @brief idVendor
         * @return USB-IF vendor ID
         */
        uint16_t idVendor() const;

        /**
         * @brief idProduct
         * @return USB-IF product ID
         */
        uint16_t idProduct() const;

        /**
         * @brief bcdDevice
         * @return device release number in binary-coded decimal
         */
        uint16_t bcdDevice() const;

        /**
         * @brief iManufacturer
         * @return index of string descriptor describing manufacturer
         * @see UsbDevice::getStringDescriptor()
         */
        uint8_t iManufacturer() const;

        /**
         * @brief iProduct
         * @return index of string descriptor describing product
         * @see UsbDevice::getStringDescriptor()
         */
        uint8_t iProduct() const;

        /**
         * @brief iSerialNumber
         * @return index of string descriptor containing device serial number
         */
        uint8_t iSerialNumber() const;

        /**
         * @brief bNumConfigurations
         * @return number of possible configurations
         */
        uint8_t bNumConfigurations() const;

        /**
         * @brief device
         * @return the device which current device descirptor belongs to
         */
        UsbDevice *device() const;

        /**
         * @brief vendorName
         * @return the defined vendor name associated with idVendor
         */
        const QString &vendorName() const;

        /**
         * @brief productName
         * @return the defined product name associated with idProduct
         */
        const QString &productName() const;

        /**
         * @brief description
         * @return a device description similar to what the `lsusb` tool shows.
         */
        const QString &description() const;

        /**
         * @brief bcdUSBInfo
         * Wrapper of parseBcdUSB()
         * @see parseBcdUSB()
         */
        const QString bcdUSBInfo() const;

        /**
         * @brief deviceClass
         * @return the USB-IF defined class description
         */
        const QString &deviceClass() const;

        /**
         * @brief deviceSubClass
         * @return the USB-IF defined sub-class description
         */
        const QString &deviceSubClass() const;

        /**
         * @brief deviceProtocol
         * @return the USB-IF defined protocol description
         */
        const QString &deviceProtocol() const;

    signals:

    private:
        uint8_t _bLength, _bDescriptorType, _bDeviceClass, _bDeviceSubClass, _bDeviceProtocol,
        _bMaxPacketSize0, _iManufacturer, _iProduct, _iSerialNumber, _bNumConfigurations;
        uint16_t _bcdUSB, _idVendor, _idProduct, _bcdDevice;
        UsbDevice *_device;
        QString _vendorName, _productName, _deviceClass, _deviceSubClass, _deviceProtocol, _description;
    };
}

#endif // USBDEVICEDESCRIPTOR_H
