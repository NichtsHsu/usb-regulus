#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <libusb.h>
#include <QObject>
#include <QString>

#include "log/logger.h"

namespace usb {
    class UsbDeviceDescriptor;
    class UsbConfigurationDescriptor;
}
#ifndef USBDEVICEDESCRIPTOR_H
#include "usbdevicedescriptor.h"
#endif
#ifndef USBCONFIGURATIONDESCRIPTOR_H
#include "usbconfigurationdescriptor.h"
#endif

namespace usb {
    /**
     * @brief The UsbDevice class
     * C++ class wrapper of libusb_device
     * @see libusb_device
     */
    class UsbDevice : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbDevice(libusb_device *device, QObject *parent = nullptr);
        ~UsbDevice();

        /**
         * @brief deviceDescriptor
         * @return device descriptor of this USB device
         */
        UsbDeviceDescriptor *deviceDescriptor() const;

        /**
         * @brief configurationDescriptor
         * @return configuration descriptor of this USB device
         */
        UsbConfigurationDescriptor *configurationDescriptor() const;

        /**
         * @brief handle
         * @return the handle of device
         * @warning know what you're doing before access the handle
         */
        libusb_device_handle *handle() const;

        /**
         * @brief device
         * @return the associated libusb_device of this wrapper
         */
        libusb_device *device() const;

        /**
         * @brief bus
         * @return bus number of this USB device
         * @note
         * Normally each UDC(USB Device Controller) has two bus,
         * one for USB 2.0 and one for USB 3.0.
         */
        uint8_t bus() const;

        /**
         * @brief port
         * @return port number of this USB device
         * @see UsbDevice::bus()
         */
        uint8_t port() const;

        /**
         * @brief valid
         * @return if is a valid device
         */
        bool valid() const;

        /**
         * @brief displayName
         * @return the name displayed on device tree view.
         */
        const QString &displayName() const;

        /**
         * @brief getStringDescriptor
         * @param index
         * the string descriptor index normally is shown as "iXxx",
         * such as "iInterface", "iProduct" and so on.
         * @return the string descriptor
         */
        QString getStringDescriptor(size_t index) const;

        /**
         * @brief operator ==
         * @return if idVendor, idProduct, bus and port are equal.
         */
        bool operator==(const UsbDevice &) const;
        bool operator==(libusb_device *);

    signals:

    private:
        libusb_device *_device;
        UsbDeviceDescriptor *_deviceDescriptor;
        UsbConfigurationDescriptor *_configurationDescriptor;
        libusb_device_handle *_handle;
        uint8_t _bus, _port;
        QString _displayName;

        bool _valid, _open;
    };
}

#endif // USBDEVICE_H
