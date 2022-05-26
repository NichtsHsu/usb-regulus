#include "usbinterface.h"
#include "__usbmacro.h"

namespace usb {
    UsbInterface::UsbInterface(const libusb_interface *interface, UsbConfigurationDescriptor *parent) : QObject(parent),
        _configDescriptor(parent)
    {
        _num_altsetting = interface->num_altsetting;
        _altsetting.reserve(_num_altsetting);
        for (int i = 0; i < _num_altsetting; ++i)
            _altsetting.append(new UsbInterfaceDescriptor(&interface->altsetting[i], this));

        _currentAltsetting = 0;
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

    const QString &UsbInterface::displayName()
    {
        /* It may not be good to cost the construct time, we generate it at first call */
        if (_displayName.isEmpty())
        {
            QString iInterface = _configDescriptor->device()->getStringDescriptor(currentInterfaceDescriptor()->iInterface());
            if (!iInterface.isEmpty())
            {
                _displayName = iInterface;
            }
            else
            {
#ifdef Q_OS_UNIX
                if(!currentInterfaceDescriptor()->interfaceProtocol().isEmpty())
                    _displayName = currentInterfaceDescriptor()->interfaceProtocol();
                else
                    _displayName = QString(currentInterfaceDescriptor()->interfaceClass()) + " "
                            + QString(currentInterfaceDescriptor()->interfaceSubClass());
#endif
            }
        }

        return _displayName;
    }

    UsbConfigurationDescriptor *UsbInterface::configDescriptor() const
    {
        return _configDescriptor;
    }

    UsbInterfaceDescriptor *UsbInterface::currentInterfaceDescriptor() const
    {
        return altsetting(_currentAltsetting);
    }

    QString UsbInterface::infomationToHtml() const
    {
        QString html;
        /* Regenerate it for language support. */
        INTERFACE;
        APPEND(currentInterfaceDescriptor());
        return html;
    }

    int UsbInterface::currentAltsetting() const
    {
        return _currentAltsetting;
    }

    int UsbInterface::claim()
    {
        UsbDevice *device = _configDescriptor->device();
        int ret  = libusb_claim_interface(device->handle(),  currentInterfaceDescriptor()->bInterfaceNumber());
        if (ret == LIBUSB_SUCCESS)
            LOGD(tr("Successfully claim the interface."));
        else
            LOGE(tr("Failed to claim interface \"%1\" of device \"%2\" (%3).")
                 .arg(_displayName)
                 .arg(device->displayName())
                 .arg(libusb_error_name(ret)));
        return ret;
    }

    void UsbInterface::release()
    {
        UsbDevice *device = _configDescriptor->device();
        int ret = libusb_release_interface(device->handle(), currentInterfaceDescriptor()->bInterfaceNumber());
        if (ret == LIBUSB_SUCCESS)
        {
            LOGD(tr("Successfully release the interface."));
        }
        if (ret == LIBUSB_ERROR_NOT_FOUND || ret == LIBUSB_ERROR_NO_DEVICE)
        {
            /* No need to handle the error to these cases */
            LOGD(tr("No need to release the interface."));
        }
        else if (ret < LIBUSB_SUCCESS)
        {
            /* Just warning */
            LOGW(tr("Failed to release interface \"%1\" of device \"%2\" (%3), it will be considered already released.")
                 .arg(_displayName)
                 .arg(device->displayName())
                 .arg(libusb_error_name(ret)));
        }
    }
}
