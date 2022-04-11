#ifndef USBCONFIGURATIONDESCRIPTOR_H
#define USBCONFIGURATIONDESCRIPTOR_H

#include <libusb.h>
#include <QObject>
#include <QVector>

namespace usb {
    class UsbDevice;
    class UsbInterface;
}
#ifndef USBDEVICE_H
#include "usbdevice.h"
#endif
#ifndef USBINTERFACE_H
#include "usbinterface.h"
#endif

namespace usb {

    /**
     * @brief The UsbConfigurationDescriptor class
     * C++ class wrapper of libusb_config_descriptor
     */
    class UsbConfigurationDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbConfigurationDescriptor(const libusb_config_descriptor *desc, UsbDevice *parent = nullptr);

        /**
         * @brief bLength
         * @return size of this descriptor (in bytes)
         */
        uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_CONFIG in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief wTotalLength
         * @return total length of data returned for this configuration
         */
        uint16_t wTotalLength() const;

        /**
         * @brief bNumInterfaces
         * @return number of interfaces supported by this configuration
         */
        uint8_t bNumInterfaces() const;

        /**
         * @brief bConfigurationValue
         * @return identifier value for this configuration
         */
        uint8_t bConfigurationValue() const;

        /**
         * @brief iConfiguration
         * @return index of string descriptor describing this configuration
         * @see UsbDevice::getStringDescriptor()
         */
        uint8_t iConfiguration() const;

        /**
         * @brief bmAttributes
         * @return configuration characteristics
         */
        uint8_t bmAttributes() const;

        /**
         * @brief MaxPower
         * @return maximum power consumption of the USB device from this bus in this
         * configuration when the device is fully operation.
         */
        uint8_t MaxPower() const;

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
         * @brief interface
         * @param index
         * the index of interface, must less than bNumInterfaces.
         * @return the specified interface
         */
        UsbInterface *interface(int index) const;

        /**
         * @brief device
         * @return the device which current configuration descirptor belongs to
         */
        UsbDevice *device() const;

    signals:

    private:
        uint8_t _bLength, _bDescriptorType, _bNumInterfaces, _bConfigurationValue, _iConfiguration, _bmAttributes, _MaxPower;
        uint16_t _wTotalLength;
        QVector<UsbInterface *> _interface;
        const unsigned char *_extra;
        int _extra_length;

        UsbDevice *_device;
    };
}
#endif // USBCONFIGURATIONDESCRIPTOR_H
