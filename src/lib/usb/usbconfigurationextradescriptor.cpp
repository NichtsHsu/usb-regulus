#include "usbconfigurationextradescriptor.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

namespace usb {
    UsbConfigurationExtraDescriptor::UsbConfigurationExtraDescriptor(UsbConfigurationDescriptor *parent)
        : QObject{parent}, _configurationDescriptor(parent)
    {

    }

    UsbConfigurationExtraDescriptor *UsbConfigurationExtraDescriptor::get(
            UsbConfigurationDescriptor *configDesc, uint8_t descPos)
    {
        ConfigurationExtraDescriptorType descriptorType =
                ConfigurationExtraDescriptorType(configDesc->extra()[descPos + 1]);
        const uint8_t *buffer = configDesc->extra() + descPos;
        UsbConfigurationExtraDescriptor *desc;

        switch (descriptorType)
        {
            case ConfigurationExtraDescriptorType::OTG:
                desc = new UsbOtgDescriptor(configDesc);
            break;
            case ConfigurationExtraDescriptorType::ASSOCIATION:
                desc = new UsbInterfaceAssociationDescriptor(configDesc);
            break;
            case ConfigurationExtraDescriptorType::SECURITY:
                desc = new UsbWirelessSecurityDescriptor(configDesc);
            break;
            case ConfigurationExtraDescriptorType::ENCRYPTION_TYPE:
                desc = new UsbWirelessEncryptionTypeDescriptor(configDesc);
            break;
            default:
            {
                log().d("UsbConfigurationExtraDescriptor",
                        tr("Unknown configuration extra descriptor type \"%1\".")
                        .arg(int(descriptorType)));
                return nullptr;
            }
        }

        desc->__initWithBuffer(buffer);
        return desc;
    }

    UsbConfigurationExtraDescriptor *UsbConfigurationExtraDescriptor::fromInterfaceExtra(
            UsbInterfaceDescriptor *interfaceDesc, uint8_t descPos)
    {
        ConfigurationExtraDescriptorType descriptorType =
                ConfigurationExtraDescriptorType(interfaceDesc->extra()[descPos + 1]);
        const uint8_t *buffer = interfaceDesc->extra() + descPos;
        UsbConfigurationExtraDescriptor *desc;

        switch (descriptorType)
        {
            case ConfigurationExtraDescriptorType::OTG:
                desc = new UsbOtgDescriptor(
                            interfaceDesc->interface()->configurationDescriptor());
            break;
            case ConfigurationExtraDescriptorType::ASSOCIATION:
                desc = new UsbInterfaceAssociationDescriptor(
                            interfaceDesc->interface()->configurationDescriptor());
            break;
            case ConfigurationExtraDescriptorType::SECURITY:
            case ConfigurationExtraDescriptorType::ENCRYPTION_TYPE:
            default:
                /* Never reach */
            return nullptr;
        }

        desc->__initWithBuffer(buffer);
        return desc;
    }

    UsbConfigurationExtraDescriptor *UsbConfigurationExtraDescriptor::fromEndpointExtra(
            UsbEndpointDescriptor *endpointDesc, uint8_t descPos)
    {
        ConfigurationExtraDescriptorType descriptorType =
                ConfigurationExtraDescriptorType(endpointDesc->extra()[descPos + 1]);
        const uint8_t *buffer = endpointDesc->extra() + descPos;
        UsbConfigurationExtraDescriptor *desc;

        switch (descriptorType)
        {
            case ConfigurationExtraDescriptorType::OTG:
                desc = new UsbOtgDescriptor(
                            endpointDesc->interfaceDescriptor()->interface()->configurationDescriptor());
            break;
            case ConfigurationExtraDescriptorType::ASSOCIATION:
                desc = new UsbInterfaceAssociationDescriptor(
                            endpointDesc->interfaceDescriptor()->interface()->configurationDescriptor());
            break;
            case ConfigurationExtraDescriptorType::SECURITY:
            case ConfigurationExtraDescriptorType::ENCRYPTION_TYPE:
            default:
                /* Never reach */
            return nullptr;
        }

        desc->__initWithBuffer(buffer);
        return desc;
    }

    UsbConfigurationDescriptor *UsbConfigurationExtraDescriptor::configurationDescriptor() const
    {
        return _configurationDescriptor;
    }

    uint8_t UsbWirelessSecurityDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbWirelessSecurityDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    ConfigurationExtraDescriptorType UsbWirelessSecurityDescriptor::type() const
    {
        return ConfigurationExtraDescriptorType::SECURITY;
    }

    UsbWirelessSecurityDescriptor::UsbWirelessSecurityDescriptor(UsbConfigurationDescriptor *configDesc):
        UsbConfigurationExtraDescriptor(configDesc)
    {

    }

    void UsbWirelessSecurityDescriptor::__initWithBuffer(const uint8_t *buffer)
    {
        _bLength = buffer[0];
        _bDescriptorType = buffer[1];
        _wTotalLength = *reinterpret_cast<const uint16_t *>(buffer + 2);
        _bNumEncryptionTypes = buffer[4];
    }

    uint8_t UsbWirelessSecurityDescriptor::bNumEncryptionTypes() const
    {
        return _bNumEncryptionTypes;
    }

    QString UsbWirelessSecurityDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Security Descriptor"))
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "SECURITY")
                .attr("wTotalLength", _wTotalLength)
                .attr("bNumEncryptionTypes", _bNumEncryptionTypes)
                .end()
                .build();
    }

    uint16_t UsbWirelessSecurityDescriptor::wTotalLength() const
    {
        return _wTotalLength;
    }

    uint8_t UsbWirelessEncryptionTypeDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbWirelessEncryptionTypeDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint8_t UsbWirelessEncryptionTypeDescriptor::bEncryptionType() const
    {
        return _bEncryptionType;
    }

    uint8_t UsbWirelessEncryptionTypeDescriptor::bEncryptionValue() const
    {
        return _bEncryptionValue;
    }

    uint8_t UsbWirelessEncryptionTypeDescriptor::bAuthKeyIndex() const
    {
        return _bAuthKeyIndex;
    }

    QString UsbWirelessEncryptionTypeDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Encryption Type Descriptor"))
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "ENCRYPTION_TYPE")
                .attr("bEncryptionType", _bEncryptionType, __parseEncryptionType())
                .attr("bEncryptionValue", _bEncryptionValue)
                .attr("bAuthKeyIndex", _bAuthKeyIndex)
                .end()
                .build();
    }

    ConfigurationExtraDescriptorType UsbWirelessEncryptionTypeDescriptor::type() const
    {
        return ConfigurationExtraDescriptorType::ENCRYPTION_TYPE;
    }

    UsbWirelessEncryptionTypeDescriptor::UsbWirelessEncryptionTypeDescriptor(UsbConfigurationDescriptor *configDesc):
        UsbConfigurationExtraDescriptor(configDesc)
    {

    }

    void UsbWirelessEncryptionTypeDescriptor::__initWithBuffer(const uint8_t *buffer)
    {
        _bLength = buffer[0];
        _bDescriptorType = buffer[1];
        _bEncryptionType = buffer[2];
        _bEncryptionValue = buffer[3];
        _bAuthKeyIndex = buffer[4];
    }

    QString UsbWirelessEncryptionTypeDescriptor::__parseEncryptionType() const
    {
        switch (_bEncryptionType)
        {
            case 1:
            return tr("Virtual encryption provided by the wire");
            case 2:
            return tr("AES-128 in CCM mode");
            default:
            return tr("Reserved");
        }
    }

} // namespace usb
