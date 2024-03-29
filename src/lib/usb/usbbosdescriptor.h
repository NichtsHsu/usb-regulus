/*! C++ class wrapper of USB BOS Descriptor

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

#ifndef USBBOSDESCRIPTOR_H
#define USBBOSDESCRIPTOR_H

#include <libusb.h>
#include <QObject>
#include <QList>

namespace usb {
    class UsbDevice;
    class UsbDeviceCapabilityDescriptor;
    enum class DeviceCapabilityType;
}

namespace usb {
    /**
     * @brief The UsbBosDescriptor class
     * C++ class wrapper of libusb_bos_descriptor
     * @see libusb_bos_descriptor
     */
    class UsbBosDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbBosDescriptor(const libusb_bos_descriptor *desc, UsbDevice *usbDevice);

        /**
         * @brief bLength
         * @return size of this descriptor
         */
        uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_BOS in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief wTotalLength
         * @return length of this descriptor and all of its sub descriptors
         */
        uint16_t wTotalLength() const;

        /**
         * @brief bNumDeviceCaps
         * @return the number of separate device capability descriptors
         */
        uint8_t bNumDeviceCaps() const;

        /**
         * @brief getFieldNames
         * @return all field names in a string list
         */
        static const QStringList &getFieldNames();

        /**
         * @brief getFieldInformation
         * Get field information
         * @param field name
         * @return field information almost from USB specs
         */
        static QString getFieldInformation(const QString &field);

        /**
         * @brief usbDeviceCapabilityDescriptorList
         * @return the list of separate device capability descriptors
         */
        const QList<UsbDeviceCapabilityDescriptor *> &usbDeviceCapabilityDescriptorList() const;

        /**
         * @brief device
         * @return the device which current BOS descirptor belongs to
         */
        UsbDevice *device() const;

        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        QString infomationToHtml() const;

    private:
        UsbDevice *_device;
        uint8_t _bLength, _bDescriptorType, _bNumDeviceCaps;
        uint16_t _wTotalLength;
        QList<UsbDeviceCapabilityDescriptor *> _usbDeviceCapabilityDescriptorList;
    };
}

#ifndef USBDEVICE_H
#include "usbdevice.h"
#endif
#ifndef USBDEVICECAPABILITYDESCRIPTOR_H
#include "usbdevicecapabilitydescriptor.h"
#endif

#endif // USBBOSDESCRIPTOR_H
