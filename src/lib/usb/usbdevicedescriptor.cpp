#include "usbdevicedescriptor.h"
#include "usbhtmlbuilder.h"

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

    const QStringList &UsbDeviceDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bcdUSB",
            "bDeviceClass",
            "bDeviceSubClass",
            "bDeviceProtocol",
            "bMaxPacketSize0",
            "idVendor",
            "idProduct",
            "bcdDevice",
            "iManufacturer",
            "iProduct",
            "iSerialNumber",
            "bNumConfigurations",
        };

        return fields;
    }

    QString UsbDeviceDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of this descriptor in bytes"},
            {"bDescriptorType", "DEVICE Descriptor Type"},
            {"bcdUSB", "USB Specification Release Number in Binary-Coded "
             "Decimal (i.e., 2.10 is 210H). This field identifies "
             "the release of the USB Specification with which the "
             "device and its descriptors are compliant."},
            {"bDeviceClass", "<p>Class code (assigned by the USB-IF).</p>"
             "</p>If this field is reset to zero, each interface within a "
             "configuration specifies its own class information "
             "and the various interfaces operate independently.</p>"
             "<p>If this field is set to a value between 1 and FEH, "
             "the device supports different class specifications "
             "on different interfaces and the interfaces may not "
             "operate independently. This value identifies the "
             "class definition used for the aggregate interfaces.</p>"
             "<p>If this field is set to FFH, the device class is "
             "vendor-specific.</p>"},
            {"bDeviceSubClass", "<p>Subclass code (assigned by the USB-IF).</p>"
             "<p>These codes are qualified by the value of the "
             "<i>bDeviceClass</i> field.</p>"
             "<p>If the <i>bDeviceClass</i> field is reset to zero, this field "
             "shall also be reset to zero.</p>"
             "<p>If the <i>bDeviceClass</i> field is not set to FFH, all values "
             "are reserved for assignment by the USB-IF.</p>"},
            {"bDeviceProtocol", "<p>Protocol code (assigned by the USB-IF). These "
             "codes are qualified by the value of the "
             "<i>bDeviceClass</i> and the <i>bDeviceSubClass</i> fields. If a "
             "device supports class-specific protocols on a "
             "device basis as opposed to an interface basis, this "
             "code identifies the protocols that the device uses "
             "as defined by the specification of the device class.</p>"
             "<p>If this field is reset to zero, the device does not use "
             "class-specific protocols on a device basis. "
             "However, it may use class-specific protocols on an "
             "interface basis.</p>"
             "<p>If this field is set to FFH, the device uses a vendor-"
             "specific protocol on a device basis.</p>"},
            {"bMaxPacketSize0", "Maximum packet size for endpoint zero. The "
             "bMaxPacketSize0 value is used as the exponent for "
             "a 2<sup>bMaxPacketSize0</sup> value; e.g., a bMaxPacketSize0 of 4 "
             "means a Max Packet size of 16 (2<sup>4</sup> → 16)."
             "09H is the only valid value in this field when "
             "operating at Gen X speed."},
            {"idVendor", "Vendor ID (assigned by the USB-IF)"},
            {"idProduct", "Product ID (assigned by the manufacturer)"},
            {"bcdDevice", "Device release number in binary-coded decimal"},
            {"iManufacturer", "Index of string descriptor describing manufacturer"},
            {"iProduct", "Index of string descriptor describing product"},
            {"iSerialNumber", "Index of string descriptor describing the device’s serial number"},
            {"bNumConfigurations", "Number of possible configurations"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
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
        return UsbHtmlBuilder()
                .start(tr("Device Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DEVICE")
                .attr("bcdUSB", _bcdUSB, bcdUSBInfo())
                .attr("bcdDevice", _bcdDevice, "")
                .attr("bDeviceClass", _bDeviceClass, _deviceClass)
                .attr("bDeviceSubClass", _bDeviceSubClass, _deviceSubClass)
                .attr("bDeviceProtocol", _bDeviceProtocol, _deviceProtocol)
                .attr("idVendor", _idVendor, _vendorName)
                .attr("idProduct", _idProduct, _productName)
                .strdesc("iManufacturer", _iManufacturer, _device)
                .strdesc("iProduct", _iProduct, _device)
                .strdesc("iSerialNumber", _iSerialNumber, _device)
                .attr("bMaxPacketSize0", _bMaxPacketSize0)
                .attr("bNumConfigurations", _bNumConfigurations)
                .end()
                .append(_hubDescriptor)
                .build();
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
