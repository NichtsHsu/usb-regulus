#include "usbendpointdescriptor.h"
#include "usbinterfaceassociationdescriptor.h"
#include "__usbmacro.h"

namespace usb {
    UsbEndpointDescriptor::UsbEndpointDescriptor(const libusb_endpoint_descriptor *desc, UsbInterfaceDescriptor *parent) :
        QObject(parent),
        _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType), _bEndpointAddress(desc->bEndpointAddress),
        _bmAttributes(desc->bmAttributes), _bInterval(desc->bInterval), _bRefresh(desc->bRefresh),
        _bSynchAddress(desc->bSynchAddress), _wMaxPacketSize(desc->wMaxPacketSize),
        _extra(desc->extra), _extraLength(desc->extra_length), _interfaceDescriptor(parent)
    {
        if (_extraLength > 0)
            /* Wait for all interface descriptor ready to set association descriptor */
            QTimer::singleShot(200, this, &UsbEndpointDescriptor::__requestExtraDescriptor);
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
        return _extraLength;
    }

    EndpointDirection UsbEndpointDescriptor::direction() const
    {
        return EndpointDirection(_bEndpointAddress & int(EndpointDirection::IN));
    }

    EndpointTransferType UsbEndpointDescriptor::transferType() const
    {
        return EndpointTransferType(CUT(_bmAttributes, 0, 1));
    }

    EndpointSyncType UsbEndpointDescriptor::syncType() const
    {
        return EndpointSyncType(CUT(_bmAttributes, 2, 3));
    }

    EndpointIsochronousUsageType UsbEndpointDescriptor::isochronousUsageType() const
    {
        return EndpointIsochronousUsageType(CUT(_bmAttributes, 4, 5));
    }

    EndpointInterruptUsageType UsbEndpointDescriptor::interruptUsageType() const
    {
        return EndpointInterruptUsageType(CUT(_bmAttributes, 4, 5));
    }

    QString UsbEndpointDescriptor::bmAttributesInfo() const
    {
        return parseEndpointDescBmAttributes(_bmAttributes);
    }

    QString UsbEndpointDescriptor::endpointAddressInfo() const
    {
        return QString(tr("Endpoint %1 %2").arg(_bEndpointAddress & 0x0F).arg(strEndpointDirection(direction())));
    }

    UsbInterfaceDescriptor *UsbEndpointDescriptor::interfaceDescriptor() const
    {
        return _interfaceDescriptor;
    }

    int UsbEndpointDescriptor::transfer(QByteArray &buffer, int &realSize, unsigned int timeout)
    {
        int ret = 0;
        UsbDevice *device = _interfaceDescriptor->interface()->configurationDescriptor()->device();
        switch (transferType())
        {
            case EndpointTransferType::CONTROL:
            {
                LOGE("Control transfer should not be happening here.");
                ret = LIBUSB_ERROR_NOT_SUPPORTED;
                break;
            }
            case EndpointTransferType::ISOCHRONOUS:
            {
                LOGE("Isochrnous transfer currently is not supported.");
                ret =  LIBUSB_ERROR_NOT_SUPPORTED;
                break;
            }
            case EndpointTransferType::INTERRUPT:
            {
                ret = libusb_interrupt_transfer(device->handle(),
                                                _bEndpointAddress,
                                                reinterpret_cast<unsigned char *>(buffer.data()),
                                                buffer.size(),
                                                &realSize,
                                                timeout);
                break;
            }
            case EndpointTransferType::BULK:
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

    QString UsbEndpointDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Endpoint Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bEndpointAddress", _bEndpointAddress, endpointAddressInfo());
        ATTR("bmAttributes", _bmAttributes, bmAttributesInfo());
        ATTR("wMaxPacketSize", _wMaxPacketSize, _wMaxPacketSize);
        ATTR("bInterval", _bInterval, _bInterval);
        ATTR("bRefresh", _bRefresh, _bRefresh);
        ATTR("bSynchAddress", _bSynchAddress, _bSynchAddress);
        END;

        return html;
    }

    void UsbEndpointDescriptor::__requestExtraDescriptor()
    {
        int len = _extraLength;
        int pos = 0;
        while (len > 1)
        {
            // Interface Association Descriptor
            if (_extra[pos + 1] == uint8_t(ConfigurationExtraDescriptorType::ASSOCIATION) ||
                    _extra[pos + 1] == uint8_t(ConfigurationExtraDescriptorType::OTG))
            {
                UsbConfigurationExtraDescriptor *configExtraDesc =
                        UsbConfigurationExtraDescriptor::fromEndpointExtra(this, pos);
                UsbConfigurationDescriptor *configDesc =
                        _interfaceDescriptor->interface()->configurationDescriptor();
                configDesc->addConfigurationExtraDescriptor(configExtraDesc);
            }
            len -= _extra[pos];
            pos += _extra[pos];
        }
    }

    uint16_t UsbEndpointDescriptor::wMaxPacketSize() const
    {
        return _wMaxPacketSize;
    }

    QString strEndpointDirection(EndpointDirection direction)
    {
        switch(direction)
        {
            case EndpointDirection::IN:
            return QString(UsbEndpointDescriptor::tr("IN"));
            case EndpointDirection::OUT:
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
            case EndpointTransferType::BULK:
            return QString(UsbEndpointDescriptor::tr("Bulk Transfer"));
            case EndpointTransferType::CONTROL:
            return QString(UsbEndpointDescriptor::tr("Control Transfer"));
            case EndpointTransferType::INTERRUPT:
            return QString(UsbEndpointDescriptor::tr("Interrupt Transfer"));
            case EndpointTransferType::ISOCHRONOUS:
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
            case EndpointSyncType::ADAPTIVE:
            return QString(UsbEndpointDescriptor::tr("Adaptive"));
            case EndpointSyncType::ASYNCHRONOUS:
            return QString(UsbEndpointDescriptor::tr("Asynchronous"));
            case EndpointSyncType::SYNCHRONOUS:
            return QString(UsbEndpointDescriptor::tr("Synchronous"));
            case EndpointSyncType::NONE:
            return QString(UsbEndpointDescriptor::tr("No Synchronization"));
            default:
                log().e("EndpointSyncType", UsbEndpointDescriptor::tr("No such item (value: %1) in enumeration.").arg(int(type)));
            return QString();
        }
    }

    QString strEndpointIsochronousUsageType(EndpointIsochronousUsageType type)
    {
        switch(type)
        {
            case EndpointIsochronousUsageType::DATA:
            return QString(UsbEndpointDescriptor::tr("Data Usage"));
            case EndpointIsochronousUsageType::FEEDBACK:
            return QString(UsbEndpointDescriptor::tr("Feedback Usage"));
            case EndpointIsochronousUsageType::IMPLICIT:
            return QString(UsbEndpointDescriptor::tr("Implicit Feedback Usage"));
            default:
                log().e("EndpointIsochronousUsageType", UsbEndpointDescriptor::tr("No such item (value: %1) in enumeration.").arg(int(type)));
            return QString();
        }
    }

    QString parseEndpointDescBmAttributes(uint8_t bmAttributes)
    {
        EndpointTransferType trasferType = EndpointTransferType(CUT(bmAttributes, 0, 1));
        if (trasferType == EndpointTransferType::ISOCHRONOUS)
        {
            QString info = strEndpointTransferType(trasferType);
            info += ", ";
            info += strEndpointSyncType(EndpointSyncType(CUT(bmAttributes, 2, 3)));
            info += ", ";
            info += strEndpointIsochronousUsageType(EndpointIsochronousUsageType(CUT(bmAttributes, 4, 5)));
            return info;
        }
        else if (trasferType == EndpointTransferType::INTERRUPT)
        {
            QString info = strEndpointTransferType(trasferType);
            info += ", ";
            info += strEndpointInterruptUsageType(EndpointInterruptUsageType(CUT(bmAttributes, 4, 5)));
            return info;
        }
        else
            return strEndpointTransferType(trasferType);
    }

    QString strEndpointInterruptUsageType(EndpointInterruptUsageType type)
    {
        switch(type)
        {
            case EndpointInterruptUsageType::PERIODIC:
            return QString(UsbEndpointDescriptor::tr("Periodic Usage"));
            case EndpointInterruptUsageType::NOTIFICATION:
            return QString(UsbEndpointDescriptor::tr("Notification Usage"));
            default:
                log().e("EndpointInterruptUsageType", UsbEndpointDescriptor::tr("No such item (value: %1) in enumeration.").arg(int(type)));
            return QString();
        }
    }

}
