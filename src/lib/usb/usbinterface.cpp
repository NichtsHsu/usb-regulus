#include "usbinterface.h"

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
#define INTERFACE do { html += QString("<h1 align='center'>%1</h1>").arg(_displayName); } while(0)
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
    .arg(_configDescriptor->device()->getStringDescriptor(_strDescInd)); \
    } while(0)
#define END do { html += QString("</table>"); } while(0)

        QString html;
        /* Regenerate it for language support. */
        INTERFACE;
        START("Interface Descriptor");
        ATTR("bLength", currentInterfaceDescriptor()->bLength(), currentInterfaceDescriptor()->bLength());
        ATTR("bInterfaceNumber", currentInterfaceDescriptor()->bInterfaceNumber(), currentInterfaceDescriptor()->bInterfaceNumber());
        ATTR("bAlternateSetting", currentInterfaceDescriptor()->bAlternateSetting(), currentInterfaceDescriptor()->bAlternateSetting());
        ATTR("bNumEndpoints", currentInterfaceDescriptor()->bNumEndpoints(), currentInterfaceDescriptor()->bNumEndpoints());
        ATTR("bInterfaceClass", currentInterfaceDescriptor()->bInterfaceClass(), currentInterfaceDescriptor()->interfaceClass());
        ATTR("bInterfaceSubClass", currentInterfaceDescriptor()->bInterfaceSubClass(), currentInterfaceDescriptor()->interfaceSubClass());
        ATTR("bInterfaceProtocol", currentInterfaceDescriptor()->bInterfaceProtocol(), currentInterfaceDescriptor()->interfaceProtocol());
        ATTRSTRDESC("iInterface", currentInterfaceDescriptor()->iInterface());
        END;
        for (int i = 0; i < currentInterfaceDescriptor()->bNumEndpoints(); ++i)
        {
            UsbEndpointDescriptor *ep = currentInterfaceDescriptor()->endpoint(i);
            START("Endpoint Descriptor");
            ATTR("bLength", ep->bLength(), ep->bLength());
            ATTR("bEndpointAddress", ep->bEndpointAddress(), ep->endpointAddressInfo());
            ATTR("bmAttributes", ep->bmAttributes(), ep->bmAttributesInfo());
            ATTR("wMaxPacketSize", ep->wMaxPacketSize(), ep->wMaxPacketSize());
            ATTR("bInterval", ep->bInterval(), ep->bInterval());
            ATTR("bRefresh", ep->bRefresh(), ep->bRefresh());
            ATTR("bSynchAddress", ep->bSynchAddress(), ep->bSynchAddress());
            END;
        }
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
