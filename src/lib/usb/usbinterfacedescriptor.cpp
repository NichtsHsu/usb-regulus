#include "usbinterfacedescriptor.h"
#include "usbhiddescriptor.h"
#include "usbdfudescriptor.h"
#include "usbaudiocontrolinterfacedescriptor.h"
#include "usbaudiostreaminterfacedescriptor.h"
#include "usbvideocontrolinterfacedescriptor.h"
#include "usbvideostreaminterfacedescriptor.h"
#include "usbhtmlbuilder.h"

namespace usb {
    UsbInterfaceDescriptor::UsbInterfaceDescriptor(const libusb_interface_descriptor *desc, UsbInterface *parent) :
        QObject(parent),
        _bLength(desc->bLength), _bDescriptorType(desc->bDescriptorType), _bInterfaceNumber(desc->bInterfaceNumber),
        _bAlternateSetting(desc->bAlternateSetting), _bNumEndpoints(desc->bNumEndpoints),
        _bInterfaceClass(desc->bInterfaceClass), _bInterfaceSubClass(desc->bInterfaceSubClass),
        _bInterfaceProtocol(desc->bInterfaceProtocol),
        _iInterface(desc->iInterface), _extra(desc->extra), _extraLength(desc->extra_length),
        _interface(parent), _associationDescriptor(nullptr)
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

        _endpoints.reserve(_bNumEndpoints);
        for (uint8_t i = 0; i < _bNumEndpoints; ++i)
            _endpoints.append(new UsbEndpointDescriptor(&desc->endpoint[i], this));

        /* Extra interface descriptor */
        if (_extraLength > 0)
            /** Wait for ready, so that we can send a GET_DESCRIPTOR request for HID report descriptor.
             * Also, wait for all interface descriptor ready to set association descriptor.
             */
            QTimer::singleShot(200, this, &UsbInterfaceDescriptor::__requestExtraDescriptor);
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

    const QStringList &UsbInterfaceDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bInterfaceNumber",
            "bAlternateSetting",
            "bNumEndpoints",
            "bInterfaceClass",
            "bInterfaceSubClass",
            "bInterfaceProtocol",
            "iInterface",
        };

        return fields;
    }

    QString UsbInterfaceDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of this descriptor in bytes"},
            {"bDescriptorType", "INTERFACE Descriptor Type"},
            {"bInterfaceNumber", "Number of this interface. Zero-based value "
             "identifying the index in the array of "
             "concurrent interfaces supported by this "
             "configuration."},
            {"bAlternateSetting", "Value used to select this alternate setting "
             "for the interface identified in the prior field"},
            {"bNumEndpoints", "Number of endpoints used by this interface "
             "(excluding the Default Control Pipe). If this "
             "value is zero, this interface only uses the "
             "Default Control Pipe."},
            {"bInterfaceClass", "<p>Class code (assigned by the USB-IF).</p>"
             "<p>A value of zero is reserved for future "
             "standardization.</p>"
             "<p>If this field is set to FFH, the interface class "
             "is vendor-specific.</p>"
             "<p>All other values are reserved for assignment "
             "by the USB-IF.</p>"},
            {"bInterfaceSubClass", "<p>Subclass code (assigned by the USB-IF). "
             "These codes are qualified by the value of the "
             "<i>bInterfaceClass</i> field.</p>"
             "<p>If the <i>bInterfaceClass</i> field is reset to zero, "
             "this field shall also be reset to zero.</p>"
             "<p>If the <i>bInterfaceClass</i> field is not set to FFH, "
             "all values are reserved for assignment by "
             "the USB-IF.</p>"},
            {"bInterfaceProtocol", "<p>Protocol code (assigned by the USB). These "
             "codes are qualified by the value of the "
             "<i>bInterfaceClass</i> and the <i>bInterfaceSubClass</i>"
             "fields. If an interface supports class-specific "
             "requests, this code identifies the protocols "
             "that the device uses as defined by the "
             "specification of the device class.</p>"
             "<p>If this field is reset to zero, the device does "
             "not use a class-specific protocol on this "
             "interface.</p>"
             "<p>If this field is set to FFH, the device uses a "
             "vendor-specific protocol for this interface.</p>"},
            {"iInterface", "Index of string descriptor describing this interface"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
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
        return _endpoints[index];
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

    const QVector<UsbInterfaceExtraDescriptor *> &UsbInterfaceDescriptor::extraDescriptors() const
    {
        return _extraDescriptors;
    }

    QString UsbInterfaceDescriptor::infomationToHtml() const
    {
        UsbHtmlBuilder builder;
        builder.start(tr("Interface Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "INTERFACE")
                .attr("bInterfaceNumber", _bInterfaceNumber)
                .attr("bAlternateSetting", _bAlternateSetting)
                .attr("bNumEndpoints", _bNumEndpoints)
                .attr("bInterfaceClass", _bInterfaceClass, _interfaceClass)
                .attr("bInterfaceSubClass", _bInterfaceSubClass, _interfaceSubClass)
                .attr("bInterfaceProtocol", _bInterfaceProtocol, _interfaceProtocol)
                .strdesc("iInterface", _iInterface, _interface->configurationDescriptor()->device())
                .end()
                .append(_associationDescriptor);
        foreach (const auto &desc, _extraDescriptors)
            builder.append(desc);
        for (uint8_t i = 0; i < _bNumEndpoints; ++i)
            builder.append(endpoint(i));

        return builder.build();
    }

    void UsbInterfaceDescriptor::__requestExtraDescriptor()
    {
        static const uint8_t IP_VERSION_01_00 = 0x00;
        static const uint8_t IP_VERSION_02_00 = 0x20;
        static const uint8_t IP_VERSION_03_00 = 0x30;

        int len = _extraLength;
        int pos = 0;
        while (len > 1)
        {
            if (_extra[pos] > len)
                break;
            /* AudioControl Interface Descriptor */
            if (_bInterfaceClass == 0x01 && _bInterfaceSubClass == 0x01 && _extra[pos + 1] == 0x24)
            {
                UsbAudioControlInterfaceDescriptorBasic *desc = nullptr;

                switch (_bInterfaceProtocol)
                {
                    case IP_VERSION_01_00:
                        /* Audio 1.0 */
                        desc = uac1::UsbAudioControlInterfaceDescriptor::get(this, pos);
                    break;
                    case IP_VERSION_02_00:
                        /* Audio 2.0 */
                        desc = uac2::UsbAudioControlInterfaceDescriptor::get(this, pos);
                    break;
                    case IP_VERSION_03_00:
                        /* Audio 3.0 */
                        LOGW(tr("Found Audio 3.0 descriptor, not support yet."));
                        desc = nullptr;
                    break;
                }

                if (desc)
                    _extraDescriptors.append(desc);
            }
            /* AudioStream Interface Descriptor */
            else if (_bInterfaceClass == 0x01 && _bInterfaceSubClass == 0x02 && _extra[pos + 1] == 0x24)
            {
                UsbAudioStreamInterfaceDescriptorBasic *desc = nullptr;

                switch (_bInterfaceProtocol)
                {
                    case IP_VERSION_01_00:
                        /* Audio 1.0 */
                        desc = uac1::UsbAudioStreamInterfaceDescriptor::get(this, pos);
                    break;
                    case IP_VERSION_02_00:
                        /* Audio 2.0 */
                        desc = uac2::UsbAudioStreamInterfaceDescriptor::get(this, pos);
                    break;
                    case IP_VERSION_03_00:
                        /* Audio 3.0 */
                        LOGW(tr("Found Audio 3.0 descriptor, not support yet."));
                        desc = nullptr;
                    break;
                }

                if (desc)
                    _extraDescriptors.append(desc);

            }
            /* VideoControl Interface Descriptor */
            else if (_bInterfaceClass == 0x0E && _bInterfaceSubClass == 0x01 && _extra[pos + 1] == 0x24)
            {
                uvc::UsbVideoControlInterfaceDescriptor *desc =
                        uvc::UsbVideoControlInterfaceDescriptor::get(this, pos);
                if (desc)
                    _extraDescriptors.append(desc);
            }
            /* VideoStream Interface Descriptor */
            else if (_bInterfaceClass == 0x0E && _bInterfaceSubClass == 0x02 && _extra[pos + 1] == 0x24)
            {
                uvc::UsbVideoStreamInterfaceDescriptor *desc =
                        uvc::UsbVideoStreamInterfaceDescriptor::get(this, pos);
                if (desc)
                    _extraDescriptors.append(desc);
            }
            /* Human Interface Device Descriptor */
            else if (_bInterfaceClass == 3 && _extra[pos + 1] == 0x21)
                _extraDescriptors.append(new UsbHidDescriptor(this, pos));
            /* Device Firmware Upgrade Descriptor */
            else if (_bInterfaceClass == 0xFE && _bInterfaceSubClass == 0x01 && _extra[pos + 1] == 0x21)
                _extraDescriptors.append(new UsbDfuDescriptor(this, pos));
            /* Interface Association Descriptor or OTG Decriptor */
            else if (_extra[pos + 1] == uint8_t(ConfigurationExtraDescriptorType::ASSOCIATION) ||
                     _extra[pos + 1] == uint8_t(ConfigurationExtraDescriptorType::OTG))
            {
                UsbConfigurationExtraDescriptor *configExtraDesc =
                        UsbConfigurationExtraDescriptor::fromInterfaceExtra(this, pos);
                UsbConfigurationDescriptor *configDesc = _interface->configurationDescriptor();
                configDesc->addConfigurationExtraDescriptor(configExtraDesc);
            }
            len -= _extra[pos];
            pos += _extra[pos];
        }
    }

    void UsbInterfaceDescriptor::setAssociationDescriptor(UsbInterfaceAssociationDescriptor *associationDescriptor)
    {
        if ((_bInterfaceNumber >= associationDescriptor->bFirstInterface()) &&
            (_bInterfaceNumber <
             (associationDescriptor->bFirstInterface() + associationDescriptor->bInterfaceCount())))
            _associationDescriptor = associationDescriptor;
    }

    UsbHidDescriptor *UsbInterfaceDescriptor::hidDescriptor()
    {
        foreach (const auto &desc, _extraDescriptors)
            if (desc->type() == InterfaceExtraDescriptorType::HID)
                return qobject_cast<UsbHidDescriptor *>(desc);
        return nullptr;
    }

    UsbInterfaceAssociationDescriptor *UsbInterfaceDescriptor::associationDescriptor() const
    {
        return _associationDescriptor;
    }

    bool UsbInterfaceDescriptor::isKeyboard() const
    {
        return _bInterfaceClass == 0x03 && _bInterfaceProtocol == 0x01;
    }

    bool UsbInterfaceDescriptor::isMouse() const
    {
        return _bInterfaceClass == 0x03 && _bInterfaceProtocol == 0x02;
    }
}
