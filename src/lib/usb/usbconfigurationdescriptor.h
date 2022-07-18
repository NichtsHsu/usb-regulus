/*! C++ class wrapper of USB Configuration Descriptor

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

#ifndef USBCONFIGURATIONDESCRIPTOR_H
#define USBCONFIGURATIONDESCRIPTOR_H

#include <libusb.h>
#include <QObject>
#include <QVector>

namespace usb {
    class UsbDevice;
    class UsbInterface;
    class UsbConfigurationExtraDescriptor;
}

namespace usb {
    /**
     * @brief parseConfigDescBmAttributes
     * Parse bmAttributes of configuration descriptor to string
     * @param bmAttributes
     * @return the string description of bmAttributes
     * @note
     * D7       Reserved, set to 1. (USB 1.0 Bus Powered)
     * D6       Self Powered
     * D5       Remote Wakeup
     * D4..0    Reserved, set to 0.
     */
    QString parseConfigDescBmAttributes(uint8_t bmAttributes);

    /**
     * @brief The UsbConfigurationDescriptor class
     * C++ class wrapper of libusb_config_descriptor
     */
    class UsbConfigurationDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbConfigurationDescriptor(const libusb_config_descriptor *desc, UsbDevice *parent);

        /**
         * @brief bLength
         * @return size of this descriptor (in bytes)
         */
        uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_CONFIG in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief wTotalLength
         * @return total length of data returned for this configuration
         */
        uint16_t wTotalLength() const;

        /**
         * @brief bNumInterfaces
         * @return number of interfaces supported by this configuration
         */
        uint8_t bNumInterfaces() const;

        /**
         * @brief bConfigurationValue
         * @return identifier value for this configuration
         */
        uint8_t bConfigurationValue() const;

        /**
         * @brief iConfiguration
         * @return index of string descriptor describing this configuration
         * @see UsbDevice::getStringDescriptor()
         */
        uint8_t iConfiguration() const;

        /**
         * @brief bmAttributes
         * @return configuration characteristics
         */
        uint8_t bmAttributes() const;

        /**
         * @brief MaxPower
         * @return maximum power consumption of the USB device from this bus in this
         * configuration when the device is fully operation.
         */
        uint8_t MaxPower() const;

        /**
         * @brief extra
         * @return extra descriptors
         */
        const unsigned char *extra() const;

        /**
         * @brief extra_length
         * @return length of the extra descriptors, in bytes
         */
        int extra_length() const;

        /**
         * @brief interface
         * @param index
         * the index of interface, must less than bNumInterfaces.
         * @return the specified interface
         */
        UsbInterface *interface(uint8_t index) const;

        /**
         * @brief device
         * @return the device which current configuration descirptor belongs to
         */
        UsbDevice *device() const;

        /**
         * @brief bmAttributesInfo
         * Wrapper of parseConfigDescBmAttributes()
         * @see parseConfigDescBmAttributes
         */
        QString bmAttributesInfo() const;

        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        QString infomationToHtml() const;

        /**
         * @brief addConfigurationExtraDescriptor
         * Add configuration extra descriptor
         */
        void addConfigurationExtraDescriptor(UsbConfigurationExtraDescriptor *desc);

    private:
        uint8_t _bLength, _bDescriptorType, _bNumInterfaces, _bConfigurationValue, _iConfiguration, _bmAttributes, _MaxPower;
        uint16_t _wTotalLength;
        QVector<UsbInterface *> _interfaces;
        QVector<UsbConfigurationExtraDescriptor *> _extraDescriptors;
        const unsigned char *_extra;
        int _extra_length;

        UsbDevice *_device;
    };
}

#ifndef USBDEVICE_H
#include "usbdevice.h"
#endif
#ifndef USBINTERFACE_H
#include "usbinterface.h"
#endif
#ifndef USBCONFIGURATIONEXTRADESCRIPTOR_H
#include "usbconfigurationextradescriptor.h"
#endif

#endif // USBCONFIGURATIONDESCRIPTOR_H
