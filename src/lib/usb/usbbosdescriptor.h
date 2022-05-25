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
#ifndef USBDEVICE_H
#include "usbdevice.h"
#endif
#ifndef USBDEVICECAPABILITYDESCRIPTOR_H
#include "usbdevicecapabilitydescriptor.h"
#endif
#ifndef USBHOST_H
#include "usbhost.h"
#endif

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
        explicit UsbBosDescriptor(const libusb_bos_descriptor *desc, UsbDevice *usbDevice = nullptr);

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

#endif // USBBOSDESCRIPTOR_H
