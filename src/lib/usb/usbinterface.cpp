#include "usbinterface.h"
#include "__usbmacro.h"

namespace usb {
    UsbInterface::UsbInterface(const libusb_interface *interface, UsbConfigurationDescriptor *parent) : QObject(parent),
        _configurationDescriptor(parent), _claimCount(0), _selfClaim(false)
    {
        _numAltsetting = interface->num_altsetting;
        _altsetting.reserve(_numAltsetting);
        for (int i = 0; i < _numAltsetting; ++i)
            _altsetting.append(new UsbInterfaceDescriptor(&interface->altsetting[i], this));

        _currentAltsetting = 0;
    }

    int UsbInterface::numAltsetting() const
    {
        return _numAltsetting;
    }

    UsbInterfaceDescriptor *UsbInterface::altsetting(int index) const
    {
        if (index < 0)
        {
            LOGE(tr("Index must be non-negative, but got %1.").arg(index));
            return nullptr;
        }
        if (index >= _numAltsetting)
        {
            LOGE(tr("Index should be 0~%1, but got %2.").arg(_numAltsetting).arg(index));
            return nullptr;
        }
        return _altsetting[index];
    }

    const QString &UsbInterface::displayName()
    {
        /* It may not be good to cost the construct time, we generate it at first call */
        if (_displayName.isEmpty())
        {
            QString iInterface = _configurationDescriptor->device()->getStringDescriptor(currentInterfaceDescriptor()->iInterface());
            if (!iInterface.isEmpty())
            {
                _displayName = iInterface;
            }
            else
            {
#ifdef Q_OS_UNIX
                if(!currentInterfaceDescriptor()->interfaceProtocol().isEmpty())
                    _displayName = currentInterfaceDescriptor()->interfaceProtocol();
                else if (!currentInterfaceDescriptor()->interfaceSubClass().isEmpty())
                    _displayName = currentInterfaceDescriptor()->interfaceSubClass();
                else
                    _displayName = currentInterfaceDescriptor()->interfaceClass();
#endif
            }
        }

        return _displayName;
    }

    UsbConfigurationDescriptor *UsbInterface::configurationDescriptor() const
    {
        return _configurationDescriptor;
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
        START(tr("Interface Informations"));
        ATTRTEXT(tr("Number of altsettings"), QString::number(_numAltsetting));
        ATTRTEXT(tr("Current altsetting"), QString::number(_currentAltsetting));
        END;
        APPEND(currentInterfaceDescriptor());
        return html;
    }

    int UsbInterface::currentAltsetting() const
    {
        return _currentAltsetting;
    }

    static std::mutex __mutW;

    int UsbInterface::claim()
    {
        if (currentInterfaceDescriptor()->isMouse() && UsbHost::instance()->protectMouse())
        {
            LOGW(tr("Refuse to claim a mouse HID interface because the 'Protect Mouse' option is checked. "
                    "During claiming your mouse device will not be avaliable to you computer, "
                    "and may be some strange issues after released it. "
                    "If you understand and really need claim you mouse interface, "
                    "please uncheck the 'Protect Mouse' option in the menu 'Device'."));
            return ERROR_PROTECT_MOUSE;
        }
        if (currentInterfaceDescriptor()->isKeyboard() && UsbHost::instance()->protectKeyboard())
        {
            LOGW(tr("Refuse to claim a keyboard HID interface because the 'Protect Keyboard' option is checked. "
                    "During claiming your keyboard device will not be avaliable to you computer, "
                    "and may be some strange issues after released it. "
                    "If you understand and really need claim you keyboard interface, "
                    "please uncheck the 'Protect Keyboard' option in the menu 'Device'."));
            return ERROR_PROTECT_KEYBOARD;
        }
        int ret = 0;
        UsbDevice *device = _configurationDescriptor->device();
        std::lock_guard<std::mutex> lock{ __mutW };
        if (_claimCount == 0)
        {
            ret  = libusb_claim_interface(device->handle(),  currentInterfaceDescriptor()->bInterfaceNumber());
            if (ret == LIBUSB_SUCCESS)
                LOGD(tr("Successfully claim the interface."));
            else
                LOGE(tr("Failed to claim interface \"%1\" of device \"%2\" (%3).")
                     .arg(_displayName)
                     .arg(device->displayName())
                     .arg(usb_error_name(ret)));
        }

        ++_claimCount;
        LOGD(tr("Claim the interface \"%1\" of device \"%2\" with counts %3.")
             .arg(_displayName)
             .arg(device->displayName())
             .arg(_claimCount));
        return ret;
    }

    void UsbInterface::release()
    {
        int ret = 0;
        UsbDevice *device = _configurationDescriptor->device();
        std::lock_guard<std::mutex> lock{ __mutW };
        if (!_claimCount)
        {
            LOGD(tr("unexpectedly release the interface \"%1\" of device \"%2\".")
                 .arg(_displayName)
                 .arg(device->displayName()));
            return;
        }
        --_claimCount;
        if (_claimCount == 0)
        {
            ret = libusb_release_interface(device->handle(), currentInterfaceDescriptor()->bInterfaceNumber());
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
                     .arg(usb_error_name(ret)));
            }
        }
        LOGD(tr("Release the interface \"%1\" of device \"%2\" with counts %3.")
             .arg(_displayName)
             .arg(device->displayName())
             .arg(_claimCount));
    }

    int UsbInterface::setAltsetting(int altsetting)
    {
        if (altsetting < 0 || altsetting >= _numAltsetting)
        {
            LOGE(tr("Selected altsetting index %1 out of range (%2-%3).")
                 .arg(altsetting)
                 .arg(0)
                 .arg(_numAltsetting));
            return ERROR_OUT_OF_RANGE;
        }

        if (!_selfClaim)
        {
            claim();
            _selfClaim = true;
        }
        int ret = libusb_set_interface_alt_setting(_configurationDescriptor->device()->handle(),
                                                   currentInterfaceDescriptor()->bInterfaceNumber(),
                                                   altsetting);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to set altsetting, error: %1.").arg(usb_error_name(ret)));
            return ret;
        }
        LOGD(tr("Interface \"%1\" of device \"%2\" set altsetting to %3.")
             .arg(displayName())
             .arg(_configurationDescriptor->device()->displayName())
             .arg(altsetting));
        _currentAltsetting = altsetting;
        if (_currentAltsetting == 0)
        {
            release();
            _selfClaim = false;
        }

        return 0;
    }
}
