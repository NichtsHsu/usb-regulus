#include "usbconfigurationdescriptor.h"

namespace usb {
    UsbConfigurationDescriptor::UsbConfigurationDescriptor(const libusb_config_descriptor *desc, UsbDevice *parent):
        QObject(parent),
        _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType), _bNumInterfaces(desc->bNumInterfaces),
        _iConfiguration(desc->iConfiguration), _bmAttributes(desc->bmAttributes), _MaxPower(desc->MaxPower),
        _wTotalLength(desc->wTotalLength), _extra(desc->extra), _extra_length(desc->extra_length), _device(parent)
    {
        _interface.reserve(_bNumInterfaces);
        for (int i = 0; i < _bNumInterfaces; ++i)
            _interface.append(new UsbInterface(&desc->interface[i], this));
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

    UsbInterface *UsbConfigurationDescriptor::interface(int index) const
    {
        if (index < 0)
        {
            LOGE(tr("Index must be non-negative, but got %1.").arg(index));
            return nullptr;
        }
        if (index >= _bNumInterfaces)
        {
            LOGE(tr("Index should be 0~%1, but got %2.").arg(_bNumInterfaces).arg(index));
            return nullptr;
        }
        return _interface[index];
    }

    UsbDevice *UsbConfigurationDescriptor::device() const
    {
        return _device;
    }
}
