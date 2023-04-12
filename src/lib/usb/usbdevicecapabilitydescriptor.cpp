#include "usbdevicecapabilitydescriptor.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

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
            return QString("WIRELESS_USB");
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
            return QString("WIRELESS_USB_EXT");
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

    const QStringList &Usb20ExtensionDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bmAttributes",
        };

        return fields;
    }

    QString Usb20ExtensionDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: USB 2.0 EXTENSION"},
            {"bmAttributes", "<p>Bitmap encoding of supported device level "
             "features. A value of one in a bit location "
             "indicates a feature is supported; a value of "
             "zero indicates it is not supported. Encodings "
             "are:</p>"
             "<table><tr><th>Bit</th><th>Encoding</th></tr>"
             "<tr><td>0</td><td><b>Reserved</b>. Must be set to zero.</td></tr>"
             "<tr><td>1</td><td><b>LPM</b>. A value of one in this bit "
             "location indicates that this device "
             "supports the Link Power "
             "Management protocol.</td></tr>"
             "<tr><td>31:2</td><td><b>Reserved</b>. Must be set to zero.</td></tr></table>"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString Usb20ExtensionDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("USB 2.0 Extension Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bmAttributes", _bmAttributes, supportLPM() ?
                          tr("Link Power Management is supported"):
                          tr("Link Power Management is NOT supported"))
                .end()
                .build();
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
        _wSpeedsSupported(desc->wSpeedSupported), _bU2DevExitLat(desc->bU2DevExitLat)
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
        return _wSpeedsSupported;
    }

    uint16_t UsbSuperSpeedDeviceCapabilityDescriptor::bU2DevExitLat() const
    {
        return _bU2DevExitLat;
    }

    const QStringList &UsbSuperSpeedDeviceCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bmAttributes",
            "wSpeedsSupported",
            "bFunctionalitySupport",
            "bU1DevExitLat",
            "wU2DevExitLat",
        };

        return fields;
    }

    QString UsbSuperSpeedDeviceCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: SUPERSPEED_USB"},
            {"bmAttributes", "<p>Bitmap encoding of supported device level "
             "features. A value of one in a bit location "
             "indicates a feature is supported; a value of "
             "zero indicates it is not supported. Encodings "
             "are:</p>"
             "<table><tr><th>Bit</th><th>Encoding</th></tr>"
             "<tr><td>0</td><td>Reserved. Shall be set to zero.</td></tr>"
             "<tr><td>1</td><td>LTM Capable. A value of one in this "
             "bit location indicates that this "
             "device has is capable of generating "
             "Latency Tolerance Messages.</td></tr>"
             "<tr><td>31:2</td><td>Reserved. Shall be set to zero.</td></tr></table>"},
            {"wSpeedsSupported", "<p>Bitmap encoding of the speed supported by this device.</p>"
             "<table><tr><th>Bit</th><th>Encoding</th></tr>"
             "<tr><td>0</td><td>If this bit is set, then the device "
             "supports operation at low-Speed "
             "USB.</td></tr>"
             "<tr><td>1</td><td>If this bit is set, then the device "
             "supports operation at full-Speed "
             "USB.</td></tr>"
             "<tr><td>2</td><td>If this bit is set, then the device "
             "supports operation at high-Speed "
             "USB.</td></tr>"
             "<tr><td>3</td><td>If this bit is set, then the device "
             "supports operation at Gen 1 speed.</td></tr>"
             "<tr><td>15:4</td><td>Reserved. Shall be set to zero.</td></tr></table>"},
            {"bFunctionalitySupport", "<p>The lowest speed at which all the functionality "
             "supported by the device is available to the user. For "
             "example if the device supports all its functionality "
             "when connected at full speed and above then it sets "
             "this value to 1.</p>"
             "<p>Refer to the <i>wSpeedsSupported</i> field for valid values "
             "that can be placed in this field.</p>"},
            {"bU1DevExitLat", "<p>U1 Device Exit Latency. Worst-case latency to "
             "transition from U1 to U0, assuming the latency is "
             "limited only by the device and not the device’s link "
             "partner.</p>"
             "<p>This field applies only to the exit latency associated "
             "with an individual port, and does not apply to the "
             "total latency through a hub (e.g., from downstream "
             "port to upstream port).</p>"
             "<p>The following are permissible values:</p>"
             "<table><tr><th>Value</th><th>Meaning</th></tr>"
             "<tr><td>00H</td><td>Zero.</td></tr>"
             "<tr><td>01H</td><td>Less than 1 µs</td></tr>"
             "<tr><td>02H</td><td>Less than 2 µs</td></tr>"
             "<tr><td>03H</td><td>Less than 3 µs</td></tr>"
             "<tr><td>04H</td><td>Less than 4 µs</td></tr>"
             "<tr><td>...</td><td>...</td></tr>"
             "<tr><td>0AH</td><td>Less than 10 µs</td></tr>"
             "<tr><td>0BH-FFH</td><td>Reserved</td></tr></table>"
             "For a hub, this is the value for both its upstream and "
             "downstream ports."},
            {"wU2DevExitLat", "<p>U2 Device Exit Latency. Worst-case latency to "
             "transition from U2 to U0, assuming the latency is "
             "limited only by the device and not the device’s link "
             "partner. Applies to all ports on a device.</p>"
             "<p>The following are permissible values:</p>"
             "<table><tr><th>Value</th><th>Meaning</th></tr>"
             "<tr><td>0000H</td><td>Zero.</td></tr>"
             "<tr><td>0001H</td><td>Less than 1 µs</td></tr>"
             "<tr><td>0002H</td><td>Less than 2 µs</td></tr>"
             "<tr><td>0003H</td><td>Less than 3 µs</td></tr>"
             "<tr><td>0004H</td><td>Less than 4 µs</td></tr>"
             "<tr><td>...</td><td>...</td></tr>"
             "<tr><td>07FFH</td><td>Less than 2047 µs</td></tr>"
             "<tr><td>0800H-FFFFH</td><td>Reserved</td></tr></table>"
             "<p>For a hub, this is the value for both its upstream and "
             "downstream ports.</p>"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    bool UsbSuperSpeedDeviceCapabilityDescriptor::supportLTM() const
    {
        return _bmAttributes & LIBUSB_BM_LTM_SUPPORT;
    }

    QStringList UsbSuperSpeedDeviceCapabilityDescriptor::speedSupportedList() const
    {
        QStringList speeds;
        if (_wSpeedsSupported & LIBUSB_LOW_SPEED_OPERATION)
            speeds.append("Low Speed");
        if (_wSpeedsSupported & LIBUSB_FULL_SPEED_OPERATION)
            speeds.append("Full Speed");
        if (_wSpeedsSupported & LIBUSB_HIGH_SPEED_OPERATION)
            speeds.append("High Speed");
        if (_wSpeedsSupported & LIBUSB_SUPER_SPEED_OPERATION)
            speeds.append("SuperSpeed");

        return speeds;
    }

    QString UsbSuperSpeedDeviceCapabilityDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("SuperSpeed Device Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bmAttributes", _bmAttributes, supportLTM() ?
                          tr("Latency Tolerance Messages is supported"):
                          tr("Latency Tolerance Messages is NOT supported"))
                .attr("wSpeedsSupported", _wSpeedsSupported,
                      tr("Supports") + QString(" ") + speedSupportedList().join(", "))
                .attr("bFunctionalitySupport", _bFunctionalitySupport,
                      _bFunctionalitySupport ? tr("The lowest speed at all functionality is available"): "")
                .attr("bU1DevExitLat", _bU1DevExitLat, tr("Less than %1 µs").arg(_bU1DevExitLat))
                .attr("bU2DevExitLat", _bU2DevExitLat, tr("Less than %1 µs").arg(_bU2DevExitLat))
                .end()
                .build();
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

    const QStringList &UsbContainerIdDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "ContainerID",
        };

        return fields;
    }

    QString UsbContainerIdDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: CONTAINER_ID"},
            {"ContainerID", "<p>This is a 128-bit number that is unique to a device "
             "instance that is used to uniquely identify the device "
             "instance across all modes of operation. This same "
             "value may be provided over other technologies as "
             "well to allow the host to identify the device "
             "independent of means of connectivity.</p>"
             "<p>Refer to IETF RFC 4122 for details on generation of "
             "a UUID.</p>"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbContainerIdDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Container ID Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("ContainerID", "", hexUuid(_containerID))
                .end()
                .build();
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
        return UsbHtmlBuilder()
                .start(tr("Wireless USB Device Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bmAttributes", _bmAttributes, __parseBmAttributes())
                .attr("wPHYRates", _wPHYRates,
                      tr("Supports %1 Mbps").arg(__parseWPHYRateList().join(", ")))
                .attr("bmTFITXPowerInfo", _bmTFITXPowerInfo, __parsePowerInfo(_bmTFITXPowerInfo))
                .attr("bmFFITXPowerInfo", _bmFFITXPowerInfo, __parsePowerInfo(_bmFFITXPowerInfo))
                .attr("bmBandGroup", _bmBandGroup,
                      tr("All bands in band groups %1 are supported").arg(__parseBmBandGroupList().join(", ")))
                .build();
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

    const QStringList &UsbWirelessDeviceCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bmAttributes",
            "wPHYRates",
            "bmTFITXPowerInfo",
            "bmFFITXPowerInfo",
            "bmBandGroup",
        };

        return fields;
    }

    QString UsbWirelessDeviceCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of this descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: WIRELESS_USB"},
            {"bmAttributes", "<p>Bitmap encoding of supported device level features. "
             "A value of one in a bit location indicates a feature is "
             "supported; a value of zero indicates it is not "
             "supported. Encodings are:</p>"
             "<table><tr><th>Bit</th><th>Encoding</th></tr>"
             "<tr><td>0</td><td><b>Reserved</b>. Must be set to zero.</td></tr>"
             "<tr><td>1</td><td><b>P2P-DRD</b>. A value of one in this "
             "bit location indicates that this "
             "device is Peer to Peer DRD "
             "capable.</td></tr>"
             "<tr><td>3:2</td><td><p><b>Beacon Behavior</b>. This field "
             "encodes the beaconing behavior "
             "of the device. The encoded "
             "values are:</p>"
             "<table><tr><th>Value</th><th>Description</th></tr>"
             "<tr><td>00B</td><td>Reserved</td></tr>"
             "<tr><td>01B</td><td>Self-Beacon</td></tr>"
             "<tr><td>10B</td><td>Reserved and "
             "shouldn’t be used in "
             "future revisions</td></tr>"
             "<tr><td>11B</td><td>Reserved and "
             "shouldn’t be used in "
             "future revisions</td></tr></table>"
             "</td></tr>"
             "<tr><td>4</td><td><b>Power Indication</b>. A value of one "
             "in this bit indicates that this "
             "device supports Power Indication "
             "and power status probing "
             "function.</td></tr>"
             "<tr><td>7:5</td><td><b>Reserved</b>. Must be set to zero.</td></tr></table>"},
            {"wPHYRates", "<p>Describes the PHY-level signaling rate capabilities "
             "of this device implementation represented as a bit-"
             "mask. Bit positions are assigned to speed "
             "capabilities possible in a PHY implementation. A ‘1’ "
             "in a bit position indicates the associated data rate is "
             "supported by the device. Encodings are:</p>"
             "<table><tr><th>Bit</th><th>Data Rate (Mbps)</th></tr>"
             "<tr><td>0</td><td>53.3 **required</td></tr>"
             "<tr><td>1</td><td>80</td></tr>"
             "<tr><td>2</td><td>106.7 **required</td></tr>"
             "<tr><td>3</td><td>160</td></tr>"
             "<tr><td>4</td><td>200 **required</td></tr>"
             "<tr><td>5</td><td>320</td></tr>"
             "<tr><td>6</td><td>400</td></tr>"
             "<tr><td>7</td><td>480</td></tr>"
             "<tr><td>15:8</td><td>Reserved. Must be zero.</td></tr></table>"
             "<p>** Required encodings must be a one (1B).</p>"},
            {"bmTFITXPowerInfo", "<p>This bit mask reports the PHY transmit power levels "
             "supported by this device when transmitting on a TFI "
             "channel. See below for details.</p>"
             "<table><tr><th>Bit</th><th>Encoding</th></tr>"
             "<tr><td>3:0</td><td><b>Power Level Steps</b>. Specifies "
             "the number of steps from the "
             "base TFI transmit power level "
             "supported by the device.</td></tr>"
             "<tr><td>7:4</td><td><p><b>Step Size</b>. The value specifies "
             "the number of dB between "
             "supported power levels as "
             "follows:</p>"
             "<table><tr><th>Value</th><th>Step Size (dB)</th></tr>"
             "<tr><td>0</td><td>1.0</td></tr>"
             "<tr><td>1</td><td>1.25</td></tr>"
             "<tr><td>2</td><td>1.5</td></tr>"
             "<tr><td>3</td><td>1.75</td></tr>"
             "<tr><td>4</td><td>2.0</td></tr>"
             "<tr><td>5</td><td>2.25</td></tr>"
             "<tr><td>6</td><td>2.5</td></tr>"
             "<tr><td>7</td><td>2.75</td></tr>"
             "<tr><td>8</td><td>3.0</td></tr>"
             "<tr><td>9</td><td>3.25</td></tr>"
             "<tr><td>10</td><td>3.5</td></tr>"
             "<tr><td>11</td><td>3.75</td></tr>"
             "<tr><td>12</td><td>4.0</td></tr>"
             "<tr><td>13</td><td>4.25</td></tr>"
             "<tr><td>14</td><td>4.5</td></tr>"
             "<tr><td>15</td><td>4.75</td></tr></table></table>"},
            {"bmFFITXPowerInfo", "This bit mask reports the PHY transmit power levels "
             "supported by this device when transmitting on an "
             "FFI channel. The encoding of this field is identical to "
             "the <i>bmTFITXPowerInfo</i> field."},
            {"bmBandGroup", "This bit map reports which UWB band groups are "
             "supported by this implementation."},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
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
        return tr("Power Level Steps: %1").arg(steps) + UsbHtmlBuilder::NEWLINE +
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
        UsbHtmlBuilder builder;
        builder.start(tr("SuperSpeedPlus Device Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bmAttributes", _bmAttributes, __parseBmAttributes())
                .attr("wFunctionalitySupport", _wFunctionalitySupport, __parseFunctionalitySupport());
        for (unsigned int i = 0; i <= CUT(_bmAttributes, 0, 4); ++i)
            builder.attr("bmSublinkSpeedAttr",
                         sublinkSpeedAttr(i),
                         __parseSublinkSpeedAttr(i),
                         i);

        return builder.end().build();
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::__parseBmAttributes() const
    {
        int ssac = CUT(_bmAttributes, 0, 4);
        int ssic = CUT(_bmAttributes, 5, 8);
        return tr("%1 Sublink Speed Attributes").arg(ssac + 1) + UsbHtmlBuilder::NEWLINE +
                tr("%1 Sublink Speed IDs").arg(ssic + 1);
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::__parseFunctionalitySupport() const
    {
        int ssid = CUT(_wFunctionalitySupport, 0, 3);
        int minRxLaneCount = CUT(_wFunctionalitySupport, 8, 11);
        int minTxLaneCount = CUT(_wFunctionalitySupport, 12, 15);
        return tr("Sublink Speed Attribute ID: %1").arg(ssid) + UsbHtmlBuilder::NEWLINE +
                tr("Min Rx Lane Count: %1").arg(minRxLaneCount) + UsbHtmlBuilder::NEWLINE +
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

        return tr("Sublink Speed Attribute ID: %1").arg(ssid) + UsbHtmlBuilder::NEWLINE +
                tr("Lane Speed: %1 %2").arg(laneSpeedMantissa).arg(bitRate) + UsbHtmlBuilder::NEWLINE +
                tr("Sublink Type: %1, %2")
                .arg((sublinkType & 0b1) ? tr("Asymmetric") : tr("Symmetric"),
                     (sublinkType & 0b10) ? tr("Transmit mode") : tr("Receive mode")) + UsbHtmlBuilder::NEWLINE +
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

    const QStringList &UsbSuperSpeedPlusDeviceCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bmAttributes",
            "wFunctionalitySupport",
            "bmSublinkSpeedAttr",
        };

        return fields;
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: SUPERSPEED_PLUS"},
            {"bmAttributes", "<p>Bitmap encoding of supported SuperSpeedPlus features:</p>"
             "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>4:0</td><td><p><b>Sublink Speed Attribute Count</b> (SSAC). The "
             "number of Sublink Speed Attribute bitmaps. A "
             "SuperSpeedPlus device shall report at least "
             "one SSAC.</p>"
             "<p>The number of Sublink Speed Attributes = "
             "SSAC + 1.</p></td></tr>"
             "<tr><td>8:5</td><td><p><b>Sublink Speed ID Count</b> (SSIC). The number "
             "of unique Sublink Speed IDs supported by the "
             "device.</p>"
             "<p>The number of Sublink Speed IDs = SSIC + 1.</p></td></tr>"
             "<tr><td>31:9</td><td><b>Reserved</b>.</td></tr></table></table>"},
            {"wFunctionalitySupport", "<p>The device shall support full functionality at all "
             "reported bandwidths at or above the minimum "
             "bandwidth described via this field.</p>"
             "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>3:0</td><td><b>Sublink Speed Attribute ID</b> (SSID). This field "
             "indicates the minimum lane speed.</td></tr>"
             "<tr><td>7:4</td><td><b>Reserved</b>. Shall be set to zero.</td></tr>"
             "<tr><td>11:8</td><td><b>Min Rx Lane Count</b>. This field indicates the "
             "minimum receive lane count.</td></tr>"
             "<tr><td>15:12</td><td><b>Min Tx Lane Count</b>. This field indicates the "
             "minimum transmit lane count.</td></tr></table></table>"},
            {"bmSublinkSpeedAttr", "<p><b>Sublink Speed Attribute</b>. Bitmap encoding of a "
             "Sublink’s characteristics:</p>"
             "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>3:0</td><td><b>Sublink Speed Attribute ID</b> (SSID). This field "
             "is an ID that uniquely identifies the speed of "
             "the sublink. Note that a maximum of 16 "
             "unique SSIDs may be defined.</td></tr>"
             "<tr><td>5:4</td><td><p><b>Lane Speed Exponent</b> (LSE). This field "
             "defines the base 10 exponent times 3, that "
             "shall be applied to the Lane Speed Mantissa "
             "(LSM) when calculating the maximum bit rate "
             "represented by this Lane Speed Attribute.</p>"
             "<table><tr><th>LSE Value</th><th>Bit Rate</th></tr>"
             "<tr><td>0</td><td>Bits per second</td></tr>"
             "<tr><td>1</td><td>Kb/s</td></tr>"
             "<tr><td>2</td><td>Mb/s</td></tr>"
             "<tr><td>3</td><td>Gb/s</td></tr></table>"
             "</td></tr>"
             "<tr><td>7:6</td><td><p><b>Sublink Type (ST)</b>. This field identifies "
             "whether the Sublink Speed Attribute defines a "
             "symmetric or asymmetric bit rate. This field "
             "also indicates if this Sublink Speed Attribute "
             "defines the receive or transmit bit rate.</p>"
             "<p>Note that the Sublink Speed Attributes shall be "
             "paired, i.e. an Rx immediately followed by a Tx, "
             "and both Attributes shall define the same "
             "value for the SSID.</p>"
             "<table><tr><th>Bit</th><th>Value</th><th>Description</th></tr>"
             "<tr><td rowspan=\"2\">6</td><td>0</td><td>Symmetric. Rx and Tx "
             "Sublinks have the same "
             "number of lanes and operate "
             "at the same speed.</td></tr>"
             "<tr><td>1</td><td>Asymmetric. Rx and Tx "
             "Sublink have different "
             "number of lanes and/or "
             "operate at different speeds.</td></tr>"
             "<tr><td rowspan=\"2\">7</td><td>0</td><td>Sublink operates in Receive mode</td></tr>"
             "<tr><td>1</td><td>Sublink operates in Transmit mode</td></tr></table>"
             "</td></tr>"
             "<tr><td>13:8</td><td><b>Reserved</b>.</td></tr>"
             "<tr><td>15:14</td><td><p><b>Link Protocol</b> (LP). This field "
             "identifies the protocol supported by the link.</p>"
             "<table><tr><th>LP Value</th><th>Protocol</th></tr>"
             "<tr><td>0</td><td>SuperSpeed</td></tr>"
             "<tr><td>1</td><td>SuperSpeedPlus</td></tr>"
             "<tr><td>3-2</td><td>Reserved</td></tr></table>"
             "</td></tr>"
             "<tr><td>31:16</td><td><p><b>Lane Speed Mantissa</b> (LSM). This field "
             "defines the mantissa that shall be applied to "
             "the LSE when calculating the maximum bit rate "
             "represented by Lane Speed Attribute.</td></tr></table>"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
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

    const QStringList &UsbPlatformDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "PlatformCapabilityUUID",
            "CapabilityData",
        };

        return fields;
    }

    QString UsbPlatformDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: PLATFORM"},
            {"PlatformCapabilityUUID", "This is a 128-bit number that uniquely identifies "
             "a platform specific capability of the device."},
            {"CapabilityData", "This is a variable-length field containing data "
             "associated with the platform specific capability. "
             "This field may be zero bytes in length."},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbPlatformDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Platform Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("PlatformCapabilityUUID", "", hexUuid(_platformCapabilityUUID))
                .attr("CapabilityData", "", QString("0x") + _capabilityData.toHex())
                .end()
                .build();
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

    const QStringList &UsbPowerDeliveryCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bmAttributes",
            "bcdBCVersion",
            "bcdPDVersion",
            "bcdUSBTypeCVersion",
        };

        return fields;
    }

    QString UsbPowerDeliveryCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: POWER_DELIVERY_CAPABILITY"},
            {"bmAttributes", "<p>Bitmap encoding of supported device level features. A "
             "value of one in a bit location indicates a feature is "
             "supported; a value of zero indicates it is not supported. "
             "Encodings are:</p>"
             "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>0</td><td>Reserved. Shall be set to zero.</td></tr>"
             "<tr><td>1</td><td>Battery Charging. This bit Shall be set to one "
             "to indicate this device supports the Battery "
             "Charging Specification as per the value "
             "reported in the <i>bcdBCVersion</i> field.</td></tr>"
             "<tr><td>2</td><td>USB Power Delivery. This bit Shall be set to "
             "one to indicate this device supports the USB "
             "Power Delivery Specification as per the value "
             "reported in the <i>bcdPDVersion</i> field.</td></tr>"
             "<tr><td>3</td><td>Provider. This bit Shall be set to one to "
             "indicate this device is capable of providing "
             "power. This field is only Valid if Bit 2 is set to "
             "one.</td></tr>"
             "<tr><td>4</td><td>Consumer. This bit Shall be set to one to "
             "indicate that this device is a consumer of "
             "power. This field is only Valid if Bit 2 is set to "
             "one.</td></tr>"
             "<tr><td>5</td><td>This bit Shall be set to 1 to indicate that this "
             "device supports the feature "
             "<i>CHARGING_POLICY</i>. Note that supporting the "
             "<i>CHARGING_POLICY</i> feature does not require a "
             "BC or PD mechanism to be implemented.</td></tr>"
             "<tr><td>6</td><td>USB Type-C Current. This bit Shall be set to "
             "one to indicate this device supports power "
             "capabilities defined in the USB Type-C "
             "Specification as per the value reported in the "
             "bcdUSBTypeCVersion field.</td></tr>"
             "<tr><td>7</td><td>Reserved. Shall be set to zero.</td></tr>"
             "<tr><td>15:8</td><td><p>bmPowerSource. At least one of the following "
             "bits 8, 9 and 14 Shall be set to indicate which "
             "power sources are supported.</p>"
             "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>8</td><td>AC Supply</td></tr>"
             "<tr><td>9</td><td>Battery</td></tr>"
             "<tr><td>10</td><td>Other</td></tr>"
             "<tr><td>13:11</td><td>NumBatteries. This field Shall only be "
             "Valid when the Battery field is set to "
             "one and Shall be used to report the number "
             "of batteries in the device.</td></tr>"
             "<tr><td>14</td><td>Uses V<sub>BUS</sub></td></tr>"
             "<tr><td>15</td><td>Reserved and Shall be set to zero.</td></tr></table></table>"
             "</td></tr>"
             "<tr><td>31:16</td><td>Reserved and Shall be set to zero.</td></tr></table></table>"},
            {"bcdBCVersion", "Battery Charging Specification Release Number in Binary-"
             "Coded Decimal (e.g., V1.20 is 120H). This field Shall only "
             "be Valid if the device indicates that it supports BC in the "
             "<i>bmAttributes</i> field."},
            {"bcdPDVersion", "USB Power Delivery Specification Release Number in "
             "Binary-Coded Decimal. This field Shall only be Valid if "
             "the device indicates that it supports PD in the "
             "<i>bmAttributes</i> field."},
            {"bcdUSBTypeCVersion", "USB Type-C Specification Release Number in Binary-"
             "Coded Decimal. This field Shall only be Valid if the "
             "device indicates that it supports USB Type-C in the "
             "<i>bmAttributes</i> field."},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbPowerDeliveryCapabilityDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("USB Power Delivery Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bmAttributes", _bmAttributes, __parseBmAttributes())
                .attr("bcdBCVersion", _bcdBCVersion, "")
                .attr("bcdPDVersion", _bcdPDVersion, "")
                .attr("bcdUSBTypeCVersion", _bcdUSBTypeCVersion, "")
                .end()
                .build();
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

        return featuresList.join(UsbHtmlBuilder::NEWLINE);
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

    const QStringList &UsbBatteryInfoCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "iBattery",
            "iSerial",
            "iManufacturer",
            "bBatteryId",
            "dwChargedThreshold",
            "dwWeakThreshold",
            "dwBatteryDesignCapacity",
            "dwBatteryLastFullchargeCapacity",
        };

        return fields;
    }

    QString UsbBatteryInfoCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: BATTERY_INFO_CAPABILITY"},
            {"iBattery", "Index of string descriptor Shall contain the user-"
             "friendly name for this Battery."},
            {"iSerial", "Index of string descriptor Shall contain the Serial "
             "Number String for this Battery."},
            {"iManufacturer", "Index of string descriptor Shall contain the name of "
             "the Manufacturer for this Battery."},
            {"bBatteryId", "Value Shall be used to uniquely identify this Battery "
             "in status Messages."},
            {"dwChargedThreshold", "Shall contain the Battery Charge value above which "
             "this Battery is considered to be fully charged but not "
             "necessarily \"topped off.\""},
            {"dwWeakThreshold", "Shall contain the minimum charge level of this "
             "Battery such that above this threshold, a device can "
             "be assured of being able to power up successfully "
             "(see Battery Charging 1.2)."},
            {"dwBatteryDesignCapacity", "Shall contain the design capacity of the Battery."},
            {"dwBatteryLastFullchargeCapacity", "Shall contain the maximum capacity of the Battery "
             "when fully charged."},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbBatteryInfoCapabilityDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Battery Info Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .strdesc("iBattery", _iBattery, _bosDescriptor->device())
                .strdesc("iSerial", _iSerial, _bosDescriptor->device())
                .strdesc("iManufacturer", _iManufacturer, _bosDescriptor->device())
                .attr("bBatteryId", _bBatteryId)
                .attr("dwChargedThreshold", _dwChargedThreshold, tr("%1 mWh").arg(_dwChargedThreshold))
                .attr("dwWeakThreshold", _dwWeakThreshold, tr("%1 mWh").arg(_dwWeakThreshold))
                .attr("dwBatteryDesignCapacity", _dwBatteryDesignCapacity,
                      tr("%1 mWh").arg(_dwBatteryDesignCapacity))
                .attr("dwBatteryLastFullchargeCapacity", _dwBatteryLastFullchargeCapacity,
                      tr("%1 mWh").arg(_dwBatteryLastFullchargeCapacity))
                .end()
                .build();
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

    const QStringList &UsbPowerDeliveryConsumerPortCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bmCapabilities",
            "wMinVoltage",
            "wMaxVoltage",
            "dwMaxOperatingPower",
            "dwMaxPeakPower",
            "dwMaxPeakPowerTime",
        };

        return fields;
    }

    QString UsbPowerDeliveryConsumerPortCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: PD_CONSUMER_PORT_CAPABILITY"},
            {"bmCapabilities", "<p>Capability: This field Shall indicate the specification "
             "the Consumer Port will operate under.</p>"
             "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>0</td><td>Battery Charging (BC)</td></tr>"
             "<tr><td>1</td><td>USB Power Delivery (PD)</td></tr>"
             "<tr><td>2</td><td>USB Type-C Current</td></tr>"
             "<tr><td>15:3</td><td>Reserved and Shall be set to zero</td></tr></table>"},
            {"wMinVoltage", "Shall contain the minimum Voltage in 50mV units "
             "that this Consumer is capable of operating at."},
            {"wMaxVoltage", "Shall contain the maximum Voltage in 50mV units "
             "that this Consumer is capable of operating at."},
            {"dwMaxOperatingPower", "Shall contain the maximum power in 10mW units "
             "this Consumer can draw when it is in a steady state "
             "operating mode."},
            {"dwMaxPeakPower", "Shall contain the maximum power in 10mW units "
             "this Consumer can draw for a short duration of time "
             "(<i>dwMaxPeakPowerTime</i>) before it falls back into a "
             "steady state."},
            {"dwMaxPeakPowerTime", "<p>Shall contain the time in 100ms units that this "
             "Consumer can draw peak current.</p>"
             "<p>A device Shall set this field to 0xFFFF if this value is unknown.</p>"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbPowerDeliveryConsumerPortCapabilityDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("PD Consumer Port Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bmCapabilities", _bmCapabilities, __parseBmCapabilities())
                .attr("wMinVoltage", _wMinVoltage, tr("%1 mV").arg(_wMinVoltage * 50))
                .attr("wMaxVoltage", _wMaxVoltage, tr("%1 mV").arg(_wMaxVoltage * 50))
                .attr("dwMaxOperatingPower", _dwMaxOperatingPower,
                      tr("%1 mW").arg(_dwMaxOperatingPower * 10))
                .attr("dwMaxPeakPower", _dwMaxPeakPower,
                      tr("%1 mW").arg(_dwMaxPeakPower * 10))
                .attr("dwMaxPeakPowerTime", _dwMaxPeakPowerTime,
                      tr("%1 ms").arg(_dwMaxPeakPowerTime * 100))
                .end()
                .build();
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
        return capabilitiesList.join(UsbHtmlBuilder::NEWLINE);
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

    const QStringList &UsbPowerDeliveryProviderPortCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bmCapabilities",
            "bNumOfPDObjects",
            "wPowerDataObject",
        };

        return fields;
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: PD_PROVIDER_PORT_CAPABILITY"},
            {"bmCapabilities", "<p>Capability: This field Shall indicate the specification "
             "the Provider Port will operate under.</p>"
             "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>0</td><td>Battery Charging (BC)</td></tr>"
             "<tr><td>1</td><td>USB Power Delivery (PD)</td></tr>"
             "<tr><td>2</td><td>USB Type-C Current</td></tr>"
             "<tr><td>15:3</td><td>Reserved. Shall be set to zero.</td></tr></table>"},
            {"bNumOfPDObjects", "Shall indicate the number of Power Data Objects."},
            {"wPowerDataObject", "Shall contain the Nth Power Data Object supported "
             "by this Provider Port. See Section 6.4.1 for details of "
             "the Power Data Objects."},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::infomationToHtml() const
    {
        UsbHtmlBuilder builder;
        builder.start(tr("PD Provider Port Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bmCapabilities", _bmCapabilities, __parseBmCapabilities());
        for (unsigned int i = 0; i <= _bNumOfPDObjects; ++i)
            builder.attr("wPowerDataObject",
                         wPowerDataObject(i),
                         __parsePowerDataObject(wPowerDataObject(i)),
                         i);

        return builder.end().build();
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
        return capabilitiesList.join(UsbHtmlBuilder::NEWLINE);
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

        return featuresList.join(UsbHtmlBuilder::NEWLINE);
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::__parseBatterySupplyPDO(uint32_t object) const
    {
        QStringList featuresList;
        featuresList.push_back(tr("Battery Supply"));
        featuresList.push_back(tr("Maximum Voltage: %1 mV").arg(CUT(object, 20, 29) * 50));
        featuresList.push_back(tr("Minimum Voltage: %1 mV").arg(CUT(object, 10, 19) * 50));
        featuresList.push_back(tr("Maximum Allowable Power: %1 mW").arg(CUT(object, 0, 9) * 250));

        return featuresList.join(UsbHtmlBuilder::NEWLINE);
    }

    QString UsbPowerDeliveryProviderPortCapabilityDescriptor::__parseVariableSupplyPDO(uint32_t object) const
    {
        QStringList featuresList;
        featuresList.push_back(tr("Variable Supply (non-Battery)"));
        featuresList.push_back(tr("Maximum Voltage: %1 mV").arg(CUT(object, 20, 29) * 50));
        featuresList.push_back(tr("Minimum Voltage: %1 mV").arg(CUT(object, 10, 19) * 50));
        featuresList.push_back(tr("Maximum Current: %1 mA").arg(CUT(object, 0, 9) * 10));

        return featuresList.join(UsbHtmlBuilder::NEWLINE);
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

        return featuresList.join(UsbHtmlBuilder::NEWLINE);
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

        return featuresList.join(UsbHtmlBuilder::NEWLINE);
    }

    UsbPrecisionTimeMeasurementCapabilityDescriptor::UsbPrecisionTimeMeasurementCapabilityDescriptor(
            const libusb_bos_dev_capability_descriptor *base,
            UsbBosDescriptor *parent):
        UsbDeviceCapabilityDescriptor(base, parent)
    {

    }

    const QStringList &UsbPrecisionTimeMeasurementCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
        };

        return fields;
    }

    QString UsbPrecisionTimeMeasurementCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: PRECISION_TIME_MEASUREMENT"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbPrecisionTimeMeasurementCapabilityDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Precision Time Measurement Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .end()
                .build();
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

    const QStringList &UsbExtendedWirelessDeviceCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "wIdleTimeout",
            "wWakeUpLatency",
            "bmControl",
        };

        return fields;
    }

    QString UsbExtendedWirelessDeviceCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: WIRELESS_USB_EXT."},
            {"wIdleTimeout", "Time interval, in units of milliseconds, from "
             "the instant there are no active transfers to a "
             "device until the host decides to send the "
             "Device Sleep IE to the device."},
            {"wWakeUpLatency", "Time interval, in units of milliseconds, during "
             "which the device prefers to remain in the "
             "Sleep state, if directed to sleep by the host."},
            {"bmControl", "<table><tr><th>Bit</th><th>Encoding</th></tr>"
             "<tr><td>0</td><td><p>A value of 0 indicates that the "
             "host should strictly use the device provided values of "
             "wIdleTimeout and wWakeUpLatency when "
             "directing a device to sleep.</p>"
             "<p>A value of 1 indicates that the "
             "host is free to choose when to direct a device to transition to "
             "the sleep state and how long the device should remain in the "
             "sleep state.</p></td></tr>"
             "<tr><td>7:1</td><td>Reserved.</td></tr></table>"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbExtendedWirelessDeviceCapabilityDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Extended Wireless Device Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("wIdleTimeout", _wIdleTimeout, tr("%1 ms").arg(_wIdleTimeout))
                .attr("wWakeUpLatency", _wWakeUpLatency, tr("%1 ms").arg(_wWakeUpLatency))
                .attr("bmControl", _bmControl, _bmControl ? tr("Free") : tr("Strict"))
                .end()
                .build();
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

    const QStringList &UsbConfigurationSummaryDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bcdVersion",
            "bClass",
            "bSubClass",
            "bProtocol",
            "bConfigurationCount",
            "bConfigurationIndex",
        };

        return fields;
    }

    QString UsbConfigurationSummaryDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: CONFIGURATION_SUMMARY"},
            {"bcdVersion", "0100H, the revision of the Configuration Summary "
             "Descriptor with this document"},
            {"bClass", "Class code of the function"},
            {"bSubClass", "Subclass code of the function"},
            {"bProtocol", "Protocol of the function"},
            {"bConfigurationCount", "Number of configurations (N) that include this "
             "class/subclass/protocol"},
            {"bConfigurationIndex", "Nth configuration descriptor index for a "
             "configuration containing this class/subclass/protocol"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbConfigurationSummaryDescriptor::infomationToHtml() const
    {
        UsbHtmlBuilder builder;
        builder.start(tr("Configuration Summary Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bcdVersion", _bcdVersion, "")
                .attr("bClass", _bClass)
                .attr("bSubClass", _bSubClass)
                .attr("bProtocol", _bProtocol)
                .attr("bConfigurationCount", _bConfigurationCount);
        for (uint8_t i = 0; i < _bConfigurationCount; ++i)
            builder.attr("bConfigurationIndex", bConfigurationIndex(i), i);

        return builder.end().build();
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

    const QStringList &UsbBillboardCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "iAddtionalInfoURL",
            "bNumberOfAlternateOrUSB4Modes",
            "bPreferredAlternateOrUSB4Mode",
            "VCONNPower",
            "bmConfigured",
            "bcdVersion",
            "bAdditionalFailureInfo",
            "wSVID",
            "bAlternateOrUSB4Mode",
            "iAlternateOrUSB4ModeString",
        };

        return fields;
    }

    QString UsbBillboardCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: BILLBOARD"},
            {"iAddtionalInfoURL", "Index of string descriptor providing a URL "
             "where the user can go to get more detailed "
             "information about the product and the "
             "various AUM it supports."},
            {"bNumberOfAlternateOrUSB4Modes", "Number of AUM supported. The maximum "
             "value that this field can be set to is "
             "MAX_NUM_ALT_OR_USB4_MODE."},
            {"bPreferredAlternateOrUSB4Mode", "<p>Index of the preferred AUM.</p>"
             "<p>If USB4 mode is supported, it shall be the "
             "preferred mode and this field shall be set to "
             "0x0 (for version 1.2.2 of this specification). "
             "This field shall be set to 0xFF to indicate no "
             "preference.</p>"
             "<p>System software may use this information "
             "to provide the user with a better user "
             "experience.</p>"},
            {"VCONNPower", "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>2..0</td><td><p>V<sub>CONN</sub> Power needed by the adapter for "
             "full functionality</p>"
             "<table><tr><td>000b = 1W</td></tr>"
             "<tr><td>001b = 1.5W</td></tr>"
             "<tr><td>010b = 2W</td></tr>"
             "<tr><td>011b = 3W</td></tr>"
             "<tr><td>100b = 4W</td></tr>"
             "<tr><td>101b = 5W</td></tr>"
             "<tr><td>110b = 6W</td></tr>"
             "<tr><td>111b = reserved</td></tr></table></td></tr>"
             "<tr><td>14..3</td><td>Reserved for future use, shall be set to zero.</td></tr>"
             "<tr><td>15</td><td>The adapter does not require any V<sub>CONN</sub> "
             "Power. The value in Bits 2..0 are "
             "ignored when this bit is set.</td></tr></table>"},
            {"bmConfigured", "<p>Each bit pair in this field indicates the state "
             "of the Alternate Modes identified by the "
             "combination of <i>wSVID[i]</i> and "
             "<i>bAlternateOrUSB4Mode[i]</i>. A Device "
             "Container may support a maximum of "
             "MAX_NUM_ALT_OR_USB4_MODE AUM.</p>"
             "<p>System software will determine what to "
             "display, if anything, based on the value of "
             "the bits in this field.</p>"
             "<table><tr><th>Value</th><th>Description</th></tr>"
             "<tr><td>00b</td><td>Unspecified Error</td></tr>"
             "<tr><td>01b</td><td>AUM configuration not attempted or exited</td></tr>"
             "<tr><td>10b</td><td>AUM configuration attempted but unsuccessful and not entered</td></tr>"
             "<tr><td>11b</td><td>AUM configuration successful</td></tr></table>"
             "<p>This bitmap corresponds to the AUM as "
             "follows:</p>"
             "<table><tr><td>Bit 0:</td><td>First AUM defined (at Offset 44)</td></tr>"
             "<tr><td>Bit 2:</td><td>Second AUM defined (at Offset 48)</td></tr>"
             "<tr><td>...</td><td></td></tr>"
             "<tr><td>Bit (n -1) * 2:</td><td>Nth AUM defined</td></tr></table>"},
            {"bcdVersion", "<p>Billboard Capability version number in "
             "Binary-Coded Decimal (e.g., 1 .10 is "
             "0x0110H). This field identifies the release "
             "of the Billboard Specification with which "
             "the Billboard descriptors are compliant.</p>"
             "<p>All Device Containers compliant to this "
             "version of the specification shall set the "
             "value to 0x0122H.</p>"
             "<p>A value of 0x0000H indicates that the "
             "device was built to the first version of this "
             "specification.</p>"
             "<p>A value of 0x0120H in this field is reserved.</p>"},
            {"bAdditionalFailureInfo", "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>0</td><td>If this field is set to one then the Device "
             "Container failed due to lack of power.</td></tr>"
             "<tr><td>1</td><td><p>If this field is set to one then the Device "
             "Container failed due to no USB-PD communication.</p>"
             "<p>This field is only valid if <i>bmConfigured</i> "
             "field for the preferred AUM is not set to 11b.</p></td></tr>"
             "<tr><td>7:2</td><td>Reserved for future use, shall be set to "
             "zero.</td></tr></table>"},
            {"wSVID", "Standard or Vendor ID. This shall match one	"
             "of the SVIDs returned in response to a USB "
             "PD Discover SVIDs command. 0xFF00 shall "
             "be used as the SVID for USB4 devices and "
             "used in wSVID[0] location when the USB PD "
             "Discover Identity response includes USB4 "
             "Device Capable in UFP VDO1. If there is "
             "more than one AUM in the same SVID, the "
             "wSVID[n] should be repeated for each AUM."},
            {"bAlternateOrUSB4Mode", "<p>Index of the AUM within the SVID as "
             "returned in response to a Discover Modes "
             "command. Example: 0 – first Mode entry, 1 – "
             "second mode entry.</p>"
             "<p>With respect to USB4 when wSVID[0] is set "
             "to 0xFF00, this field shall be set as 0x00 "
             "allowing for new revisions to be set from "
             "0x01 to 0xFF.</p>"},
            {"iAlternateOrUSB4ModeString", "Index of string descriptor describing "
             "protocol. It is optional to support this string."},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbBillboardCapabilityDescriptor::infomationToHtml() const
    {
        UsbHtmlBuilder builder;
        builder.start(tr("Billboard Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .strdesc("iAddtionalInfoURL", _iAddtionalInfoURL, _bosDescriptor->device())
                .attr("bNumberOfAlternateOrUSB4Modes", _bNumberOfAlternateOrUSB4Modes)
                .attr("bPreferredAlternateOrUSB4Mode", _bPreferredAlternateOrUSB4Mode)
                .attr("VCONNPower", _VCONNPower, __parseVCONNPower())
                .attr("bmConfigured", "", __parseBmConfigured())
                .attr("bcdVersion", _bcdVersion, "")
                .attr("bAdditionalFailureInfo", _bAdditionalFailureInfo, __parseBAdditionalFailureInfo());
        for (uint8_t i = 0; i < _bNumberOfAlternateOrUSB4Modes; ++i)
        {
            builder.attr("wSVID", _wSVID[i],
             #ifdef Q_OS_UNIX
                         __getSVID(i),
             #else
                         "",
             #endif
                         i)
                    .attr("bAlternateOrUSB4Mode", _bAlternateOrUSB4Mode[i], i)
                    .strdesc("iAlternateOrUSB4ModeString",
                             _iAlternateOrUSB4ModeString[i],
                             _bosDescriptor->device(),
                             i);
        }

        return builder.end().build();
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

        return aums.join(UsbHtmlBuilder::NEWLINE);
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

        return info.join(UsbHtmlBuilder::NEWLINE);
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

    const QStringList &UsbBillboardAumCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bIndex",
            "dwAlternateModeVdo",
        };

        return fields;
    }

    QString UsbBillboardAumCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: BILLBOARD_AUM"},
            {"bIndex", "Index at which the AUM appears in the array "
             "of Alternate Modes described in the Billboard "
             "Capability Descriptor."},
            {"dwAlternateModeVdo", "<p>Contents of the Mode VDO for the AUM "
             "identified by bIndex.</p>"
             "<p>Refer to the Standard or Vendor specification "
             "for details on information contained in the "
             "Mode VDO. For USB4, the EUDO sent by the "
             "DFP port partner shall be reflected here.</p>"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbBillboardAumCapabilityDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Billboard AUM Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bIndex", _bIndex)
                .attr("dwAlternateModeVdo", _dwAlternateModeVdo)
                .end()
                .build();
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

    const QStringList &UsbAuthenticationCapabilityDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bDevCapabilityType",
            "bmAttributes",
            "bcdProtocolVersion",
            "bcdCapability",
        };

        return fields;
    }

    QString UsbAuthenticationCapabilityDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of descriptor"},
            {"bDescriptorType", "Descriptor type: DEVICE CAPABILITY Type"},
            {"bDevCapabilityType", "Capability type: AUTHENTICATION"},
            {"bmAttributes", "<p>Bitmap encoding of supported features.</p>"
             "<table><tr><th>Bit</th><th>Description</th></tr>"
             "<tr><td>0</td><td>Shall be set to 1 to indicate that "
             "firmware can be updated. Otherwise, "
             "shall be set to zero.</td></tr>"
             "<tr><td>1</td><td>Shall be set to 1 to indicate that "
             "Device changes interfaces when "
             "updated. Otherwise, shall be set to zero.</td></tr>"
             "<tr><td>7:2</td><td>Reserved. Shall be set to zero.</td></tr></table>"},
            {"bcdProtocolVersion", "Shall be set to the USB Type-C Authentication "
             "Protocol Version"},
            {"bcdCapability", "Shall be set to the same value as <i>Param1</i> in a "
             "<b>DIGESTS</b> Authentication Response"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbAuthenticationCapabilityDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Authentication Capability Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE_CAPABILITY")
                .attr("bDevCapabilityType", _bDevCapabilityType, strType(_bDevCapabilityType))
                .attr("bmAttributes", _bmAttributes, __parseBmAttributes())
                .attr("bcdProtocolVersion", _bcdProtocolVersion, "")
                .attr("bcdCapability", _bcdCapability, "")
                .end()
                .build();
    }

    QString UsbAuthenticationCapabilityDescriptor::__parseBmAttributes() const
    {
        QStringList features;
        if (BIT(_bmAttributes, 0))
            features.append(tr("Firmware can be updated"));
        if (BIT(_bmAttributes, 1))
            features.append(tr("Device changes interfaces when updated"));

        return features.join(UsbHtmlBuilder::NEWLINE);
    }

}
