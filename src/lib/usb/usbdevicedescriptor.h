#ifndef USBDEVICEDESCRIPTOR_H
#define USBDEVICEDESCRIPTOR_H

#include <libusb.h>
#include <QObject>

namespace usb{
    class UsbDevice;
}
#ifndef USBDEVICE_H
#include "usbdevice.h"
#endif

namespace usb{
    /**
     * @brief The UsbDeviceDescriptor class
     * C++ class wrapper of libusb_device_descriptor
     */
    class UsbDeviceDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbDeviceDescriptor(const libusb_device_descriptor *desc, UsbDevice *parent = nullptr);

        /**
         * @brief bLength
         * @return size of this descriptor (in bytes)
         */
        uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_DEVICE in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief bcdUSB
         * @return USB specification release number in binary-coded decimal
         */
        uint8_t bcdUSB() const;

        /**
         * @brief bDeviceClass
         * @return USB-IF class code for the device
         * @see libusb_class_code
         */
        uint8_t bDeviceClass() const;

        /**
         * @brief bDeviceSubClass
         * @return USB-IF subclass code for the device
         */
        uint8_t bDeviceSubClass() const;

        /**
         * @brief bDeviceProtocol
         * @return USB-IF protocol code for the device
         */
        uint8_t bDeviceProtocol() const;

        /**
         * @brief bMaxPacketSize0
         * @return maximum packet size for endpoint 0
         */
        uint8_t bMaxPacketSize0() const;

        /**
         * @brief idVendor
         * @return USB-IF vendor ID
         */
        uint16_t idVendor() const;

        /**
         * @brief idProduct
         * @return USB-IF product ID
         */
        uint16_t idProduct() const;

        /**
         * @brief bcdDevice
         * @return device release number in binary-coded decimal
         */
        uint16_t bcdDevice() const;

        /**
         * @brief iManufacturer
         * @return index of string descriptor describing manufacturer
         * @see UsbDevice::getStringDescriptor()
         */
        uint8_t iManufacturer() const;

        /**
         * @brief iProduct
         * @return index of string descriptor describing product
         * @see UsbDevice::getStringDescriptor()
         */
        uint8_t iProduct() const;

        /**
         * @brief iSerialNumber
         * @return index of string descriptor containing device serial number
         */
        uint8_t iSerialNumber() const;

        /**
         * @brief bNumConfigurations
         * @return number of possible configurations
         */
        uint8_t bNumConfigurations() const;

        /**
         * @brief device
         * @return the device which current device descirptor belongs to
         */
        UsbDevice *device() const;

        /**
         * @brief vendorName
         * @return the defined vendor name associated with idVendor
         */
        const QString &vendorName() const;

        /**
         * @brief productName
         * @return the defined product name associated with idProduct
         */
        const QString &productName() const;

    signals:

    private:
        uint8_t _bLength, _bDescriptorType, _bcdUSB, _bDeviceClass, _bDeviceSubClass, _bDeviceProtocol,
        _bMaxPacketSize0, _iManufacturer, _iProduct, _iSerialNumber, _bNumConfigurations;
        uint16_t _idVendor, _idProduct, _bcdDevice;
        UsbDevice *_device;
        QString _vendorName, _productName;
    };
}

#endif // USBDEVICEDESCRIPTOR_H
