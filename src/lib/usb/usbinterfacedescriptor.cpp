#include "usbinterfacedescriptor.h"

namespace usb {
    UsbInterfaceDescriptor::UsbInterfaceDescriptor(const libusb_interface_descriptor *desc, UsbInterface *parent) :
        QObject(parent),
        _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType), _bInterfaceNumber(desc->bInterfaceNumber),
        _bAlternateSetting(desc->bAlternateSetting), _bNumEndpoints(desc->bNumEndpoints), _bInterfaceClass(desc->bInterfaceClass),
        _bInterfaceProtocol(desc->bInterfaceProtocol), _iInterface(desc->iInterface), _extra(desc->extra), _extra_length(desc->extra_length),
        _interface(parent)
    {
        _endpoint.reserve(_bNumEndpoints);
        for (int i = 0; i < _bNumEndpoints; ++i)
            _endpoint.append(new UsbEndpointDescriptor(&desc->endpoint[i], this));
    }

    uint8_t UsbInterfaceDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbInterfaceDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint8_t UsbInterfaceDescriptor::bInterfaceNumber() const
    {
        return _bInterfaceNumber;
    }

    uint8_t UsbInterfaceDescriptor::bAlternateSetting() const
    {
        return _bAlternateSetting;
    }

    uint8_t UsbInterfaceDescriptor::bNumEndpoints() const
    {
        return _bNumEndpoints;
    }

    uint8_t UsbInterfaceDescriptor::bInterfaceClass() const
    {
        return _bInterfaceClass;
    }

    uint8_t UsbInterfaceDescriptor::bInterfaceSubClass() const
    {
        return _bInterfaceSubClass;
    }

    uint8_t UsbInterfaceDescriptor::bInterfaceProtocol() const
    {
        return _bInterfaceProtocol;
    }

    uint8_t UsbInterfaceDescriptor::iInterface() const
    {
        return _iInterface;
    }

    const unsigned char *UsbInterfaceDescriptor::extra() const
    {
        return _extra;
    }

    int UsbInterfaceDescriptor::extra_length() const
    {
        return _extra_length;
    }

    UsbEndpointDescriptor *UsbInterfaceDescriptor::endpoint(int index) const
    {
        if (index < 0)
        {
            LOGE(tr("Index must be non-negative, but got %1.").arg(index));
            return nullptr;
        }
        if (index >= _bNumEndpoints)
        {
            LOGE(tr("Index should be 0~%1, but got %2.").arg(_bNumEndpoints).arg(index));
            return nullptr;
        }
        return _endpoint[index];
    }

    UsbInterface *UsbInterfaceDescriptor::interface() const
    {
        return _interface;
    }
}
