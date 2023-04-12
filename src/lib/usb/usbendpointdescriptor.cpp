#include "usbendpointdescriptor.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

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

    const QStringList &UsbEndpointDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bEndpointAddress",
            "bmAttributes",
            "wMaxPacketSize",
            "bInterval",
            "bRefresh",
            "bSynchAddress"
        };

        return fields;
    }

    QString UsbEndpointDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of this descriptor in bytes"},
            {"bDescriptorType", "ENDPOINT Descriptor Type"},
            {"bEndpointAddress", "<p>The address of the endpoint on the device described by this "
             "descriptor. The address is encoded as follows:</p>"
             "<table><tr><td>Bit 3...0:</td><td>The endpoint number</td></tr>"
             "<tr><td>Bit 6...4:</td><td>Reserved, reset to zero</td></tr>"
             "<tr><td>Bit 7:</td><td><div>Direction, ignored for control endpoints</div>"
             "<div>0 = OUT endpoint</div><div>1 = IN endpoint</div></td></tr></table>"},
            {"bmAttributes", "<p>This field describes the endpoint’s attributes when it is "
             "configured using the <i>bConfigurationValue</i>.</p>"
             "<table><tr><td>Bits 1..0:</td><td><div>Transfer Type</div>"
             "<div>00 = Control</div><div>01 = Isochronous</div>"
             "<div>10 = Bulk</div><div>11 = Interrupt</div></td></tr></table>"
             "<p>If an interrupt endpoint, bits 5..2 are defined as follows:</p>"
             "<table><tr><td>Bits 3..2:</td><td>Reserved</td></tr>"
             "<tr><td>Bits 5..4:</td><td><div>Usage Type</div>"
             "<div>00 = Periodic</div><div>01 = Notification</div>"
             "<div>10 = Reserved</div><div>11 = Reserved</div></td></tr></table>"
             "<p>If isochronous, they are defined as follows:</p>"
             "<table><tr><td>Bits 3..2:</td><td><div>Synchronization Type</div>"
             "<div>00 = No Synchronization</div><div>01 = Asynchronous</div>"
             "<div>10 = Adaptive</div><div>11 = Synchronous</div></td></tr>"
             "<tr><td>Bits 5..4:</td><td><div>Usage Type</div>"
             "<div>00 = Data endpoint</div><div>01 = Feedback endpoint</div>"
             "<div>10 = Implicit feedback Data endpoint</div><div>11 = Reserved</div></td></tr></table>"
             "<p>If not an isochronous or interrupt endpoint, bits 5..2 are "
             "reserved and shall be set to zero.</p>"
             "<p>If not an isochronous or interrupt endpoint, bits 5..2 are "
             "reserved and shall be set to zero.</p>"
            },
            {"wMaxPacketSize","<h3>USB 2.0</h3>"
             "<p>Maximum packet size this endpoint is capable of "
             "sending or receiving when this configuration is "
             "selected.</p>"
             "<p>For isochronous endpoints, this value is used to "
             "reserve the bus time in the schedule, required for the "
             "per-(micro)frame data payloads. The pipe may, on an "
             "ongoing basis, actually use less bandwidth than that "
             "reserved. The device reports, if necessary, the actual "
             "bandwidth used via its normal, non-USB defined "
             "mechanisms.</p>"
             "<p>For all endpoints, bits 10..0 specify the maximum "
             "packet size (in bytes).</p>"
             "<p>For high-speed isochronous and interrupt endpoints:</p>"
             "<p>Bits 12..11 specify the number of additional transaction "
             "opportunities per microframe:</p>"
             "<table><tr><td>00 = None (1 transaction per microframe)</td></tr>"
             "<tr><td>01 = 1 additional (2 per microframe)</td></tr>"
             "<tr><td>10 = 2 additional (3 per microframe)</td></tr>"
             "<tr><td>11 = Reserved</td></tr></table>"
             "<p>Bits 15..13 are reserved and must be set to zero.</p>"
             "<p>Refer to Chapter 5 for more information.</p>"
             "<h3>USB 3.2</h3>"
             "<p>Maximum packet size this endpoint is capable of sending or "
             "receiving when this configuration is selected.</p>"
             "<p>For control endpoints this field shall be set to 512. For bulk "
             "endpoint types this field shall be set to 1024.</p>"
             "<p>For interrupt and isochronous endpoints this field shall be "
             "set to 1024 if this endpoint defines a value in the bMaxBurst"
             "field greater than zero. If the value in the bMaxBurst field is "
             "set to zero then this field can have any value from 0 to 1024 "
             "for an isochronous endpoint and 1 to 1024 for an interrupt "
             "endpoint.</p>"},
            {"bInterval", "<h3>USB 2.0</h3>"
             "<p>Interval for polling endpoint for data transfers. "
             "Expressed in frames or microframes depending on the "
             "device operating speed (i.e., either 1 millisecond or "
             "125 µs units).</p>"
             "<p>For full-/high-speed isochronous endpoints, this value "
             "must be in the range from 1 to 16. The <i>bInterval</i> value "
             "is used as the exponent for a 2<sup>bInterval-1</sup> value; e.g., a "
             "<i>bInterval</i> of 4 means a period of 8 (2<sup>4-1</sup>).</p>"
             "<p>For full-/low-speed interrupt endpoints, the value of "
             "this field may be from 1 to 255.</p>"
             "<p>For high-speed interrupt endpoints, the bInterval value "
             "is used as the exponent for a 2<sup>bInterval-1</sup> value; e.g., a "
             "<i>bInterval</i> of 4 means a period of 8 (2<sup>4-1</sup>). This value "
             "must be from 1 to 16.</p>"
             "<p>For high-speed bulk/control OUT endpoints, the "
             "<i>bInterval</i> must specify the maximum NAK rate of the "
             "endpoint. A value of 0 indicates the endpoint never "
             "NAKs. Other values indicate at most 1 NAK each "
             "<i>bInterval</i> number of microframes. This value must be "
             "in the range from 0 to 255.</p>"
             "<p>See Chapter 5 description of periods for more detail.</p>"
             "<h3>USB 3.2</h3>"
             "<p>Interval for servicing the endpoint for data transfers. "
             "Expressed in 125 µs units.</p>"
             "<p>For Enhanced SuperSpeed isochronous and interrupt "
             "endpoints, this value shall be in the range from 1 to 16. "
             "However, the valid ranges are 8 to 16 for Notification type "
             "Interrupt endpoints. The bInterval value is used as the "
             "exponent for a 2<sup>(bInterval-1)</sup> value; e.g., a bInterval of 4 means a "
             "period of 8 (2<sup>(4-1)</sup> → 23 → 8).</p>"
             "<p>This field is reserved and shall not be used for Enhanced "
             "SuperSpeed bulk or control endpoints.</p>"},
            {"bRefresh", "<p>For AudioStream Isochronous Synch Endpoint Descriptor</p>"
             "<p>This field indicates the rate at which an "
             "isochronous synchronization pipe "
             "provides new synchronization feedback "
             "data. This rate must be a power of 2, "
             "therefore only the power is reported back "
             "and the range of this field is from 1 "
             "(2 ms) to 9 (512 ms).<p>"},
            {"bSynchAddress", "<p>For AudioStream Isochronous Audio Data Endpoint Descriptor</p>"
             "<p>The address of the endpoint used to "
             "communicate synchronization information "
             "if required by this endpoint. Reset to zero "
             "if no synchronization pipe is used.</p>"}
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
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
        return tr("Endpoint %1 %2").arg(_bEndpointAddress & 0x0F).arg(strEndpointDirection(direction()));
    }

    qulonglong UsbEndpointDescriptor::realInterval() const
    {
        UsbSpeed speed = interfaceDescriptor()->interface()->configurationDescriptor()->device()->speed();
        EndpointTransferType type = transferType();
        qulonglong unit = 1000;
        if (speed >= UsbSpeed::HIGH)
            unit = 125;

        if ((type == EndpointTransferType::ISOCHRONOUS && speed >= UsbSpeed::FULL) ||
            (type == EndpointTransferType::INTERRUPT && speed >= UsbSpeed::HIGH))
            return qulonglong(pow(2, _bInterval - 1) + 0.5) * unit;
        else
            return _bInterval * unit;
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
        UsbHtmlBuilder builder;
        builder.start(tr("Endpoint Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "ENDPOINT")
                .attr("bEndpointAddress", _bEndpointAddress, endpointAddressInfo())
                .attr("bmAttributes", _bmAttributes, bmAttributesInfo())
                .attr("wMaxPacketSize", _wMaxPacketSize, __strWMaxPacketSize())
                .attr("bInterval", _bInterval, __strBInterval());

        // Only for AudioStream Interface
        if (_interfaceDescriptor->bInterfaceClass() == 0x01 && _interfaceDescriptor->bInterfaceSubClass() == 0x02)
            builder.attr("bRefresh", _bRefresh)
                    .attr("bSynchAddress", _bSynchAddress);

        return builder.end().build();
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
                                .arg(QString("packet %1").arg(i),
                                     QString("status %1").arg(usb_error_name(status))));
                        if (status == libusb_transfer_status::LIBUSB_TRANSFER_COMPLETED ||
                            status == libusb_transfer_status::LIBUSB_TRANSFER_OVERFLOW)
                        {

                            log().d("UsbEndpointDescriptor",
                                    tr("Isochronous transfer result: %1, %2.")
                                    .arg(QString("packet %1").arg(i),
                                         QString("actual_length %1").arg(desc->actual_length)));
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
                            .arg(QString("isError %1").arg(isError),
                                 QString("length %1").arg(data.length())));
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

    QString UsbEndpointDescriptor::__strWMaxPacketSize() const
    {
        return tr("%1 byte(s), %2 transaction(s) per microframe")
                .arg(CUT(_wMaxPacketSize, 0, 10))
                .arg(CUT(_wMaxPacketSize, 11, 12) + 1);
    }

    QString UsbEndpointDescriptor::__strBInterval() const
    {
        UsbSpeed speed = interfaceDescriptor()->interface()->configurationDescriptor()->device()->speed();
        EndpointTransferType type = transferType();
        qulonglong us = realInterval();
        QString interval;

        if (us % 1000 == 0)
            interval = tr("%1 ms").arg(us / 1000);
        else
            interval = tr("%1 µs").arg(us);

        if (speed == UsbSpeed::HIGH && (type == EndpointTransferType::BULK || type == EndpointTransferType::CONTROL))
        {
            if (_bInterval == 0)
                return tr("never NAKs");
            else
                return tr("most 1 NAK each %1").arg(interval);
        }

        if (_bInterval == 0)
            return tr("Reserved");

        return interval;
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
