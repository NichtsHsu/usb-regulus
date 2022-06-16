#include "usbdfudescriptor.h"
#include "__usbmacro.h"

namespace usb {

    UsbDfuDescriptor::UsbDfuDescriptor(UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos):
        UsbInterfaceExtraDescriptor(interfaceDescriptor)
    {
        const unsigned char *extra = interfaceDescriptor->extra() + descPos;

        _bLength = *extra;
        _bDescriptorType = *(extra + 1);
        _bmAttributes = *(extra + 2);
        _wDetachTimeOut = *reinterpret_cast<const uint16_t *>(extra + 3);
        _wTransferSize = *reinterpret_cast<const uint16_t *>(extra + 5);
        _bcdDFUVersion = *reinterpret_cast<const uint16_t *>(extra + 7);
    }

    uint8_t UsbDfuDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbDfuDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    InterfaceExtraDescriptorType UsbDfuDescriptor::type() const
    {
        return InterfaceExtraDescriptorType::DFU;
    }

    uint8_t UsbDfuDescriptor::bmAttributes() const
    {
        return _bmAttributes;
    }

    uint16_t UsbDfuDescriptor::wDetachTimeOut() const
    {
        return _wDetachTimeOut;
    }

    uint16_t UsbDfuDescriptor::wTransferSize() const
    {
        return _wTransferSize;
    }

    uint16_t UsbDfuDescriptor::bcdDFUVersion() const
    {
        return _bcdDFUVersion;
    }

    QString UsbDfuDescriptor::infomationToHtml() const
    {
        QString html;
        START(tr("Device Firmware Upgrade Functional Descriptor"));
        ATTR("bLength", _bLength, _bLength);
        ATTR("bmAttributes", _bmAttributes, __parseBmAttributes());
        ATTR("wDetachTimeOut", _wDetachTimeOut, QString("%1 ms").arg(_wDetachTimeOut));
        ATTR("wTransferSize", _wTransferSize, _wTransferSize);
        ATTR("bcdDFUVersion", _bcdDFUVersion, "");
        END;

        return html;
    }

    QString UsbDfuDescriptor::__parseBmAttributes() const
    {
        QStringList features;
        if (BIT(_bmAttributes, 3))
            features.append(tr("Will perform a bus detach-attach sequence"));
        if (BIT(_bmAttributes, 2))
            features.append(tr("Is able to communicate after Manifestation phase"));
        if (BIT(_bmAttributes, 1))
            features.append(tr("Upload capable"));
        if (BIT(_bmAttributes, 0))
            features.append(tr("Download capable"));

        return features.join(NEWLINE);
    }
} // namespace usb
