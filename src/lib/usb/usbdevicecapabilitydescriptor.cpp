#include "usbdevicecapabilitydescriptor.h"
#include "__usbmacro.h"

namespace usb {
    UsbDeviceCapabilityDescriptor::UsbDeviceCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *desc,
            UsbBosDescriptor *parent)
        : QObject{parent}, _bLength(desc->bLength),
          _bDevCapabilityType(desc->bDevCapabilityType),
          _bDescriptorType(desc->bDescriptorType),
          _bosDescriptor(parent)
    {

    }

    DeviceCapabilityType UsbDeviceCapabilityDescriptor::type() const
    {
        return DeviceCapabilityType(_bDevCapabilityType);
    }

    UsbBosDescriptor *UsbDeviceCapabilityDescriptor::bosDescriptor() const
    {
        return _bosDescriptor;
    }

    QString UsbDeviceCapabilityDescriptor::strType(uint8_t bDescriptorType)
    {
        return strType(DeviceCapabilityType(bDescriptorType));
    }

    QString UsbDeviceCapabilityDescriptor::strType(DeviceCapabilityType type)
    {
        switch (type)
        {
            case DeviceCapabilityType::WIRELESS:
            return QString("Wireless_USB");
            case DeviceCapabilityType::USB_2_0_EXTENSION:
            return QString("USB 2.0 EXTENSION");
            case DeviceCapabilityType::SUPERSPEED:
            return QString("SUPERSPEED_USB");
            case DeviceCapabilityType::CONTAINER_ID:
            return QString("CONTAINER_ID");
            case DeviceCapabilityType::PLATFORM:
            return QString("PLATFORM");
            case DeviceCapabilityType::POWER_DELIVERY:
            return QString("POWER_DELIVERY_CAPABILITY");
            case DeviceCapabilityType::BATTERY_INFO:
            return QString("BATTERY_INFO_CAPABILITY");
            case DeviceCapabilityType::PD_CONSUMER_PORT:
            return QString("PD_CONSUMER_PORT_CAPABILITY");
            case DeviceCapabilityType::PD_PROVIDER_PORT:
            return QString("PD_PROVIDER_PORT_CAPABILITY");
            case DeviceCapabilityType::SUPERSPEED_PLUS:
            return QString("SUPERSPEED_PLUS");
            case DeviceCapabilityType::PRECISION_TIME_MEASUREMENT:
            return QString("PRECISION_TIME_MEASUREMENT");
            case DeviceCapabilityType::WIRELESS_EXT:
            return QString("Wireless_USB_Ext");
            case DeviceCapabilityType::BILLBOARD:
            return QString("BILLBOARD");
            case DeviceCapabilityType::AUTHENTICATION:
            return QString("AUTHENTICATION");
            case DeviceCapabilityType::BILLBOARD_AUM:
            return QString("BILLBOARD_AUM");
            case DeviceCapabilityType::CONFIGURATION_SUMMARY:
            return QString("CONFIGURATION SUMMARY");
        }

        return QString();
    }

    uint8_t UsbDeviceCapabilityDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint8_t UsbDeviceCapabilityDescriptor::bLength() const
    {
        return _bLength;
    }

    Usb20ExtensionDescriptor::Usb20ExtensionDescriptor(
            const libusb_usb_2_0_extension_descriptor *desc,
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent),
        _bLength(desc->bLength),
        _bmAttributes(desc->bmAttributes)
    {

    }

    uint8_t Usb20ExtensionDescriptor::bLength() const
    {
        return _bLength;
    }

    bool Usb20ExtensionDescriptor::supportLPM() const
    {
        return _bmAttributes & LIBUSB_BM_LPM_SUPPORT;
    }

    QString Usb20ExtensionDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("USB 2.0 Extension Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bmAttributes", _bmAttributes, supportLPM() ?
                 tr("Link Power Management is supported"):
                 tr("Link Power Management is NOT supported"));
        END;

        return html;
    }

    uint32_t Usb20ExtensionDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    UsbSuperSpeedDeviceCapabilityDescriptor::UsbSuperSpeedDeviceCapabilityDescriptor(
            const libusb_ss_usb_device_capability_descriptor *desc,
            const libusb_bos_dev_capability_descriptor *base, UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent),
        _bLength(desc->bLength), _bmAttributes(desc->bmAttributes),
        _bFunctionalitySupport(desc->bFunctionalitySupport), _bU1DevExitLat(desc->bU1DevExitLat),
        _wSpeedSupported(desc->wSpeedSupported), _bU2DevExitLat(desc->bU2DevExitLat)
    {

    }

    uint8_t UsbSuperSpeedDeviceCapabilityDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbSuperSpeedDeviceCapabilityDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    uint8_t UsbSuperSpeedDeviceCapabilityDescriptor::bFunctionalitySupport() const
    {
        return _bFunctionalitySupport;
    }

    uint8_t UsbSuperSpeedDeviceCapabilityDescriptor::bU1DevExitLat() const
    {
        return _bU1DevExitLat;
    }

    uint16_t UsbSuperSpeedDeviceCapabilityDescriptor::wSpeedSupported() const
    {
        return _wSpeedSupported;
    }

    uint16_t UsbSuperSpeedDeviceCapabilityDescriptor::bU2DevExitLat() const
    {
        return _bU2DevExitLat;
    }

    bool UsbSuperSpeedDeviceCapabilityDescriptor::supportLTM() const
    {
        return _bmAttributes & LIBUSB_BM_LTM_SUPPORT;
    }

    QStringList UsbSuperSpeedDeviceCapabilityDescriptor::speedSupportedList() const
    {
        QStringList speeds;
        if (_wSpeedSupported & LIBUSB_LOW_SPEED_OPERATION)
            speeds.append("Low Speed");
        if (_wSpeedSupported & LIBUSB_FULL_SPEED_OPERATION)
            speeds.append("Full Speed");
        if (_wSpeedSupported & LIBUSB_HIGH_SPEED_OPERATION)
            speeds.append("High Speed");
        if (_wSpeedSupported & LIBUSB_SUPER_SPEED_OPERATION)
            speeds.append("SuperSpeed");

        return speeds;
    }

    QString UsbSuperSpeedDeviceCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("USB SuperSpeed Device Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bmAttributes", _bmAttributes, supportLTM() ?
                 tr("Latency Tolerance Messages is supported"):
                 tr("Latency Tolerance Messages is NOT supported"));
        ATTR("wSpeedSupported", _wSpeedSupported,
             tr("Supports") + QString(" ") + speedSupportedList().join(", "));
        ATTR("bFunctionalitySupport", _bFunctionalitySupport,
             _bFunctionalitySupport ? tr("The lowest speed at all functionality is available"): "");
        ATTR("bU1DevExitLat", _bU1DevExitLat, tr("Less than %1 µs").arg(_bU1DevExitLat));
        ATTR("bU2DevExitLat", _bU2DevExitLat, tr("Less than %1 µs").arg(_bU2DevExitLat));
        END;
        return html;
    }

    UsbContainerIdDescriptor::UsbContainerIdDescriptor(
            const libusb_container_id_descriptor *desc,
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent),
        _bLength(desc->bLength),
        _containerID(reinterpret_cast<const char *>(desc->ContainerID), 16)
    {

    }

    const QByteArray &UsbContainerIdDescriptor::containerID() const
    {
        return _containerID;
    }

    QString UsbContainerIdDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("USB Container ID Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTRTEXT("ContainerID ", hexUuid(_containerID));
        END;

        return html;
    }

    uint8_t UsbContainerIdDescriptor::bLength() const
    {
        return _bLength;
    }

    UsbWirelessDeviceCapabilityDescriptor::UsbWirelessDeviceCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _bmAttributes = *base->dev_capability_data;
        _wPHYRates = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 1);
        _bmTFITXPowerInfo = *(base->dev_capability_data + 3);
        _bmFFITXPowerInfo = *(base->dev_capability_data + 4);
        _bmBandGroup = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 5);
    }

    QString UsbWirelessDeviceCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Wireless USB Device Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bmAttributes", _bmAttributes, __parseBmAttributes());
        ATTR("wPHYRates", _wPHYRates,
             tr("Supports %1 Mbps").arg(__parseWPHYRateList().join(", ")));
        ATTR("bmTFITXPowerInfo", _bmTFITXPowerInfo, __parsePowerInfo(_bmTFITXPowerInfo));
        ATTR("bmFFITXPowerInfo", _bmFFITXPowerInfo, __parsePowerInfo(_bmFFITXPowerInfo));
        ATTR("bmBandGroup", _bmBandGroup,
             tr("All bands in band groups %1 are supported").arg(__parseBmBandGroupList().join(", ")));
        return html;
    }

    uint8_t UsbWirelessDeviceCapabilityDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    uint16_t UsbWirelessDeviceCapabilityDescriptor::wPHYRates() const
    {
        return _wPHYRates;
    }

    uint8_t UsbWirelessDeviceCapabilityDescriptor::bmTFITXPowerInfo() const
    {
        return _bmTFITXPowerInfo;
    }

    uint8_t UsbWirelessDeviceCapabilityDescriptor::bmFFITXPowerInfo() const
    {
        return _bmFFITXPowerInfo;
    }

    uint16_t UsbWirelessDeviceCapabilityDescriptor::bmBandGroup() const
    {
        return _bmBandGroup;
    }

    QString UsbWirelessDeviceCapabilityDescriptor::__parseBmAttributes() const
    {
        QString str;
        if (BIT(_bmAttributes, 1))
            str += tr("Peer to Peer DRD capable") + QString(", ");
        if (BIT(_bmAttributes, 2))
            str += tr("Self-Beacon") + QString(", ");
        if (BIT(_bmAttributes, 4))
            str += tr("Power Indication is supported");
        else
            str += tr("Power Indication is NOT supported");
        return str;
    }

    QStringList UsbWirelessDeviceCapabilityDescriptor::__parseWPHYRateList() const
    {
        QStringList rateList;
        if (BIT(_wPHYRates, 0))
            rateList.push_back("53.3");
        if (BIT(_wPHYRates, 1))
            rateList.push_back("80");
        if (BIT(_wPHYRates, 2))
            rateList.push_back("106.7");
        if (BIT(_wPHYRates, 3))
            rateList.push_back("160");
        if (BIT(_wPHYRates, 4))
            rateList.push_back("200");
        if (BIT(_wPHYRates, 5))
            rateList.push_back("320");
        if (BIT(_wPHYRates, 6))
            rateList.push_back("400");
        if (BIT(_wPHYRates, 7))
            rateList.push_back("480");

        return rateList;
    }

    QString UsbWirelessDeviceCapabilityDescriptor::__parsePowerInfo(uint8_t info) const
    {
        int steps = CUT(info, 0, 3);
        int size = CUT(info, 4, 7);
        return tr("Power Level Steps: %1").arg(steps) + NEWLINE +
                tr("Step Size: %1 dB").arg(size);
    }

    QStringList UsbWirelessDeviceCapabilityDescriptor::__parseBmBandGroupList() const
    {
        QStringList bandGroupList;
        for (uint8_t i = 0; i < 16; ++i)
            if (BIT(_bmBandGroup, i))
                bandGroupList.append(QString::number(i));
        return bandGroupList;
    }

    UsbSuperSpeedPlusDeviceCapabilityDescriptor::UsbSuperSpeedPlusDeviceCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)

    {
        _bmAttributes = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 1);
        _wFunctionalitySupport = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 5);
        _bmSublinkSpeedAttr = reinterpret_cast<const uint32_t *>(base->dev_capability_data + 9);
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("USB SuperSpeed Plus Device Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bmAttributes", _bmAttributes, __parseBmAttributes());
        ATTR("wFunctionalitySupport", _wFunctionalitySupport, __parseFunctionalitySupport());
        for (unsigned int i = 0; i <= CUT(_bmAttributes, 0, 4); ++i)
            ATTR(QString("bmSublinkSpeedAttr[%1]").arg(i),
                 sublinkSpeedAttr(i),
                 __parseSublinkSpeedAttr(i));
        END;

        return html;
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::__parseBmAttributes() const
    {
        int ssac = CUT(_bmAttributes, 0, 4);
        int ssic = CUT(_bmAttributes, 5, 8);
        return tr("%1 Sublink Speed Attributes").arg(ssac + 1) + NEWLINE +
                tr("%1 Sublink Speed IDs").arg(ssic + 1);
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::__parseFunctionalitySupport() const
    {
        int ssid = CUT(_wFunctionalitySupport, 0, 3);
        int minRxLaneCount = CUT(_wFunctionalitySupport, 8, 11);
        int minTxLaneCount = CUT(_wFunctionalitySupport, 12, 15);
        return tr("Sublink Speed Attribute ID: %1").arg(ssid) + NEWLINE +
                tr("Min Rx Lane Count: %1").arg(minRxLaneCount) + NEWLINE +
                tr("Min Tx Lane Count: %1").arg(minTxLaneCount);
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::__parseSublinkSpeedAttr(size_t index) const
    {
        uint32_t attr = sublinkSpeedAttr(index);
        int ssid = CUT(attr, 0, 3);
        int laneSpeedExponent = CUT(attr, 4, 5);
        int sublinkType = CUT(attr, 6, 7);
        int linkProtocol = CUT(attr, 14, 15);
        int laneSpeedMantissa = CUT(attr, 16, 31);

        QString bitRate;
        switch (laneSpeedExponent)
        {
            case 0:
                bitRate = tr("b/s");
            break;
            case 1:
                bitRate = tr("kb/s");
            break;
            case 2:
                bitRate = tr("Mb/s");
            break;
            case 3:
                bitRate = tr("Gb/s");
            break;
        }

        return tr("Sublink Speed Attribute ID: %1").arg(ssid) + NEWLINE +
                tr("Lane Speed: %1 %2").arg(laneSpeedMantissa).arg(bitRate) + NEWLINE +
                tr("Sublink Type: %1, %2")
                .arg((sublinkType & 0b1) ? tr("Asymmetric") : tr("Symmetric"))
                .arg((sublinkType & 0b10) ? tr("Transmit mode") : tr("Receive mode")) + NEWLINE +
                tr("Link Protocol: %1").arg(linkProtocol ? "SuperSpeed Plus" : "SuperSpeed");
    }

    uint16_t UsbSuperSpeedPlusDeviceCapabilityDescriptor::wFunctionalitySupport() const
    {
        return _wFunctionalitySupport;
    }

    uint32_t UsbSuperSpeedPlusDeviceCapabilityDescriptor::sublinkSpeedAttr(size_t index) const
    {
        if (index > CUT(_bmAttributes, 0, 4) + 1)
        {
            LOGE(tr("Index %1 out of range with Sublink Speed Attribute count %2.")
                 .arg(index).arg(CUT(_bmAttributes, 0, 4) + 1));
            return 0;
        }

        return _bmSublinkSpeedAttr[index];
    }

    uint32_t UsbSuperSpeedPlusDeviceCapabilityDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    UsbPlatformDescriptor::UsbPlatformDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent),
        _platformCapabilityUUID(
            reinterpret_cast<const char *>(base->dev_capability_data + 1),
            16),
        _capabilityData(
            reinterpret_cast<const char *>(base->dev_capability_data + 17),
            base->bLength - 20)
    {

    }

    const QByteArray &UsbPlatformDescriptor::platformCapabilityUUID() const
    {
        return _platformCapabilityUUID;
    }

    const QByteArray &UsbPlatformDescriptor::capabilityData() const
    {
        return _capabilityData;
    }

    QString UsbPlatformDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("USB Platform Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTRTEXT("PlatformCapabilityUUID", hexUuid(_platformCapabilityUUID));
        ATTRTEXT("CapabilityData", QString("0x") + _capabilityData.toHex());
        END;

        return html;
    }

    UsbPowerDeliveryCapabilityDescriptor::UsbPowerDeliveryCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _bmAttributes = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 1);
        _bcdBCVersion = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 5);
        _bcdPDVersion = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 7);
        _bcdUSBTypeCVersion = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 9);
    }

    uint32_t UsbPowerDeliveryCapabilityDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    uint16_t UsbPowerDeliveryCapabilityDescriptor::bcdBCVersion() const
    {
        return _bcdBCVersion;
    }

    uint16_t UsbPowerDeliveryCapabilityDescriptor::bcdPDVersion() const
    {
        return _bcdPDVersion;
    }

    uint16_t UsbPowerDeliveryCapabilityDescriptor::bcdUSBTypeCVersion() const
    {
        return _bcdUSBTypeCVersion;
    }

    QString UsbPowerDeliveryCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("USB Power Delivery Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bmAttributes", _bmAttributes, __parseBmAttributes());
        ATTR("bcdBCVersion", _bcdBCVersion, "");
        ATTR("bcdPDVersion", _bcdPDVersion, "");
        ATTR("bcdUSBTypeCVersion", _bcdUSBTypeCVersion, "");
        END;

        return html;
    }

    QString UsbPowerDeliveryCapabilityDescriptor::__parseBmAttributes() const
    {
        QStringList featuresList;
        if (BIT(_bmAttributes, 1))
            featuresList.push_back(tr("Supports the Battery Charging Specification"));
        if (BIT(_bmAttributes, 2))
            featuresList.push_back(tr("Supports the USB Power Delivery Specification"));
        if (BIT(_bmAttributes, 3))
            featuresList.push_back(tr("Provider"));
        if (BIT(_bmAttributes, 4))
            featuresList.push_back(tr("Consumer"));
        if (BIT(_bmAttributes, 5))
            featuresList.push_back(tr("Supports the feature CHARGING_POLICY"));
        if (BIT(_bmAttributes, 6))
            featuresList.push_back(tr("Supports USB Type-C Specification defined power capabilities"));
        if (BIT(_bmAttributes, 8))
            featuresList.push_back(tr("Supports AC Supply Power Source"));
        if (BIT(_bmAttributes, 9))
        {
            featuresList.push_back(tr("Supports Battery Power Source"));
            featuresList.push_back(tr("Number of batteries: %1").arg(CUT(_bmAttributes, 11, 13)));
        }
        if (BIT(_bmAttributes, 10))
            featuresList.push_back(tr("Supports Other Power Source"));
        if (BIT(_bmAttributes, 14))
            featuresList.push_back(tr("Uses VBUS"));

        return featuresList.join(NEWLINE);
    }

    UsbBatteryInfoCapabilityDescriptor::UsbBatteryInfoCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _iBattery = *base->dev_capability_data;
        _iSerial = *(base->dev_capability_data + 1);
        _iManufacturer = *(base->dev_capability_data + 2);
        _bBatteryId = *(base->dev_capability_data + 3);
        _dwChargedThreshold = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 5);
        _dwWeakThreshold = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 9);
        _dwBatteryDesignCapacity = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 13);
        _dwBatteryLastFullchargeCapacity = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 17);
    }

    uint8_t UsbBatteryInfoCapabilityDescriptor::iBattery() const
    {
        return _iBattery;
    }

    uint8_t UsbBatteryInfoCapabilityDescriptor::iSerial() const
    {
        return _iSerial;
    }

    uint8_t UsbBatteryInfoCapabilityDescriptor::iManufacturer() const
    {
        return _iManufacturer;
    }

    uint8_t UsbBatteryInfoCapabilityDescriptor::bBatteryId() const
    {
        return _bBatteryId;
    }

    uint32_t UsbBatteryInfoCapabilityDescriptor::dwChargedThreshold() const
    {
        return _dwChargedThreshold;
    }

    uint32_t UsbBatteryInfoCapabilityDescriptor::dwWeakThreshold() const
    {
        return _dwWeakThreshold;
    }

    uint32_t UsbBatteryInfoCapabilityDescriptor::dwBatteryDesignCapacity() const
    {
        return _dwBatteryDesignCapacity;
    }

    uint32_t UsbBatteryInfoCapabilityDescriptor::dwBatteryLastFullchargeCapacity() const
    {
        return _dwBatteryLastFullchargeCapacity;
    }

    QString UsbBatteryInfoCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("USB Battery Info Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTRSTRDESC("iBattery", _iBattery, _bosDescriptor->device());
        ATTRSTRDESC("iSerial", _iSerial, _bosDescriptor->device());
        ATTRSTRDESC("iManufacturer", _iManufacturer, _bosDescriptor->device());
        ATTR("bBatteryId", _bBatteryId, _bBatteryId);
        ATTR("dwChargedThreshold", _dwChargedThreshold, tr("%1 mWh").arg(_dwChargedThreshold));
        ATTR("dwWeakThreshold", _dwWeakThreshold, tr("%1 mWh").arg(_dwWeakThreshold));
        ATTR("dwBatteryDesignCapacity", _dwBatteryDesignCapacity,
             tr("%1 mWh").arg(_dwBatteryDesignCapacity));
        ATTR("dwBatteryLastFullchargeCapacity", _dwBatteryLastFullchargeCapacity,
             tr("%1 mWh").arg(_dwBatteryLastFullchargeCapacity));
        END;

        return html;
    }

    UsbPowerDeliveryConsumerPortCapabilityDescriptor::UsbPowerDeliveryConsumerPortCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _bmCapabilities = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 1);
        _wMinVoltage = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 3);
        _wMaxVoltage = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 5);
        _dwMaxOperatingPower = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 9);
        _dwMaxPeakPower = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 13);
        _dwMaxPeakPowerTime = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 17);
    }

    uint16_t UsbPowerDeliveryConsumerPortCapabilityDescriptor::bmCapabilities() const
    {
        return _bmCapabilities;
    }

    uint16_t UsbPowerDeliveryConsumerPortCapabilityDescriptor::wMinVoltage() const
    {
        return _wMinVoltage;
    }

    uint16_t UsbPowerDeliveryConsumerPortCapabilityDescriptor::wMaxVoltage() const
    {
        return _wMaxVoltage;
    }

    uint32_t UsbPowerDeliveryConsumerPortCapabilityDescriptor::dwMaxOperatingPower() const
    {
        return _dwMaxOperatingPower;
    }

    uint32_t UsbPowerDeliveryConsumerPortCapabilityDescriptor::dwMaxPeakPower() const
    {
        return _dwMaxPeakPower;
    }

    uint32_t UsbPowerDeliveryConsumerPortCapabilityDescriptor::dwMaxPeakPowerTime() const
    {
        return _dwMaxPeakPowerTime;
    }

    QString UsbPowerDeliveryConsumerPortCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("USB PD Consumer Port Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bmCapabilities", _bmCapabilities, __parseBmCapabilities());
        ATTR("wMinVoltage", _wMinVoltage, tr("%1 mV").arg(_wMinVoltage * 50));
        ATTR("wMaxVoltage", _wMaxVoltage, tr("%1 mV").arg(_wMaxVoltage * 50));
        ATTR("dwMaxOperatingPower", _dwMaxOperatingPower,
             tr("%1 mW").arg(_dwMaxOperatingPower * 10));
        ATTR("dwMaxPeakPower", _dwMaxPeakPower,
             tr("%1 mW").arg(_dwMaxPeakPower * 10));
        ATTR("dwMaxPeakPowerTime", _dwMaxPeakPowerTime,
             tr("%1 ms").arg(_dwMaxPeakPowerTime * 100));
        END;

        return html;
    }

    QString UsbPowerDeliveryConsumerPortCapabilityDescriptor::__parseBmCapabilities() const
    {
        QStringList capabilitiesList;
        if (BIT(_bmCapabilities, 0))
            capabilitiesList.push_back(tr("Operates Battery Charging Specification"));
        if (BIT(_bmCapabilities, 1))
            capabilitiesList.push_back(tr("Operates USB Power Delivery Specification"));
        if (BIT(_bmCapabilities, 2))
            capabilitiesList.push_back(tr("Operates USB Type-C Current Specification"));
        return capabilitiesList.join(NEWLINE);
    }

    UsbPowerDeliveryProviderPortCapabilityDescriptor::UsbPowerDeliveryProviderPortCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _bmCapabilities = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 1);
        _bNumOfPDObjects = *(base->dev_capability_data + 3);
        _wPowerDataObject = reinterpret_cast<const uint32_t *>(base->dev_capability_data + 5);
    }

    uint16_t UsbPowerDeliveryProviderPortCapabilityDescriptor::bmCapabilities() const
    {
        return _bmCapabilities;
    }

    uint8_t UsbPowerDeliveryProviderPortCapabilityDescriptor::bNumOfPDObjects() const
    {
        return _bNumOfPDObjects;
    }

    uint32_t UsbPowerDeliveryProviderPortCapabilityDescriptor::wPowerDataObject(size_t index) const
    {
        if (index > _bNumOfPDObjects)
        {
            LOGE(tr("Index %1 out of range with Power Data Object count %2.")
                 .arg(index).arg(_bNumOfPDObjects));
            return 0;
        }

        return _wPowerDataObject[index];
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("USB PD Provider Port Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bmCapabilities", _bmCapabilities, __parseBmCapabilities());
        for (unsigned int i = 0; i <= _bNumOfPDObjects; ++i)
            ATTR(QString("wPowerDataObject[%1]").arg(i),
                 wPowerDataObject(i),
                 __parsePowerDataObject(wPowerDataObject(i)));
        END;

        return html;
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::__parseBmCapabilities() const
    {
        QStringList capabilitiesList;
        if (BIT(_bmCapabilities, 0))
            capabilitiesList.push_back(tr("Operates Battery Charging Specification"));
        if (BIT(_bmCapabilities, 1))
            capabilitiesList.push_back(tr("Operates USB Power Delivery Specification"));
        if (BIT(_bmCapabilities, 2))
            capabilitiesList.push_back(tr("Operates USB Type-C Current Specification"));
        return capabilitiesList.join(NEWLINE);
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::__parsePowerDataObject(uint32_t object) const
    {
        switch (CUT(object, 30, 31))
        {
            case 0:
            return __parseFixedSupplyPDO(object);
            case 1:
            return __parseBatterySupplyPDO(object);
            case 2:
            return __parseVariableSupplyPDO(object);
            case 3:
            default:
                switch(CUT(object, 28, 29))
                {
                    case 0:
                    return __parseSPRProgrammablePowerSupply(object);
                    case 1:
                    return __parseEPRAdjustableVoltageSupply(object);
                    default:
                        LOGE(tr("Unhandled Power Data Object (0x%1)").arg(object, sizeof(object) * 2, 16, QChar('0')));
                    return tr("Unhandled");
                }
        }
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::__parseFixedSupplyPDO(uint32_t object) const
    {
        QStringList featuresList;
        featuresList.push_back(tr("Fixed Supply"));
        if (BIT(object, 29))
            featuresList.push_back(tr("Dual-Role Power"));
        if (BIT(object, 28))
            featuresList.push_back(tr("USB Suspend Supported"));
        if (BIT(object, 27))
            featuresList.push_back(tr("Unconstrained Power"));
        if (BIT(object, 26))
            featuresList.push_back(tr("USB Communications Capable"));
        if (BIT(object, 25))
            featuresList.push_back(tr("Dual-Role Data"));
        if (BIT(object, 24))
            featuresList.push_back(tr("Unchunked Extended Messages Supported"));
        if (BIT(object, 23))
            featuresList.push_back(tr("EPR Mode Capable"));
        switch (CUT(object, 20, 21))
        {
            case 0:
                featuresList.push_back(tr("Peak current equals IOC"));
            break;
            case 1:
                featuresList.push_back(tr("Peak current equals 150% IOC for 1ms @ 5% duty cycle"));
                featuresList.push_back(tr("Peak current equals 125% IOC for 2ms @ 10% duty cycle"));
                featuresList.push_back(tr("Peak current equals 110% IOC for 10ms @ 50% duty cycle"));
            break;
            case 2:
                featuresList.push_back(tr("Peak current equals 200% IOC for 1ms @ 5% duty cycle"));
                featuresList.push_back(tr("Peak current equals 150% IOC for 2ms @ 10% duty cycle"));
                featuresList.push_back(tr("Peak current equals 125% IOC for 10ms @ 50% duty cycle"));
            break;
            case 3:
                featuresList.push_back(tr("Peak current equals 200% IOC for 1ms @ 5% duty cycle"));
                featuresList.push_back(tr("Peak current equals 175% IOC for 2ms @ 10% duty cycle"));
                featuresList.push_back(tr("Peak current equals 150% IOC for 10ms @ 50% duty cycle"));
            break;
        }
        featuresList.push_back(tr("Voltage: %1 mV").arg(CUT(object, 10, 19) * 50));
        featuresList.push_back(tr("Maximum Current: %1 mA").arg(CUT(object, 0, 9) * 10));

        return featuresList.join(NEWLINE);
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::__parseBatterySupplyPDO(uint32_t object) const
    {
        QStringList featuresList;
        featuresList.push_back(tr("Battery Supply"));
        featuresList.push_back(tr("Maximum Voltage: %1 mV").arg(CUT(object, 20, 29) * 50));
        featuresList.push_back(tr("Minimum Voltage: %1 mV").arg(CUT(object, 10, 19) * 50));
        featuresList.push_back(tr("Maximum Allowable Power: %1 mW").arg(CUT(object, 0, 9) * 250));

        return featuresList.join(NEWLINE);
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::__parseVariableSupplyPDO(uint32_t object) const
    {
        QStringList featuresList;
        featuresList.push_back(tr("Variable Supply (non-Battery)"));
        featuresList.push_back(tr("Maximum Voltage: %1 mV").arg(CUT(object, 20, 29) * 50));
        featuresList.push_back(tr("Minimum Voltage: %1 mV").arg(CUT(object, 10, 19) * 50));
        featuresList.push_back(tr("Maximum Current: %1 mA").arg(CUT(object, 0, 9) * 10));

        return featuresList.join(NEWLINE);
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::__parseSPRProgrammablePowerSupply(uint32_t object) const
    {
        QStringList featuresList;
        featuresList.push_back(tr("SPR Programmable Power Supply"));
        if (BIT(object, 27))
            featuresList.push_back(tr("PPS Power Limited"));
        featuresList.push_back(tr("Maximum Voltage: %1 mV").arg(CUT(object, 17, 24) * 100));
        featuresList.push_back(tr("Minimum Voltage: %1 mV").arg(CUT(object, 8, 15) * 100));
        featuresList.push_back(tr("Maximum Current: %1 mA").arg(CUT(object, 0, 6) * 50));

        return featuresList.join(NEWLINE);
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::__parseEPRAdjustableVoltageSupply(uint32_t object) const
    {
        QStringList featuresList;
        featuresList.push_back(tr("EPR Adjustable Voltage Supply"));
        switch (CUT(object, 26, 27))
        {
            case 0:
                featuresList.push_back(tr("Peak current equals IOC"));
            break;
            case 1:
                featuresList.push_back(tr("Peak current equals 150% IOC for 1ms @ 5% duty cycle"));
                featuresList.push_back(tr("Peak current equals 125% IOC for 2ms @ 10% duty cycle"));
                featuresList.push_back(tr("Peak current equals 110% IOC for 10ms @ 50% duty cycle"));
            break;
            case 2:
                featuresList.push_back(tr("Peak current equals 200% IOC for 1ms @ 5% duty cycle"));
                featuresList.push_back(tr("Peak current equals 150% IOC for 2ms @ 10% duty cycle"));
                featuresList.push_back(tr("Peak current equals 125% IOC for 10ms @ 50% duty cycle"));
            break;
            case 3:
                featuresList.push_back(tr("Peak current equals 200% IOC for 1ms @ 5% duty cycle"));
                featuresList.push_back(tr("Peak current equals 175% IOC for 2ms @ 10% duty cycle"));
                featuresList.push_back(tr("Peak current equals 150% IOC for 10ms @ 50% duty cycle"));
            break;
        }

        featuresList.push_back(tr("Maximum Voltage: %1 mV").arg(CUT(object, 17, 25) * 100));
        featuresList.push_back(tr("Minimum Voltage: %1 mV").arg(CUT(object, 8, 15) * 100));
        featuresList.push_back(tr("PDP: %1 W").arg(CUT(object, 0, 7)));

        return featuresList.join(NEWLINE);
    }

    UsbPrecisionTimeMeasurementCapabilityDescriptor::UsbPrecisionTimeMeasurementCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {

    }

    QString UsbPrecisionTimeMeasurementCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Precision Time Measurement Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        END;

        return html;
    }

    UsbExtendedWirelessDeviceCapabilityDescriptor::UsbExtendedWirelessDeviceCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _wIdleTimeout = *reinterpret_cast<const uint16_t *>(base->dev_capability_data);
        _wWakeUpLatency = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 2);
        _bmControl = *(base->dev_capability_data + 4);
    }

    uint16_t UsbExtendedWirelessDeviceCapabilityDescriptor::wIdleTimeout() const
    {
        return _wIdleTimeout;
    }

    uint16_t UsbExtendedWirelessDeviceCapabilityDescriptor::wWakeUpLatency() const
    {
        return _wWakeUpLatency;
    }

    uint8_t UsbExtendedWirelessDeviceCapabilityDescriptor::bmControl() const
    {
        return _bmControl;
    }

    QString UsbExtendedWirelessDeviceCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Extended Wireless Device Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("wIdleTimeout", _wIdleTimeout, tr("%1 ms").arg(_wIdleTimeout));
        ATTR("wWakeUpLatency", _wWakeUpLatency, tr("%1 ms").arg(_wWakeUpLatency));
        ATTR("bmControl", _bmControl, _bmControl ? tr("Free") : tr("Strict"));
        END;

        return html;
    }

    UsbConfigurationSummaryDescriptor::UsbConfigurationSummaryDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _bcdVersion = *reinterpret_cast<const uint16_t *>(base->dev_capability_data);
        _bClass = *(base->dev_capability_data + 2);
        _bSubClass = *(base->dev_capability_data + 3);
        _bProtocol = *(base->dev_capability_data + 4);
        _bConfigurationCount = *(base->dev_capability_data + 5);
        _bConfigurationIndex = base->dev_capability_data + 6;
    }

    uint16_t UsbConfigurationSummaryDescriptor::bcdVersion() const
    {
        return _bcdVersion;
    }

    uint8_t UsbConfigurationSummaryDescriptor::bClass() const
    {
        return _bClass;
    }

    uint8_t UsbConfigurationSummaryDescriptor::bSubClass() const
    {
        return _bSubClass;
    }

    uint8_t UsbConfigurationSummaryDescriptor::bProtocol() const
    {
        return _bProtocol;
    }

    uint8_t UsbConfigurationSummaryDescriptor::bConfigurationCount() const
    {
        return _bConfigurationCount;
    }

    uint8_t UsbConfigurationSummaryDescriptor::bConfigurationIndex(size_t index) const
    {
        if (index > _bConfigurationCount)
        {
            LOGE(tr("Index %1 out of range with configuration count %2.")
                 .arg(index).arg(_bConfigurationCount));
            return 0;
        }

        return _bConfigurationIndex[index];
    }

    QString UsbConfigurationSummaryDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Configuration Summary Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bcdVersion", _bcdVersion, "");
        ATTR("bClass", _bClass, _bClass);
        ATTR("bSubClass", _bSubClass, _bSubClass);
        ATTR("bProtocol", _bProtocol, _bProtocol);
        ATTR("bConfigurationCount", _bConfigurationCount, _bConfigurationCount);
        for (uint8_t i = 0; i < _bConfigurationCount; ++i)
            ATTR(QString("bConfigurationIndex[%1]").arg(i),
                 bConfigurationIndex(i),
                 bConfigurationIndex(i));
        END;

        return html;
    }

    UsbBillboardCapabilityDescriptor::UsbBillboardCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _iAddtionalInfoURL = *base->dev_capability_data;
        _bNumberOfAlternateOrUSB4Modes = *(base->dev_capability_data + 1);
        _bPreferredAlternateOrUSB4Mode = *(base->dev_capability_data + 2);
        _VCONNPower = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 3);
        _bmConfigured.reserve(32);
        for (uint8_t i = 0; i < 32; ++i)
            _bmConfigured.push_back(*(base->dev_capability_data + 5 + i));
        _bcdVersion = *reinterpret_cast<const uint16_t *>(base->dev_capability_data + 37);
        _bAdditionalFailureInfo = *(base->dev_capability_data + 39);
        _wSVID.reserve(_bNumberOfAlternateOrUSB4Modes);
        _bAlternateOrUSB4Mode.reserve(_bNumberOfAlternateOrUSB4Modes);
        _iAlternateOrUSB4ModeString.reserve(_bNumberOfAlternateOrUSB4Modes);
        for (uint8_t i = 0; i < _bNumberOfAlternateOrUSB4Modes; ++i)
        {
            _wSVID.append(*reinterpret_cast<const uint16_t *>(base->dev_capability_data + 41 + i * 4));
            _bAlternateOrUSB4Mode.append(*(base->dev_capability_data + 43 + i * 4));
            _iAlternateOrUSB4ModeString.append(*(base->dev_capability_data + 44 + i * 4));
        }
    }

    uint8_t UsbBillboardCapabilityDescriptor::iAddtionalInfoURL() const
    {
        return _iAddtionalInfoURL;
    }

    uint8_t UsbBillboardCapabilityDescriptor::bNumberOfAlternateOrUSB4Modes() const
    {
        return _bNumberOfAlternateOrUSB4Modes;
    }

    uint8_t UsbBillboardCapabilityDescriptor::bPreferredAlternateOrUSB4Mode() const
    {
        return _bPreferredAlternateOrUSB4Mode;
    }

    uint16_t UsbBillboardCapabilityDescriptor::VCONNPower() const
    {
        return _VCONNPower;
    }

    QByteArray UsbBillboardCapabilityDescriptor::bmConfigured() const
    {
        return _bmConfigured;
    }

    uint16_t UsbBillboardCapabilityDescriptor::bcdVersion() const
    {
        return _bcdVersion;
    }

    uint8_t UsbBillboardCapabilityDescriptor::bAdditionalFailureInfo() const
    {
        return _bAdditionalFailureInfo;
    }

    uint16_t UsbBillboardCapabilityDescriptor::wSVID(uint8_t index) const
    {
        if (index < _bNumberOfAlternateOrUSB4Modes)
            return _wSVID[index];
        else
        {
            LOGE(tr("Index %1 for wSVID out of range, must be 0~%2.")
                 .arg(index).arg(_bNumberOfAlternateOrUSB4Modes - 1));
            return 0;
        }
    }

    uint8_t UsbBillboardCapabilityDescriptor::bAlternateOrUSB4Mode(uint8_t index) const
    {
        if (index < _bNumberOfAlternateOrUSB4Modes)
            return _bAlternateOrUSB4Mode[index];
        else
        {
            LOGE(tr("Index %1 for bAlternateOrUSB4Mode out of range, must be 0~%2.")
                 .arg(index).arg(_bNumberOfAlternateOrUSB4Modes - 1));
            return 0;
        }
    }

    uint8_t UsbBillboardCapabilityDescriptor::iAlternateOrUSB4ModeString(uint8_t index) const
    {
        if (index < _bNumberOfAlternateOrUSB4Modes)
            return _iAlternateOrUSB4ModeString[index];
        else
        {
            LOGE(tr("Index %1 for iAlternateOrUSB4ModeString out of range, must be 0~%2.")
                 .arg(index).arg(_bNumberOfAlternateOrUSB4Modes - 1));
            return 0;
        }
    }

    QString UsbBillboardCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Billboard Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTRSTRDESC("iAddtionalInfoURL", _iAddtionalInfoURL, _bosDescriptor->device());
        ATTR("bNumberOfAlternateOrUSB4Modes",
             _bNumberOfAlternateOrUSB4Modes,
             _bNumberOfAlternateOrUSB4Modes);
        ATTR("bPreferredAlternateOrUSB4Mode",
             _bPreferredAlternateOrUSB4Mode,
             _bPreferredAlternateOrUSB4Mode);
        ATTR("VCONNPower", _VCONNPower, __parseVCONNPower());
        ATTRTEXT("bmConfigured", __parseBmConfigured());
        ATTR("bcdVersion", _bcdVersion, "");
        ATTR("bAdditionalFailureInfo", _bAdditionalFailureInfo, __parseBAdditionalFailureInfo());
        for (uint8_t i = 0; i < _bNumberOfAlternateOrUSB4Modes; ++i)
        {
#ifdef Q_OS_UNIX
            ATTR(QString("wSVID[%1]").arg(i), _wSVID[i], __getSVID(i));
#else
            ATTR(QString("wSVID[%1]").arg(i), _wSVID[i], "");
#endif
            ATTR(QString("bAlternateOrUSB4Mode[%1]").arg(i),
                 _bAlternateOrUSB4Mode[i],
                 _bAlternateOrUSB4Mode[i]);
            ATTRSTRDESC(QString("iAlternateOrUSB4ModeString[%1]").arg(i),
                        _iAlternateOrUSB4ModeString[i],
                        _bosDescriptor->device());
        }
        END;

        return html;
    }

    QString UsbBillboardCapabilityDescriptor::__parseVCONNPower() const
    {
        static const char *const power[] = {
            "1", "1.5", "2", "3", "4", "5", "6"
        };
        return BIT(_VCONNPower, 15) ?
                    tr("NOT require any VCONN Power"):
                    tr("%1 W").arg(power[_VCONNPower]);
    }

    QString UsbBillboardCapabilityDescriptor::__parseBmConfigured() const
    {
        QStringList aums;
        for (uint8_t i = 0; i < _bNumberOfAlternateOrUSB4Modes; ++i)
            aums.append(__parseBmConfigured(i).arg(i));

        return aums.join(NEWLINE);
    }

    QString UsbBillboardCapabilityDescriptor::__parseBmConfigured(uint8_t index) const
    {
        uint8_t byte = index / 4;
        uint8_t offsite = (index % 4) * 2;
        uint8_t curBmConfigured = _bmConfigured[byte];
        switch (CUT(curBmConfigured, offsite, offsite + 1))
        {
            case 0b00:
            return tr("AUM %1 unspecified Error");
            case 0b01:
            return tr("AUM %1 not attempted or exited");
            case 0b10:
            return tr("AUM %1 attempted but unsuccessful and not entered");
            case 0b11:
            return tr("AUM %1 successful");

        }

        return QString();
    }

    QString UsbBillboardCapabilityDescriptor::__parseBAdditionalFailureInfo() const
    {
        QStringList info;
        if (BIT(_bAdditionalFailureInfo, 0))
            info.append(tr("Lack of power"));
        if (BIT(_bAdditionalFailureInfo, 1))
            info.append(tr("No USB-PD communication"));

        return info.join(NEWLINE);
    }

#ifdef Q_OS_UNIX
    QString UsbBillboardCapabilityDescriptor::__getSVID(uint8_t index) const
    {

        /* Get vendor name */
        char vendorName[128];
        memset(vendorName, '\0', sizeof(vendorName));
        get_vendor_string(vendorName, sizeof(vendorName), _wSVID[index]);
        return QString(vendorName);
    }
#endif

    UsbBillboardAumCapabilityDescriptor::UsbBillboardAumCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _bIndex = *base->dev_capability_data;
        _dwAlternateModeVdo = *reinterpret_cast<const uint32_t *>(base->dev_capability_data + 1);
    }

    uint8_t UsbBillboardAumCapabilityDescriptor::bIndex() const
    {
        return _bIndex;
    }

    uint32_t UsbBillboardAumCapabilityDescriptor::dwAlternateModeVdo() const
    {
        return _dwAlternateModeVdo;
    }

    QString UsbBillboardAumCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Billboard AUM Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bIndex", _bIndex, _bIndex);
        ATTR("dwAlternateModeVdo", _dwAlternateModeVdo, _dwAlternateModeVdo);
        END;

        return html;
    }

    UsbAuthenticationCapabilityDescriptor::UsbAuthenticationCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {
        _bmAttributes = *base->dev_capability_data;
        _bcdProtocolVersion = *(base->dev_capability_data + 1);
        _bcdCapability = *(base->dev_capability_data + 2);
    }
    uint8_t UsbAuthenticationCapabilityDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    uint8_t UsbAuthenticationCapabilityDescriptor::bcdProtocolVersion() const
    {
        return _bcdProtocolVersion;
    }

    uint8_t UsbAuthenticationCapabilityDescriptor::bcdCapability() const
    {
        return _bcdCapability;
    }

    QString UsbAuthenticationCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Authentication Capability Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType));
        ATTR("bmAttributes", _bmAttributes, __parseBmAttributes());
        ATTR("bcdProtocolVersion", _bcdProtocolVersion, "");
        ATTR("bcdCapability", _bcdCapability, "");
        END;

        return html;
    }

    QString UsbAuthenticationCapabilityDescriptor::__parseBmAttributes() const
    {
        QStringList features;
        if (BIT(_bmAttributes, 0))
            features.append(tr("Firmware can be updated"));
        if (BIT(_bmAttributes, 1))
            features.append(tr("Device changes interfaces when updated"));

        return features.join(NEWLINE);
    }

}
