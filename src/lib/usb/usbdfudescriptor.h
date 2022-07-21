/*! C++ class wrapper of USB Device Firmware Upgrade Descriptor

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

#ifndef USBDFUDESCRIPTOR_H
#define USBDFUDESCRIPTOR_H

#include "usbinterfaceextradescriptor.h"

namespace usb {
    /**
     * @brief The UsbDfuDescriptor class
     * USB DFU (Device Firmware Upgrade) Descriptor C++ class.
     */
    class UsbDfuDescriptor : public UsbInterfaceExtraDescriptor
    {
        Q_OBJECT
    public:
        UsbDfuDescriptor(UsbInterfaceDescriptor *interfaceDescriptor,
                         uint8_t descPos);

        /**
         * @brief bLength
         * @return the total size of the DFU descriptor
         */
        virtual uint8_t bLength() const override;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         */
        virtual uint8_t bDescriptorType() const override;

        /**
         * @brief type
         * @return InterfaceExtraDescriptorType::DFU
         */
        virtual InterfaceExtraDescriptorType type() const override;

        /**
         * @brief bmAttributes
         * @return bitmap of DFU attributes
         * @note
         * D0       download capable
         * D1       upload capable
         * D2       device is able to communicate via USB after Manifestation phase
         * D3       device will perform a bus detach-attach sequence when it receives a DFU_DETACH request
         * D4..7    Reserved
         */
        uint8_t bmAttributes() const;

        /**
         * @brief wDetachTimeOut
         * @return time in milliseconds that the device will wait after receipt of the DFU_DETACH request
         */
        uint16_t wDetachTimeOut() const;

        /**
         * @brief wTransferSize
         * @return maximum number of bytes that the device can accept per control-write transaction
         */
        uint16_t wTransferSize() const;

        /**
         * @brief bcdDFUVersion
         * @return numeric expression identifying the version of the DFU Specification release
         */
        uint16_t bcdDFUVersion() const;

        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        virtual QString infomationToHtml() const override;

    private:
        inline QString __parseBmAttributes() const;

        uint8_t _bLength, _bDescriptorType, _bmAttributes;
        uint16_t _wDetachTimeOut, _wTransferSize, _bcdDFUVersion;
    };

} // namespace usb

#endif // USB_USBDFUDESCRIPTOR_H
