#ifndef USBINTERFACEDESCRIPTOR_H
#define USBINTERFACEDESCRIPTOR_H

#include <libusb.h>
#include <QObject>

namespace usb {
    class UsbInterface;
    class UsbEndpointDescriptor;
}
#ifndef USBENDPOINTDESCRIPTOR_H
#include "usbendpointdescriptor.h"
#endif
#ifndef USBINTERFACE_H
#include "usbinterface.h"
#endif
#include "log/logger.h"

namespace usb {

    /**
     * @brief The UsbInterfaceDescriptor class
     * C++ class wrapper of libusb_interface_descriptor
     */
    class UsbInterfaceDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbInterfaceDescriptor(const libusb_interface_descriptor *desc, UsbInterface *parent = nullptr);

        /**
         * @brief bLength
         * @return size of this descriptor (in bytes)
         */
        uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_INTERFACE in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief bInterfaceNumber
         * @return number of this interface
         */
        uint8_t bInterfaceNumber() const;

        /**
         * @brief bAlternateSetting
         * @return value used to select this alternate setting for this interface
         */
        uint8_t bAlternateSetting() const;

        /**
         * @brief bNumEndpoints
         * @return number of endpoints used by this interface
         * @note excluding the control endpoint
         */
        uint8_t bNumEndpoints() const;

        /**
         * @brief bInterfaceClass
         * @return USB-IF class code for this interface
         * @see libusb_class_code
         */
        uint8_t bInterfaceClass() const;

        /**
         * @brief bInterfaceSubClass
         * @return USB-IF subclass code for this interface
         */
        uint8_t bInterfaceSubClass() const;

        /**
         * @brief bInterfaceProtocol
         * @return USB-IF protocol code for this interface
         */
        uint8_t bInterfaceProtocol() const;

        /**
         * @brief iInterface
         * @return index of string descriptor describing this interface
         */
        uint8_t iInterface() const;

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
         * @brief endpoint
         * @param index
         * the index of endpoint which want to get, must less than bNumEndpoints
         * @return the specified endpoint
         */
        UsbEndpointDescriptor *endpoint(int index) const;

        /**
         * @brief interface
         * @return the interface which current interface descriptor is belong to
         */
        UsbInterface *interface() const;

    signals:

    private:
        uint8_t _bLength, _bDescriptorType, _bInterfaceNumber, _bAlternateSetting, _bNumEndpoints, _bInterfaceClass,
        _bInterfaceSubClass, _bInterfaceProtocol, _iInterface;
        QVector<UsbEndpointDescriptor *> _endpoint;
        const unsigned char *_extra;
        int _extra_length;
        UsbInterface *_interface;
    };
}

#endif // USBINTERFACEDESCRIPTOR_H
