#include "usbdevicecapabilitydescriptor.h"

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

#define START(_title) do { html += QString("<h2 align='center'>%1</h2><table width=\"100%\">").arg(tr(_title)); } while (0)
#define ATTR(_name, _hex, _text) do { \
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>") \
    .arg(_name).arg(_hex, sizeof(_hex) * 2, 16, QChar('0')).arg(_text); \
} while(0)
#define ATTRTEXT(_name, _text) do {\
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\"></td><td>%2</td></tr>").arg(_name).arg(_text);\
} while(0)
#define ATTRSTRDESC(_name, _strDescInd) do { \
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>") \
    .arg(_name) \
    .arg(_strDescInd, 2, 16, QChar('0')) \
    .arg(getStringDescriptor(_strDescInd)); \
} while(0)
#define END do { html += QString("</table>"); } while(0)
#define NEWLINE QString("<br />")

    QString Usb20ExtensionDescriptor::infomationToHtml() const
    {
        QString html;
        START("USB 2.0 Extension Descriptor");
        ATTR("bLength", _bLength, _bLength);
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
            speeds.append("Super Speed");

        return speeds;
    }

    QString UsbSuperSpeedDeviceCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        START("USB Super Speed Device Capability Descriptor");
        ATTR("bLength", _bLength, _bLength);
        ATTR("bmAttributes", _bmAttributes, supportLTM() ?
                 tr("Latency Tolerance Messages is supported"):
                 tr("Latency Tolerance Messages is NOT supported"));
        ATTR("wSpeedSupported", _wSpeedSupported,
             tr("Supports") + QString(" ") + speedSupportedList().join(", "));
        ATTR("bFunctionalitySupport", _bFunctionalitySupport,
             _bFunctionalitySupport ? tr("The lowest speed at all functionality is available"): QString(""));
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
        START("USB Container ID Descriptor");
        ATTR("bLength", _bLength, _bLength);
        ATTRTEXT("ContainerID ", _containerID.toHex());
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

    }

    QString UsbWirelessDeviceCapabilityDescriptor::infomationToHtml() const
    {
        QString html;
        html += QString("<h2 align='center'>%1</h2>").arg("Wireless USB Device Capability Descriptor");
        html += QString("<p><i>%1</i></p>")
                .arg(tr("Wireless USB device capability descriptor is NOT supported by libusb"));
        return html;
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
        START("SuperSpeed Plus Device Capability Descriptor");
        ATTR("bLength", _bLength, _bLength);
        ATTR("bmAttributes", _bmAttributes, __parseBmAttributes());
        ATTR("wFunctionalitySupport", _wFunctionalitySupport, __parseFunctionalitySupport());
        for (unsigned int i = 0; i <= (_bmAttributes & 0b11111); ++i)
            ATTR(QString("bmSublinkSpeedAttr[%1]").arg(i),
                 sublinkSpeedAttr(i),
                 __parseSublinkSpeedAttr(i));
        END;

        return html;
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::__parseBmAttributes() const
    {
        int ssac = _bmAttributes & 0b11111;
        int ssic = (_bmAttributes & 0b111100000) >> 5;
        return tr("%1 Sublink Speed Attributes").arg(ssac + 1) + NEWLINE +
                tr("%1 Sublink Speed IDs").arg(ssic + 1);
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::__parseFunctionalitySupport() const
    {
        int ssid = _wFunctionalitySupport & 0b111;
        int minRxLaneCount = (_wFunctionalitySupport & 0x0F00) >> 8;
        int minTxLaneCount = (_wFunctionalitySupport & 0xF000) >> 12;
        return tr("Sublink Speed Attribute ID: %1").arg(ssid) + NEWLINE +
                tr("Min Rx Lane Count: %1").arg(minRxLaneCount) + NEWLINE +
                tr("Min Tx Lane Count: %1").arg(minTxLaneCount);
    }

    QString UsbSuperSpeedPlusDeviceCapabilityDescriptor::__parseSublinkSpeedAttr(size_t index) const
    {
        uint32_t attr = sublinkSpeedAttr(index);
        int ssid = attr & 0xF;
        int laneSpeedExponent = (attr & 0b110000) >> 4;
        int sublinkType = (attr & 0b11000000) >> 6;
        int linkProtocol = (attr & 0b1100000000000000) >> 14;
        int laneSpeedMantissa = (attr & 0xFFFF0000) >> 16;

        QString bitRate;
        switch (laneSpeedExponent)
        {
            case 0:
                bitRate = "b/s";
            break;
            case 1:
                bitRate = "Kb/s";
            break;
            case 2:
                bitRate = "Mb/s";
            break;
            case 3:
                bitRate = "Gb/s";
            break;
        }

        return tr("Sublink Speed Attribute ID: %1").arg(ssid) + NEWLINE +
                tr("Lane Speed: %1 %2").arg(laneSpeedMantissa).arg(bitRate) + NEWLINE +
                tr("Sublink Type: %1, %2")
                .arg((sublinkType & 0b1) ? tr("Asymmetric") : tr("Symmetric"))
                .arg((sublinkType & 0b10) ? tr("Transmit mode") : tr("Receive mode")) + NEWLINE +
                tr("Link Protocol: %1").arg(linkProtocol ? tr("SuperSpeedPlus") : tr("SuperSpeed"));
    }

    uint16_t UsbSuperSpeedPlusDeviceCapabilityDescriptor::wFunctionalitySupport() const
    {
        return _wFunctionalitySupport;
    }

    uint32_t UsbSuperSpeedPlusDeviceCapabilityDescriptor::sublinkSpeedAttr(size_t index) const
    {
        if (index > (_bmAttributes & 0b11111) + 1)
        {
            LOGE(tr("Index %1 out of range with Sublink Speed Attribute count %2.")
                 .arg(index).arg((_bmAttributes & 0b11111) + 1));
            return 0;
        }

        return _bmSublinkSpeedAttr[index];
    }

    uint32_t UsbSuperSpeedPlusDeviceCapabilityDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }
}
