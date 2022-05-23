﻿/*! C++ class wrapper of libusb_endpoint_descriptor

 * Copyright (C) 2022 Nichts Hsu

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

namespace usb {
    class UsbInterfaceDescriptor;
}
#ifndef USBINTERFACEDESCRIPTOR_H
#include "usbinterfacedescriptor.h"
#endif

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
     * @brief The EndpointUsageType enum
     * @see UsbEndpointDescriptor::usageType()
     */
    enum class EndpointUsageType {
        DATA = LIBUSB_ISO_USAGE_TYPE_DATA,
        FEEDBACK = LIBUSB_ISO_USAGE_TYPE_FEEDBACK,
        IMPLICIT = LIBUSB_ISO_USAGE_TYPE_IMPLICIT,
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
     * @brief EndpointUsageType
     * @param string description of EndpointUsageType
     */
    QString strEndpointUsageType(EndpointUsageType type);

    /**
     * @brief parseEndpointDescBmAttributes
     * Parse bmAttributes of endpoint descriptor to string
     * @param bmAttributes
     * @return the string description of bmAttributes
     * @note
     * D1..0    Transfer Type
     * D3..2    Synchronization Type (For isochronous transfer only)
     * D5..4    Usage Type (For isochronous transfer only)
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
         * @brief usageType
         * @return the endpoint usage type
         * @note only used for isochronous transfer type
         */
        EndpointUsageType usageType() const;

        /**
         * @brief bmAttributesInfo
         * Wrapper of parseEndpointDescBmAttributes()
         * @see parseEndpointDescBmAttributes
         */
        QString bmAttributesInfo();

        /**
         * @brief endpointAddressInfo
         * @return string description of endpoint address
         */
        QString endpointAddressInfo();

        /**
         * @brief interfaceDescriptor
         * @return the interface descriptor which current endpoint descriptor is belong to
         */
        UsbInterfaceDescriptor *interfaceDescriptor() const;

        /**
         * @brief transfer
         * Read data to buffer or write data from buffer.
         * @param buffer
         * The buffer that carries data.
         * @note
         * Buffers must be pre-allocated because its raw pointers will be used in transfers.
         * @param realSize
         * Real read/write size.
         * @param timeout
         * Timeout to wait.
         * @return
         */
        int transfer(QByteArray &buffer, int &realSize, unsigned int timeout);

    signals:

    private:
        uint8_t _bLength, _bDescriptorType, _bEndpointAddress, _bmAttributes, _bInterval, _bRefresh, _bSynchAddress;
        uint16_t _wMaxPacketSize;
        const unsigned char *_extra;
        int _extra_length;
        UsbInterfaceDescriptor *_interfaceDescriptor;
    };
}
#endif // USBENDPOINTDESCRIPTOR_H
