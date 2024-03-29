/*! C++ class wrapper of USB Interface Descriptor

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

#ifndef USBINTERFACEDESCRIPTOR_H
#define USBINTERFACEDESCRIPTOR_H

#include <libusb.h>
#include <QObject>

namespace usb {
    class UsbInterface;
    class UsbEndpointDescriptor;
    class UsbInterfaceExtraDescriptor;
    class UsbInterfaceAssociationDescriptor;
    class UsbHidDescriptor;
}

namespace usb {

    /**
     * @brief The UsbInterfaceDescriptor class
     * C++ class wrapper of libusb_interface_descriptor
     */
    class UsbInterfaceDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbInterfaceDescriptor(const libusb_interface_descriptor *desc, UsbInterface *parent);

        /**
         * @brief bLength
         * @return size of this descriptor (in bytes)
         */
        uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_INTERFACE in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief bInterfaceNumber
         * @return number of this interface
         */
        uint8_t bInterfaceNumber() const;

        /**
         * @brief bAlternateSetting
         * @return value used to select this alternate setting for this interface
         */
        uint8_t bAlternateSetting() const;

        /**
         * @brief bNumEndpoints
         * @return number of endpoints used by this interface
         * @note excluding the control endpoint
         */
        uint8_t bNumEndpoints() const;

        /**
         * @brief bInterfaceClass
         * @return USB-IF class code for this interface
         * @see libusb_class_code
         */
        uint8_t bInterfaceClass() const;

        /**
         * @brief bInterfaceSubClass
         * @return USB-IF subclass code for this interface
         */
        uint8_t bInterfaceSubClass() const;

        /**
         * @brief bInterfaceProtocol
         * @return USB-IF protocol code for this interface
         */
        uint8_t bInterfaceProtocol() const;

        /**
         * @brief iInterface
         * @return index of string descriptor describing this interface
         */
        uint8_t iInterface() const;

        /**
         * @brief extra
         * @return extra descriptors
         */
        const unsigned char *extra() const;

        /**
         * @brief extra_length
         * @return length of the extra descriptors, in bytes
         */
        int extraLength() const;

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
         * @brief endpoint
         * @param index
         * the index of endpoint which want to get, must less than bNumEndpoints
         * @return the specified endpoint
         */
        UsbEndpointDescriptor *endpoint(int index) const;

        /**
         * @brief interface
         * @return the interface which current interface descriptor is belong to
         */
        UsbInterface *interface() const;

        /**
         * @brief interfaceClass
         * @return USB-IF defined class code description
         */
        const QString &interfaceClass() const;

        /**
         * @brief interfaceSubClass
         * @return USB-IF defined sub-class code description
         */
        const QString &interfaceSubClass() const;

        /**
         * @brief interfaceProtocol
         * @return USB-IF defined protocol code description
         */
        const QString &interfaceProtocol() const;


        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        QString infomationToHtml() const;

        /**
         * @brief isMouse
         * @return if is a mouse HID interface
         */
        bool isMouse() const;

        /**
         * @brief isKeyboard
         * @return if is a keyboard HID interface
         */
        bool isKeyboard() const;

        /**
         * @brief associationDescriptor
         * @return the association descriptor
         */
        UsbInterfaceAssociationDescriptor *associationDescriptor() const;

        /**
         * @brief extraDescriptor
         * @return the extra descriptor
         */
        const QVector<UsbInterfaceExtraDescriptor *> &extraDescriptors() const;

        /**
         * @brief setAssociationDescriptor
         * Set the association descriptor
         * @param associationDescriptor
         */
        void setAssociationDescriptor(UsbInterfaceAssociationDescriptor *associationDescriptor);

        /**
         * @brief getHidDescriptor
         * @return pointer to HID descriptor if is a HID interface, nullptr if not
         */
        UsbHidDescriptor *hidDescriptor();

    private slots:
        void __requestExtraDescriptor();

    private:
        uint8_t _bLength, _bDescriptorType, _bInterfaceNumber, _bAlternateSetting, _bNumEndpoints, _bInterfaceClass,
        _bInterfaceSubClass, _bInterfaceProtocol, _iInterface;
        QVector<UsbEndpointDescriptor *> _endpoints;
        const unsigned char *_extra;
        int _extraLength;
        UsbInterface *_interface;
        QVector<UsbInterfaceExtraDescriptor *> _extraDescriptors;
        // Can only have one Association Descriptor
        UsbInterfaceAssociationDescriptor *_associationDescriptor;
        QString _interfaceClass, _interfaceSubClass, _interfaceProtocol;
    };
}

#ifndef USBENDPOINTDESCRIPTOR_H
#include "usbendpointdescriptor.h"
#endif
#ifndef USBINTERFACE_H
#include "usbinterface.h"
#endif
#ifndef USBINTERFACEEXTRADESCRIPTOR_H
#include "usbinterfaceextradescriptor.h"
#endif
#ifndef USBINTERFACEASSOCIATIONDESCRIPTOR_H
#include "usbinterfaceassociationdescriptor.h"
#endif

#endif // USBINTERFACEDESCRIPTOR_H
