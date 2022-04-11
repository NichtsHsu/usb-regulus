#include "usbdevicedescriptor.h"
#include <usbutils/names.h>
#include <usbutils/sysfs.h>

namespace usb {
    UsbDeviceDescriptor::UsbDeviceDescriptor(const libusb_device_descriptor *desc, UsbDevice *parent) : QObject(parent), _device(parent)
    {
        _bLength = desc->bLength;
        _bDescriptorType = desc->bDescriptorType;
        _bcdUSB = desc->bcdUSB;
        _bDeviceClass = desc->bDeviceClass;
        _bDeviceSubClass = desc->bDeviceSubClass;
        _bDeviceProtocol = desc->bDeviceProtocol;
        _bMaxPacketSize0 = desc->bMaxPacketSize0;
        _idVendor = desc->idVendor;
        _idProduct = desc->idProduct;
        _bcdDevice = desc->bcdDevice;
        _iManufacturer = desc->iManufacturer;
        _iProduct = desc->idProduct;
        _iSerialNumber = desc->iSerialNumber;
        _bNumConfigurations = desc->bNumConfigurations;

        // Get vendor name and product name
        char vendorName[128], productName[128], sysfs_name[PATH_MAX];;
        bool hasVendor, hasProduct;
        memset(vendorName, '\0', 128);
        memset(productName, '\0', 128);
        hasVendor = !!get_vendor_string(vendorName, 128, _idVendor);
        hasProduct = !!get_product_string(productName, 128, _idVendor, _idProduct);
        if (get_sysfs_name(sysfs_name, sizeof(sysfs_name), parent->device()) >= 0) {
            if (!hasVendor)
                read_sysfs_prop(vendorName, 128, sysfs_name,
                                "manufacturer");
            if (!hasProduct)
                read_sysfs_prop(productName, 128, sysfs_name,
                                "product");
        }
        _vendorName = strlen(vendorName) > 0 ? vendorName : "Unknown";
        _productName = productName;
    }

    uint8_t UsbDeviceDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbDeviceDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint8_t UsbDeviceDescriptor::bcdUSB() const
    {
        return _bcdUSB;
    }

    uint8_t UsbDeviceDescriptor::bDeviceClass() const
    {
        return _bDeviceClass;
    }

    uint8_t UsbDeviceDescriptor::bDeviceSubClass() const
    {
        return _bDeviceSubClass;
    }

    uint8_t UsbDeviceDescriptor::bDeviceProtocol() const
    {
        return _bDeviceProtocol;
    }

    uint8_t UsbDeviceDescriptor::bMaxPacketSize0() const
    {
        return _bMaxPacketSize0;
    }

    uint16_t UsbDeviceDescriptor::idVendor() const
    {
        return _idVendor;
    }

    uint16_t UsbDeviceDescriptor::idProduct() const
    {
        return _idProduct;
    }

    uint16_t UsbDeviceDescriptor::bcdDevice() const
    {
        return _bcdDevice;
    }

    uint8_t UsbDeviceDescriptor::iManufacturer() const
    {
        return _iManufacturer;
    }

    uint8_t UsbDeviceDescriptor::iProduct() const
    {
        return _iProduct;
    }

    uint8_t UsbDeviceDescriptor::iSerialNumber() const
    {
        return _iSerialNumber;
    }

    uint8_t UsbDeviceDescriptor::bNumConfigurations() const
    {
        return _bNumConfigurations;
    }

    UsbDevice *UsbDeviceDescriptor::device() const
    {
        return _device;
    }

    const QString &UsbDeviceDescriptor::vendorName() const
    {
        return _vendorName;
    }

    const QString &UsbDeviceDescriptor::productName() const
    {
        return _productName;
    }
}
