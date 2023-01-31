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

        QString UsbClassSpecificVCInterfaceHeaderDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Class-Specific VC Interface Header Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bcdUVC", _bcdUVC, "")
                    .attr("wTotalLength", _wTotalLength)
                    .attr("dwClockFrequency", _dwClockFrequency, tr("%1 Hz").arg(_dwClockFrequency))
                    .attr("bInCollection", _bInCollection);
            for (uint8_t i = 1; i <= _bInCollection; ++i)
                builder.attr(QString("baInterfaceNr(%1)").arg(i), baInterfaceNr(i));
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

        QString UsbInputTerminalDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Input Terminal Descriptor"));
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

        QString UsbOutputTerminalDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Output Terminal Descriptor"));
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

        QString UsbCameraTerminalDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Camera Terminal Descriptor"));
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

        QString UsbSelectorUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            UsbHtmlBuilder builder;
            builder.start(tr("Selector Unit Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bUnitID", _bUnitID)
                    .attr("bNrInPins", _bNrInPins);
            for (uint8_t i = 1; i <= _bNrInPins; ++i)
                builder.attr(QString("baSourceID(%1)").arg(i), baSourceID(i));
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

        QString UsbProcessingUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("Processing Unit Descriptor"))
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

        QString UsbExtensionUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            UsbHtmlBuilder builder;
            builder.start(tr("Extension Unit Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bUnitID", _bUnitID)
                    .attr("guidExtensionCode", "", hexUuid(_guidExtensionCode))
                    .attr("bNumControls", _bNumControls)
                    .attr("bNrInPins", _bNrInPins);
            for (uint8_t i = 1; i <= _bNrInPins; ++i)
                builder.attr(QString("baSourceID(%1)").arg(i), baSourceID(i));
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

        QString UsbEncodingUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("Encoding Unit Descriptor"))
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
