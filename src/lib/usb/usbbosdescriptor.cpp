#include "usbbosdescriptor.h"
#include "usbhost.h"
#include "usbhtmlbuilder.h"

namespace usb {
    UsbBosDescriptor::UsbBosDescriptor(const libusb_bos_descriptor *desc, UsbDevice *usbDevice)
        : QObject{usbDevice}, _device(usbDevice),
          _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType),
          _bNumDeviceCaps(desc->bNumDeviceCaps),
          _wTotalLength(desc->wTotalLength)
    {
        for (uint8_t i = 0; i < _bNumDeviceCaps; ++i)
        {
            libusb_bos_dev_capability_descriptor *baseDesc = desc->dev_capability[i];
            UsbDeviceCapabilityDescriptor *desc = nullptr;
            switch (DeviceCapabilityType(baseDesc->bDevCapabilityType))
            {
                case DeviceCapabilityType::WIRELESS:
                    desc = new UsbWirelessDeviceCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::USB_2_0_EXTENSION:
                {
                    libusb_usb_2_0_extension_descriptor *rawDesc = nullptr;
                    libusb_get_usb_2_0_extension_descriptor(
                                UsbHost::instance()->context(), baseDesc, &rawDesc);
                    desc = new Usb20ExtensionDescriptor(rawDesc, baseDesc, this);
                }
                break;
                case DeviceCapabilityType::SUPERSPEED:
                {
                    libusb_ss_usb_device_capability_descriptor *rawDesc = nullptr;
                    libusb_get_ss_usb_device_capability_descriptor(
                                UsbHost::instance()->context(), baseDesc, &rawDesc);
                    desc = new UsbSuperSpeedDeviceCapabilityDescriptor(rawDesc, baseDesc, this);
                }
                break;
                case DeviceCapabilityType::CONTAINER_ID:
                {
                    libusb_container_id_descriptor *rawDesc = nullptr;
                    libusb_get_container_id_descriptor(
                                UsbHost::instance()->context(), baseDesc, &rawDesc);
                    desc = new UsbContainerIdDescriptor(rawDesc, baseDesc, this);
                }
                break;
                case DeviceCapabilityType::PLATFORM:
                    desc = new UsbPlatformDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::POWER_DELIVERY:
                    desc = new UsbPowerDeliveryCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::BATTERY_INFO:
                    desc = new UsbBatteryInfoCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::PD_CONSUMER_PORT:
                    desc = new UsbPowerDeliveryConsumerPortCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::PD_PROVIDER_PORT:
                    desc = new UsbPowerDeliveryProviderPortCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::SUPERSPEED_PLUS:
                    desc = new UsbSuperSpeedPlusDeviceCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::PRECISION_TIME_MEASUREMENT:
                    desc = new UsbPrecisionTimeMeasurementCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::WIRELESS_EXT:
                    desc = new UsbExtendedWirelessDeviceCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::BILLBOARD:
                    desc = new UsbBillboardCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::AUTHENTICATION:
                    desc = new UsbAuthenticationCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::BILLBOARD_AUM:
                    desc = new UsbBillboardAumCapabilityDescriptor(baseDesc, this);
                break;
                case DeviceCapabilityType::CONFIGURATION_SUMMARY:
                    desc = new UsbConfigurationSummaryDescriptor(baseDesc, this);
                break;
                default:
                    LOGW(tr("USB Device Capability Type %1 is not supported yet.").arg(baseDesc->bDevCapabilityType));
                continue;
            }

            _usbDeviceCapabilityDescriptorList.push_back(desc);
        }
    }

    UsbDevice *UsbBosDescriptor::device() const
    {
        return _device;
    }

    QString UsbBosDescriptor::infomationToHtml() const
    {
        UsbHtmlBuilder builder;
        builder.start(tr("BOS Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "BOS")
                .attr("wTotalLength", _wTotalLength)
                .attr("bNumDeviceCaps", _bNumDeviceCaps)
                .end();
        foreach (const auto &devCapDesc, _usbDeviceCapabilityDescriptorList)
            builder.append(devCapDesc);

        return builder.build();
    }

    const QList<UsbDeviceCapabilityDescriptor *> &UsbBosDescriptor::usbDeviceCapabilityDescriptorList() const
    {
        return _usbDeviceCapabilityDescriptorList;
    }

    uint8_t UsbBosDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbBosDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint16_t UsbBosDescriptor::wTotalLength() const
    {
        return _wTotalLength;
    }

    uint8_t UsbBosDescriptor::bNumDeviceCaps() const
    {
        return _bNumDeviceCaps;
    }

    const QStringList &UsbBosDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "wTotalLength",
            "bNumDeviceCaps",
        };

        return fields;
    }

    QString UsbBosDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "BOS Descriptor type"},
            {"wTotalLength", "Length of this descriptor and all of its sub descriptors"},
            {"bNumDeviceCaps", "The number of separate device capability descriptors in the BOS"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }
}
