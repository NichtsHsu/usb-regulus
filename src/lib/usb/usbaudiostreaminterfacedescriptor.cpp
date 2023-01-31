#include "usbaudiostreaminterfacedescriptor.h"
#include "usbaudiocontrolinterfacedescriptor.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

namespace usb {

    UsbAudioStreamInterfaceDescriptorBasic::UsbAudioStreamInterfaceDescriptorBasic(
            UsbInterfaceDescriptor *parent):
        UsbInterfaceExtraDescriptor(parent)
    {

    }

    InterfaceExtraDescriptorType UsbAudioStreamInterfaceDescriptorBasic::type() const
    {
        return InterfaceExtraDescriptorType::AS;
    }

    namespace uac1 {
        UsbAudioStreamInterfaceDescriptor::UsbAudioStreamInterfaceDescriptor(
                UsbInterfaceDescriptor *parent):
            UsbAudioStreamInterfaceDescriptorBasic(parent)
        {

        }

        uint8_t UsbAudioStreamInterfaceDescriptor::specVersion() const
        {
            return 1;
        }

        QString UsbAudioStreamInterfaceDescriptor::strSubtype(uint8_t subtype)
        {
            static const QString subtypes[] = {
                "AS_DESCRIPTOR_UNDEFINED",
                "AS_GENERAL",
                "FORMAT_TYPE",
                "FORMAT_SPECIFIC",
            };

            return subtypes[subtype < (sizeof(subtypes) / sizeof(QString)) ? subtype : 0];
        }

        QString UsbAudioStreamInterfaceDescriptor::strWFormatTag(uint16_t wFormatTag)
        {
            static const QMap<uint16_t, const char *> formatTags = {
                { 0x0000, "TYPE_I_UNDEFINED" },
                { 0x0001, "PCM" },
                { 0x0002, "PCM8" },
                { 0x0003, "IEEE_FLOAT" },
                { 0x0004, "ALAW" },
                { 0x0005, "MULAW" },
                { 0x1000, "TYPE_II_UNDEFINED" },
                { 0x1001, "MPEG" },
                { 0x1002, "AC-3" },
                { 0x2000, "TYPE_III_UNDEFINED" },
                { 0x2001, "IEC1937_AC-3" },
                { 0x2002, "IEC1937_MPEG-1_Layer1" },
                { 0x2003, "IEC1937_MPEG-1_Layer2/3 or IEC1937_MPEG-2_NOEXT" },
                { 0x2004, "IEC1937_MPEG-2_EXT" },
                { 0x2005, "IEC1937_MPEG-2_Layer1_LS" },
                { 0x2006, "IEC1937_MPEG-2_Layer2/3_LS" },
            };

            return formatTags.value(wFormatTag, "");
        }

        UsbAudioStreamInterfaceDescriptor *UsbAudioStreamInterfaceDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24)
                return nullptr;

            UsbAudioStreamInterfaceDescriptor *desc;
            switch (extra[2])
            {
                case 1:
                    /* AS_GENERAL */
                    desc = new UsbClassSpecificASInterfaceDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 2:
                    /* FORMAT_TYPE */
                    desc = UsbFormatTypeDescriptor::get(
                                interfaceDescriptor, descPos);
                break;
                case 3:
                    /* FORMAT_SPECIFIC */
                    desc = UsbFormatSpecificDescriptor::get(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
            }

            return desc;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bTerminalLink() const
        {
            return _bTerminalLink;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bDelay() const
        {
            return _bDelay;
        }

        uint16_t UsbClassSpecificASInterfaceDescriptor::wFormatTag() const
        {
            return _wFormatTag;
        }

        QString UsbClassSpecificASInterfaceDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Class-Specific AS Interface Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bTerminalLink", _bTerminalLink)
                    .attr("bDelay", _bDelay)
                    .attr("wFormatTag", _wFormatTag, strWFormatTag(_wFormatTag))
                    .end()
                    .build();
        }

        UsbClassSpecificASInterfaceDescriptor::UsbClassSpecificASInterfaceDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bTerminalLink = *(extra + 3);
            _bDelay = *(extra + 4);
            _wFormatTag = *reinterpret_cast<const uint16_t *>(extra + 5);
        }

        QString UsbFormatTypeDescriptor::strBFormatType(uint8_t bFormatType)
        {
            const static QString formatTypes[] = {
                "FORMAT_TYPE_UNDEFINED",
                "FORMAT_TYPE_I",
                "FORMAT_TYPE_II",
                "FORMAT_TYPE_II",
            };

            return formatTypes[bFormatType < (sizeof(formatTypes) / sizeof(QString)) ? bFormatType : 0];
        }

        UsbFormatTypeDescriptor::UsbFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent):
            UsbAudioStreamInterfaceDescriptor(parent)

        {

        }

        UsbFormatTypeDescriptor *UsbFormatTypeDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24 || extra[2] != 0x02)
                return nullptr;

            UsbFormatTypeDescriptor *desc;
            switch (extra[3])
            {
                case 1:
                    /* FORMAT_TYPE_I */
                    desc = new UsbTypeIFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 2:
                    /* FORMAT_TYPE_II */
                    desc = new UsbTypeIIFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 3:
                    /* FORMAT_TYPE_III */
                    desc = new UsbTypeIIIFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
            }

            return desc;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bSubframeSize() const
        {
            return _bSubframeSize;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bBitResolution() const
        {
            return _bBitResolution;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bSamFreqType() const
        {
            return _bSamFreqType;
        }

        QByteArray UsbTypeIFormatTypeDescriptor::tLowerSamFreq() const
        {
            if (_bSamFreqType == 0)
                return _tSamFreq[0];
            else
            {
                LOGE(tr("The tLowerSamFreq field is only valid when the bSamFreqType field is 0."));
                return QByteArray();
            }
        }

        QByteArray UsbTypeIFormatTypeDescriptor::tUpperSamFreq() const
        {
            if (_bSamFreqType == 0)
                return _tSamFreq[1];
            else
            {
                LOGE(tr("The tUpperSamFreq field is only valid when the bSamFreqType field is 0."));
                return QByteArray();
            }
        }

        QByteArray UsbTypeIFormatTypeDescriptor::tSamFreq(uint8_t index) const
        {
            if (_bSamFreqType == 0)
            {
                LOGE(tr("The tSamFreq field is only valid when the bSamFreqType field is NOT 0."));
                return QByteArray();
            }
            else if (index > 0 && index <= _bSamFreqType)
                return _tSamFreq[index - 1];
            else
            {
                LOGE(tr("Index %1 for tSamFreq out of range, must be 1~%2.")
                     .arg(index).arg(_bSamFreqType));
                return 0;
            }
        }

        QString UsbTypeIFormatTypeDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder = UsbHtmlBuilder();
            builder.start(tr("Type I Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("bNrChannels", _bNrChannels)
                    .attr("bSubframeSize", _bSubframeSize, tr("%1 byte(s)").arg(_bSubframeSize))
                    .attr("bBitResolution", _bBitResolution)
                    .attr("bSamFreqType", _bSamFreqType, _bSamFreqType ?
                              tr("%1 discrete sampling frequencies") :
                              tr("Continuous sampling frequency"));

            if (_bSamFreqType)
            {
                for (uint8_t i = 1; i <= _bSamFreqType; ++i)
                    builder.attr(QString("tSamFreq[%1]").arg(i), QString("0x") + tSamFreq(i).toHex(), "");
            }
            else
            {
                builder.attr("tLowerSamFreq", QString("0x") + tLowerSamFreq().toHex(), "");
                builder.attr("tUpperSamFreq", QString("0x") + tUpperSamFreq().toHex(), "");
            }

            return builder.end().build();
        }

        UsbTypeIFormatTypeDescriptor::UsbTypeIFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
            _bNrChannels = *(extra + 4);
            _bSubframeSize = *(extra + 5);
            _bBitResolution = *(extra + 6);
            _bSamFreqType = *(extra + 7);
            if (_bSamFreqType)
            {
                _tSamFreq.reserve(_bSamFreqType);
                for (uint8_t i = 0; i < _bSamFreqType; ++i)
                    _tSamFreq.append(QByteArray(reinterpret_cast<const char *>(
                                                    extra + 8 + i * 3), 3));
            }
            else
            {
                _tSamFreq.reserve(2);
                _tSamFreq.append(QByteArray(reinterpret_cast<const char *>(
                                                extra + 8), 3));
                _tSamFreq.append(QByteArray(reinterpret_cast<const char *>(
                                                extra + 11), 3));
            }
        }

        uint8_t UsbTypeIIFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbTypeIIFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbTypeIIFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbTypeIIFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint16_t UsbTypeIIFormatTypeDescriptor::wMaxBitRate() const
        {
            return _wMaxBitRate;
        }

        uint16_t UsbTypeIIFormatTypeDescriptor::wSamplesPerFrame() const
        {
            return _wSamplesPerFrame;
        }

        uint8_t UsbTypeIIFormatTypeDescriptor::bSamFreqType() const
        {
            return _bSamFreqType;
        }

        QByteArray UsbTypeIIFormatTypeDescriptor::tLowerSamFreq() const
        {
            if (_bSamFreqType == 0)
                return _tSamFreq[0];
            else
            {
                LOGE(tr("The tLowerSamFreq field is only valid when the bSamFreqType field is 0."));
                return QByteArray();
            }
        }

        QByteArray UsbTypeIIFormatTypeDescriptor::tUpperSamFreq() const
        {
            if (_bSamFreqType == 0)
                return _tSamFreq[1];
            else
            {
                LOGE(tr("The tUpperSamFreq field is only valid when the bSamFreqType field is 0."));
                return QByteArray();
            }
        }

        QByteArray UsbTypeIIFormatTypeDescriptor::tSamFreq(uint8_t index) const
        {
            if (_bSamFreqType == 0)
            {
                LOGE(tr("The tSamFreq field is only valid when the bSamFreqType field is NOT 0."));
                return QByteArray();
            }
            else if (index > 0 && index <= _bSamFreqType)
                return _tSamFreq[index - 1];
            else
            {
                LOGE(tr("Index %1 for tSamFreq out of range, must be 1~%2.")
                     .arg(index).arg(_bSamFreqType));
                return 0;
            }
        }

        QString UsbTypeIIFormatTypeDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder = UsbHtmlBuilder();
            builder.start(tr("Type II Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("wMaxBitRate", _wMaxBitRate, QString("%1 kb/s").arg(_wMaxBitRate))
                    .attr("wSamplesPerFrame", _wSamplesPerFrame)
                    .attr("bSamFreqType", _bSamFreqType, _bSamFreqType ?
                              tr("%1 discrete sampling frequencies") :
                              tr("Continuous sampling frequency"));

            if (_bSamFreqType)
            {
                for (uint8_t i = 1; i <= _bSamFreqType; ++i)
                    builder.attr(QString("tSamFreq[%1]").arg(i), QString("0x") + tSamFreq(i).toHex(), "");
            }
            else
            {
                builder.attr("tLowerSamFreq", QString("0x") + tLowerSamFreq().toHex(), "");
                builder.attr("tUpperSamFreq", QString("0x") + tUpperSamFreq().toHex(), "");
            }

            return builder.end().build();
        }

        UsbTypeIIFormatTypeDescriptor::UsbTypeIIFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
            _wMaxBitRate = *reinterpret_cast<const uint16_t *>(extra + 4);
            _wSamplesPerFrame = *reinterpret_cast<const uint16_t *>(extra + 6);
            _bSamFreqType = *(extra + 8);
            if (_bSamFreqType)
            {
                _tSamFreq.reserve(_bSamFreqType);
                for (uint8_t i = 0; i < _bSamFreqType; ++i)
                    _tSamFreq.append(QByteArray(reinterpret_cast<const char *>(
                                                    extra + 9 + i * 3), 3));
            }
            else
            {
                _tSamFreq.reserve(2);
                _tSamFreq.append(QByteArray(reinterpret_cast<const char *>(
                                                extra + 9), 3));
                _tSamFreq.append(QByteArray(reinterpret_cast<const char *>(
                                                extra + 12), 3));
            }
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bSubframeSize() const
        {
            return _bSubframeSize;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bBitResolution() const
        {
            return _bBitResolution;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bSamFreqType() const
        {
            return _bSamFreqType;
        }

        QByteArray UsbTypeIIIFormatTypeDescriptor::tLowerSamFreq() const
        {
            if (_bSamFreqType == 0)
                return _tSamFreq[0];
            else
            {
                LOGE(tr("The tLowerSamFreq field is only valid when the bSamFreqType field is 0."));
                return QByteArray();
            }
        }

        QByteArray UsbTypeIIIFormatTypeDescriptor::tUpperSamFreq() const
        {
            if (_bSamFreqType == 0)
                return _tSamFreq[1];
            else
            {
                LOGE(tr("The tUpperSamFreq field is only valid when the bSamFreqType field is 0."));
                return QByteArray();
            }
        }

        QByteArray UsbTypeIIIFormatTypeDescriptor::tSamFreq(uint8_t index) const
        {
            if (_bSamFreqType == 0)
            {
                LOGE(tr("The tSamFreq field is only valid when the bSamFreqType field is NOT 0."));
                return QByteArray();
            }
            else if (index > 0 && index <= _bSamFreqType)
                return _tSamFreq[index - 1];
            else
            {
                LOGE(tr("Index %1 for tSamFreq out of range, must be 1~%2.")
                     .arg(index).arg(_bSamFreqType));
                return 0;
            }
        }

        QString UsbTypeIIIFormatTypeDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder = UsbHtmlBuilder();
            builder.start(tr("Type III Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("bNrChannels", _bNrChannels)
                    .attr("bSubframeSize", _bSubframeSize, tr("%1 byte(s)").arg(_bSubframeSize))
                    .attr("bBitResolution", _bBitResolution)
                    .attr("bSamFreqType", _bSamFreqType, _bSamFreqType ?
                              tr("%1 discrete sampling frequencies") :
                              tr("Continuous sampling frequency"));

            if (_bSamFreqType)
            {
                for (uint8_t i = 1; i <= _bSamFreqType; ++i)
                    builder.attr(QString("tSamFreq[%1]").arg(i), QString("0x") + tSamFreq(i).toHex(), "");
            }
            else
            {
                builder.attr("tLowerSamFreq", QString("0x") + tLowerSamFreq().toHex(), "");
                builder.attr("tUpperSamFreq", QString("0x") + tUpperSamFreq().toHex(), "");
            }

            return builder.end().build();
        }

        UsbTypeIIIFormatTypeDescriptor::UsbTypeIIIFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
            _bNrChannels = *(extra + 4);
            _bSubframeSize = *(extra + 5);
            _bBitResolution = *(extra + 6);
            _bSamFreqType = *(extra + 7);
            if (_bSamFreqType)
            {
                _tSamFreq.reserve(_bSamFreqType);
                for (uint8_t i = 0; i < _bSamFreqType; ++i)
                    _tSamFreq.append(QByteArray(reinterpret_cast<const char *>(
                                                    extra + 8 + i * 3), 3));
            }
            else
            {
                _tSamFreq.reserve(2);
                _tSamFreq.append(QByteArray(reinterpret_cast<const char *>(
                                                extra + 8), 3));
                _tSamFreq.append(QByteArray(reinterpret_cast<const char *>(
                                                extra + 11), 3));
            }
        }

        UsbFormatSpecificDescriptor::UsbFormatSpecificDescriptor(
                UsbInterfaceDescriptor *parent):
            UsbAudioStreamInterfaceDescriptor(parent)
        {

        }

        UsbFormatSpecificDescriptor *UsbFormatSpecificDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24 || extra[2] != 0x02)
                return nullptr;

            UsbFormatSpecificDescriptor *desc;
            switch (*reinterpret_cast<const uint16_t *>(extra + 3))
            {
                case 0x1001:
                    /* MPEG */
                    desc = new UsbMpegFormatSpecificDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x1002:
                    /* AC-3 */
                    desc = new UsbAc3FormatSpecificDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
            }

            return desc;
        }

        uint8_t UsbMpegFormatSpecificDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbMpegFormatSpecificDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbMpegFormatSpecificDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint16_t UsbMpegFormatSpecificDescriptor::wFormatTag() const
        {
            return _wFormatTag;
        }

        uint16_t UsbMpegFormatSpecificDescriptor::bmMPEGCapabilities() const
        {
            return _bmMPEGCapabilities;
        }

        uint8_t UsbMpegFormatSpecificDescriptor::bmMPEGFeatures() const
        {
            return _bmMPEGFeatures;
        }

        QString UsbMpegFormatSpecificDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("MPEG Format-Specific Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("wFormatTag", _wFormatTag, strWFormatTag(_wFormatTag))
                    .attr("bmMPEGCapabilities", _bmMPEGCapabilities, __parseBmMPEGCapabilities())
                    .attr("bmMPEGFeatures", _bmMPEGFeatures, __parseBmMPEGFeatures())
                    .end()
                    .build();
        }

        UsbMpegFormatSpecificDescriptor::UsbMpegFormatSpecificDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatSpecificDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _wFormatTag = *reinterpret_cast<const uint16_t *>(extra + 3);
            _bmMPEGCapabilities = *reinterpret_cast<const uint16_t *>(extra + 5);
            _bmMPEGFeatures = *(extra + 7);
        }

        QString UsbMpegFormatSpecificDescriptor::__parseBmMPEGCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmMPEGCapabilities, 0))
                supports.append(tr("Supports Layer I"));
            if (BIT(_bmMPEGCapabilities, 1))
                supports.append(tr("Supports Layer II"));
            if (BIT(_bmMPEGCapabilities, 2))
                supports.append(tr("Supports Layer III"));
            if (BIT(_bmMPEGCapabilities, 3))
                supports.append(tr("MPEG-1 only"));
            if (BIT(_bmMPEGCapabilities, 4))
                supports.append(tr("MPEG-1 dual-channel"));
            if (BIT(_bmMPEGCapabilities, 5))
                supports.append(tr("MPEG-2 second stereo"));
            if (BIT(_bmMPEGCapabilities, 6))
                supports.append(tr("MPEG-2 7.1 channel augmentation"));
            if (BIT(_bmMPEGCapabilities, 7))
                supports.append(tr("Adaptive multi-channel prediction"));
            switch (CUT(_bmMPEGCapabilities, 8, 9))
            {
                case 0b01:
                    supports.append(tr("Supports MPEG-2 multilingual at Fs"));
                break;
                case 0b11:
                    supports.append(tr("Supports MPEG-2 multilingual at Fs and ½Fs"));
                break;
            }

            return supports.join(UsbHtmlBuilder::UsbHtmlBuilder::NEWLINE);
        }

        QString UsbMpegFormatSpecificDescriptor::__parseBmMPEGFeatures() const
        {
            QStringList supports;

            if (CUT(_bmMPEGFeatures, 4, 5))
                supports.append(tr("Internal Dynamic Range Control:"));
            switch (CUT(_bmMPEGFeatures, 4, 5))
            {
                case 0b01:
                    supports.append(UsbHtmlBuilder::TAB + tr("not scalable"));
                break;
                case 0b10:
                    supports.append(UsbHtmlBuilder::TAB + tr("scalable"));
                    supports.append(UsbHtmlBuilder::TAB + tr("common boost"));
                    supports.append(UsbHtmlBuilder::TAB + tr("cut scaling value"));
                break;
                case 0b11:
                    supports.append(UsbHtmlBuilder::TAB + tr("scalable"));
                    supports.append(UsbHtmlBuilder::TAB + tr("separate boost"));
                    supports.append(UsbHtmlBuilder::TAB + tr("cut scaling value"));
                break;
            }

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        uint8_t UsbAc3FormatSpecificDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbAc3FormatSpecificDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbAc3FormatSpecificDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint16_t UsbAc3FormatSpecificDescriptor::wFormatTag() const
        {
            return _wFormatTag;
        }

        uint32_t UsbAc3FormatSpecificDescriptor::bmBSID() const
        {
            return _bmBSID;
        }

        uint8_t UsbAc3FormatSpecificDescriptor::bmAC3Features() const
        {
            return _bmAC3Features;
        }

        QString UsbAc3FormatSpecificDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("AC-3 Format-Specific Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("wFormatTag", _wFormatTag, strWFormatTag(_wFormatTag))
                    .attr("bmBSID", _bmBSID, "")
                    .attr("bmAC3Features", _bmAC3Features, __parseBmAC3Features())
                    .end()
                    .build();
        }

        UsbAc3FormatSpecificDescriptor::UsbAc3FormatSpecificDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatSpecificDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _wFormatTag = *reinterpret_cast<const uint16_t *>(extra + 3);
            _bmBSID = *reinterpret_cast<const uint32_t *>(extra + 5);
            _bmAC3Features = *(extra + 9);
        }

        QString UsbAc3FormatSpecificDescriptor::__parseBmAC3Features() const
        {
            QStringList supports;

            if (BIT(_bmAC3Features, 0))
                supports.append(tr("RF mode"));
            if (BIT(_bmAC3Features, 1))
                supports.append(tr("Line mode"));
            if (BIT(_bmAC3Features, 2))
                supports.append(tr("Custom0 mode"));
            if (BIT(_bmAC3Features, 3))
                supports.append(tr("Custom1 mode"));
            if (CUT(_bmAC3Features, 4, 5))
                supports.append(tr("Internal Dynamic Range Control:"));
            switch (CUT(_bmAC3Features, 4, 5))
            {
                case 0b01:
                    supports.append(UsbHtmlBuilder::TAB + tr("not scalable"));
                break;
                case 0b10:
                    supports.append(UsbHtmlBuilder::TAB + tr("scalable"));
                    supports.append(UsbHtmlBuilder::TAB + tr("common boost"));
                    supports.append(UsbHtmlBuilder::TAB + tr("cut scaling value"));
                break;
                case 0b11:
                    supports.append(UsbHtmlBuilder::TAB + tr("scalable"));
                    supports.append(UsbHtmlBuilder::TAB + tr("separate boost"));
                    supports.append(UsbHtmlBuilder::TAB + tr("cut scaling value"));
                break;
            }

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }
    } // namespace uac1

    namespace uac2 {
        UsbAudioStreamInterfaceDescriptor::UsbAudioStreamInterfaceDescriptor(UsbInterfaceDescriptor *parent):
            UsbAudioStreamInterfaceDescriptorBasic(parent)
        {

        }

        uint8_t UsbAudioStreamInterfaceDescriptor::specVersion() const
        {
            return 2;
        }

        QString UsbAudioStreamInterfaceDescriptor::strSubtype(uint8_t subtype)
        {
            static const QString subtypes[] = {
                "AS_DESCRIPTOR_UNDEFINED",
                "AS_GENERAL",
                "FORMAT_TYPE",
                "ENCODER",
                "DECODER",
            };

            return subtypes[subtype < (sizeof(subtypes) / sizeof(QString)) ? subtype : 0];
        }

        QString UsbAudioStreamInterfaceDescriptor::strBFormatType(uint8_t bFormatType)
        {
            const static QMap<uint8_t, QString> formatTypes = {
                { 0x00, "FORMAT_TYPE_UNDEFINED" },
                { 0x01, "FORMAT_TYPE_I" },
                { 0x02, "FORMAT_TYPE_II" },
                { 0x03, "FORMAT_TYPE_III" },
                { 0x04, "FORMAT_TYPE_IV" },
                { 0x81, "EXT_FORMAT_TYPE_I" },
                { 0x82, "EXT_FORMAT_TYPE_II" },
                { 0x83, "EXT_FORMAT_TYPE_III" },
            };

            return formatTypes.value(bFormatType, "FORMAT_TYPE_UNDEFINED");
        }

        QStringList UsbAudioStreamInterfaceDescriptor::parseBmChannelConfig(uint32_t bmChannelConfig)
        {
            return UsbAudioControlInterfaceDescriptor::parseBmChannelConfig(bmChannelConfig);
        }

        QStringList UsbAudioStreamInterfaceDescriptor::parseBmFormats(uint8_t bFormatType, uint32_t bmFormats)
        {
            QStringList formats;
            switch (bFormatType)
            {
                case 0x01:
                    /* FORMAT_TYPE_I */
                    if (BIT(bmFormats, 0))
                        formats.append("PCM");
                    if (BIT(bmFormats, 1))
                        formats.append("PCM8");
                    if (BIT(bmFormats, 2))
                        formats.append("IEEE_FLOAT");
                    if (BIT(bmFormats, 3))
                        formats.append("ALAW");
                    if (BIT(bmFormats, 4))
                        formats.append("MULAW");
                    if (BIT(bmFormats, 31))
                        formats.append("TYPE_I_RAW_DATA");
                break;
                case 0x02:
                    /* FORMAT_TYPE_II */
                    if (BIT(bmFormats, 0))
                        formats.append("MPEG");
                    if (BIT(bmFormats, 1))
                        formats.append("AC-3");
                    if (BIT(bmFormats, 2))
                        formats.append("WMA");
                    if (BIT(bmFormats, 3))
                        formats.append("DTS");
                    if (BIT(bmFormats, 31))
                        formats.append("TYPE_II_RAW_DATA");
                break;
                case 0x03:
                    /* FORMAT_TYPE_III */
                    if (BIT(bmFormats, 0))
                        formats.append("IEC61937_AC-3");
                    if (BIT(bmFormats, 1))
                        formats.append("IEC61937_MPEG-1_Layer1");
                    if (BIT(bmFormats, 2))
                        formats.append("IEC61937_MPEG-1_Layer2/3 or IEC61937_MPEG-2_NOEXT");
                    if (BIT(bmFormats, 3))
                        formats.append("IEC61937_MPEG-2_EXT");
                    if (BIT(bmFormats, 4))
                        formats.append("IEC61937_MPEG-2_AAC_ADTS");
                    if (BIT(bmFormats, 5))
                        formats.append("IEC61937_MPEG-2_Layer1_LS");
                    if (BIT(bmFormats, 6))
                        formats.append("IEC61937_MPEG-2_Layer2/3_LS");
                    if (BIT(bmFormats, 7))
                        formats.append("IEC61937_DTS-I");
                    if (BIT(bmFormats, 8))
                        formats.append("IEC61937_DTS-II");
                    if (BIT(bmFormats, 9))
                        formats.append("IEC61937_DTS-III");
                    if (BIT(bmFormats, 10))
                        formats.append("IEC61937_ATRAC");
                    if (BIT(bmFormats, 11))
                        formats.append("IEC61937_ATRAC2/3");
                    if (BIT(bmFormats, 12))
                        formats.append("TYPE_III_WMA");
                break;
                case 0x04:
                    /* FORMAT_TYPE_IV */
                    if (BIT(bmFormats, 0))
                        formats.append("PCM");
                    if (BIT(bmFormats, 1))
                        formats.append("PCM8");
                    if (BIT(bmFormats, 2))
                        formats.append("IEEE_FLOAT");
                    if (BIT(bmFormats, 3))
                        formats.append("ALAW");
                    if (BIT(bmFormats, 4))
                        formats.append("MULAW");
                    if (BIT(bmFormats, 5))
                        formats.append("MPEG");
                    if (BIT(bmFormats, 6))
                        formats.append("AC-3");
                    if (BIT(bmFormats, 7))
                        formats.append("WMA");
                    if (BIT(bmFormats, 8))
                        formats.append("IEC61937_AC-3");
                    if (BIT(bmFormats, 9))
                        formats.append("IEC61937_MPEG-1_Layer1");
                    if (BIT(bmFormats, 10))
                        formats.append("IEC61937_MPEG-1_Layer2/3 or IEC61937_MPEG-2_NOEXT");
                    if (BIT(bmFormats, 11))
                        formats.append("IEC61937_MPEG-2_EXT");
                    if (BIT(bmFormats, 12))
                        formats.append("IEC61937_MPEG-2_AAC_ADTS");
                    if (BIT(bmFormats, 13))
                        formats.append("IEC61937_MPEG-2_Layer1_LS");
                    if (BIT(bmFormats, 14))
                        formats.append("IEC61937_MPEG-2_Layer2/3_LS");
                    if (BIT(bmFormats, 15))
                        formats.append("IEC61937_DTS-I");
                    if (BIT(bmFormats, 16))
                        formats.append("IEC61937_DTS-II");
                    if (BIT(bmFormats, 17))
                        formats.append("IEC61937_DTS-III");
                    if (BIT(bmFormats, 18))
                        formats.append("IEC61937_ATRAC");
                    if (BIT(bmFormats, 19))
                        formats.append("IEC61937_ATRAC2/3");
                    if (BIT(bmFormats, 20))
                        formats.append("TYPE_III_WMA");
                    if (BIT(bmFormats, 21))
                        formats.append("IEC60958_PCM");
                break;
            }

            return formats;
        }

        UsbAudioStreamInterfaceDescriptor *UsbAudioStreamInterfaceDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24)
                return nullptr;

            UsbAudioStreamInterfaceDescriptor *desc;
            switch (extra[2])
            {
                case 1:
                    /* AS_GENERAL */
                    desc = new UsbClassSpecificASInterfaceDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 2:
                    /* FORMAT_TYPE */
                    desc = UsbFormatTypeDescriptor::get(
                                interfaceDescriptor, descPos);
                break;
                case 3:
                    /* ENCODER */
                    desc = new UsbEncoderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 4:
                    /* DECODER */
                    desc = UsbDecoderDescriptor::get(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
            }

            return desc;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bTerminalLink() const
        {
            return _bTerminalLink;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint32_t UsbClassSpecificASInterfaceDescriptor::bmFormats() const
        {
            return _bmFormats;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint32_t UsbClassSpecificASInterfaceDescriptor::bmChannelConfig() const
        {
            return _bmChannelConfig;
        }

        uint8_t UsbClassSpecificASInterfaceDescriptor::iChannelNames() const
        {
            return _iChannelNames;
        }

        QString UsbClassSpecificASInterfaceDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("Class-Specific AS Interface Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bTerminalLink", _bTerminalLink)
                    .attr("bmControls", _bmControls, __parseBmControls())
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("bmFormats", _bmFormats, parseBmFormats(_bFormatType, _bmFormats).join(UsbHtmlBuilder::NEWLINE))
                    .attr("bNrChannels", _bNrChannels)
                    .attr("bmChannelConfig", _bmChannelConfig, parseBmChannelConfig(_bmChannelConfig).join(UsbHtmlBuilder::NEWLINE))
                    .strdesc("iChannelNames", _iChannelNames, device)
                    .end()
                    .build();
        }

        UsbClassSpecificASInterfaceDescriptor::UsbClassSpecificASInterfaceDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bTerminalLink = *(extra + 3);
            _bmControls = *(extra + 4);
            _bFormatType = *(extra + 5);
            _bmFormats = *reinterpret_cast<const uint32_t *>(extra + 6);
            _bNrChannels = *(extra + 10);
            _bmChannelConfig = *reinterpret_cast<const uint32_t *>(extra + 11);
            _iChannelNames = *(extra + 15);
        }

        QString UsbClassSpecificASInterfaceDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Active Alternate Setting Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Active Alternate Setting Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Valid Alternate Settings Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Valid Alternate Settings Control (Host programmable)"));
                break;
            }

            return controls.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbFormatTypeDescriptor::strBSideBandProtocol(uint8_t bSideBandProtocol)
        {
            return bSideBandProtocol ?
                        "PRES_TIMESTAMP_PROTOCOL" :
                        "PROTOCOL_UNDEFINED";
        }

        UsbFormatTypeDescriptor::UsbFormatTypeDescriptor(UsbInterfaceDescriptor *parent):
            UsbAudioStreamInterfaceDescriptor(parent)
        {

        }

        UsbFormatTypeDescriptor *UsbFormatTypeDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24 || extra[2] != 0x02)
                return nullptr;

            UsbFormatTypeDescriptor *desc;
            switch (extra[3])
            {
                case 0x01:
                    /* FORMAT_TYPE_I */
                    desc = new UsbTypeIFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x02:
                    /* FORMAT_TYPE_II */
                    desc = new UsbTypeIIFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x03:
                    /* FORMAT_TYPE_III */
                    desc = new UsbTypeIIIFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x04:
                    /* FORMAT_TYPE_IV */
                    desc = new UsbTypeIVFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x81:
                    /* EXT_FORMAT_TYPE_I */
                    desc = new UsbExtendedTypeIFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x82:
                    /* EXT_FORMAT_TYPE_II */
                    desc = new UsbExtendedTypeIIFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x83:
                    /* EXT_FORMAT_TYPE_III */
                    desc = new UsbExtendedTypeIIIFormatTypeDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
            }

            return desc;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bSubslotSize() const
        {
            return _bSubslotSize;
        }

        uint8_t UsbTypeIFormatTypeDescriptor::bBitResolution() const
        {
            return _bBitResolution;
        }

        QString UsbTypeIFormatTypeDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Type I Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("bSubslotSize", _bSubslotSize, tr("%1 byte(s)").arg(_bSubslotSize))
                    .attr("bBitResolution", _bBitResolution)
                    .end()
                    .build();
        }

        UsbTypeIFormatTypeDescriptor::UsbTypeIFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
            _bSubslotSize = *(extra + 4);
            _bBitResolution = *(extra + 5);
        }

        uint8_t UsbTypeIIFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbTypeIIFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbTypeIIFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbTypeIIFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint16_t UsbTypeIIFormatTypeDescriptor::wMaxBitRate() const
        {
            return _wMaxBitRate;
        }

        uint16_t UsbTypeIIFormatTypeDescriptor::wSlotsPerFrame() const
        {
            return _wSlotsPerFrame;
        }

        QString UsbTypeIIFormatTypeDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Type II Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("wMaxBitRate", _wMaxBitRate, QString("%1 kb/s").arg(_wMaxBitRate))
                    .attr("wSlotsPerFrame", _wSlotsPerFrame)
                    .end()
                    .build();
        }

        UsbTypeIIFormatTypeDescriptor::UsbTypeIIFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
            _wMaxBitRate = *reinterpret_cast<const uint16_t *>(extra + 4);
            _wSlotsPerFrame = *reinterpret_cast<const uint16_t *>(extra + 6);
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bSubslotSize() const
        {
            return _bSubslotSize;
        }

        uint8_t UsbTypeIIIFormatTypeDescriptor::bBitResolution() const
        {
            return _bBitResolution;
        }

        QString UsbTypeIIIFormatTypeDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Type III Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("bSubslotSize", _bSubslotSize, tr("%1 byte(s)").arg(_bSubslotSize))
                    .attr("bBitResolution", _bBitResolution)
                    .end()
                    .build();
        }

        UsbTypeIIIFormatTypeDescriptor::UsbTypeIIIFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
            _bSubslotSize = *(extra + 4);
            _bBitResolution = *(extra + 5);
        }

        uint8_t UsbTypeIVFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbTypeIVFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbTypeIVFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbTypeIVFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        QString UsbTypeIVFormatTypeDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Type IV Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .end()
                    .build();
        }

        UsbTypeIVFormatTypeDescriptor::UsbTypeIVFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
        }

        uint8_t UsbExtendedTypeIFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbExtendedTypeIFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbExtendedTypeIFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbExtendedTypeIFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint8_t UsbExtendedTypeIFormatTypeDescriptor::bSubslotSize() const
        {
            return _bSubslotSize;
        }

        uint8_t UsbExtendedTypeIFormatTypeDescriptor::bBitResolution() const
        {
            return _bBitResolution;
        }

        uint8_t UsbExtendedTypeIFormatTypeDescriptor::bSideBandProtocol() const
        {
            return _bSideBandProtocol;
        }

        uint8_t UsbExtendedTypeIFormatTypeDescriptor::bControlSize() const
        {
            return _bControlSize;
        }

        uint8_t UsbExtendedTypeIFormatTypeDescriptor::bHeaderLength() const
        {
            return _bHeaderLength;
        }

        QString UsbExtendedTypeIFormatTypeDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Extended Type I Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("bSubslotSize", _bSubslotSize, tr("%1 byte(s)").arg(_bSubslotSize))
                    .attr("bBitResolution", _bBitResolution)
                    .attr("bHeaderLength", _bHeaderLength, tr("%1 byte(s)").arg(_bHeaderLength))
                    .attr("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize))
                    .attr("bSideBandProtocol", _bSideBandProtocol, strBSideBandProtocol(_bSideBandProtocol))
                    .end()
                    .build();
        }

        UsbExtendedTypeIFormatTypeDescriptor::UsbExtendedTypeIFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
            _bSubslotSize = *(extra + 4);
            _bBitResolution = *(extra + 5);
            _bHeaderLength = *(extra + 6);
            _bControlSize = *(extra + 7);
            _bSideBandProtocol = *(extra + 8);
        }

        uint16_t UsbExtendedTypeIIFormatTypeDescriptor::wSamplesPerFrame() const
        {
            return _wSamplesPerFrame;
        }

        uint8_t UsbExtendedTypeIIFormatTypeDescriptor::bHeaderLength() const
        {
            return _bHeaderLength;
        }

        uint8_t UsbExtendedTypeIIFormatTypeDescriptor::bSideBandProtocol() const
        {
            return _bSideBandProtocol;
        }

        uint8_t UsbExtendedTypeIIFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbExtendedTypeIIFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbExtendedTypeIIFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbExtendedTypeIIFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint16_t UsbExtendedTypeIIFormatTypeDescriptor::wMaxBitRate() const
        {
            return _wMaxBitRate;
        }

        QString UsbExtendedTypeIIFormatTypeDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Extended Type II Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("wMaxBitRate", _wMaxBitRate, QString("%1 kb/s").arg(_wMaxBitRate))
                    .attr("wSamplesPerFrame", _wSamplesPerFrame)
                    .attr("bHeaderLength", _bHeaderLength, tr("%1 byte(s)").arg(_bHeaderLength))
                    .attr("bSideBandProtocol", _bSideBandProtocol, strBSideBandProtocol(_bSideBandProtocol))
                    .end()
                    .build();
        }

        UsbExtendedTypeIIFormatTypeDescriptor::UsbExtendedTypeIIFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
            _wMaxBitRate = *reinterpret_cast<const uint16_t *>(extra + 4);
            _wSamplesPerFrame = *reinterpret_cast<const uint16_t *>(extra + 6);
            _bHeaderLength = *(extra + 8);
            _bSideBandProtocol = *(extra + 9);
        }

        uint8_t UsbExtendedTypeIIIFormatTypeDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbExtendedTypeIIIFormatTypeDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbExtendedTypeIIIFormatTypeDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbExtendedTypeIIIFormatTypeDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        uint8_t UsbExtendedTypeIIIFormatTypeDescriptor::bSubslotSize() const
        {
            return _bSubslotSize;
        }

        uint8_t UsbExtendedTypeIIIFormatTypeDescriptor::bBitResolution() const
        {
            return _bBitResolution;
        }

        uint8_t UsbExtendedTypeIIIFormatTypeDescriptor::bSideBandProtocol() const
        {
            return _bSideBandProtocol;
        }

        uint8_t UsbExtendedTypeIIIFormatTypeDescriptor::bHeaderLength() const
        {
            return _bHeaderLength;
        }

        QString UsbExtendedTypeIIIFormatTypeDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Extended Type III Format Type Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatType", _bFormatType, strBFormatType(_bFormatType))
                    .attr("bSubslotSize", _bSubslotSize, tr("%1 byte(s)").arg(_bSubslotSize))
                    .attr("bBitResolution", _bBitResolution)
                    .attr("bHeaderLength", _bHeaderLength, tr("%1 byte(s)").arg(_bHeaderLength))
                    .attr("bSideBandProtocol", _bSideBandProtocol, strBSideBandProtocol(_bSideBandProtocol))
                    .end()
                    .build();
        }

        UsbExtendedTypeIIIFormatTypeDescriptor::UsbExtendedTypeIIIFormatTypeDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbFormatTypeDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatType = *(extra + 3);
            _bSubslotSize = *(extra + 4);
            _bBitResolution = *(extra + 5);
            _bHeaderLength = *(extra + 6);
            _bSideBandProtocol = *(extra + 7);
        }

        uint8_t UsbEncoderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbEncoderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbEncoderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbEncoderDescriptor::bEncoderID() const
        {
            return _bEncoderID;
        }

        uint8_t UsbEncoderDescriptor::bEncoder() const
        {
            return _bEncoder;
        }

        uint32_t UsbEncoderDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbEncoderDescriptor::iParam1() const
        {
            return _iParam1;
        }

        uint8_t UsbEncoderDescriptor::iParam2() const
        {
            return _iParam2;
        }

        uint8_t UsbEncoderDescriptor::iParam3() const
        {
            return _iParam3;
        }

        uint8_t UsbEncoderDescriptor::iParam4() const
        {
            return _iParam4;
        }

        uint8_t UsbEncoderDescriptor::iParam5() const
        {
            return _iParam5;
        }

        uint8_t UsbEncoderDescriptor::iParam6() const
        {
            return _iParam6;
        }

        uint8_t UsbEncoderDescriptor::iParam7() const
        {
            return _iParam7;
        }

        uint8_t UsbEncoderDescriptor::iParam8() const
        {
            return _iParam8;
        }

        uint8_t UsbEncoderDescriptor::iEncoder() const
        {
            return _iEncoder;
        }

        QString UsbEncoderDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("Class-Specific AS Interface Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bEncoderID", _bEncoderID)
                    .attr("bEncoder", _bEncoder, strBEncoder(_bEncoder))
                    .attr("bmControls", _bmControls, __parseBmControls())
                    .strdesc("iParam1", _iParam1, device)
                    .strdesc("iParam2", _iParam2, device)
                    .strdesc("iParam3", _iParam3, device)
                    .strdesc("iParam4", _iParam4, device)
                    .strdesc("iParam5", _iParam5, device)
                    .strdesc("iParam6", _iParam6, device)
                    .strdesc("iParam7", _iParam7, device)
                    .strdesc("iParam8", _iParam8, device)
                    .strdesc("iEncoder", _iEncoder, device)
                    .end()
                    .build();
        }

        QString UsbEncoderDescriptor::strBEncoder(uint8_t bEncoder)
        {
            const static QString encoders[] = {
                "ENCODER_UNDEFINED",
                "OTHER_ENCODER",
                "MPEG_ENCODER",
                "AC-3_ENCODER",
                "WMA_ENCODER",
                "DTS_ENCODER",
            };

            return encoders[bEncoder < (sizeof(encoders) / sizeof(QString)) ? bEncoder : 0];
        }

        UsbEncoderDescriptor::UsbEncoderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bEncoderID = *(extra + 3);
            _bEncoder = *(extra + 4);
            _bmControls = *reinterpret_cast<const uint32_t *>(extra + 8);
            _iParam1 = *(extra + 12);
            _iParam2 = *(extra + 13);
            _iParam3 = *(extra + 14);
            _iParam4 = *(extra + 15);
            _iParam5 = *(extra + 16);
            _iParam6 = *(extra + 17);
            _iParam7 = *(extra + 18);
            _iParam8 = *(extra + 19);
            _iEncoder = *(extra + 20);
        }

        QString UsbEncoderDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Bit Rate Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Bit Rate Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Quality Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Quality Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("VBR Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("VBR Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 6, 7))
            {
                case 0b01:
                    controls.append(tr("Type Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Type Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 8, 9))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 10, 11))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 12, 13))
            {
                case 0b01:
                    controls.append(tr("Encoder Error Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Encoder Error Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 14, 15))
            {
                case 0b01:
                    controls.append(tr("Param1 Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Param1 Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 16, 17))
            {
                case 0b01:
                    controls.append(tr("Param2 Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Param2 Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 18, 19))
            {
                case 0b01:
                    controls.append(tr("Param3 Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Param3 Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 20, 21))
            {
                case 0b01:
                    controls.append(tr("Param4 Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Param4 Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 22, 23))
            {
                case 0b01:
                    controls.append(tr("Param5 Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Param5 Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 24, 25))
            {
                case 0b01:
                    controls.append(tr("Param6 Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Param6 Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 26, 27))
            {
                case 0b01:
                    controls.append(tr("Param7 Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Param7 Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 28, 29))
            {
                case 0b01:
                    controls.append(tr("Param8 Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Param8 Control (Host programmable)"));
                break;
            }

            return controls.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbDecoderDescriptor::strBDecoder(uint8_t bDecoder)
        {
            const static QString decoders[] = {
                "DECODER_UNDEFINED",
                "OTHER_DECODER",
                "MPEG_DECODER",
                "AC-3_DECODER",
                "WMA_DECODER",
                "DTS_DECODER",
            };

            return decoders[bDecoder < (sizeof(decoders) / sizeof(QString)) ? bDecoder : 0];
        }

        UsbDecoderDescriptor::UsbDecoderDescriptor(UsbInterfaceDescriptor *parent):
            UsbAudioStreamInterfaceDescriptor(parent)
        {

        }

        UsbDecoderDescriptor *UsbDecoderDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24 || extra[2] != 0x02)
                return nullptr;

            UsbDecoderDescriptor *desc;
            switch (extra[4])
            {
                case 2:
                    /* MPEG_DECODER */
                    desc = new UsbMpegDecoderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 3:
                    /* AC-3_DECODER */
                    desc = new UsbAc3DecoderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 4:
                    /* WMA_DECODER */
                    desc = new UsbWmaDecoderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 5:
                    /* DTS_DECODER */
                    desc = new UsbDtsDecoderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
            }

            return desc;
        }

        uint8_t UsbMpegDecoderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbMpegDecoderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbMpegDecoderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbMpegDecoderDescriptor::bDecoderID() const
        {
            return _bDecoderID;
        }

        uint8_t UsbMpegDecoderDescriptor::bDecoder() const
        {
            return _bDecoder;
        }

        uint16_t UsbMpegDecoderDescriptor::bmMPEGCapabilities() const
        {
            return _bmMPEGCapabilities;
        }

        uint8_t UsbMpegDecoderDescriptor::bmMPEGFeatures() const
        {
            return _bmMPEGFeatures;
        }

        uint8_t UsbMpegDecoderDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbMpegDecoderDescriptor::iDecoder() const
        {
            return _iDecoder;
        }

        QString UsbMpegDecoderDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("MPEG Decoder Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bDecoderID", _bDecoderID)
                    .attr("bDecoder", _bDecoder, strBDecoder(_bDecoder))
                    .attr("bmMPEGCapabilities", _bmMPEGCapabilities, __parseBmMPEGCapabilities())
                    .attr("bmMPEGFeatures", _bmMPEGFeatures, __parseBmMPEGFeatures())
                    .attr("bmControls", _bmControls, __parseBmControls())
                    .strdesc("iDecoder", _iDecoder, device)
                    .end()
                    .build();
        }

        UsbMpegDecoderDescriptor::UsbMpegDecoderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbDecoderDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bDecoderID = *(extra + 3);
            _bDecoder = *(extra + 4);
            _bmMPEGCapabilities = *reinterpret_cast<const uint16_t *>(extra + 5);
            _bmMPEGFeatures = *(extra + 7);
            _bmControls = *(extra + 9);
            _iDecoder = *(extra + 10);
        }

        QString UsbMpegDecoderDescriptor::__parseBmMPEGCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmMPEGCapabilities, 0))
                supports.append(tr("Supports Layer I"));
            if (BIT(_bmMPEGCapabilities, 1))
                supports.append(tr("Supports Layer II"));
            if (BIT(_bmMPEGCapabilities, 2))
                supports.append(tr("Supports Layer III"));
            if (BIT(_bmMPEGCapabilities, 3))
                supports.append(tr("MPEG-1 only"));
            if (BIT(_bmMPEGCapabilities, 4))
                supports.append(tr("MPEG-1 dual-channel"));
            if (BIT(_bmMPEGCapabilities, 5))
                supports.append(tr("MPEG-2 second stereo"));
            if (BIT(_bmMPEGCapabilities, 6))
                supports.append(tr("MPEG-2 7.1 channel augmentation"));
            if (BIT(_bmMPEGCapabilities, 7))
                supports.append(tr("Adaptive multi-channel prediction"));
            switch (CUT(_bmMPEGCapabilities, 8, 9))
            {
                case 0b01:
                    supports.append(tr("Supports MPEG-2 multilingual at Fs"));
                break;
                case 0b11:
                    supports.append(tr("Supports MPEG-2 multilingual at Fs and ½Fs"));
                break;
            }

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbMpegDecoderDescriptor::__parseBmMPEGFeatures() const
        {
            QStringList supports;

            if (CUT(_bmMPEGFeatures, 4, 5))
                supports.append(tr("Internal Dynamic Range Control:"));
            switch (CUT(_bmMPEGFeatures, 4, 5))
            {
                case 0b01:
                    supports.append(UsbHtmlBuilder::TAB + tr("not scalable"));
                break;
                case 0b10:
                    supports.append(UsbHtmlBuilder::TAB + tr("scalable"));
                    supports.append(UsbHtmlBuilder::TAB + tr("common boost"));
                    supports.append(UsbHtmlBuilder::TAB + tr("cut scaling value"));
                break;
                case 0b11:
                    supports.append(UsbHtmlBuilder::TAB + tr("scalable"));
                    supports.append(UsbHtmlBuilder::TAB + tr("separate boost"));
                    supports.append(UsbHtmlBuilder::TAB + tr("cut scaling value"));
                break;
            }

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbMpegDecoderDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Decoder Error Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Decoder Error Control (Host programmable)"));
                break;
            }

            return controls.join(UsbHtmlBuilder::NEWLINE);
        }

        uint8_t UsbAc3DecoderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbAc3DecoderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbAc3DecoderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbAc3DecoderDescriptor::bDecoderID() const
        {
            return _bDecoderID;
        }

        uint8_t UsbAc3DecoderDescriptor::bDecoder() const
        {
            return _bDecoder;
        }

        uint32_t UsbAc3DecoderDescriptor::bmBSID() const
        {
            return _bmBSID;
        }

        uint8_t UsbAc3DecoderDescriptor::bmAC3Features() const
        {
            return _bmAC3Features;
        }

        uint8_t UsbAc3DecoderDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbAc3DecoderDescriptor::iDecoder() const
        {
            return _iDecoder;
        }

        QString UsbAc3DecoderDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("AC-3 Decoder Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bDecoderID", _bDecoderID)
                    .attr("bDecoder", _bDecoder, strBDecoder(_bDecoder))
                    .attr("bmBSID", _bmBSID, "")
                    .attr("bmAC3Features", _bmAC3Features, __parseBmAC3Features())
                    .attr("bmControls", _bmControls, __parseBmControls())
                    .strdesc("iDecoder", _iDecoder, device)
                    .end()
                    .build();
        }

        UsbAc3DecoderDescriptor::UsbAc3DecoderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbDecoderDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bDecoderID = *(extra + 3);
            _bDecoder = *(extra + 4);
            _bmBSID = *reinterpret_cast<const uint32_t *>(extra + 5);
            _bmAC3Features = *(extra + 9);
            _bmControls = *(extra + 10);
            _iDecoder = *(extra + 11);
        }

        QString UsbAc3DecoderDescriptor::__parseBmAC3Features() const
        {
            QStringList supports;

            if (BIT(_bmAC3Features, 0))
                supports.append(tr("RF mode"));
            if (BIT(_bmAC3Features, 1))
                supports.append(tr("Line mode"));
            if (BIT(_bmAC3Features, 2))
                supports.append(tr("Custom0 mode"));
            if (BIT(_bmAC3Features, 3))
                supports.append(tr("Custom1 mode"));
            if (CUT(_bmAC3Features, 4, 5))
                supports.append(tr("Internal Dynamic Range Control:"));
            switch (CUT(_bmAC3Features, 4, 5))
            {
                case 0b01:
                    supports.append(UsbHtmlBuilder::TAB + tr("not scalable"));
                break;
                case 0b10:
                    supports.append(UsbHtmlBuilder::TAB + tr("scalable"));
                    supports.append(UsbHtmlBuilder::TAB + tr("common boost"));
                    supports.append(UsbHtmlBuilder::TAB + tr("cut scaling value"));
                break;
                case 0b11:
                    supports.append(UsbHtmlBuilder::TAB + tr("scalable"));
                    supports.append(UsbHtmlBuilder::TAB + tr("separate boost"));
                    supports.append(UsbHtmlBuilder::TAB + tr("cut scaling value"));
                break;
            }

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbAc3DecoderDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Decoder Error Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Decoder Error Control (Host programmable)"));
                break;
            }

            return controls.join(UsbHtmlBuilder::NEWLINE);
        }

        uint8_t UsbWmaDecoderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbWmaDecoderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbWmaDecoderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbWmaDecoderDescriptor::bDecoderID() const
        {
            return _bDecoderID;
        }

        uint8_t UsbWmaDecoderDescriptor::bDecoder() const
        {
            return _bDecoder;
        }

        uint16_t UsbWmaDecoderDescriptor::bmWMAProfile() const
        {
            return _bmWMAProfile;
        }

        uint8_t UsbWmaDecoderDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbWmaDecoderDescriptor::iDecoder() const
        {
            return _iDecoder;
        }

        QString UsbWmaDecoderDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("WMA Decoder Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bDecoderID", _bDecoderID)
                    .attr("bDecoder", _bDecoder, strBDecoder(_bDecoder))
                    .attr("bmWMAProfile", _bmWMAProfile, __parseBmWMAProfile())
                    .attr("bmControls", _bmControls, __parseBmControls())
                    .strdesc("iDecoder", _iDecoder, device)
                    .end()
                    .build();
        }

        UsbWmaDecoderDescriptor::UsbWmaDecoderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbDecoderDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bDecoderID = *(extra + 3);
            _bDecoder = *(extra + 4);
            _bmWMAProfile = *reinterpret_cast<const uint16_t *>(extra + 5);
            _bmControls = *(extra + 7);
            _iDecoder = *(extra + 8);
        }

        QString UsbWmaDecoderDescriptor::__parseBmWMAProfile() const
        {
            QStringList supports;
            if (BIT(_bmWMAProfile, 0))
                supports.append(tr("WMA Profile 1, L1"));
            if (BIT(_bmWMAProfile, 1))
                supports.append(tr("WMA Profile 2, L2"));
            if (BIT(_bmWMAProfile, 2))
                supports.append(tr("WMA Profile 3, L3"));
            if (BIT(_bmWMAProfile, 3))
                supports.append(tr("WMA Profile Other, L"));
            if (BIT(_bmWMAProfile, 4))
                supports.append(tr("WMA Speech 1, S1"));
            if (BIT(_bmWMAProfile, 5))
                supports.append(tr("WMA Speech 2, S2"));
            if (BIT(_bmWMAProfile, 6))
                supports.append(tr("WMAPro Profile 1, M1"));
            if (BIT(_bmWMAProfile, 7))
                supports.append(tr("WMAPro Profile 2, M2"));
            if (BIT(_bmWMAProfile, 8))
                supports.append(tr("WMAPro Profile 3, M3"));
            if (BIT(_bmWMAProfile, 9))
                supports.append(tr("WMAPro Profile Other, M"));
            if (BIT(_bmWMAProfile, 10))
                supports.append(tr("WMA lossless decoding"));

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbWmaDecoderDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Decoder Error Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Decoder Error Control (Host programmable)"));
                break;
            }

            return controls.join(UsbHtmlBuilder::NEWLINE);
        }

        uint8_t UsbDtsDecoderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbDtsDecoderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbDtsDecoderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbDtsDecoderDescriptor::bDecoderID() const
        {
            return _bDecoderID;
        }

        uint8_t UsbDtsDecoderDescriptor::bDecoder() const
        {
            return _bDecoder;
        }

        uint8_t UsbDtsDecoderDescriptor::bmCapabilities() const
        {
            return _bmCapabilities;
        }

        uint8_t UsbDtsDecoderDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbDtsDecoderDescriptor::iDecoder() const
        {
            return _iDecoder;
        }

        QString UsbDtsDecoderDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            return UsbHtmlBuilder()
                    .start(tr("DTS Decoder Descriptor"))
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bDecoderID", _bDecoderID)
                    .attr("bDecoder", _bDecoder, strBDecoder(_bDecoder))
                    .attr("bmCapabilities", _bmCapabilities, __parseBmCapabilities())
                    .attr("bmControls", _bmControls, __parseBmControls())
                    .strdesc("iDecoder", _iDecoder, device)
                    .end()
                    .build();
        }

        UsbDtsDecoderDescriptor::UsbDtsDecoderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbDecoderDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bDecoderID = *(extra + 3);
            _bDecoder = *(extra + 4);
            _bmCapabilities = *(extra + 5);
            _bmControls = *(extra + 6);
            _iDecoder = *(extra + 7);
        }

        QString UsbDtsDecoderDescriptor::__parseBmCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmCapabilities, 0))
                supports.append(tr("Core"));
            if (BIT(_bmCapabilities, 1))
                supports.append(tr("Lossless"));
            if (BIT(_bmCapabilities, 2))
                supports.append(tr("LBR"));
            if (BIT(_bmCapabilities, 3))
                supports.append(tr("MultipleStreamMixing"));
            if (BIT(_bmCapabilities, 4))
                supports.append(tr("DualDecode"));

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbDtsDecoderDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Decoder Error Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Decoder Error Control (Host programmable)"));
                break;
            }

            return controls.join(UsbHtmlBuilder::NEWLINE);
        }
    } // namespace uac2
} // namespace usb
