#include "usbinterfacedescriptor.h"
#include "__usbmacro.h"

namespace usb {
    UsbInterfaceDescriptor::UsbInterfaceDescriptor(const libusb_interface_descriptor *desc, UsbInterface *parent) :
        QObject(parent),
        _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType), _bInterfaceNumber(desc->bInterfaceNumber),
        _bAlternateSetting(desc->bAlternateSetting), _bNumEndpoints(desc->bNumEndpoints), _bInterfaceClass(desc->bInterfaceClass),
        _bInterfaceProtocol(desc->bInterfaceProtocol), _iInterface(desc->iInterface), _extra(desc->extra), _extraLength(desc->extra_length),
        _interface(parent), _hidDescriptor(nullptr)
    {
#ifdef Q_OS_UNIX
        char strClass[128], strSubClass[128], strProtocol[128];
        memset(strClass, '\0', sizeof(strClass));
        memset(strSubClass, '\0', sizeof(strSubClass));
        memset(strProtocol, '\0', sizeof(strProtocol));
        get_class_string(strClass, sizeof(strClass), _bInterfaceClass);
        get_subclass_string(strSubClass, sizeof(strSubClass), _bInterfaceClass, _bInterfaceSubClass);
        get_protocol_string(strProtocol, sizeof(strProtocol), _bInterfaceClass,
                            _bInterfaceSubClass,
                            _bInterfaceProtocol);
        _interfaceClass = strClass;
        _interfaceSubClass = strSubClass;
        _interfaceProtocol = strProtocol;
#endif

        _endpoint.reserve(_bNumEndpoints);
        for (int i = 0; i < _bNumEndpoints; ++i)
            _endpoint.append(new UsbEndpointDescriptor(&desc->endpoint[i], this));

        /* HID interface */
        if (_bInterfaceClass == 3 && _extraLength > 0)
            /* Wait for ready */
            QTimer::singleShot(200, this, &UsbInterfaceDescriptor::__requestHidDescriptor);
    }

    uint8_t UsbInterfaceDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbInterfaceDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint8_t UsbInterfaceDescriptor::bInterfaceNumber() const
    {
        return _bInterfaceNumber;
    }

    uint8_t UsbInterfaceDescriptor::bAlternateSetting() const
    {
        return _bAlternateSetting;
    }

    uint8_t UsbInterfaceDescriptor::bNumEndpoints() const
    {
        return _bNumEndpoints;
    }

    uint8_t UsbInterfaceDescriptor::bInterfaceClass() const
    {
        return _bInterfaceClass;
    }

    uint8_t UsbInterfaceDescriptor::bInterfaceSubClass() const
    {
        return _bInterfaceSubClass;
    }

    uint8_t UsbInterfaceDescriptor::bInterfaceProtocol() const
    {
        return _bInterfaceProtocol;
    }

    uint8_t UsbInterfaceDescriptor::iInterface() const
    {
        return _iInterface;
    }

    const unsigned char *UsbInterfaceDescriptor::extra() const
    {
        return _extra;
    }

    int UsbInterfaceDescriptor::extraLength() const
    {
        return _extraLength;
    }

    UsbEndpointDescriptor *UsbInterfaceDescriptor::endpoint(int index) const
    {
        if (index < 0)
        {
            LOGE(tr("Index must be non-negative, but got %1.").arg(index));
            return nullptr;
        }
        if (index >= _bNumEndpoints)
        {
            LOGE(tr("Index should be 0~%1, but got %2.").arg(_bNumEndpoints).arg(index));
            return nullptr;
        }
        return _endpoint[index];
    }

    UsbInterface *UsbInterfaceDescriptor::interface() const
    {
        return _interface;
    }

    const QString &UsbInterfaceDescriptor::interfaceClass() const
    {
        return _interfaceClass;
    }

    const QString &UsbInterfaceDescriptor::interfaceSubClass() const
    {
        return _interfaceSubClass;
    }

    const QString &UsbInterfaceDescriptor::interfaceProtocol() const
    {
        return _interfaceProtocol;
    }

    UsbHidDescriptor *UsbInterfaceDescriptor::hidDescriptor() const
    {
        return _hidDescriptor;
    }

    QString UsbInterfaceDescriptor::infomationToHtml() const
    {
        QString html;
        START("Interface Descriptor");
        ATTR("bLength", _bLength, _bLength);
        ATTR("bInterfaceNumber", _bInterfaceNumber, _bInterfaceNumber);
        ATTR("bAlternateSetting", _bAlternateSetting, _bAlternateSetting);
        ATTR("bNumEndpoints", _bNumEndpoints, _bNumEndpoints);
        ATTR("bInterfaceClass", _bInterfaceClass, _interfaceClass);
        ATTR("bInterfaceSubClass", _bInterfaceSubClass, _interfaceSubClass);
        ATTR("bInterfaceProtocol", _bInterfaceProtocol, _interfaceProtocol);
        ATTRSTRDESC("iInterface", _iInterface, _interface->configDescriptor()->device());
        END;
        for (int i = 0; i < _bNumEndpoints; ++i)
            APPEND(endpoint(i));
        if (_hidDescriptor)
            APPEND(_hidDescriptor);

        return html;
    }

    void UsbInterfaceDescriptor::__requestHidDescriptor()
    {
        _hidDescriptor = new UsbHidDescriptor(this);
    }
}
