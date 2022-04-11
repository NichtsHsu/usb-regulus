#ifndef USBENDPOINTDESCRIPTOR_H
#define USBENDPOINTDESCRIPTOR_H

#include <libusb.h>
#include <QObject>

namespace usb {

    /**
     * @brief The UsbEndpointDescriptor class
     * C++ class wrapper of libusb_endpoint_descriptor
     */
    class UsbEndpointDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbEndpointDescriptor(const libusb_endpoint_descriptor *desc, QObject *parent = nullptr);

        /**
         * @brief The Direction enum
         * @see UsbEndpointDescriptor::direction()
         */
        enum Direction {
            OUT = LIBUSB_ENDPOINT_OUT,
            IN = LIBUSB_ENDPOINT_IN,
        };

        /**
         * @brief The TransferType enum
         * @see UsbEndpointDescriptor::transferType()
         */
        enum TransferType {
            CONTROL = LIBUSB_TRANSFER_TYPE_CONTROL,
            ISOCHRONOUS = LIBUSB_TRANSFER_TYPE_ISOCHRONOUS,
            BULK = LIBUSB_TRANSFER_TYPE_BULK,
            INTERRUPT = LIBUSB_TRANSFER_TYPE_INTERRUPT,
        };

        /**
         * @brief The SyncType enum
         * @see UsbEndpointDescriptor::syncType()
         */
        enum SyncType {
            NONE = LIBUSB_ISO_SYNC_TYPE_NONE,
            ASYNC = LIBUSB_ISO_SYNC_TYPE_ASYNC,
            ADAPTIVE = LIBUSB_ISO_SYNC_TYPE_ADAPTIVE,
            SYNC = LIBUSB_ISO_SYNC_TYPE_SYNC,
        };

        /**
         * @brief The UsageType enum
         * @see UsbEndpointDescriptor::usageType()
         */
        enum UsageType {
            DATA = LIBUSB_ISO_USAGE_TYPE_DATA,
            FEEDBACK = LIBUSB_ISO_USAGE_TYPE_FEEDBACK,
            IMPLICIT = LIBUSB_ISO_USAGE_TYPE_IMPLICIT,
        };

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
        Direction direction() const;

        /**
         * @brief transferType
         * @return the endpoint transfer type
         */
        TransferType transferType() const;

        /**
         * @brief syncType
         * @return the endpoint sync type
         * @note only used for isochronous transfer type
         */
        SyncType syncType() const;

        /**
         * @brief usageType
         * @return the endpoint usage type
         * @note only used for isochronous transfer type
         */
        UsageType usageType() const;

    signals:

    private:
        uint8_t _bLength, _bDescriptorType, _bEndpointAddress, _bmAttributes, _bInterval, _bRefresh, _bSynchAddress;
        const unsigned char *_extra;
        int _extra_length;
    };
}
#endif // USBENDPOINTDESCRIPTOR_H
