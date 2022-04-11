#include "usbendpointdescriptor.h"

namespace usb {
    UsbEndpointDescriptor::UsbEndpointDescriptor(const libusb_endpoint_descriptor *desc, QObject *parent) :
        QObject(parent),
        _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType), _bEndpointAddress(desc->bEndpointAddress),
        _bmAttributes(desc->bmAttributes), _bInterval(desc->bInterval), _bRefresh(desc->bRefresh),
        _bSynchAddress(desc->bSynchAddress), _extra(desc->extra), _extra_length(desc->extra_length)
    {

    }

    uint8_t UsbEndpointDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbEndpointDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint8_t UsbEndpointDescriptor::bEndpointAddress() const
    {
        return _bEndpointAddress;
    }

    uint8_t UsbEndpointDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    uint8_t UsbEndpointDescriptor::bInterval() const
    {
        return _bInterval;
    }

    uint8_t UsbEndpointDescriptor::bRefresh() const
    {
        return _bRefresh;
    }

    uint8_t UsbEndpointDescriptor::bSynchAddress() const
    {
        return _bSynchAddress;
    }

    const unsigned char *UsbEndpointDescriptor::extra() const
    {
        return _extra;
    }

    int UsbEndpointDescriptor::extra_length() const
    {
        return _extra_length;
    }

    UsbEndpointDescriptor::Direction UsbEndpointDescriptor::direction() const
    {
        return Direction(_bEndpointAddress & IN);
    }

    UsbEndpointDescriptor::TransferType UsbEndpointDescriptor::transferType() const
    {
        return TransferType(_bmAttributes & 0b11);
    }

    UsbEndpointDescriptor::SyncType UsbEndpointDescriptor::syncType() const
    {
        return SyncType((_bmAttributes & 0b1100) >> 2);
    }

    UsbEndpointDescriptor::UsageType UsbEndpointDescriptor::usageType() const
    {
        return UsageType((_bmAttributes & 0b110000) >> 4);
    }
}
