/*! C++ class wrapper of USB Endpoint Descriptor

 * Copyright (C) 2022-2023 Nichts Hsu

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef USBENDPOINTDESCRIPTOR_H
#define USBENDPOINTDESCRIPTOR_H

#include <libusb.h>
#include <QObject>
#include <QTimer>

namespace usb {
    class UsbInterfaceDescriptor;
}

#include <log/logger.h>
namespace usb {
    /**
     * @brief The EndpointDirection enum
     * @see UsbEndpointDescriptor::direction()
     */
    enum class EndpointDirection {
        OUT = LIBUSB_ENDPOINT_OUT,
        IN = LIBUSB_ENDPOINT_IN,
    };

    /**
     * @brief The EndpointTransferType enum
     * @see UsbEndpointDescriptor::transferType()
     */
    enum class EndpointTransferType {
        CONTROL = LIBUSB_TRANSFER_TYPE_CONTROL,
        ISOCHRONOUS = LIBUSB_TRANSFER_TYPE_ISOCHRONOUS,
        BULK = LIBUSB_TRANSFER_TYPE_BULK,
        INTERRUPT = LIBUSB_TRANSFER_TYPE_INTERRUPT,
    };

    /**
     * @brief The EndpointSyncType enum
     * @see UsbEndpointDescriptor::syncType()
     */
    enum class EndpointSyncType {
        NONE = LIBUSB_ISO_SYNC_TYPE_NONE,
        ASYNCHRONOUS = LIBUSB_ISO_SYNC_TYPE_ASYNC,
        ADAPTIVE = LIBUSB_ISO_SYNC_TYPE_ADAPTIVE,
        SYNCHRONOUS = LIBUSB_ISO_SYNC_TYPE_SYNC,
    };

    /**
     * @brief The EndpointIsocUsageType enum
     * @see UsbEndpointDescriptor::isochronousUsageType()
     */
    enum class EndpointIsochronousUsageType {
        DATA = LIBUSB_ISO_USAGE_TYPE_DATA,
        FEEDBACK = LIBUSB_ISO_USAGE_TYPE_FEEDBACK,
        IMPLICIT = LIBUSB_ISO_USAGE_TYPE_IMPLICIT,
    };

    /**
     * @brief The EndpointInterruptUsageType enum
     * @see UsbEndpointDescriptor::interruptUsageType()
     */
    enum class EndpointInterruptUsageType {
        PERIODIC = 0,
        NOTIFICATION = 1,
    };

    /**
     * @brief strEndpointDirection
     * @return string description of EndpointDirection
     */
    QString strEndpointDirection(EndpointDirection direction);

    /**
     * @brief strEndpointTransferType
     * @param string description of EndpointTransferType
     */
    QString strEndpointTransferType(EndpointTransferType type);

    /**
     * @brief strEndpointSyncType
     * @param string description of strEndpointSyncType
     */
    QString strEndpointSyncType(EndpointSyncType type);

    /**
     * @brief strEndpointIsochronousUsageType
     * @param string description of EndpointIsochronousUsageType
     */
    QString strEndpointIsochronousUsageType(EndpointIsochronousUsageType type);

    /**
     * @brief strEndpointInterruptUsageType
     * @param string description of EndpointInterruptUsageType
     */
    QString strEndpointInterruptUsageType(EndpointInterruptUsageType type);

    /**
     * @brief parseEndpointDescBmAttributes
     * Parse bmAttributes of endpoint descriptor to string
     * @param bmAttributes
     * @return the string description of bmAttributes
     * @note
     * D1..0    Transfer Type
     * D3..2    Synchronization Type (For isochronous transfer only)
     * D5..4    Usage Type (For interrupt/isochronous transfer only)
     */
    QString parseEndpointDescBmAttributes(uint8_t bmAttributes);

    /**
     * @brief The UsbEndpointDescriptor class
     * C++ class wrapper of libusb_endpoint_descriptor
     */
    class UsbEndpointDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbEndpointDescriptor(const libusb_endpoint_descriptor *desc, UsbInterfaceDescriptor *parent = nullptr);

        /**
         * @brief bLength
         * @return size of this descriptor (in bytes)
         */
        uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_ENDPOINT in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief bEndpointAddress
         * @return the address of the endpoint described by this descriptor
         */
        uint8_t bEndpointAddress() const;

        /**
         * @brief bmAttributes
         * @return attributes which apply to the endpoint when it is configured using
         * the bConfigurationValue
         */
        uint8_t bmAttributes() const;

        /**
         * @brief bInterval
         * @return interval for polling endpoint for data transfers
         * @see realInterval()
         */
        uint8_t bInterval() const;

        /**
         * @brief bRefresh
         * @return the rate at which synchronization feedback is provided
         * @note For audio devices only
         */
        uint8_t bRefresh() const;

        /**
         * @brief bSynchAddress
         * @return the address if the synch endpoint
         * @note For audio devices only
         */
        uint8_t bSynchAddress() const;

        /**
         * @brief wMaxPacketSize
         * @return maximum packet size this endpoint
         */
        uint16_t wMaxPacketSize() const;

        /**
         * @brief extra
         * @return extra descriptors
         */
        const unsigned char *extra() const;

        /**
         * @brief extra_length
         * @return length of the extra descriptors, in bytes
         */
        int extra_length() const;

        /**
         * @brief getFieldNames
         * @return all field names in a string list
         */
        static const QStringList &getFieldNames();

        /**
         * @brief getFieldInformation
         * Get field information
         * @param field name
         * @return field information almost from USB specs
         */
        static QString getFieldInformation(const QString &field);

        /**
         * @brief direction
         * @return the endpoint direction IN (device to host) or OUT (host to device)
         */
        EndpointDirection direction() const;

        /**
         * @brief transferType
         * @return the endpoint transfer type
         */
        EndpointTransferType transferType() const;

        /**
         * @brief syncType
         * @return the endpoint sync type
         * @note only used for isochronous transfer type
         */
        EndpointSyncType syncType() const;

        /**
         * @brief isochronousUsageType
         * @return the endpoint usage type
         * @note only used for isochronous transfer type
         */
        EndpointIsochronousUsageType isochronousUsageType() const;

        /**
         * @brief interruptUsageType
         * @return the endpoint usage type
         * @note only used for interrupt transfer type
         */
        EndpointInterruptUsageType interruptUsageType() const;

        /**
         * @brief bmAttributesInfo
         * Wrapper of parseEndpointDescBmAttributes()
         * @see parseEndpointDescBmAttributes
         */
        QString bmAttributesInfo() const;

        /**
         * @brief endpointAddressInfo
         * @return string description of endpoint address
         */
        QString endpointAddressInfo() const;

        /**
         * @brief realInterval
         * The bInterval values have different units under different USB speed and transfer types.
         * They are:
         *      125 µs as unit under High-Speed & SuperSpeed;
         *      1ms as unit under Low-Speed & Full-Speed;
         *      2^(bInterval-1)*unit if isochronous transfer under Full/High/Super Speed
         *          and interrupt transfer under High/Super Speed.
         * @return the real interval value in µs
         */
        qulonglong realInterval() const;

        /**
         * @brief interfaceDescriptor
         * @return the interface descriptor which current endpoint descriptor is belong to
         */
        UsbInterfaceDescriptor *interfaceDescriptor() const;

        /**
         * @brief syncTransfer
         * Synchronously read data to buffer or write data from buffer.
         * Only interrupt transfer and bulk transfer can use synchronous transfer.
         * @see startAsyncTransfer() for asynchronous transfer
         * @param buffer
         * The buffer who carries data.
         * @note
         * Buffer must be pre-allocated because its raw pointers will be used in transfers.
         * @param realSize
         * Real read/write size.
         * @param timeout
         * Timeout to wait. Zero for unlimited timeout.
         * @return error code
         * @note
         * Control transfer should not be happening here
         * @see UsbDevice::controlTransfer()
         */
        int syncTransfer(QByteArray &buffer, int &realSize, unsigned int timeout = 0);

        /**
         * @brief startAsyncTransfer
         * Start an asychronous transfer without blocking
         * @see asyncTransferCompleted() for the received data
         * @see asyncTransferFailed() for transfer failure
         * @see cancelAsyncTransfer() for cancel aynchronous transfer
         * @see syncTransfer() for synchronous transfer
         * @param buffer
         * The buffer to write. Keep emtpy if reading data.
         * @param readLength
         * Set the reading buffer length.
         * If isochronous transfer, set 0 to indicate the length of one isochronous packet.
         * Else, set to 0 indicate the wMaxPacketSize.
         * @note
         * If isochronous transfer, Read length will be raised to multiples of
         * the max isochronous packet size (not wMaxPacketSize).
         * @param timeout
         * Timeout to wait. Zero for unlimited timeout.
         * @return error code
         */
        int startAsyncTransfer(const QByteArray &buffer = QByteArray(), int readLength = 0, unsigned int timeout = 0);

        /**
         * @brief cancelAsyncTransfer
         * @return error code
         * @see asyncTransferCancelled()
         */
        int cancelAsyncTransfer();

        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        QString infomationToHtml() const;

    signals:
        /**
         * @brief asyncTransferCompleted
         * Emit when asynchronous transfer is completed
         * @see startAsyncTransfer()
         * @param data
         * Received data if an IN endpoint
         */
        void asyncTransferCompleted(const QByteArray &data = QByteArray());

        /**
         * @brief asyncTransferFailed
         * Emit when asynchronous transfer is failed
         * @param code
         * Error code
         */
        void asyncTransferFailed(int code);

        /**
         * @brief asyncTransferCanceled
         * Emit when asynchronous transfer is canceled
         * @see cancelAsyncTransfer()
         */
        void asyncTransferCancelled();

    private slots:
        void __requestExtraDescriptor();

    private:
        static void LIBUSB_CALL __asyncTransferCallback(struct libusb_transfer *transfer);
        QString __strWMaxPacketSize() const;
        QString __strBInterval() const;

        uint8_t _bLength, _bDescriptorType, _bEndpointAddress, _bmAttributes, _bInterval, _bRefresh, _bSynchAddress;
        uint16_t _wMaxPacketSize;
        const unsigned char *_extra;
        int _extraLength;
        UsbInterfaceDescriptor *_interfaceDescriptor;
        libusb_transfer *_transfer;
    };
}

#ifndef USBINTERFACEDESCRIPTOR_H
#include "usbinterfacedescriptor.h"
#endif

#endif // USBENDPOINTDESCRIPTOR_H
