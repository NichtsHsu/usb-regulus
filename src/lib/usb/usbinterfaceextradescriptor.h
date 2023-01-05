/*! C++ class wrapper of extra descriptors belongs to USB Interface Descriptor

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

#ifndef USBINTERFACEEXTRADESCRIPTOR_H
#define USBINTERFACEEXTRADESCRIPTOR_H

#include <QObject>

namespace usb {
    class UsbInterfaceDescriptor;
}

namespace usb {
    enum class InterfaceExtraDescriptorType {
        HID,            // Human Interface Device
        DFU,            // Device Firmware Upgrade
        AC,             // AudioControl
        AS,             // AudioStream
        VC,             // VideoControl
        VS,             // VideoStream
    };

    class UsbInterfaceExtraDescriptor : public QObject
    {
        Q_OBJECT
    public:
        UsbInterfaceExtraDescriptor(UsbInterfaceDescriptor *parent);

        /**
         * @brief interfaceDescriptor
         * @return the Interface Desciptor which current extra Descriptor belongs to
         */
        UsbInterfaceDescriptor *interfaceDescriptor() const;

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
         * @return the interface extra descriptor type
         */
        virtual InterfaceExtraDescriptorType type() const = 0;

    private:
        UsbInterfaceDescriptor *_interfaceDescriptor;
    };
}

#ifndef USBINTERFACE_H
#include "usbinterfacedescriptor.h"
#endif

#endif // USBINTERFACEEXTRADESCRIPTOR_H
