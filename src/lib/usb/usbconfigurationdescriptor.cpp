#include "usbconfigurationdescriptor.h"
#include "usbinterfaceassociationdescriptor.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

namespace usb {
    UsbConfigurationDescriptor::UsbConfigurationDescriptor(const libusb_config_descriptor *desc, UsbDevice *parent):
        QObject(parent),
        _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType), _bNumInterfaces(desc->bNumInterfaces),
        _bConfigurationValue(desc->bConfigurationValue), _iConfiguration(desc->iConfiguration),
        _bmAttributes(desc->bmAttributes), _MaxPower(desc->MaxPower), _wTotalLength(desc->wTotalLength),
        _extra(desc->extra), _extra_length(desc->extra_length), _device(parent)
    {
        _interfaces.reserve(_bNumInterfaces);
        for (uint8_t i = 0; i < _bNumInterfaces; ++i)
            _interfaces.append(new UsbInterface(&desc->interface[i], this));
        uint8_t len = _extra_length;
        uint8_t pos = 0;
        while (len > 1)
        {
            if (_extra[pos] > len)
                break;
            UsbConfigurationExtraDescriptor *extraDesc = UsbConfigurationExtraDescriptor::get(this, pos);
            len -= _extra[pos];
            pos += _extra[pos];
            addConfigurationExtraDescriptor(extraDesc);
        }
    }

    uint8_t UsbConfigurationDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbConfigurationDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint16_t UsbConfigurationDescriptor::wTotalLength() const
    {
        return _wTotalLength;
    }

    uint8_t UsbConfigurationDescriptor::bNumInterfaces() const
    {
        return _bNumInterfaces;
    }

    uint8_t UsbConfigurationDescriptor::bConfigurationValue() const
    {
        return _bConfigurationValue;
    }

    uint8_t UsbConfigurationDescriptor::iConfiguration() const
    {
        return _iConfiguration;
    }

    uint8_t UsbConfigurationDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    uint8_t UsbConfigurationDescriptor::MaxPower() const
    {
        return _MaxPower;
    }

    const unsigned char *UsbConfigurationDescriptor::extra() const
    {
        return _extra;
    }

    int UsbConfigurationDescriptor::extra_length() const
    {
        return _extra_length;
    }

    const QStringList &UsbConfigurationDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "wTotalLength",
            "bNumInterfaces",
            "bConfigurationValue",
            "iConfiguration",
            "bmAttributes",
            "bMaxPower",
        };

        return fields;
    }

    QString UsbConfigurationDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of this descriptor in bytes"},
            {"bDescriptorType", "CONFIGURATION Descriptor Type"},
            {"wTotalLength", "Total length of data returned for this "
             "configuration. Includes the combined length "
             "of all descriptors (configuration, interface, "
             "endpoint, and class- or vendor-specific) "
             "returned for this configuration"},
            {"bNumInterfaces", "Number of interfaces supported by this configuration"},
            {"bConfigurationValue", "Value to use as an argument to the "
             "SetConfiguration() request to select this "
             "configuration"},
            {"iConfiguration", "Index of string descriptor describing this configuration"},
            {"bmAttributes", "<p>Configuration characteristics:</p>"
             "<table><tr><td>D7:</td><td>Reserved (set to one)</td></tr>"
            "<tr><td>D6:</td><td>Self-powered</td></tr>"
            "<tr><td>D5:</td><td>Remote Wakeup</td></tr>"
            "<tr><td>D4...0:</td><td>Reserved (reset to zero)</td></tr></table>"
            "<p>D7 is reserved and shall be set to one for "
             "historical reasons.</p>"
             "<p>A device configuration that uses power from "
             "the bus and a local source reports a non-zero "
             "value in <i>bMaxPower</i> to indicate the amount of "
             "bus power required and sets D6. The actual "
             "power source at runtime may be determined "
             "using the GetStatus(DEVICE) request (refer to "
             "Section 9.4.5).</p>"
             "<p>If a device configuration supports remote "
             "wakeup, D5 is set to one.</p>"},
            {"bMaxPower", "<p>Maximum power consumption of the device "
             "from the bus in this specific configuration "
             "when the device is fully operational. "
             "Expressed in 2 mA units when the device is "
             "operating in high-speed mode and in 8 mA "
             "units when operating at Gen X speed."
             "(i.e., 50 = 100 mA when operating at high-"
             "speed and 50 = 400 mA when operating at "
             "Gen X speed).</p>"
             "<p>Note: A device configuration reports whether "
             "the configuration is bus-powered or self-"
             "powered. Device status reports whether the"
             "device is currently self-powered. If a device "
             "is disconnected from its external power "
             "source, it updates device status to indicate "
             "that it is no longer self-powered."
             "A device may not increase its power draw "
             "from the bus, when it loses its external power "
             "source, beyond the amount reported by its "
             "configuration.</p>"
             "<p>If a device can continue to operate when "
             "disconnected from its external power source, "
             "it continues to do so. If the device cannot "
             "continue to operate, it shall return to the "
             "Powered state.</p>"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    UsbInterface *UsbConfigurationDescriptor::interface(uint8_t index) const
    {
        if (index >= _bNumInterfaces)
        {
            LOGE(tr("Index should be 0~%1, but got %2.").arg(_bNumInterfaces).arg(index));
            return nullptr;
        }
        return _interfaces[index];
    }

    UsbDevice *UsbConfigurationDescriptor::device() const
    {
        return _device;
    }

    QString UsbConfigurationDescriptor::bmAttributesInfo() const
    {
        return parseConfigDescBmAttributes(_bmAttributes);
    }

    QString UsbConfigurationDescriptor::infomationToHtml() const
    {
        UsbHtmlBuilder builder;
        builder.start(tr("Configuration Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "CONFIGURATION")
                .attr("wTotalLength", _wTotalLength)
                .attr("bNumInterfaces", _bNumInterfaces)
                .attr("bConfigurationValue", _bConfigurationValue)
                .strdesc("iConfiguration", _iConfiguration, _device)
                .attr("bmAttributes", _bmAttributes, bmAttributesInfo())
                .attr("bMaxPower", _MaxPower, tr("%1 mA").arg(_MaxPower))
                .end();
        foreach (const auto &extraDesc, _extraDescriptors)
            if (extraDesc->type() != ConfigurationExtraDescriptorType::ASSOCIATION)
                builder.append(extraDesc);

        return builder.build();
    }

    void UsbConfigurationDescriptor::addConfigurationExtraDescriptor(UsbConfigurationExtraDescriptor *desc)
    {
        if (desc)
        {
            _extraDescriptors.append(desc);
            if (desc->type() == ConfigurationExtraDescriptorType::ASSOCIATION)
            {
                UsbInterfaceAssociationDescriptor *associationDescriptor =
                        qobject_cast<UsbInterfaceAssociationDescriptor *>(desc);
                for (uint8_t i = associationDescriptor->bFirstInterface();
                     i < (associationDescriptor->bFirstInterface() + associationDescriptor->bInterfaceCount());
                     ++i)
                    for (uint8_t j = 0; j < _interfaces[i]->numAltsetting(); ++j)
                        _interfaces[i]->altsetting(j)->setAssociationDescriptor(associationDescriptor);
            }
        }
    }

    QString parseConfigDescBmAttributes(uint8_t bmAttributes)
    {
        uint8_t self_powered = BIT(bmAttributes, 6);
        uint8_t remote_wakeup = BIT(bmAttributes, 5);
        if (self_powered && remote_wakeup)
            return UsbConfigurationDescriptor::tr("Self Powered, Remote Wakeup");
        else if (self_powered)
            return UsbConfigurationDescriptor::tr("Self Powered");
        else if (remote_wakeup)
            return UsbConfigurationDescriptor::tr("Bus Powered, Remote Wakeup");
        else
            return UsbConfigurationDescriptor::tr("Bus Powered");
    }

}
