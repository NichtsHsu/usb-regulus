#include "usbotgdescriptor.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

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

        return features.join(UsbHtmlBuilder::NEWLINE);
    }

    uint16_t UsbOtgDescriptor::bcdOTG() const
    {
        return _bcdOTG;
    }

    const QStringList &UsbOtgDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bmAttributes",
            "bcdOTG",
        };

        return fields;
    }

    QString UsbOtgDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of Descriptor"},
            {"bDescriptorType", "Descriptor Type: OTG type"},
            {"bmAttributes", "<p>Attribute Fields.</p>"
             "<table><tr><td>D7…4:</td><td>Reserved (reset to zero)</td></tr>"
             "<tr><td>D3:</td><td>RSP support</td></tr>"
             "<tr><td>D2:</td><td>ADP support</td></tr>"
             "<tr><td>D1:</td><td>HNP support</td></tr>"
             "<tr><td>D0:</td><td>SRP support</td></tr></table>"
            },
            {"bcdOTG", "This field contains a BCD version "
             "number that identifies the release of "
             "the OTG and EH supplement with "
             "which the device corresponds to."},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
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
        return UsbHtmlBuilder()
                .start(tr("OTG Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "OTG")
                .attr("bmAttributes", _bmAttributes, __parseBmAttributes())
                .attr("bcdOTG", _bcdOTG, "")
                .end()
                .build();
    }
} // namespace usb
