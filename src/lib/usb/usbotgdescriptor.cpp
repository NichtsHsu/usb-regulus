#include "usbotgdescriptor.h"
#include "__usbmacro.h"

namespace usb {
    UsbOtgDescriptor::UsbOtgDescriptor(UsbConfigurationDescriptor *configDesc):
        UsbConfigurationExtraDescriptor(configDesc)
    {

    }

    void UsbOtgDescriptor::__initWithBuffer(const uint8_t *buffer)
    {
        _bLength = buffer[0];
        _bDescriptorType = buffer[1];
        _bmAttributes = buffer[2];
        _bcdOTG = *reinterpret_cast<const uint16_t *>(buffer + 3);
    }

    QString UsbOtgDescriptor::__parseBmAttributes() const
    {
        QStringList features;
        if (BIT(_bmAttributes, 0))
            features.append(tr("SRP support"));
        if (BIT(_bmAttributes, 1))
            features.append(tr("HNP support"));
        if (BIT(_bmAttributes, 2))
            features.append(tr("ADP support"));
        if (BIT(_bmAttributes, 3))
            features.append(tr("RSP support"));

        return features.join(NEWLINE);
    }

    uint16_t UsbOtgDescriptor::bcdOTG() const
    {
        return _bcdOTG;
    }

    uint8_t UsbOtgDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    uint8_t UsbOtgDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbOtgDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    ConfigurationExtraDescriptorType UsbOtgDescriptor::type() const
    {
        return ConfigurationExtraDescriptorType::OTG;
    }

    QString UsbOtgDescriptor::infomationToHtml() const
    {
        QString html;
        START("OTG Descriptor");
        ATTR("bLength", _bLength, _bLength);
        ATTR("bmAttributes", _bmAttributes, __parseBmAttributes());
        ATTR("bcdOTG", _bcdOTG, "");
        END;

        return html;
    }
} // namespace usb
