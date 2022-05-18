#include "usbendpointdescriptor.h"

namespace usb {
    UsbEndpointDescriptor::UsbEndpointDescriptor(const libusb_endpoint_descriptor *desc, UsbInterfaceDescriptor *parent) :
        QObject(parent),
        _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType), _bEndpointAddress(desc->bEndpointAddress),
        _bmAttributes(desc->bmAttributes), _bInterval(desc->bInterval), _bRefresh(desc->bRefresh),
        _bSynchAddress(desc->bSynchAddress), _wMaxPacketSize(desc->wMaxPacketSize),
        _extra(desc->extra), _extra_length(desc->extra_length), _interfaceDescriptor(parent)
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

    EndpointDirection UsbEndpointDescriptor::direction() const
    {
        return EndpointDirection(_bEndpointAddress & EP_IN);
    }

    EndpointTransferType UsbEndpointDescriptor::transferType() const
    {
        return EndpointTransferType(_bmAttributes & 0b11);
    }

    EndpointSyncType UsbEndpointDescriptor::syncType() const
    {
        return EndpointSyncType((_bmAttributes & 0b1100) >> 2);
    }

    EndpointUsageType UsbEndpointDescriptor::usageType() const
    {
        return EndpointUsageType((_bmAttributes & 0b110000) >> 4);
    }

    QString UsbEndpointDescriptor::bmAttributesInfo()
    {
        return parseEndpointDescBmAttributes(_bmAttributes);
    }

    QString UsbEndpointDescriptor::endpointAddressInfo()
    {
        return QString(tr("Endpoint %1 %2").arg(_bEndpointAddress & 0x0F).arg(strEndpointDirection(direction())));
    }

    UsbInterfaceDescriptor *UsbEndpointDescriptor::interfaceDescriptor() const
    {
        return _interfaceDescriptor;
    }

    int UsbEndpointDescriptor::transfer(QByteArray &buffer, int &realSize, unsigned int timeout)
    {
        int ret;
        UsbDevice *device = _interfaceDescriptor->interface()->configDescriptor()->device();
        switch (transferType())
        {
            case EndpointTransferType::EP_CONTROL:
            {
                LOGE("Control transfer currently is not supported.");
                ret = LIBUSB_ERROR_NOT_SUPPORTED;
                break;
            }
            case EndpointTransferType::EP_ISOCHRONOUS:
            {
                LOGE("Isochrnous transfer currently is not supported.");
                ret =  LIBUSB_ERROR_NOT_SUPPORTED;
                break;
            }
            case EndpointTransferType::EP_INTERRUPT:
            {
                ret = libusb_interrupt_transfer(device->handle(),
                                                _bEndpointAddress,
                                                reinterpret_cast<unsigned char *>(buffer.data()),
                                                buffer.size(),
                                                &realSize,
                                                timeout);
                break;
            }
            case EndpointTransferType::EP_BULK:
            {
                ret = libusb_bulk_transfer(device->handle(),
                                           _bEndpointAddress,
                                           reinterpret_cast<unsigned char *>(buffer.data()),
                                           buffer.size(),
                                           &realSize,
                                           timeout);
                break;
            }
        }
        return ret;
    }

    uint16_t UsbEndpointDescriptor::wMaxPacketSize() const
    {
        return _wMaxPacketSize;
    }

    QString strEndpointDirection(EndpointDirection direction)
    {
        switch(direction)
        {
            case EndpointDirection::EP_IN:
            return QString(UsbEndpointDescriptor::tr("IN"));
            case EndpointDirection::EP_OUT:
            return QString(UsbEndpointDescriptor::tr("OUT"));
            default:
                log().e("EndpointDirection", UsbEndpointDescriptor::tr("No such item (value: %1) in enumeration.").arg(int(direction)));
            return QString();
        }
    }

    QString strEndpointTransferType(EndpointTransferType type)
    {
        switch(type)
        {
            case EndpointTransferType::EP_BULK:
            return QString(UsbEndpointDescriptor::tr("Bulk Transfer"));
            case EndpointTransferType::EP_CONTROL:
            return QString(UsbEndpointDescriptor::tr("Control Transfer"));
            case EndpointTransferType::EP_INTERRUPT:
            return QString(UsbEndpointDescriptor::tr("Interrupt Transfer"));
            case EndpointTransferType::EP_ISOCHRONOUS:
            return QString(UsbEndpointDescriptor::tr("Isochronous Transfer"));
            default:
                log().e("EndpointTransferType", UsbEndpointDescriptor::tr("No such item (value: %1) in enumeration.").arg(int(type)));
            return QString();
        }
    }

    QString strEndpointSyncType(EndpointSyncType type)
    {
        switch(type)
        {
            case EndpointSyncType::EP_ADAPTIVE:
            return QString(UsbEndpointDescriptor::tr("Adaptive"));
            case EndpointSyncType::EP_ASYNC:
            return QString(UsbEndpointDescriptor::tr("Asynchronous"));
            case EndpointSyncType::EP_SYNC:
            return QString(UsbEndpointDescriptor::tr("Synchronous"));
            case EndpointSyncType::EP_NONE:
            return QString(UsbEndpointDescriptor::tr("No Synchronization"));
            default:
                log().e("EndpointSyncType", UsbEndpointDescriptor::tr("No such item (value: %1) in enumeration.").arg(int(type)));
            return QString();
        }
    }

    QString strEndpointUsageType(EndpointUsageType type)
    {
        switch(type)
        {
            case EndpointUsageType::EP_DATA:
            return QString(UsbEndpointDescriptor::tr("Data Usage"));
            case EndpointUsageType::EP_FEEDBACK:
            return QString(UsbEndpointDescriptor::tr("Feedback Usage"));
            case EndpointUsageType::EP_IMPLICIT:
            return QString(UsbEndpointDescriptor::tr("Implicit Feedback Usage"));
            default:
                log().e("EndpointUsageType", UsbEndpointDescriptor::tr("No such item (value: %1) in enumeration.").arg(int(type)));
            return QString();
        }
    }

    QString parseEndpointDescBmAttributes(uint8_t bmAttributes)
    {
        EndpointTransferType trasferType = EndpointTransferType(bmAttributes & 0b11);
        if (trasferType == EndpointTransferType::EP_ISOCHRONOUS)
        {
            QString info = strEndpointTransferType(trasferType);
            info += ",";
            info += strEndpointSyncType(EndpointSyncType((bmAttributes & 0b1100) >> 2));
            info += ",";
            info += strEndpointUsageType(EndpointUsageType((bmAttributes & 0b110000) >> 4));
            return info;
        }
        else
            return strEndpointTransferType(trasferType);
    }

}
