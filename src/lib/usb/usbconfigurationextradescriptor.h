/*! C++ class wrapper of extra descriptors belongs to USB Configuration Descriptor

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

#ifndef USBCONFIGURATIONEXTRADESCRIPTOR_H
#define USBCONFIGURATIONEXTRADESCRIPTOR_H

#include <libusb.h>
#include <QObject>
#include <QString>
#include <log/logger.h>

namespace usb {
    class UsbConfigurationDescriptor;
    class UsbInterfaceAssociationDescriptor;
    class UsbOtgDescriptor;
    class UsbInterfaceDescriptor;
    class UsbEndpointDescriptor;
}

namespace usb {
    enum class ConfigurationExtraDescriptorType {
        OTG = 0x09,
        ASSOCIATION = 0x0B,
        SECURITY = 0x0C,
        ENCRYPTION_TYPE = 0x0E,
    };

    class UsbConfigurationExtraDescriptor : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief get
         * Get the configuration extra descriptor
         * @param configDesc
         * Configuration descriptor with extra data
         * @param descPos
         * Extra descriptor's position at extra data
         * @return pointer to the instance of configuration extra descriptor,
         * nullptr if parse failed.
         */
        static UsbConfigurationExtraDescriptor *get(
                UsbConfigurationDescriptor *configDesc, uint32_t descPos);

        /**
         * @brief fromInterfaceExtra
         * Descriptor may be got from interface descriptor
         * @param interfaceDesc
         * Interface descriptor
         * @param descPos
         * Interface association descriptor's position at extra data
         * @return pointer to interface association descriptor,
         * nullptr if parse failed.
         */
        static UsbConfigurationExtraDescriptor *fromInterfaceExtra(
                UsbInterfaceDescriptor *interfaceDesc, uint32_t descPos);

        /**
         * @brief fromEndpointExtra
         * Descriptor may be got from endpoint descriptor
         * @param endpointDesc
         * Endpoint descriptor
         * @param descPos
         * Interface association descriptor's position at extra data
         * @return pointer to endpoint association descriptor,
         * nullptr if parse failed.
         */
        static UsbConfigurationExtraDescriptor *fromEndpointExtra(
                UsbEndpointDescriptor *endpointDesc, uint32_t descPos);

        /**
         * @brief bLength
         * @return the total size of the descriptor
         */
        virtual uint8_t bLength() const = 0;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         */
        virtual uint8_t bDescriptorType() const = 0;

        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        virtual QString infomationToHtml() const = 0;

        /**
         * @brief type
         * @return the configuration extra descriptor type
         */
        virtual ConfigurationExtraDescriptorType type() const = 0;

        /**
         * @brief configurationDescriptor
         * @return the configuration descriptor which this descriptor is belong to
         */
        UsbConfigurationDescriptor *configurationDescriptor() const;

    protected:
        explicit UsbConfigurationExtraDescriptor(UsbConfigurationDescriptor *parent);
        virtual void __initWithBuffer(const uint8_t *buffer) = 0;

    private:
        UsbConfigurationDescriptor *_configurationDescriptor;
    };

    /**
     * @brief The UsbWirelessSecurityDescriptor class
     * Security Descriptor C++ wrapper for wireless USB device
     */
    class UsbWirelessSecurityDescriptor : public UsbConfigurationExtraDescriptor
    {
        Q_OBJECT
    public:
        /**
         * @brief bLength
         * @return the total size of the descriptor
         */
        uint8_t bLength() const override;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         */
        uint8_t bDescriptorType() const override;

        /**
         * @brief wTotalLength
         * @return length of this descriptor and all sub-descriptors returned
         */
        uint16_t wTotalLength() const;

        /**
         * @brief bNumEncryptionTypes
         * @return number of supported encryption types
         */
        uint8_t bNumEncryptionTypes() const;

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
         * @brief infomationToHtml
         * @return HTML form informations
         */
        QString infomationToHtml() const override;

        /**
         * @brief type
         * @return ConfigurationExtraDescriptorType::SECURITY
         */
        ConfigurationExtraDescriptorType type() const override;

    private:
        explicit UsbWirelessSecurityDescriptor(UsbConfigurationDescriptor *configDesc);
        virtual void __initWithBuffer(const uint8_t *buffer) override;
        friend UsbConfigurationExtraDescriptor;

        uint8_t _bLength, _bDescriptorType, _bNumEncryptionTypes;
        uint16_t _wTotalLength;
    };

    /**
     * @brief The UsbWirelessSecurityDescriptor class
     * Security Descriptor C++ wrapper for wireless USB device
     */
    class UsbWirelessEncryptionTypeDescriptor : public UsbConfigurationExtraDescriptor
    {
        Q_OBJECT
    public:
        /**
         * @brief bLength
         * @return the total size of the descriptor
         */
        uint8_t bLength() const override;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         */
        uint8_t bDescriptorType() const override;

        /**
         * @brief bEncryptionType
         * @return type of encryption
         * @note
         * 1 => Virtual encryption provided by the wire
         * 2 => AES-128 in CCM mode
         */
        uint8_t bEncryptionType() const;

        /**
         * @brief bEncryptionValue
         * @return value to use with Set Encryption
         */
        uint8_t bEncryptionValue() const;

        /**
         * @brief bAuthKeyIndex
         * @return the value specifies the Key Index to use for authentication
         */
        uint8_t bAuthKeyIndex() const;

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
         * @brief infomationToHtml
         * @return HTML form informations
         */
        QString infomationToHtml() const override;

        /**
         * @brief type
         * @return ConfigurationExtraDescriptorType::ENCRYPTION_TYPE
         */
        ConfigurationExtraDescriptorType type() const override;

    private:
        explicit UsbWirelessEncryptionTypeDescriptor(UsbConfigurationDescriptor *configDesc);
        virtual void __initWithBuffer(const uint8_t *buffer) override;
        friend UsbConfigurationExtraDescriptor;
        inline QString __parseEncryptionType() const;

        uint8_t _bLength, _bDescriptorType, _bEncryptionType, _bEncryptionValue, _bAuthKeyIndex;
    };
} // namespace usb

#ifndef USBCONFIGURATIONDESCRIPTOR_H
#include "usbconfigurationdescriptor.h"
#endif
#ifndef USBINTERFACEDESCRIPTOR_H
#include "usbinterfacedescriptor.h"
#endif
#ifndef USBENDPOINTDESCRIPTOR_H
#include "usbendpointdescriptor.h"
#endif
#ifndef USBOTGDESCRIPTOR_H
#include "usbotgdescriptor.h"
#endif

#endif // USB_USBCONFIGURATIONEXTRADESCRIPTOR_H
