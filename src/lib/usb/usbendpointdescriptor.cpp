#include "usbendpointdescriptor.h"
#include "usbinterfaceassociationdescriptor.h"
#include "__usbmacro.h"

namespace usb {
    UsbEndpointDescriptor::UsbEndpointDescriptor(const libusb_endpoint_descriptor *desc, UsbInterfaceDescriptor *parent) :
        QObject(parent),
        _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType), _bEndpointAddress(desc->bEndpointAddress),
        _bmAttributes(desc->bmAttributes), _bInterval(desc->bInterval), _bRefresh(desc->bRefresh),
        _bSynchAddress(desc->bSynchAddress), _wMaxPacketSize(desc->wMaxPacketSize),
        _extra(desc->extra), _extraLength(desc->extra_length), _interfaceDescriptor(parent),
        _transfer(nullptr)
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

    int UsbEndpointDescriptor::syncTransfer(QByteArray &buffer, int &realSize, unsigned int timeout)
    {
        if (_transfer)
            return ERROR_DUPLICATE_REQUEST;

        int ret = 0;
        UsbDevice *device = _interfaceDescriptor->interface()->configurationDescriptor()->device();
        switch (transferType())
        {
            case EndpointTransferType::CONTROL:
            {
                // Never reach here
                LOGE(tr("Control transfer should not happen here."));
                ret = LIBUSB_ERROR_NOT_SUPPORTED;
                break;
            }
            case EndpointTransferType::ISOCHRONOUS:
            {
                LOGE(tr("Isochrnous transfer does not support blocking transfer."));
                ret =  LIBUSB_ERROR_NOT_SUPPORTED;
                break;
            }
            case EndpointTransferType::INTERRUPT:
            {
                ret = libusb_interrupt_transfer(
                            device->handle(),
                            _bEndpointAddress,
                            reinterpret_cast<unsigned char *>(buffer.data()),
                            buffer.size(),
                            &realSize,
                            timeout);
                break;
            }
            case EndpointTransferType::BULK:
            {
                ret = libusb_bulk_transfer(
                            device->handle(),
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

    int UsbEndpointDescriptor::startAsyncTransfer(const QByteArray &buffer, int readLength, unsigned int timeout)
    {
        if (_transfer)
            return ERROR_DUPLICATE_REQUEST;

        int isoPackets = 1;
        int packetSize = _wMaxPacketSize;
        if (transferType() == EndpointTransferType::ISOCHRONOUS)
        {
            packetSize = libusb_get_max_iso_packet_size(
                        _interfaceDescriptor->interface()->configurationDescriptor()->device()->device(),
                        _bEndpointAddress);
            if (packetSize < LIBUSB_SUCCESS)
                return packetSize;

            if (direction() == EndpointDirection::OUT)
                isoPackets = buffer.size() ? ((buffer.size() - 1) / packetSize) + 1 : 1;
            else
                isoPackets = readLength > 0 ? ((readLength - 1) / packetSize) + 1 : 1;
            LOGD(tr("Isochronous transfer: %1 packets with packet size %2.")
                 .arg(isoPackets)
                 .arg(packetSize));
        }

        _transfer = libusb_alloc_transfer(isoPackets);
        if (transferType() == EndpointTransferType::ISOCHRONOUS)
            libusb_set_iso_packet_lengths(_transfer, packetSize);

        uint8_t *rawBuffer;
        int length;

        if (direction() == EndpointDirection::OUT)
        {
            length = buffer.length();
            rawBuffer = new uint8_t[length];
            memcpy(rawBuffer, buffer.data(), length);
        }
        else
        {
            length = readLength > 0 ? readLength : isoPackets * packetSize;
            rawBuffer = new uint8_t[length];
            memset(rawBuffer, 0, length);
        }

        switch (transferType())
        {
            case EndpointTransferType::CONTROL:
            {
                LOGE(tr("Control transfer should not happen here."));
                return LIBUSB_ERROR_NOT_SUPPORTED;
            }
            case EndpointTransferType::ISOCHRONOUS:
                libusb_fill_iso_transfer(
                            _transfer,
                            _interfaceDescriptor->interface()->configurationDescriptor()->device()->handle(),
                            _bEndpointAddress,
                            rawBuffer,
                            length,
                            isoPackets,
                            &UsbEndpointDescriptor::__asyncTransferCallback,
                            this,
                            timeout);
            break;
            case EndpointTransferType::INTERRUPT:
                libusb_fill_interrupt_transfer(
                            _transfer,
                            _interfaceDescriptor->interface()->configurationDescriptor()->device()->handle(),
                            _bEndpointAddress,
                            rawBuffer,
                            length,
                            &UsbEndpointDescriptor::__asyncTransferCallback,
                            this,
                            timeout);
            break;
            case EndpointTransferType::BULK:
                libusb_fill_bulk_transfer(
                            _transfer,
                            _interfaceDescriptor->interface()->configurationDescriptor()->device()->handle(),
                            _bEndpointAddress,
                            rawBuffer,
                            length,
                            &UsbEndpointDescriptor::__asyncTransferCallback,
                            this,
                            timeout);
            break;
        }

        int ret = libusb_submit_transfer(_transfer);
        if (ret < LIBUSB_SUCCESS)
        {
            delete[] rawBuffer;
            libusb_free_transfer(_transfer);
            _transfer = nullptr;
        }

        return ret;
    }

    int UsbEndpointDescriptor::cancelAsyncTransfer()
    {
        if (!_transfer)
            return LIBUSB_SUCCESS;
        return libusb_cancel_transfer(_transfer);
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

    void UsbEndpointDescriptor::__asyncTransferCallback(libusb_transfer *transfer)
    {
        UsbEndpointDescriptor *ep = reinterpret_cast<UsbEndpointDescriptor *>(transfer->user_data);
        int status = transfer->status;
        log().d("UsbEndpointDescriptor",
                tr("Asynchronous transfer callback: status %1.")
                .arg(usb_error_name(status)));

        switch(status)
        {
            case libusb_transfer_status::LIBUSB_TRANSFER_COMPLETED:
                if (ep->transferType() == EndpointTransferType::ISOCHRONOUS)
                {
                    QByteArray data;
                    bool isError = false;
                    for (int i = 0; i < transfer->num_iso_packets; ++i)
                    {
                        libusb_iso_packet_descriptor *desc = &transfer->iso_packet_desc[i];
                        int status = desc->status;
                        log().d("UsbEndpointDescriptor",
                                tr("Isochronous transfer result: %1, %2.")
                                .arg(QString("packet %1").arg(i))
                                .arg(QString("status %1").arg(usb_error_name(status))));
                        if (status == libusb_transfer_status::LIBUSB_TRANSFER_COMPLETED ||
                                status == libusb_transfer_status::LIBUSB_TRANSFER_OVERFLOW)
                        {

                            log().d("UsbEndpointDescriptor",
                                    tr("Isochronous transfer result: %1, %2.")
                                    .arg(QString("packet %1").arg(i))
                                    .arg(QString("actual_length %1").arg(desc->actual_length)));
                            if (ep->direction() == EndpointDirection::IN)
                                data.append(reinterpret_cast<const char *>(
                                                libusb_get_iso_packet_buffer_simple(transfer, i)),
                                            desc->actual_length);
                        }
                        else if (status == libusb_transfer_status::LIBUSB_TRANSFER_CANCELLED)
                        {
                            QTimer::singleShot(0, ep, [ep]() {
                                emit ep->asyncTransferCancelled();
                            });
                            isError = true;
                            break;
                        }
                        else
                        {
                            QTimer::singleShot(0, ep, [ep, status]() {
                                emit ep->asyncTransferFailed(status);
                            });
                            isError = true;
                            break;
                        }
                    }

                    log().d("UsbEndpointDescriptor",
                            tr("Isochronous transfer result: %1, %2.")
                            .arg(QString("isError %1").arg(isError))
                            .arg(QString("length %1").arg(data.length())));
                    if (!isError)
                        QTimer::singleShot(0, ep, [ep, data]() {
                            emit ep->asyncTransferCompleted(data);
                        });
                }
                else
                {
                    if (ep->direction() == EndpointDirection::OUT)
                        QTimer::singleShot(0, ep, [ep]() {
                            emit ep->asyncTransferCompleted();
                        });
                    else
                    {
                        QByteArray data(reinterpret_cast<const char *>(transfer->buffer),
                                        transfer->actual_length);
                        QTimer::singleShot(0, ep, [ep, data]() {
                            emit ep->asyncTransferCompleted(data);
                        });
                    }
                }
            break;
            case libusb_transfer_status::LIBUSB_TRANSFER_CANCELLED:
                QTimer::singleShot(0, ep, [ep]() {
                    emit ep->asyncTransferCancelled();
                });
            break;
            default:
                QTimer::singleShot(0, ep, [ep, status]() {
                    emit ep->asyncTransferFailed(status);
                });
            break;
        }

        delete[] ep->_transfer->buffer;
        libusb_free_transfer(ep->_transfer);
        ep->_transfer = nullptr;
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
