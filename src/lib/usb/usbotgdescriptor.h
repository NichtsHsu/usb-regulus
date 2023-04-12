/*! C++ class wrapper of USB OTG Descriptor

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

#ifndef USBOTGDESCRIPTOR_H
#define USBOTGDESCRIPTOR_H

#include "usbconfigurationextradescriptor.h"
#include <QObject>

namespace usb {
    /**
     * @brief The UsbOtgDescriptor class
     * The OTG (On-The-Go) Descriptor C++ wrapper
     */
    class UsbOtgDescriptor : public UsbConfigurationExtraDescriptor
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
         * @return ConfigurationExtraDescriptorType::OTG
         */
        ConfigurationExtraDescriptorType type() const override;

        /**
         * @brief bmAttributes
         * @return bitmap of attributes
         * @note
         * D0       SRP support
         * D1       HNP support
         * D2       ADP support
         * D3       RSP support
         * D7..4    Reserved
         */
        uint8_t bmAttributes() const;

        /**
         * @brief bcdOTG
         * @return  BCD version number that identifies the release of the OTG and EH supplement
         */
        uint16_t bcdOTG() const;

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

    private:
        explicit UsbOtgDescriptor(UsbConfigurationDescriptor *configDesc);
        friend UsbConfigurationExtraDescriptor;
        void __initWithBuffer(const uint8_t *buffer) override;
        inline QString __parseBmAttributes() const;

        uint8_t _bLength, _bDescriptorType, _bmAttributes;
        uint16_t _bcdOTG;
    };
} // namespace usb

#endif // USB_USBOTGDESCRIPTOR_H
