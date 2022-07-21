#include "usbdevicedescriptor.h"
#include "__usbmacro.h"

namespace usb {
    UsbDeviceDescriptor::UsbDeviceDescriptor(const libusb_device_descriptor *desc, UsbDevice *parent) :
        QObject(parent), _device(parent), _hubDescriptor(nullptr)
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
        _iProduct = desc->iProduct;
        _iSerialNumber = desc->iSerialNumber;
        _bNumConfigurations = desc->bNumConfigurations;

#ifdef Q_OS_UNIX
        /* Get class, subclass, protocol string */
        {
            char deviceClass[128], deviceSubClass[128], deviceProtocol[128];
            memset(deviceClass, '\0', sizeof(deviceClass));
            memset(deviceSubClass, '\0', sizeof(deviceSubClass));
            memset(deviceProtocol, '\0', sizeof(deviceProtocol));
            get_class_string(deviceClass, sizeof(deviceClass), _bDeviceClass);
            get_subclass_string(deviceSubClass, sizeof(deviceSubClass), _bDeviceClass, _bDeviceSubClass);
            get_protocol_string(deviceProtocol, sizeof(deviceProtocol),
                                _bDeviceClass, _bDeviceSubClass, _bDeviceProtocol);
            _deviceClass = deviceClass;
            _deviceSubClass = deviceSubClass;
            _deviceProtocol = deviceProtocol;
        }

        /* Get vendor name and product name */
        {
            char vendorName[128], productName[128], manufacturer[128], product[128], sysfs_name[PATH_MAX];
            bool hasVendor, hasProduct;
            memset(vendorName, '\0', sizeof(vendorName));
            memset(productName, '\0', sizeof(productName));
            hasVendor = !!get_vendor_string(vendorName, sizeof(vendorName), _idVendor);
            hasProduct = !!get_product_string(productName, sizeof(productName), _idVendor, _idProduct);
            if (get_sysfs_name(sysfs_name, sizeof(sysfs_name), parent->device()) >= 0) {
                if (!hasVendor)
                    read_sysfs_prop(manufacturer, sizeof(manufacturer), sysfs_name,
                                    "manufacturer");
                if (!hasProduct)
                    read_sysfs_prop(product, sizeof(product), sysfs_name,
                                    "product");
            }
            _vendorName = hasVendor ? vendorName : "";
            _productName = productName;
            _description = ((hasVendor) ? _vendorName : (strlen(manufacturer) > 0 ? QString(manufacturer) : tr("Unknown")))
                    + (hasProduct ? (QString(" ") + _productName) : (strlen(product) > 0 ? QString(" ") + QString(product): QString()));
        }
#endif

        /* Hub Device */
        if (_bDeviceClass == 0x09)
            QTimer::singleShot(200, this, &UsbDeviceDescriptor::__requestHubDescriptor);
    }

    uint8_t UsbDeviceDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbDeviceDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint16_t UsbDeviceDescriptor::bcdUSB() const
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

    const QString &UsbDeviceDescriptor::description() const
    {
        return _description;
    }

    const QString UsbDeviceDescriptor::bcdUSBInfo() const
    {
        return parseBcdUSB(_bcdUSB);
    }

    const QString &UsbDeviceDescriptor::deviceClass() const
    {
        return _deviceClass;
    }

    const QString &UsbDeviceDescriptor::deviceSubClass() const
    {
        return _deviceSubClass;
    }

    const QString &UsbDeviceDescriptor::deviceProtocol() const
    {
        return _deviceProtocol;
    }

    QString UsbDeviceDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Device Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
        ATTR("bcdUSB", _bcdUSB, bcdUSBInfo());
        ATTR("bcdDevice", _bcdDevice, "");
        ATTR("bDeviceClass", _bDeviceClass, _deviceClass);
        ATTR("bDeviceSubClass", _bDeviceSubClass, _deviceSubClass);
        ATTR("bDeviceProtocol", _bDeviceProtocol, _deviceProtocol);
        ATTR("idVendor", _idVendor, _vendorName);
        ATTR("idProduct", _idProduct, _productName);
        ATTRSTRDESC("iManufacturer", _iManufacturer, _device);
        ATTRSTRDESC("iProduct", _iProduct, _device);
        ATTRSTRDESC("iSerialNumber", _iSerialNumber, _device);
        ATTR("bMaxPacketSize0", _bMaxPacketSize0, _bMaxPacketSize0);
        ATTR("bNumConfigurations", _bNumConfigurations, _bNumConfigurations);
        END;
        if (_hubDescriptor)
            APPEND(_hubDescriptor);

        return html;
    }

    QString parseBcdUSB(uint16_t bcdUSB)
    {
        static const QMap<uint16_t, QString> bcdUSBMap = {
            { 0x0100, "USB Specification verion 1.0" },
            { 0x0110, "USB Specification verion 1.1" },
            { 0x0200, "USB Specification verion 2.0" },
            { 0x0210, "USB Specification verion 2.0 with BOS support" },
            { 0x0250, "Wireless USB (obsolete)" },
            { 0x0300, "USB Specification verion 3.0" },
            { 0x0310, "USB Specification verion 3.1" },
            { 0x0320, "USB Specification verion 3.2" },
        };

        /* USB specification defined the bcdUSB should be **at least**
         * 0x201H (USB 2.0 spec) or 0x210H (USB 3.2 spec)  to indicate
         * that the device supports the request to read the BOS Descriptor.
         * Even though almost all vendors use 0x210H as bcdUSB,
         * we should still respect the specification's definitions.
         */
        if (bcdUSB >= 0x0201 && bcdUSB < 0x0300 && bcdUSB != 0x0250)
            bcdUSB= 0x0210;

        if (bcdUSBMap.contains(bcdUSB))
            return bcdUSBMap[bcdUSB];

        uint8_t major = (bcdUSB & 0xFF00) >> 8;
        uint8_t minor = (bcdUSB & 0x00F0) >> 4;
        uint8_t subminor = bcdUSB & 0x000F;
        return QString("USB %1.%2%3").arg(major).arg(minor).arg(subminor);
    }


    void UsbDeviceDescriptor::__requestHubDescriptor()
    {
        _hubDescriptor = UsbHubDescriptor::tryGet(this);
    }

    UsbHubDescriptor *UsbDeviceDescriptor::hubDescriptor() const
    {
        return _hubDescriptor;
    }
}
