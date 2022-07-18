/*! C++ class wrapper of USB BOS Descriptor

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

#ifndef USBBOSDESCRIPTOR_H
#define USBBOSDESCRIPTOR_H

#include <libusb.h>
#include <QObject>
#include <QList>

namespace usb {
    class UsbHost;
    class UsbDevice;
    class UsbDeviceCapabilityDescriptor;
    class UsbWirelessDeviceCapabilityDescriptor;
    class Usb20ExtensionDescriptor;
    class UsbSuperSpeedDeviceCapabilityDescriptor;
    class UsbContainerIdDescriptor;
    class UsbPlatformDescriptor;
    class UsbPowerDeliveryCapabilityDescriptor;
    class UsbBatteryInfoCapabilityDescriptor;
    class UsbPowerDeliveryConsumerPortCapabilityDescriptor;
    class UsbPowerDeliveryProviderPortCapabilityDescriptor;
    class UsbSuperSpeedPlusDeviceCapabilityDescriptor;
    class UsbPrecisionTimeMeasurementCapabilityDescriptor;
    class UsbExtendedWirelessDeviceCapabilityDescriptor;
    class UsbConfigurationSummaryDescriptor;
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
         * @brief usbDeviceCapabilityDescriptorList
         * @return the list of separate device capability descriptors
         */
        const QList<UsbDeviceCapabilityDescriptor *> &usbDeviceCapabilityDescriptorList() const;

        /**
         * @brief device
         * @return the device which current BOS descirptor belongs to
         */
        UsbDevice *device() const;

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
#ifndef USBHOST_H
#include "usbhost.h"
#endif

#endif // USBBOSDESCRIPTOR_H
