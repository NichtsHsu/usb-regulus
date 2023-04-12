#include "usbdfudescriptor.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

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

    const QStringList &UsbDfuDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bmAttributes",
            "wDetachTimeOut",
            "wTransferSize",
            "bcdDFUVersion",
        };

        return fields;
    }

    QString UsbDfuDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Size of this descriptor, in bytes"},
            {"bDescriptorType", "DFU FUNCTIONAL descriptor type"},
            {"bmAttributes", "<p>DFU attributes.</p>"
            "<p>Bit 7..4: reserved.</p>"
            "<p>Bit 3: device will perform a bus detach-attach sequence when it "
             "receives a DFU_DETACH request. The host must not issue a USB "
             "Reset. (<i>bitWillDetach</i>)</p>"
            "<table><tr><td>0 = no</td></tr>"
            "<tr><td>1 = yes</td></tr></table>"
            "<p>Bit 2: device is able to communicate via USB after Manifestation phase. "
             "(<i>bitManifestationTolerant</i>)</p>"
             "<table><tr><td>0 = no, must see bus reset</td></tr>"
             "<tr><td>1 = yes</td></tr></table>"
            "<p>Bit 1: upload capable.(<i>bitCanUpload</i>)</p>"
             "<table><tr><td>0 = no</td></tr>"
             "<tr><td>1 = yes</td></tr></table>"
            "<p>Bit 0: download capable. (<i>bitCanDnload</i>)</p>"
             "<table><tr><td>0 = no</td></tr>"
             "<tr><td>1 = yes</td></tr></table>"},
            {"wDetachTimeOut", "Time, in milliseconds, that the device "
             "will wait after receipt of the "
             "DFU_DETACH request. If this time "
             "elapses without a USB reset, then "
             "the device will terminate the "
             "Reconfiguration phase and revert "
             "back to normal operation. This "
             "represents the maximum time that "
             "the device can wait (depending on its "
             "timers, etc.). The host may specify a "
             "shorter timeout in the DFU_DETACH "
             "request."},
            {"wTransferSize", "Maximum number of bytes that the "
             "device can accept per control-write "
             "transaction."},
            {"bcdDFUVersion", "Numeric expression identifying the "
             "version of the DFU Specification "
             "release."},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbDfuDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Device Firmware Upgrade Functional Descriptor"), true)
                .attr("bLength", _bLength)
                .attr("bDescriptorType", _bDescriptorType, "DFU_FUNCTIONAL")
                .attr("bmAttributes", _bmAttributes, __parseBmAttributes())
                .attr("wDetachTimeOut", _wDetachTimeOut, QString("%1 ms").arg(_wDetachTimeOut))
                .attr("wTransferSize", _wTransferSize)
                .attr("bcdDFUVersion", _bcdDFUVersion, "")
                .end()
                .build();
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

        return features.join(UsbHtmlBuilder::NEWLINE);
    }
} // namespace usb
