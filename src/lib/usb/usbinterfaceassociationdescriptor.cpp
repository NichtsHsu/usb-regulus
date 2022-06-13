#include "usbinterfaceassociationdescriptor.h"
#include "__usbmacro.h"

namespace usb {
    UsbInterfaceAssociationDescriptor::UsbInterfaceAssociationDescriptor(
            UsbConfigurationDescriptor *configDesc):
        UsbConfigurationExtraDescriptor(configDesc)
    {

    }

    void UsbInterfaceAssociationDescriptor::__initWithBuffer(const uint8_t *buffer)
    {
        _bLength = buffer[0];
        _bDescriptorType = buffer[1];
        _bFirstInterface = buffer[2];
        _bInterfaceCount = buffer[3];
        _bFunctionClass = buffer[4];
        _bFunctionSubClass = buffer[5];
        _bFunctionProtocol = buffer[6];
        _iFunction = buffer[7];

#ifdef Q_OS_UNIX
        char strClass[128], strSubClass[128], strProtocol[128];
        memset(strClass, '\0', sizeof(strClass));
        memset(strSubClass, '\0', sizeof(strSubClass));
        memset(strProtocol, '\0', sizeof(strProtocol));
        get_class_string(strClass, sizeof(strClass), _bFunctionClass);
        get_subclass_string(strSubClass, sizeof(strSubClass), _bFunctionClass, _bFunctionSubClass);
        get_protocol_string(strProtocol, sizeof(strProtocol), _bFunctionClass,
                            _bFunctionSubClass,
                            _bFunctionProtocol);
        _functionClass = strClass;
        _functionSubClass = strSubClass;
        _functionProtocol = strProtocol;
#endif
    }

    uint8_t UsbInterfaceAssociationDescriptor::iFunction() const
    {
        return _iFunction;
    }

    uint8_t UsbInterfaceAssociationDescriptor::bFunctionProtocol() const
    {
        return _bFunctionProtocol;
    }

    uint8_t UsbInterfaceAssociationDescriptor::bFunctionSubClass() const
    {
        return _bFunctionSubClass;
    }

    uint8_t UsbInterfaceAssociationDescriptor::bFunctionClass() const
    {
        return _bFunctionClass;
    }

    uint8_t UsbInterfaceAssociationDescriptor::bInterfaceCount() const
    {
        return _bInterfaceCount;
    }

    uint8_t UsbInterfaceAssociationDescriptor::bFirstInterface() const
    {
        return _bFirstInterface;
    }

    uint8_t UsbInterfaceAssociationDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbInterfaceAssociationDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    ConfigurationExtraDescriptorType UsbInterfaceAssociationDescriptor::type() const
    {
        return ConfigurationExtraDescriptorType::ASSOCIATION;
    }

    QString UsbInterfaceAssociationDescriptor::infomationToHtml() const
    {
        QString html;
        START("Interface Association Descriptor");
        ATTR("bLength", _bLength, _bLength);
        ATTR("bFirstInterface", _bFirstInterface, _bFirstInterface);
        ATTR("bInterfaceCount", _bInterfaceCount, _bInterfaceCount);
        ATTR("bFunctionClass", _bFunctionClass, _functionClass);
        ATTR("bFunctionSubClass", _bFunctionSubClass, _functionSubClass);
        ATTR("bFunctionProtocol", _bFunctionProtocol, _functionProtocol);
        ATTRSTRDESC("iFunction", _iFunction, configurationDescriptor()->device());
        END;

        return html;
    }
} // namespace usb
