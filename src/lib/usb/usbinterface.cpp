#include "usbinterface.h"

namespace usb {
    UsbInterface::UsbInterface(const libusb_interface *interface, UsbConfigurationDescriptor *parent) : QObject(parent),
        _configDescriptor(parent)
    {
        _num_altsetting = interface->num_altsetting;
        _altsetting.reserve(_num_altsetting);
        for (int i = 0; i < _num_altsetting; ++i)
            _altsetting.append(new UsbInterfaceDescriptor(&interface->altsetting[i], this));

        QString strInterface = _configDescriptor->device()->getStringDescriptor(interface->altsetting[0].iInterface);
        if (!strInterface.isEmpty())
        {
            _displayName = strInterface;
        }
        else
        {
            char strClass[128], strSubclass[128], strProtocol[128];
            get_class_string(strClass, 128, interface->altsetting[0].bInterfaceClass);
            get_subclass_string(strSubclass, 128, interface->altsetting[0].bInterfaceClass,
                    interface->altsetting[0].bInterfaceSubClass);
            get_protocol_string(strProtocol, 128, interface->altsetting[0].bInterfaceClass,
                    interface->altsetting[0].bInterfaceSubClass,
                    interface->altsetting[0].bInterfaceProtocol);
            if(strlen(strProtocol))
                _displayName = strProtocol;
            else
                _displayName = QString(strClass) + " " + QString(strSubclass);
        }
    }

    int UsbInterface::num_altsetting() const
    {
        return _num_altsetting;
    }

    UsbInterfaceDescriptor *UsbInterface::altsetting(int index) const
    {
        if (index < 0)
        {
            LOGE(tr("Index must be non-negative, but got %1.").arg(index));
            return nullptr;
        }
        if (index >= _num_altsetting)
        {
            LOGE(tr("Index should be 0~%1, but got %2.").arg(_num_altsetting).arg(index));
            return nullptr;
        }
        return _altsetting[index];
    }

    const QString &UsbInterface::displayName() const
    {
        return _displayName;
    }

    UsbConfigurationDescriptor *UsbInterface::configDescriptor() const
    {
        return _configDescriptor;
    }
}
