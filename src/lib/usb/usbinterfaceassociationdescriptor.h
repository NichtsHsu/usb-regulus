/*! C++ class wrapper of USB Interface Association Descriptors

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

#ifndef USBINTERFACEASSOCIATIONDESCRIPTOR_H
#define USBINTERFACEASSOCIATIONDESCRIPTOR_H

#include <libusb.h>
#include <QObject>
#include "usbconfigurationextradescriptor.h"

namespace usb {
    /**
     * @brief The UsbInterfaceAssociationDescriptor class
     * The Interface Association Descriptor C++ wrapper
     */
    class UsbInterfaceAssociationDescriptor : public UsbConfigurationExtraDescriptor
    {
        Q_OBJECT
    public:
        /**
         * @brief bLength
         * @return the total size of the interface association descriptor
         */
        uint8_t bLength() const override;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         */
        uint8_t bDescriptorType() const override;

        /**
         * @brief type
         * @return ConfigurationExtraDescriptorType::ASSOCIATION
         */
        ConfigurationExtraDescriptorType type() const override;

        /**
         * @brief bFirstInterface
         * @return interface number of the first interface that is associated with this function
         */
        uint8_t bFirstInterface() const;

        /**
         * @brief bInterfaceCount
         * @return number of contiguous interfaces that are associated with this function
         */
        uint8_t bInterfaceCount() const;

        /**
         * @brief bFunctionClass
         * @return USB-IF class code
         */
        uint8_t bFunctionClass() const;

        /**
         * @brief bFunctionSubClass
         * @return USB-IF subclass code
         */
        uint8_t bFunctionSubClass() const;

        /**
         * @brief bFunctionProtocol
         * @return USB-IF protocol code
         */
        uint8_t bFunctionProtocol() const;

        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        QString infomationToHtml() const override;

        /**
         * @brief iFunction
         * @return index of string descriptor describing this function
         */
        uint8_t iFunction() const;

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

    private:
        explicit UsbInterfaceAssociationDescriptor(UsbConfigurationDescriptor *configDesc);
        friend UsbConfigurationExtraDescriptor;
        void __initWithBuffer(const uint8_t *buffer) override;

        uint8_t _bLength, _bDescriptorType, _bFirstInterface, _bInterfaceCount,
            _bFunctionClass, _bFunctionSubClass, _bFunctionProtocol, _iFunction;

        QString _functionClass, _functionSubClass, _functionProtocol;
    };

} // namespace usb

#endif // USB_USBINTERFACEASSOCIATIONDESCRIPTOR_H
