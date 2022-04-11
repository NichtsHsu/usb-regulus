#include "usbdevice.h"

namespace usb {
    UsbDevice::UsbDevice(libusb_device *device, QObject *parent) : QObject(parent), _device(device), _bus(0), _port(0)
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
        LOGD("New USB device opened.");
        _open = true;

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

        _valid = true;

        _bus = libusb_get_bus_number(_device);
        _port = libusb_get_port_number(_device);

        _displayName = QString("%1:%2 ").arg(qulonglong(_deviceDescriptor->idVendor()), 4, 16, QChar('0'))
                .arg(qulonglong(_deviceDescriptor->idProduct()), 4, 16, QChar('0')) +
                _deviceDescriptor->vendorName() + (_deviceDescriptor->productName().length() > 0 ?
                                                       QString(" ") + _deviceDescriptor->productName():
                                                       QString(""));
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

    bool UsbDevice::operator==(const UsbDevice &device) const
    {
        return _deviceDescriptor->idVendor() == device._deviceDescriptor->idVendor() &&
                _deviceDescriptor->idProduct() == device._deviceDescriptor->idProduct() &&
                _bus == device._bus && _port == device._port;
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
}
