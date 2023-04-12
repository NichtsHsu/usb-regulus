#include "usbvideocontrolinterfacedescriptor.h"
#include "__usbmacro.h"

namespace usb {
    namespace uvc {
        UsbVideoControlInterfaceDescriptor::UsbVideoControlInterfaceDescriptor(
                UsbInterfaceDescriptor *parent):
            UsbInterfaceExtraDescriptor(parent)
        {

        }

        InterfaceExtraDescriptorType UsbVideoControlInterfaceDescriptor::type() const
        {
            return InterfaceExtraDescriptorType::VC;
        }

        QString UsbVideoControlInterfaceDescriptor::strSubtype(uint8_t subtype)
        {
            static const QString subtypes[] = {
                "VC_DESCRIPTOR_UNDEFINED",
                "VC_HEADER",
                "VC_INPUT_TERMINAL",
                "VC_OUTPUT_TERMINAL",
                "VC_SELECTOR_UNIT",
                "VC_PROCESSING_UNIT",
                "VC_EXTENSION_UNIT",
                "VC_ENCODING_UNIT",
            };

            return subtypes[subtype < (sizeof(subtypes) / sizeof(QString)) ? subtype : 0];
        }

        QString UsbVideoControlInterfaceDescriptor::strWTerminalType(uint16_t wTerminalType)
        {
            static const QMap<uint16_t, const char *> TERMINAL_TYPES = {
                { 0x0100, "TT_VENDOR_SPECIFIC" },
                { 0x0101, "TT_STREAMING" },
                { 0x0200, "ITT_VENDOR_SPECIFIC" },
                { 0x0201, "ITT_CAMERA" },
                { 0x0202, "ITT_MEDIA_TRANSPORT_INPUT" },
                { 0x0300, "OTT_ VENDOR_SPECIFIC" },
                { 0x0301, "OTT_DISPLAY" },
                { 0x0302, "OTT_MEDIA_TRANSPORT_OUTPUT" },
                { 0x0400, "EXTERNAL_ VENDOR_SPECIFIC" },
                { 0x0401, "COMPOSITE_CONNECTOR" },
                { 0x0402, "SVIDEO_CONNECTOR" },
                { 0x0403, "COMPONENT_CONNECTOR" },
            };

            return TERMINAL_TYPES.value(wTerminalType, "");
        }

        QString UsbVideoControlInterfaceDescriptor::terminalTypesInformation()
        {
            return "<p>Constant that characterizes the type of Terminal.</p>"
                   "<h4>1. USB Terminal Types</h4>"
                   "<p>These Terminal types describe Terminals that handle signals carried over the USB, through "
                   "isochronous or bulk pipes. These Terminal types are valid for both Input and Output Terminals.</p>"
                   "<table><tr><th>Terminal Type</th><th>Code</th><th>I/O</th><th>Description</th></tr>"
                   "<tr><td>TT_VENDOR_SPECIFIC</td><td>0x0100</td><td>I/O</td><td>A Terminal dealing with a "
                   "signal carried over a vendor-specific interface. "
                   "The vendor-specific interface descriptor must "
                   "contain a field that references the Terminal.</td></tr>"
                   "<tr><td>TT_STREAMING</td><td>0x0101</td><td>I/O</td><td>A Terminal dealing with a "
                   "signal carried over an endpoint in a VideoStreaming interface. "
                   "The VideoStreaming interface descriptor points to "
                   "the associated Terminal through the <i>bTerminalLink</i> field.</td></tr></table>"
                   "<h4>2. Input Terminal Types</h4>"
                   "<p>These Terminal Types describe Terminals that are designed to capture video. They either are "
                   "physically part of the video function or can be assumed to be connected to it in normal operation. "
                   "These Terminal Types are valid only for Input Terminals.</p>"
                   "<table><tr><th>Terminal Type</th><th>Code</th><th>I/O</th><th>Description</th></tr>"
                   "<tr><td>ITT_VENDOR_SPECIFIC</td><td>0x0200</td><td>I</td>"
                   "<td>Vendor-Specific Input Terminal.</td></tr>"
                   "<tr><td>ITT_CAMERA</td><td>0x0201</td><td>I</td><td>Camera sensor. To be used "
                   "only in Camera Terminal descriptors</td></tr>"
                   "<tr><td>ITT_MEDIA_TRANSPORT_INPUT</td><td>0x0202</td><td>I</td><td>Sequential media. To be "
                   "used only in Media Transport Terminal Descriptors.</td></tr></table>"
                   "<h4>3. Output Terminal Types</h4>"
                   "<p>These Terminal types describe Terminals that are designed to render video. They are either "
                   "physically part of the video function or can be assumed to be connected to it in normal operation. "
                   "These Terminal types are only valid for Output Terminals.</p>"
                   "<table><tr><th>Terminal Type</th><th>Code</th><th>I/O</th><th>Description</th></tr>"
                   "<tr><td>OTT_VENDOR_SPECIFIC</td><td>0x0300</td><td>O</td>"
                   "<td>Vendor-Specific Output Terminal.</td></tr>"
                   "<tr><td>OTT_DISPLAY</td><td>0x0301</td><td>O</td>"
                   "<td>Generic display (LCD, CRT, etc.).</td></tr>"
                   "<tr><td>OTT_MEDIA_TRANSPORT_OUTPUT</td><td>0x0302</td><td>O</td><td>Sequential media. To be "
                   "used only in Media Transport Terminal Descriptors.</td></tr></table>"
                   "<h4>4. External Terminal Types</h4>"
                   "<p>These Terminal types describe external resources and connections that do not fit under the "
                   "categories of Input or Output Terminals because they do not necessarily translate video signals to "
                   "or from the user of the computer. Most of them may be either Input or Output Terminals.</p>"
                   "<table><tr><th>Terminal Type</th><th>Code</th><th>I/O</th><th>Description</th></tr>"
                   "<tr><td>EXTERNAL_VENDOR_SPECIFIC</td><td>0x0400</td><td>I/O</td>"
                   "<td>Vendor-Specific External Terminal.</td></tr>"
                   "<tr><td>COMPOSITE_CONNECTOR</td><td>0x0401</td><td>I/O</td>"
                   "<td>Composite video connector.</td></tr>"
                   "<tr><td>SVIDEO_CONNECTOR</td><td>0x0402</td><td>I/O</td>"
                   "<td>S-video connector.</td></tr>"
                   "<tr><td>COMPONENT_CONNECTOR</td><td>0x0403</td><td>I/O</td>"
                   "<td>Component video connector.</td></tr></table>";
        };

        UsbVideoControlInterfaceDescriptor *UsbVideoControlInterfaceDescriptor::get(UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24)
                return nullptr;

            UsbVideoControlInterfaceDescriptor *desc;
            switch (extra[2])
            {
                case 1:
                    /* VC_HEADER */
                    desc = new UsbClassSpecificVCInterfaceHeaderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 2:
                    /* VC_INPUT_TERMINAL */
                    desc = UsbInputTerminalDescriptor::get(
                                interfaceDescriptor, descPos);
                break;
                case 3:
                    /* VC_OUTPUT_TERMINAL */
                    desc = UsbOutputTerminalDescriptor::get(
                                interfaceDescriptor, descPos);
                break;
                case 4:
                    /* VC_SELECTOR_UNIT */
                    desc = new UsbSelectorUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 5:
                    /* VC_PROCESSING_UNIT */
                    desc = new UsbProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 6:
                    /* VC_EXTENSION_UNIT */
                    desc = new UsbExtensionUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 7:
                    /* VC_ENCODING_UNIT */
                    desc = new UsbExtensionUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
                break;
            }

            return desc;
        }

        uint8_t UsbClassSpecificVCInterfaceHeaderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbClassSpecificVCInterfaceHeaderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbClassSpecificVCInterfaceHeaderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint16_t UsbClassSpecificVCInterfaceHeaderDescriptor::bcdUVC() const
        {
            return _bcdUVC;
        }

        uint16_t UsbClassSpecificVCInterfaceHeaderDescriptor::wTotalLength() const
        {
            return _wTotalLength;
        }

        uint32_t UsbClassSpecificVCInterfaceHeaderDescriptor::dwClockFrequency() const
        {
            return _dwClockFrequency;
        }

        uint8_t UsbClassSpecificVCInterfaceHeaderDescriptor::bInCollection() const
        {
            return _bInCollection;
        }

        uint8_t UsbClassSpecificVCInterfaceHeaderDescriptor::baInterfaceNr(uint8_t index) const
        {
            if (index > 0 && index <= _bInCollection)
                return _baInterfaceNr[index - 1];
            else
            {
                LOGE(tr("Index %1 for baInterfaceNr out of range, must be 1~%2.")
                     .arg(index).arg(_bInCollection));
                return 0;
            }
        }

        const QStringList &UsbClassSpecificVCInterfaceHeaderDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bcdUVC",
                "wTotalLength",
                "dwClockFrequency",
                "bInCollection",
                "baInterfaceNr",
            };

            return fields;
        }

        QString UsbClassSpecificVCInterfaceHeaderDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VC_HEADER descriptor subtype"},
                {"bcdUVC", "Video Device Class Specification release "
                 "number in binary-coded decimal. (i.e. 2.10 "
                 "is 210H and 1.50 is 150H)"},
                {"wTotalLength", "Total number of bytes returned for the "
                 "class-specific VideoControl interface "
                 "descriptor. Includes the combined length "
                 "of this descriptor header and all Unit and "
                 "Terminal descriptors."},
                {"dwClockFrequency", "<p>Use of this field has been deprecated.</p>"
                 "<p>The device clock frequency in Hz. This "
                 "will specify the units used for the time "
                 "information fields in the Video Payload "
                 "Headers of the primary data stream and "
                 "format.</p>"
                 "<p>The <i>dwClockFrequency</i> field of the "
                 "Video Probe and Commit control replaces "
                 "this descriptor field. A value for this field "
                 "shall be chosen such that the primary or "
                 "default function of the device will be "
                 "available to host software that implements "
                 "Version 1.0 of this specification.</p>"},
                {"bInCollection", "The number of VideoStreaming interfaces "
                 "in the Video Interface Collection to which "
                 "this VideoControl interface belongs."},
                {"baInterfaceNr", "Interface number of the nth "
                 "VideoStreaming interface in the Collection."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbClassSpecificVCInterfaceHeaderDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Class-Specific VC Interface Header Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bcdUVC", _bcdUVC, "")
                    .attr("wTotalLength", _wTotalLength)
                    .attr("dwClockFrequency", _dwClockFrequency, tr("%1 Hz").arg(_dwClockFrequency))
                    .attr("bInCollection", _bInCollection);
            for (uint8_t i = 1; i <= _bInCollection; ++i)
                builder.attr("baInterfaceNr", baInterfaceNr(i), i);
            return builder.end().build();
        }

        UsbClassSpecificVCInterfaceHeaderDescriptor::UsbClassSpecificVCInterfaceHeaderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bcdUVC = *reinterpret_cast<const uint16_t *>(extra + 3);
            _wTotalLength = *reinterpret_cast<const uint16_t *>(extra + 5);
            _dwClockFrequency = *reinterpret_cast<const uint32_t *>(extra + 7);
            _bInCollection = *(extra + 11);
            _baInterfaceNr.reserve(_bInCollection);
            for (uint8_t i = 12; i < 12 + _bInCollection; ++i)
                _baInterfaceNr.append(*(extra + i));
        }

        uint8_t UsbInputTerminalDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbInputTerminalDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbInputTerminalDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbInputTerminalDescriptor::bTerminalID() const
        {
            return _bTerminalID;
        }

        uint16_t UsbInputTerminalDescriptor::wTerminalType() const
        {
            return _wTerminalType;
        }

        uint8_t UsbInputTerminalDescriptor::bAssocTerminal() const
        {
            return _bAssocTerminal;
        }

        uint8_t UsbInputTerminalDescriptor::iTerminal() const
        {
            return _iTerminal;
        }

        const QStringList &UsbInputTerminalDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bTerminalID",
                "wTerminalType",
                "bAssocTerminal",
                "iTerminal",
            };

            return fields;
        }

        QString UsbInputTerminalDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VC_INPUT_TERMINAL descriptor subtype"},
                {"bTerminalID", "A non-zero constant that uniquely identifies "
                 "the Terminal within the video function. This "
                 "value is used in all requests to address this "
                 "Terminal."},
                {"wTerminalType", terminalTypesInformation()},
                {"bAssocTerminal", "ID of the Output Terminal to which this Input "
                 "Terminal is associated, or zero (0) if no such "
                 "association exists."},
                {"iTerminal", "Index of a string descriptor, describing the Input Terminal."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbInputTerminalDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Input Terminal Descriptor"), true, "UVC 1.5");
            __parseBase(builder);
            return builder.end().build();
        }

        UsbInputTerminalDescriptor::UsbInputTerminalDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bTerminalID = *(extra + 3);
            _wTerminalType = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bAssocTerminal = *(extra + 6);
            _iTerminal = *(extra + 7);
        }

        void UsbInputTerminalDescriptor::__parseBase(UsbHtmlBuilder &builder) const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            builder.attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bTerminalID", _bTerminalID)
                    .attr("wTerminalType", _wTerminalType, strWTerminalType(_wTerminalType))
                    .attr("bAssocTerminal", _bAssocTerminal)
                    .strdesc("iTerminal", _iTerminal, device);
        }

        UsbInputTerminalDescriptor *UsbInputTerminalDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24 || extra[2] != 0x2)
                return nullptr;

            UsbInputTerminalDescriptor *desc;
            switch (*reinterpret_cast<const uint16_t *>(extra + 4))
            {
                case 0x0201:
                    /* ITT_CAMERA */
                    desc = new UsbCameraTerminalDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = new UsbInputTerminalDescriptor(
                                interfaceDescriptor, descPos);
                break;
            }

            return desc;
        }

        uint8_t UsbOutputTerminalDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbOutputTerminalDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbOutputTerminalDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbOutputTerminalDescriptor::bTerminalID() const
        {
            return _bTerminalID;
        }

        uint16_t UsbOutputTerminalDescriptor::wTerminalType() const
        {
            return _wTerminalType;
        }

        uint8_t UsbOutputTerminalDescriptor::bAssocTerminal() const
        {
            return _bAssocTerminal;
        }

        uint8_t UsbOutputTerminalDescriptor::bSourceID() const
        {
            return _bSourceID;
        }

        uint8_t UsbOutputTerminalDescriptor::iTerminal() const
        {
            return _iTerminal;
        }

        const QStringList &UsbOutputTerminalDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bTerminalID",
                "wTerminalType",
                "bAssocTerminal",
                "bSourceID",
                "iTerminal",
            };

            return fields;
        }

        QString UsbOutputTerminalDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VC_OUTPUT_TERMINAL descriptor subtype"},
                {"bTerminalID", "A non-zero constant that uniquely identifies "
                 "the Terminal within the video function. This "
                 "value is used in all requests to address this "
                 "Terminal."},
                {"wTerminalType", terminalTypesInformation()},
                {"bAssocTerminal", "Constant, identifying the Input Terminal to "
                 "which this Output Terminal is associated, or "
                 "zero (0) if no such association exists."},
                {"bSourceID", "ID of the Unit or Terminal to which this "
                 "Terminal is connected."},
                {"iTerminal", "Index of a string descriptor, describing the Output Terminal."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbOutputTerminalDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Output Terminal Descriptor"), true, "UVC 1.5");
            __parseBase(builder);
            return builder.end().build();
        }

        UsbOutputTerminalDescriptor::UsbOutputTerminalDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bTerminalID = *(extra + 3);
            _wTerminalType = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bAssocTerminal = *(extra + 6);
            _bSourceID = *(extra + 7);
            _iTerminal = *(extra + 8);
        }

        void UsbOutputTerminalDescriptor::__parseBase(UsbHtmlBuilder &builder) const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            builder.attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bTerminalID", _bTerminalID)
                    .attr("wTerminalType", _wTerminalType, strWTerminalType(_wTerminalType))
                    .attr("bAssocTerminal", _bAssocTerminal)
                    .attr("bSourceID", _bSourceID)
                    .strdesc("iTerminal", _iTerminal, device);
        }

        UsbOutputTerminalDescriptor *UsbOutputTerminalDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24 || extra[2] != 0x3)
                return nullptr;

            UsbOutputTerminalDescriptor *desc;
            switch (*reinterpret_cast<const uint16_t *>(extra + 4))
            {
                default:
                    desc = new UsbOutputTerminalDescriptor(
                                interfaceDescriptor, descPos);
                break;
            }

            return desc;
        }

        uint16_t UsbCameraTerminalDescriptor::wObjectiveFocalLengthMin() const
        {
            return _wObjectiveFocalLengthMin;
        }

        uint16_t UsbCameraTerminalDescriptor::wObjectiveFocalLengthMax() const
        {
            return _wObjectiveFocalLengthMax;
        }

        uint16_t UsbCameraTerminalDescriptor::wOcularFocalLength() const
        {
            return _wOcularFocalLength;
        }

        uint8_t UsbCameraTerminalDescriptor::bControlSize() const
        {
            return _bControlSize;
        }

        QByteArray UsbCameraTerminalDescriptor::bmControls() const
        {
            return _bmControls;
        }

        const QStringList &UsbCameraTerminalDescriptor::getFieldNames()
        {
            static const QStringList fields =
                    QStringList(UsbInputTerminalDescriptor::getFieldNames())
                    << "wObjectiveFocalLengthMin"
                    << "wObjectiveFocalLengthMax"
                    << "wOcularFocalLength"
                    << "bControlSize"
                    << "bmControls";

            return fields;
        }

        QString UsbCameraTerminalDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"wObjectiveFocalLengthMin", "The value of L<sub>min</sub> If Optical Zoom "
                 "is not supported; this field shall be set to 0."},
                {"wObjectiveFocalLengthMax", "The value of L<sub>max</sub> If Optical Zoom "
                 "is not supported; this field shall be set to 0."},
                {"wOcularFocalLength", "The value of L<sub>ocular</sub> If Optical "
                 "Zoom is not supported; this field shall be set to 0."},
                {"bControlSize", "Size in bytes of the <i>bmControls</i> field."},
                {"bmControls", "<p>A bit set to 1 indicates that the "
                 "mentioned Control is supported for the video stream.</p>"
                "<table><tr><td>D0:</td><td>Scanning Mode</td></th>"
                "<tr><td>D1:</td><td>Auto-Exposure Mode</td></th>"
                "<tr><td>D2:</td><td>Auto-Exposure Priority</td></th>"
                "<tr><td>D3:</td><td>Exposure Time (Absolute)</td></th>"
                "<tr><td>D4:</td><td>Exposure Time (Relative)</td></th>"
                "<tr><td>D5:</td><td>Focus (Absolute)</td></th>"
                "<tr><td>D6:</td><td>Focus (Relative)</td></th>"
                "<tr><td>D7:</td><td>Iris (Absolute)</td></th>"
                "<tr><td>D8:</td><td>Iris (Relative)</td></th>"
                "<tr><td>D9:</td><td>Zoom (Absolute)</td></th>"
                "<tr><td>D10:</td><td>Zoom (Relative)</td></th>"
                "<tr><td>D11:</td><td>PanTilt (Absolute)</td></th>"
                "<tr><td>D12:</td><td>PanTilt (Relative)</td></th>"
                "<tr><td>D13:</td><td>Roll (Absolute)</td></th>"
                "<tr><td>D14:</td><td>Roll (Relative)</td></th>"
                "<tr><td>D15:</td><td>Reserved</td></th>"
                "<tr><td>D16:</td><td>Reserved</td></th>"
                "<tr><td>D17:</td><td>Focus, Auto</td></th>"
                "<tr><td>D18:</td><td>Privacy</td></th>"
                "<tr><td>D19:</td><td>Focus, Simple</td></th>"
                "<tr><td>D20:</td><td>Window</td></th>"
                "<tr><td>D21:</td><td>Region of Interest</td></th>"
                "<tr><td>D22-D23:</td><td>Reserved, set to zero</td></th></table>"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return UsbInputTerminalDescriptor::getFieldInformation(field);
        }

        QString UsbCameraTerminalDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Camera Terminal Descriptor"), true, "UVC 1.5");
            __parseBase(builder);
            return builder.attr("wObjectiveFocalLengthMin", _wObjectiveFocalLengthMin)
                    .attr("wObjectiveFocalLengthMax", _wObjectiveFocalLengthMax)
                    .attr("wOcularFocalLength", _wOcularFocalLength)
                    .attr("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize))
                    .attr("bmControls", QString("0x") + _bmControls.toHex(), __parseBmControls())
                    .end()
                    .build();
        }

        UsbCameraTerminalDescriptor::UsbCameraTerminalDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbInputTerminalDescriptor(parent, descPos)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _wObjectiveFocalLengthMin = *reinterpret_cast<const uint16_t *>(extra + 8);
            _wObjectiveFocalLengthMax = *reinterpret_cast<const uint16_t *>(extra + 10);
            _wOcularFocalLength = *reinterpret_cast<const uint16_t *>(extra + 12);
            _bControlSize = *(extra + 14);
            _bmControls.reserve(_bControlSize);
            for (uint8_t i = 15; i < 15 + _bControlSize; ++i)
                _bmControls.append(*(extra + i));
        }

        QString UsbCameraTerminalDescriptor::__parseBmControls() const
        {
            QStringList supports;
            if (BIT(_bmControls[0], 0))
                supports.append(tr("Scanning Mode"));
            if (BIT(_bmControls[0], 1))
                supports.append(tr("Auto-Exposure Mode"));
            if (BIT(_bmControls[0], 2))
                supports.append(tr("Auto-Exposure Priority"));
            if (BIT(_bmControls[0], 3))
                supports.append(tr("Exposure Time (Absolute)"));
            if (BIT(_bmControls[0], 4))
                supports.append(tr("Exposure Time (Relative)"));
            if (BIT(_bmControls[0], 5))
                supports.append(tr("Focus (Absolute)"));
            if (BIT(_bmControls[0], 6))
                supports.append(tr("Focus (Relative)"));
            if (BIT(_bmControls[0], 7))
                supports.append(tr("Iris (Absolute)"));
            if (_bControlSize > 1)
            {
                if (BIT(_bmControls[1], 0))
                    supports.append(tr("Iris (Relative)"));
                if (BIT(_bmControls[1], 1))
                    supports.append(tr("Zoom (Absolute)"));
                if (BIT(_bmControls[1], 2))
                    supports.append(tr("Zoom (Relative)"));
                if (BIT(_bmControls[1], 3))
                    supports.append(tr("PanTilt (Absolute)"));
                if (BIT(_bmControls[1], 4))
                    supports.append(tr("PanTilt (Relative)"));
                if (BIT(_bmControls[1], 5))
                    supports.append(tr("Roll (Absolute)"));
                if (BIT(_bmControls[1], 6))
                    supports.append(tr("Roll (Relative)"));
            }
            if (_bControlSize > 2)
            {
                if (BIT(_bmControls[2], 1))
                    supports.append(tr("Focus, Auto"));
                if (BIT(_bmControls[2], 2))
                    supports.append(tr("Privacy"));
                if (BIT(_bmControls[2], 3))
                    supports.append(tr("Focus, Simple"));
                if (BIT(_bmControls[2], 4))
                    supports.append(tr("Window"));
                if (BIT(_bmControls[2], 5))
                    supports.append(tr("Region of Interest"));
            }

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        uint8_t UsbSelectorUnitDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbSelectorUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbSelectorUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbSelectorUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        uint8_t UsbSelectorUnitDescriptor::bNrInPins() const
        {
            return _bNrInPins;
        }

        uint8_t UsbSelectorUnitDescriptor::baSourceID(uint8_t index) const
        {
            if (index > 0 && index <= _bNrInPins)
                return _baSourceID[index - 1];
            else
            {
                LOGE(tr("Index %1 for baSourceID out of range, must be 1~%2.")
                     .arg(index).arg(_bNrInPins));
                return 0;
            }
        }

        uint8_t UsbSelectorUnitDescriptor::iSelector() const
        {
            return _iSelector;
        }

        const QStringList &UsbSelectorUnitDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bUnitID",
                "bNrInPins",
                "baSourceID",
                "iSelector",
            };

            return fields;
        }

        QString UsbSelectorUnitDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VC_SELECTOR_UNIT descriptor subtype"},
                {"bUnitID", "A non-zero constant that uniquely identifies "
                 "the Unit within the video function. This "
                 "value is used in all requests to address this "
                 "Unit."},
                {"bNrInPins", "Number of Input Pins of this Unit."},
                {"baSourceID", "ID of the Unit or Terminal to which the nth "
                 "Input Pin of this Selector Unit is connected."},
                {"iSelector", "Index of a string descriptor, describing the "
                 "Selector Unit."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbSelectorUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            UsbHtmlBuilder builder;
            builder.start(tr("Selector Unit Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bUnitID", _bUnitID)
                    .attr("bNrInPins", _bNrInPins);
            for (uint8_t i = 1; i <= _bNrInPins; ++i)
                builder.attr("baSourceID", baSourceID(i), i);
            return builder.strdesc("iSelector", _iSelector, device)
                    .end()
                    .build();
        }

        UsbSelectorUnitDescriptor::UsbSelectorUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bNrInPins = *(extra + 4);
            _baSourceID.reserve(_bNrInPins);
            for (uint8_t i = 5; i < 5 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _iSelector = *(extra + 5 + _bNrInPins);
        }

        uint8_t UsbProcessingUnitDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbProcessingUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbProcessingUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbProcessingUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        uint8_t UsbProcessingUnitDescriptor::bSourceID() const
        {
            return _bSourceID;
        }

        uint16_t UsbProcessingUnitDescriptor::wMaxMultiplier() const
        {
            return _wMaxMultiplier;
        }

        uint8_t UsbProcessingUnitDescriptor::bControlSize() const
        {
            return _bControlSize;
        }

        QByteArray UsbProcessingUnitDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbProcessingUnitDescriptor::iProcessing() const
        {
            return _iProcessing;
        }

        uint8_t UsbProcessingUnitDescriptor::bmVideoStandards() const
        {
            return _bmVideoStandards;
        }

        const QStringList &UsbProcessingUnitDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bUnitID",
                "bSourceID",
                "wMaxMultiplier",
                "bControlSize",
                "bmControls",
                "iProcessing",
                "bmVideoStandards",
            };

            return fields;
        }

        QString UsbProcessingUnitDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VC_PROCESSING_UNIT descriptor subtype"},
                {"bUnitID", "A non-zero constant that uniquely identifies "
                 "the Unit within the video function. This "
                 "value is used in all requests to address this "
                 "Unit."},
                {"bSourceID", "ID of the Unit or Terminal to which this Unit is connected."},
                {"wMaxMultiplier", "If the Digital Multiplier control is supported, "
                 "this field indicates the maximum digital magnification, multiplied by 100. "
                 "For example, for a device that supports 1-4.5X digital zoom (a multiplier of 4.5), "
                 "this field would be set to 450. If the Digital Multiplier control is not supported, "
                 "this field shall be set to 0."},
                {"bControlSize", "Size of the bmControls field, in bytes."},
                {"bmControls", "<p>A bit set to 1 indicates that the mentioned "
                 "Control is supported for the video stream.</p>"
                 "<table><tr><td>D0:</td><td>Brightness</td></th>"
                 "<tr><td>D1:</td><td>Contrast</td></th>"
                 "<tr><td>D2:</td><td>Hue</td></th>"
                 "<tr><td>D3:</td><td>Saturation</td></th>"
                 "<tr><td>D4:</td><td>Sharpness</td></th>"
                 "<tr><td>D5:</td><td>Gamma</td></th>"
                 "<tr><td>D6:</td><td>White Balance Temperature</td></th>"
                 "<tr><td>D7:</td><td>White Balance Component</td></th>"
                 "<tr><td>D8:</td><td>Backlight Compensation</td></th>"
                 "<tr><td>D9:</td><td>Gain</td></th>"
                 "<tr><td>D10:</td><td>Power Line Frequency</td></th>"
                 "<tr><td>D11:</td><td>Hue, Auto</td></th>"
                 "<tr><td>D12:</td><td>White Balance Temperature, Auto</td></th>"
                 "<tr><td>D13:</td><td>White Balance Component, Auto</td></th>"
                 "<tr><td>D14:</td><td>Digital Multiplier</td></th>"
                 "<tr><td>D15:</td><td>Digital Multiplier Limit</td></th>"
                 "<tr><td>D16:</td><td>Analog Video Standard</td></th>"
                 "<tr><td>D17:</td><td>Analog Video Lock Status</td></th>"
                 "<tr><td>D18:</td><td>Contrast, Auto</td></th>"
                 "<tr><td>D19-D23:</td><td>Reserved, set to zero</td></th></table>"},
                {"iProcessing", "Index of a string descriptor that describes this "
                 "processing unit."},
                {"bmVideoStandards", "<p>A bitmap of all analog video standards "
                 "supported by the Processing Unit.</p>"
                "<p>A value of zero indicates that this bitmap "
                 "should be ignored.</p>"
                 "<table><tr><td>D0:</td><td>None</td></th>"
                 "<tr><td>D1:</td><td>NTSC – 525/60</td></th>"
                 "<tr><td>D2:</td><td>PAL – 625/50</td></th>"
                 "<tr><td>D3:</td><td>SECAM – 625/50</td></th>"
                 "<tr><td>D4:</td><td>NTSC – 625/50</td></th>"
                 "<tr><td>D5:</td><td>PAL – 525/60</td></th>"
                 "<tr><td>D6-D7:</td><td>Reserved, set to zero</td></th></table>"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbProcessingUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("Processing Unit Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bUnitID", _bUnitID)
                    .attr("bSourceID", _bSourceID)
                    .attr("wMaxMultiplier", _wMaxMultiplier, QString("%1 x 100").arg(_wMaxMultiplier))
                    .attr("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize))
                    .attr("bmControls", QString("0x") + _bmControls.toHex(), __parseBmControls())
                    .strdesc("iProcessing", _iProcessing, device)
                    .attr("bmVideoStandards", _bmVideoStandards, __parseBmVideoStandards())
                    .end()
                    .build();
        }

        UsbProcessingUnitDescriptor::UsbProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bSourceID = *(extra + 4);
            _wMaxMultiplier = *reinterpret_cast<const uint16_t *>(extra + 5);
            _bControlSize = *(extra + 7);
            _bmControls.reserve(_bControlSize);
            for (uint8_t i = 8; i < 8 + _bControlSize; ++i)
                _bmControls.append(*(extra + i));
            _iProcessing = *(extra + 11);
            _bmVideoStandards = *(extra + 12);
        }

        QString UsbProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList supports;
            if (BIT(_bmControls[0], 0))
                supports.append(tr("Brightness"));
            if (BIT(_bmControls[0], 1))
                supports.append(tr("Contrast"));
            if (BIT(_bmControls[0], 2))
                supports.append(tr("Hue"));
            if (BIT(_bmControls[0], 3))
                supports.append(tr("Saturation"));
            if (BIT(_bmControls[0], 4))
                supports.append(tr("Sharpness"));
            if (BIT(_bmControls[0], 5))
                supports.append(tr("Gamma"));
            if (BIT(_bmControls[0], 6))
                supports.append(tr("White Balance Temperature"));
            if (BIT(_bmControls[0], 7))
                supports.append(tr("White Balance Component"));
            if (_bControlSize > 1)
            {
                if (BIT(_bmControls[1], 0))
                    supports.append(tr("Backlight Compensation"));
                if (BIT(_bmControls[1], 1))
                    supports.append(tr("Gain"));
                if (BIT(_bmControls[1], 2))
                    supports.append(tr("Power Line Frequency"));
                if (BIT(_bmControls[1], 3))
                    supports.append(tr("Hue, Auto"));
                if (BIT(_bmControls[1], 4))
                    supports.append(tr("White Balance Temperature, Auto"));
                if (BIT(_bmControls[1], 5))
                    supports.append(tr("White Balance Component, Auto"));
                if (BIT(_bmControls[1], 6))
                    supports.append(tr("Digital Multiplier"));
                if (BIT(_bmControls[1], 7))
                    supports.append(tr("Digital Multiplier Limit"));
            }
            if (_bControlSize > 2)
            {
                if (BIT(_bmControls[2], 0))
                    supports.append(tr("Analog Video Standard"));
                if (BIT(_bmControls[2], 1))
                    supports.append(tr("Analog Video Lock Status"));
                if (BIT(_bmControls[2], 2))
                    supports.append(tr("Contrast, Auto"));
            }

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbProcessingUnitDescriptor::__parseBmVideoStandards() const
        {
            QStringList supports;
            if (BIT(_bmVideoStandards, 0))
                supports.append(tr("None"));
            if (BIT(_bmVideoStandards, 1))
                supports.append(tr("NTSC - 525/60"));
            if (BIT(_bmVideoStandards, 2))
                supports.append(tr("PAL - 625/50"));
            if (BIT(_bmVideoStandards, 3))
                supports.append(tr("SECAM - 625/50"));
            if (BIT(_bmVideoStandards, 4))
                supports.append(tr("NTSC - 625/50"));
            if (BIT(_bmVideoStandards, 5))
                supports.append(tr("PAL - 525/60"));

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        uint8_t UsbExtensionUnitDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbExtensionUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbExtensionUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbExtensionUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        QByteArray UsbExtensionUnitDescriptor::guidExtensionCode() const
        {
            return _guidExtensionCode;
        }

        uint8_t UsbExtensionUnitDescriptor::bNumControls() const
        {
            return _bNumControls;
        }

        uint8_t UsbExtensionUnitDescriptor::bNrInPins() const
        {
            return _bNrInPins;
        }

        uint8_t UsbExtensionUnitDescriptor::baSourceID(uint8_t index) const
        {
            if (index > 0 && index <= _bNrInPins)
                return _baSourceID[index - 1];
            else
            {
                LOGE(tr("Index %1 for baSourceID out of range, must be 1~%2.")
                     .arg(index).arg(_bNrInPins));
                return 0;
            }
        }

        uint8_t UsbExtensionUnitDescriptor::bControlSize() const
        {
            return _bControlSize;
        }

        QByteArray UsbExtensionUnitDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbExtensionUnitDescriptor::iExtension() const
        {
            return _iExtension;
        }

        const QStringList &UsbExtensionUnitDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bUnitID",
                "guidExtensionCode",
                "bNumControls",
                "bNrInPins",
                "baSourceID",
                "bControlSize",
                "bmControls",
                "iExtension",
            };

            return fields;
        }

        QString UsbExtensionUnitDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VC_EXTENSION_UNIT descriptor subtype"},
                {"bUnitID", "A non-zero constant that uniquely identifies "
                 "the Unit within the video function. This "
                 "value is used in all requests to address this "
                 "Unit."},
                {"guidExtensionCode", "Vendor-specific code identifying the Extension Unit"},
                {"bNumControls", "Number of controls in this extension unit"},
                {"bNrInPins", "Number of Input Pins of this Unit."},
                {"baSourceID", "ID of the Unit or Terminal to which the "
                 "nth Input Pin of this Extension Unit is "
                 "connected."},
                {"bControlSize", "Size of the bmControls field, in bytes."},
                {"bmControls", "<p>A bit set to 1 indicates that the mentioned "
                 "Control is supported:</p>"
                 "<p>D(n*8-1)..0: Vendor-specific</p>"},
                {"iExtension", "Index of a string descriptor that describes "
                 "this extension unit."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbExtensionUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            UsbHtmlBuilder builder;
            builder.start(tr("Extension Unit Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bUnitID", _bUnitID)
                    .attr("guidExtensionCode", "", hexUuid(_guidExtensionCode))
                    .attr("bNumControls", _bNumControls)
                    .attr("bNrInPins", _bNrInPins);
            for (uint8_t i = 1; i <= _bNrInPins; ++i)
                builder.attr("baSourceID", baSourceID(i), i);
            return builder.attr("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize))
                    .attr("bmControls", QString("0x") + _bmControls.toHex(), "")
                    .strdesc("iExtension", _iExtension, device)
                    .end()
                    .build();
        }

        UsbExtensionUnitDescriptor::UsbExtensionUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _guidExtensionCode.reserve(16);
            for (uint8_t i = 4; i < 20; ++i)
                _guidExtensionCode.append(*(extra + i));
            _bNumControls = *(extra + 20);
            _bNrInPins = *(extra + 21);
            _baSourceID.reserve(_bNrInPins);
            for (uint8_t i = 22; i < 22 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _bControlSize = *(extra + 22 + _bNrInPins);
            _bmControls.reserve(_bControlSize);
            for (uint8_t i = 23 + _bNrInPins; i < 23 + _bNrInPins + _bControlSize; ++i)
                _bmControls.append(*(extra + i));
            _iExtension = *(extra + 23 + _bNrInPins + _bControlSize);
        }

        uint8_t UsbEncodingUnitDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbEncodingUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbEncodingUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbEncodingUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        uint8_t UsbEncodingUnitDescriptor::bSourceID() const
        {
            return _bSourceID;
        }

        uint8_t UsbEncodingUnitDescriptor::iEncoding() const
        {
            return _iEncoding;
        }

        uint8_t UsbEncodingUnitDescriptor::bControlSize() const
        {
            return _bControlSize;
        }

        QByteArray UsbEncodingUnitDescriptor::bmControls() const
        {
            return _bmControls;
        }

        QByteArray UsbEncodingUnitDescriptor::bmControlsRuntime() const
        {
            return _bmControlsRuntime;
        }

        const QStringList &UsbEncodingUnitDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bUnitID",
                "bSourceID",
                "iEncoding",
                "bControlSize",
                "bmControls",
                "bmControlsRuntime",
            };

            return fields;
        }

        QString UsbEncodingUnitDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VC_ENCODING_UNIT descriptor subtype"},
                {"bUnitID", "A non-zero constant that uniquely identifies "
                 "the Unit within the video function. This "
                 "value is used in all requests to address this "
                 "Unit."},
                {"bSourceID", "ID of the Unit or Terminal to which this "
                 "Unit is connected."},
                {"iEncoding", "Index of a string descriptor that describes "
                 "this encoding unit."},
                {"bControlSize", "Size, in bytes, of the bmControls and "
                 "bmControlsRuntime fields: The value "
                 "must be 3."},
                {"bmControls", "<p>A bit set to 1 indicates that the specified "
                 "control is supported for initialization:</p>"
                 "<table><tr><td>D0:</td><td>Select Layer</td></th>"
                 "<tr><td>D1:</td><td>Profile and Toolset</td></th>"
                 "<tr><td>D2:</td><td>Video Resolution</td></th>"
                 "<tr><td>D3:</td><td>Minimum Frame Interval</td></th>"
                 "<tr><td>D4:</td><td>Slice Mode</td></th>"
                 "<tr><td>D5:</td><td>Rate Control Mode</td></th>"
                 "<tr><td>D6:</td><td>Average Bit Rate</td></th>"
                 "<tr><td>D7:</td><td>CPB Size</td></th>"
                 "<tr><td>D8:</td><td>Peak Bit Rate</td></th>"
                 "<tr><td>D9:</td><td>Quantization Parameter</td></th>"
                 "<tr><td>D10:</td><td>Synchronization and Long-Term Reference Frame</td></th>"
                 "<tr><td>D11:</td><td>Long-Term Buffer</td></th>"
                 "<tr><td>D12:</td><td>Picture Long-Term Reference</td></th>"
                 "<tr><td>D13:</td><td>LTR Validation</td></th>"
                 "<tr><td>D14:</td><td>Level IDC</td></th>"
                 "<tr><td>D15:</td><td>SEI Message</td></th>"
                 "<tr><td>D16:</td><td>QP Range</td></th>"
                 "<tr><td>D17:</td><td>Priority ID</td></th>"
                 "<tr><td>D18:</td><td>Start or Stop Layer/View</td></th>"
                 "<tr><td>D19:</td><td>Error Resiliency</td></th>"
                 "<tr><td>D20-D23:</td><td>Reserved; set to zero</td></th></table>"},
                {"bmControlsRuntime", "<p>A bit set to 1 indicates that the mentioned "
                 "control is supported during runtime:</p>"
                 "<table><tr><td>D0:</td><td>Select Layer</td></th>"
                 "<tr><td>D1:</td><td>Profile and Toolset</td></th>"
                 "<tr><td>D2:</td><td>Video Resolution</td></th>"
                 "<tr><td>D3:</td><td>Minimum Frame Interval</td></th>"
                 "<tr><td>D4:</td><td>Slice Mode</td></th>"
                 "<tr><td>D5:</td><td>Rate Control Mode</td></th>"
                 "<tr><td>D6:</td><td>Average Bit Rate</td></th>"
                 "<tr><td>D7:</td><td>CPB Size</td></th>"
                 "<tr><td>D8:</td><td>Peak Bit Rate</td></th>"
                 "<tr><td>D9:</td><td>Quantization Parameter</td></th>"
                 "<tr><td>D10:</td><td>Synchronization and Long-Term Reference Frame</td></th>"
                 "<tr><td>D11:</td><td>Long-Term Buffer</td></th>"
                 "<tr><td>D12:</td><td>Picture Long-Term Reference</td></th>"
                 "<tr><td>D13:</td><td>LTR Validation</td></th>"
                 "<tr><td>D14:</td><td>Level IDC</td></th>"
                 "<tr><td>D15:</td><td>SEI Message</td></th>"
                 "<tr><td>D16:</td><td>QP Range</td></th>"
                 "<tr><td>D17:</td><td>Priority ID</td></th>"
                 "<tr><td>D18:</td><td>Start or Stop Layer/View</td></th>"
                 "<tr><td>D19:</td><td>Error Resiliency</td></th>"
                 "<tr><td>D20-D23:</td><td>Reserved; set to zero</td></th></table>"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbEncodingUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("Encoding Unit Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bUnitID", _bUnitID)
                    .attr("bSourceID", _bSourceID)
                    .strdesc("iEncoding", _iEncoding, device)
                    .attr("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize))
                    .attr("bmControls", QString("0x") + _bmControls.toHex(), __parseBmControls(_bmControls))
                    .attr("bmControlsRuntime", QString("0x") + _bmControlsRuntime.toHex(),
                          __parseBmControls(_bmControlsRuntime))
                    .end()
                    .build();
        }

        UsbEncodingUnitDescriptor::UsbEncodingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bSourceID = *(extra + 4);
            _iEncoding = *(extra + 5);
            _bControlSize = *(extra + 6);
            _bmControls.reserve(_bControlSize);
            for (uint8_t i = 7; i < 7 + _bControlSize; ++i)
                _bmControls.append(*(extra + i));
            _bmControlsRuntime.reserve(_bControlSize);
            for (uint8_t i = 7 + _bControlSize; i < 7 + _bControlSize * 2; ++i)
                _bmControlsRuntime.append(*(extra + i));
        }

        QString UsbEncodingUnitDescriptor::__parseBmControls(const QByteArray &bmControls) const
        {
            QStringList supports;
            if (BIT(bmControls[0], 0))
                supports.append(tr("Select Layer"));
            if (BIT(bmControls[0], 1))
                supports.append(tr("Profile and Toolset"));
            if (BIT(bmControls[0], 2))
                supports.append(tr("Video Resolution"));
            if (BIT(bmControls[0], 3))
                supports.append(tr("Minimum Frame Interval"));
            if (BIT(bmControls[0], 4))
                supports.append(tr("Slice Mode"));
            if (BIT(bmControls[0], 5))
                supports.append(tr("Rate Control Mode"));
            if (BIT(bmControls[0], 6))
                supports.append(tr("Average Bit Rate"));
            if (BIT(bmControls[0], 7))
                supports.append(tr("CPB Size"));
            if (BIT(bmControls[1], 0))
                supports.append(tr("Peak Bit Rate"));
            if (BIT(bmControls[1], 1))
                supports.append(tr("Quantization Parameter"));
            if (BIT(bmControls[1], 2))
                supports.append(tr("Synchronization and Long-Term Reference Frame"));
            if (BIT(bmControls[1], 3))
                supports.append(tr("Long-Term Buffer"));
            if (BIT(bmControls[1], 4))
                supports.append(tr("Picture Long-Term Reference"));
            if (BIT(bmControls[1], 5))
                supports.append(tr("LTR Validation"));
            if (BIT(bmControls[1], 6))
                supports.append(tr("Level IDC"));
            if (BIT(bmControls[1], 7))
                supports.append(tr("SEI Message"));
            if (BIT(bmControls[2], 0))
                supports.append(tr("QP Range"));
            if (BIT(bmControls[2], 1))
                supports.append(tr("Priority ID"));
            if (BIT(bmControls[2], 2))
                supports.append(tr("Start or Stop Layer/View"));
            if (BIT(bmControls[2], 3))
                supports.append(tr("Error Resiliency"));

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }
    } // namespace uvc
} // namespace usb
