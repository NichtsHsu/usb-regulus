#include "usbdevice.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

namespace usb {
    UsbDevice::UsbDevice(libusb_device *device, QObject *parent) :
        QObject(parent), _device(device), _bosDescriptor(nullptr), _bus(0), _port(0)
    {
        int ret;
        _valid = false;
        ret = libusb_open(_device, &_handle);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to open device (%1).").arg(usb_error_name(ret)));
            _open = false;
            return;
        }
        LOGD(tr("Open USB device successfully."));
        _open = true;

        /* Auto attach/detach the kernel driver */
        libusb_set_auto_detach_kernel_driver(_handle, true);

        libusb_device_descriptor devDesc;
        ret = libusb_get_device_descriptor(_device, &devDesc);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to get device descriptor (%1).").arg(usb_error_name(ret)));
            return;
        }
        _deviceDescriptor = new UsbDeviceDescriptor(&devDesc, this);

        libusb_config_descriptor *configDesc;
        ret = libusb_get_config_descriptor(_device, 0, &configDesc);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to get config descriptor (%1).").arg(usb_error_name(ret)));
            return;
        }
        _configurationDescriptor = new UsbConfigurationDescriptor(configDesc, this);
        QTimer::singleShot(5000, this, [configDesc](){
            libusb_free_config_descriptor(configDesc);
        });

        _speed = UsbSpeed(libusb_get_device_speed(_device));

        _valid = true;

        _bus = libusb_get_bus_number(_device);
        _port = libusb_get_port_number(_device);
        _address = libusb_get_device_address(_device);

        _displayName = QString("%1:%2 %3")
                .arg(qulonglong(_deviceDescriptor->idVendor()), 4, 16, QChar('0'))
                .arg(qulonglong(_deviceDescriptor->idProduct()), 4, 16, QChar('0'))
                .arg(_deviceDescriptor->description());

        LOGD(tr("New USB device - bus: %1, port: %2, address: %3, speed: %4, display name: %5.")
             .arg(_bus)
             .arg(_port)
             .arg(_address)
             .arg(usbSpeedToString(_speed), _displayName));

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
        unsigned char strDesc[MAX_STRDESC_LENGTH];
        int ret = libusb_get_string_descriptor_ascii(_handle, index, strDesc, MAX_STRDESC_LENGTH);
        if (ret < LIBUSB_SUCCESS)
            return QString();
        return QString(reinterpret_cast<char *>(strDesc));
    }

    QString UsbDevice::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .title(_deviceDescriptor->description())
                .start(tr("Port Information"))
                .attr(tr("Bus Number"), _bus)
                .attr(tr("Port Number"), _port)
                .end()
                .start(tr("Connection Information"))
                .attr(tr("Connection Speed"), "", usbSpeedToString(_speed))
                .attr(tr("Address"), _address)
                .end()
                .append(_deviceDescriptor)
                .append(_configurationDescriptor)
                .append(_bosDescriptor)
                .build();
    }

    int UsbDevice::controlTransfer(uint8_t bmRequestType,
                                   uint8_t bRequest,
                                   uint16_t wValue,
                                   uint16_t wIndex,
                                   QByteArray &buffer,
                                   unsigned int timeout)
    {
        return libusb_control_transfer(_handle,
                                       bmRequestType,
                                       bRequest,
                                       wValue,
                                       wIndex,
                                       reinterpret_cast<unsigned char *>(buffer.data()),
                                       buffer.size(),
                                       timeout);

    }

    int UsbDevice::claimInterface(uint8_t bInterfaceNumber)
    {
        return _configurationDescriptor->interface(bInterfaceNumber)->claim();
    }

    void UsbDevice::releaseInterface(uint8_t bInterfaceNumber)
    {
        return _configurationDescriptor->interface(bInterfaceNumber)->release();
    }

    void UsbDevice::reset(const UsbDevice &device)
    {
        for (uint8_t i = 0;
             i < device.configurationDescriptor()->bNumInterfaces();
             ++i)
            device.configurationDescriptor()->interface(i)->setAltsetting(0);

        int ret = libusb_reset_device(device.handle());
        if (ret == LIBUSB_ERROR_NOT_FOUND)
            log().w("UsbDevice", tr("Re-enumeration is required, or the device has been disconnected."));
        else if (ret != LIBUSB_SUCCESS)
            log().e("UsbDevice", tr("Unhandled error: %1.").arg(usb_error_name(ret)));
        else
        {
            log().i("UsbDevice", tr("The device \"%1\" has been reset.").arg(device.displayName()));
        }
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

    bool UsbDevice::operator==(libusb_device *device) const
    {
        libusb_device_descriptor devDesc;
        int ret = libusb_get_device_descriptor(device, &devDesc);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to get device descriptor (%1).").arg(usb_error_name(ret)));
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
