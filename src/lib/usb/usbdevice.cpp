#include "usbdevice.h"

namespace usb {
    UsbDevice::UsbDevice(libusb_device *device, QObject *parent) :
        QObject(parent), _device(device), _bosDescriptor(nullptr), _bus(0), _port(0)
    {
        int ret;
        _valid = false;
        ret = libusb_open(_device, &_handle);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to open device (%1).").arg(libusb_error_name(ret)));
            _open = false;
            return;
        }
        LOGD(tr("New USB device opened."));
        _open = true;

        /* Auto attach/detach the kernel driver */
        libusb_set_auto_detach_kernel_driver(_handle, true);

        libusb_device_descriptor devDesc;
        ret = libusb_get_device_descriptor(_device, &devDesc);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to get device descriptor (%1).").arg(libusb_error_name(ret)));
            return;
        }
        _deviceDescriptor = new UsbDeviceDescriptor(&devDesc, this);

        int config;
        ret = libusb_get_configuration(_handle, &config);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to get configuration (%1).").arg(libusb_error_name(ret)));
            return;
        }

        libusb_config_descriptor *configDesc;
        ret = libusb_get_config_descriptor(_device, 0, &configDesc);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to get config descriptor (%1).").arg(libusb_error_name(ret)));
            return;
        }
        _configurationDescriptor = new UsbConfigurationDescriptor(configDesc, this);

        _speed = UsbSpeed(libusb_get_device_speed(_device));

        _valid = true;

        _bus = libusb_get_bus_number(_device);
        _port = libusb_get_port_number(_device);
        _address = libusb_get_device_address(_device);

        _displayName = QString("%1:%2 %3")
                .arg(qulonglong(_deviceDescriptor->idVendor()), 4, 16, QChar('0'))
                .arg(qulonglong(_deviceDescriptor->idProduct()), 4, 16, QChar('0'))
                .arg(_deviceDescriptor->description());

        libusb_bos_descriptor *rawBosDesc;
        ret = libusb_get_bos_descriptor(_handle, &rawBosDesc);
        if (ret >= LIBUSB_SUCCESS)
            _bosDescriptor = new UsbBosDescriptor(rawBosDesc, this);
    }

    UsbDevice::~UsbDevice()
    {
        if (_open)
            libusb_close(_handle);
    }

    UsbDeviceDescriptor *UsbDevice::deviceDescriptor() const
    {
        return _deviceDescriptor;
    }

    UsbConfigurationDescriptor *UsbDevice::configurationDescriptor() const
    {
        return _configurationDescriptor;
    }

    bool UsbDevice::valid() const
    {
        return _valid;
    }

    const QString &UsbDevice::displayName() const
    {
        return _displayName;
    }

    QString UsbDevice::getStringDescriptor(size_t index) const
    {
        unsigned char strDesc[128];
        int ret = libusb_get_string_descriptor_ascii(_handle, index, strDesc, 128);
        if (ret < LIBUSB_SUCCESS)
            return QString();
        return QString(reinterpret_cast<char *>(strDesc));
    }

    QString UsbDevice::infomationToHtml() const
    {
#define DEVICE do { html += QString("<h1 align='center'>%1</h1>").arg(_deviceDescriptor->description()); } while(0)
#define START(_title) do { html += QString("<h2 align='center'>%1</h2><table width=\"100%\">").arg(tr(_title)); } while (0)
#define ATTR(_name, _hex, _text) do { \
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>") \
    .arg(_name).arg(_hex, sizeof(_hex) * 2, 16, QChar('0')).arg(_text); \
    } while(0)
#define ATTRTEXT(_name, _text) do {\
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\"></td><td>%2</td></tr>").arg(_name).arg(_text);\
    } while(0)
#define ATTRSTRDESC(_name, _strDescInd) do { \
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>") \
    .arg(_name) \
    .arg(_strDescInd, 2, 16, QChar('0')) \
    .arg(getStringDescriptor(_strDescInd)); \
    } while(0)
#define APPEND(_sub) do { html += _sub->infomationToHtml(); } while(0)
#define END do { html += QString("</table>"); } while(0)

        QString html;
        /* Regenerate it for language support. */
        DEVICE;
        START("Port Information");
        ATTR("Bus Number", _bus, _bus);
        ATTR("Port Number", _port, _port);
        END;
        START("Connection Information");
        ATTRTEXT("Connection Speed", usbSpeedToString(_speed));
        ATTR("Address", _address, _address);
        END;
        START("Device Descriptor");
        ATTR("bLength", _deviceDescriptor->bLength(), _deviceDescriptor->bLength());
        ATTR("bcdUSB", _deviceDescriptor->bcdUSB(), _deviceDescriptor->bcdUSBInfo());
        ATTR("bcdDevice", _deviceDescriptor->bcdDevice(), "");
        ATTR("bDeviceClass", _deviceDescriptor->bDeviceClass(), _deviceDescriptor->deviceClass());
        ATTR("bDeviceSubClass", _deviceDescriptor->bDeviceSubClass(), _deviceDescriptor->deviceSubClass());
        ATTR("bDeviceProtocol", _deviceDescriptor->bDeviceProtocol(), _deviceDescriptor->deviceProtocol());
        ATTR("idVendor", _deviceDescriptor->idVendor(), _deviceDescriptor->vendorName());
        ATTR("idProduct", _deviceDescriptor->idProduct(), _deviceDescriptor->productName());
        ATTRSTRDESC("iManufacturer", _deviceDescriptor->iManufacturer());
        ATTRSTRDESC("iProduct", _deviceDescriptor->iProduct());
        ATTRSTRDESC("iSerialNumber", _deviceDescriptor->iSerialNumber());
        ATTR("bMaxPacketSize0", _deviceDescriptor->bMaxPacketSize0(), _deviceDescriptor->bMaxPacketSize0());
        ATTR("bNumConfigurations", _deviceDescriptor->bNumConfigurations(), _deviceDescriptor->bNumConfigurations());
        END;
        START("Configuration Descriptor");
        ATTR("bLength", _configurationDescriptor->bLength(), _configurationDescriptor->bLength());
        ATTR("wTotalLength", _configurationDescriptor->wTotalLength(), _configurationDescriptor->wTotalLength());
        ATTR("bNumInterfaces", _configurationDescriptor->bNumInterfaces(), _configurationDescriptor->bNumInterfaces());
        ATTR("bConfigurationValue", _configurationDescriptor->bConfigurationValue(), _configurationDescriptor->bConfigurationValue());
        ATTRSTRDESC("iConfiguration", _configurationDescriptor->iConfiguration());
        ATTR("bmAttributes", _configurationDescriptor->bmAttributes(), _configurationDescriptor->bmAttributesInfo());
        ATTR("MaxPower", _configurationDescriptor->MaxPower(), _configurationDescriptor->MaxPower());
        END;
        if (_bosDescriptor)
        {
            foreach (const auto &devCapDesc, _bosDescriptor->usbDeviceCapabilityDescriptorList())
                APPEND(devCapDesc);
        }

        return html;
    }

    void UsbDevice::reset(const UsbDevice &device)
    {
        int ret = libusb_reset_device(device.handle());
        if (ret == LIBUSB_ERROR_NOT_FOUND)
            log().w("UsbDevice", tr("Re-enumeration is required, or the device has been disconnected."));
        else if (ret != LIBUSB_SUCCESS)
            log().e("UsbDevice", tr("Unhandled libusb error: %1.").arg(libusb_error_name(ret)));
    }

    void UsbDevice::reset(const UsbDevice *device)
    {
        UsbDevice::reset(*device);
    }

    bool UsbDevice::operator==(const UsbDevice &device) const
    {
        return _deviceDescriptor->idVendor() == device._deviceDescriptor->idVendor() &&
                _deviceDescriptor->idProduct() == device._deviceDescriptor->idProduct() &&
                _bus == device._bus && _port == device._port;
    }

    UsbSpeed UsbDevice::speed() const
    {
        return _speed;
    }

    UsbBosDescriptor *UsbDevice::bosDescriptor() const
    {
        return _bosDescriptor;
    }

    libusb_device_handle *UsbDevice::handle() const
    {
        return _handle;
    }

    bool UsbDevice::operator==(libusb_device *device)
    {
        libusb_device_descriptor devDesc;
        int ret = libusb_get_device_descriptor(device, &devDesc);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to get device descriptor (%1).").arg(libusb_error_name(ret)));
            return false;
        }
        int bus = libusb_get_bus_number(device);
        int port = libusb_get_port_number(device);
        return _deviceDescriptor->idVendor() == devDesc.idVendor &&
                _deviceDescriptor->idProduct() == devDesc.idProduct &&
                _bus == bus && _port == port;
    }

    uint8_t UsbDevice::bus() const
    {
        return _bus;
    }

    uint8_t UsbDevice::port() const
    {
        return _port;
    }

    libusb_device *UsbDevice::device() const
    {
        return _device;
    }

    QString usbSpeedToString(UsbSpeed speed)
    {
        static const QMap<UsbSpeed, QString> bcdUSBMap = {
            { UsbSpeed::UNKNOWN, "Unknown Speed" },
            { UsbSpeed::LOW, "USB 2.0 LowSpeed 1.5 Mbps" },
            { UsbSpeed::FULL, "USB 2.0 FullSpeed 12 Mbps" },
            { UsbSpeed::HIGH, "USB 2.0 HiSpeed 480 Mbps" },
            { UsbSpeed::SUPER, "USB 3.2 Gen 1 SuperSpeed 5 Gbps" },
            { UsbSpeed::SUPER_PLUS, "USB 3.2 Gen 2 SuperSpeed+ 10 Gbps" },
        };

        return bcdUSBMap[speed];
    }

}
