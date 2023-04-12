#include "usbinterfaceassociationdescriptor.h"
#include "usbhtmlbuilder.h"

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

    const QStringList &UsbInterfaceAssociationDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bFirstInterface",
            "bInterfaceCount",
            "bFunctionClass",
            "bFunctionSubClass",
            "bFunctionProtocol",
            "iFunction",
        };

        return fields;
    }

    QString UsbInterfaceAssociationDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of Descriptor"},
            {"bDescriptorType", "Descriptor Type: INTERFACE ASSOCIATION"},
            {"bFirstInterface", "Interface number of the first interface that is "
             "associated with this function"},
            {"bInterfaceCount", "Number of contiguous interfaces that are associated "
             "with this function"},
            {"bFunctionClass", "<p>Class code (assigned by USB-IF).</p>"
            "<p>A value of zero is not allowed in this descriptor.</p>"
            "<p>If this field is FFH, the function class is vendor-specific.</p>"
            "<p>All other values are reserved for assignment by the USB-IF.</p>"},
            {"bFunctionSubClass", "<p>Subclass code (assigned by USB-IF).</p>"
            "<p>If the <i>bFunctionClass</i> field is not set to FFH, all "
             "values are reserved for assignment by the USB-IF<p>"},
            {"bFunctionProtocol", "Protocol code (assigned by USB-IF). These codes are "
             "qualified by the values of the <i>bFunctionClass</i> and "
             "<i>bFunctionSubClass</i> fields."},
            {"iFunction", "Index of string descriptor describing this function"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
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
        return UsbHtmlBuilder()
                .start(tr("Interface Association Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "INTERFACE_ASSOCIATION")
                .attr("bFirstInterface", _bFirstInterface)
                .attr("bInterfaceCount", _bInterfaceCount)
                .attr("bFunctionClass", _bFunctionClass, _functionClass)
                .attr("bFunctionSubClass", _bFunctionSubClass, _functionSubClass)
                .attr("bFunctionProtocol", _bFunctionProtocol, _functionProtocol)
                .strdesc("iFunction", _iFunction, configurationDescriptor()->device())
                .end()
                .build();
    }
} // namespace usb
