#include "usbaudiocontrolinterfacedescriptor.h"
#include "__usbmacro.h"
#include "usbterminaltypes.h"

namespace usb {
    UsbAudioControlInterfaceDescriptorBasic::UsbAudioControlInterfaceDescriptorBasic(
            UsbInterfaceDescriptor *parent):
        UsbInterfaceExtraDescriptor(parent)
    {

    }

    InterfaceExtraDescriptorType UsbAudioControlInterfaceDescriptorBasic::type() const
    {
        return InterfaceExtraDescriptorType::AC;
    }

    namespace uac1 {
        UsbAudioControlInterfaceDescriptor::UsbAudioControlInterfaceDescriptor(UsbInterfaceDescriptor *parent):
            UsbAudioControlInterfaceDescriptorBasic(parent)
        {

        }

        uint8_t UsbAudioControlInterfaceDescriptor::specVersion() const
        {
            return 1;
        }

        QString UsbAudioControlInterfaceDescriptor::strSubtype(uint8_t subtype)
        {
            static const QString subtypes[] = {
                "AC_DESCRIPTOR_UNDEFINED",
                "HEADER",
                "INPUT_TERMINAL",
                "OUTPUT_TERMINAL",
                "MIXER_UNIT",
                "SELECTOR_UNIT",
                "FEATURE_UNIT",
                "PROCESSING_UNIT",
                "EXTENSION_UNIT",
            };

            return subtypes[subtype < (sizeof(subtypes) / sizeof(QString)) ? subtype : 0];
        }

        QStringList UsbAudioControlInterfaceDescriptor::parseWChannelConfig(uint16_t wChannelConfig)
        {
            QStringList locations;
            if (BIT(wChannelConfig, 0))
                locations.append(tr("Left Front (L)"));
            if (BIT(wChannelConfig, 1))
                locations.append(tr("Right Front (R)"));
            if (BIT(wChannelConfig, 2))
                locations.append(tr("Center Front (C)"));
            if (BIT(wChannelConfig, 3))
                locations.append(tr("Low Frequency Enhancement (LFE)"));
            if (BIT(wChannelConfig, 4))
                locations.append(tr("Left Surround (LS)"));
            if (BIT(wChannelConfig, 5))
                locations.append(tr("Right Surround (RS)"));
            if (BIT(wChannelConfig, 6))
                locations.append(tr("Left of Center (LC)"));
            if (BIT(wChannelConfig, 7))
                locations.append(tr("Right of Center (RC)"));
            if (BIT(wChannelConfig, 8))
                locations.append(tr("Surround (S)"));
            if (BIT(wChannelConfig, 9))
                locations.append(tr("Side Left (SL)"));
            if (BIT(wChannelConfig, 10))
                locations.append(tr("Side Right (SR)"));
            if (BIT(wChannelConfig, 11))
                locations.append(tr("Top (T)"));

            return locations;
        }

        UsbAudioControlInterfaceDescriptor *UsbAudioControlInterfaceDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24)
                return nullptr;

            UsbAudioControlInterfaceDescriptor *desc;
            switch (extra[2])
            {
                case 1:
                    /* HEADER */
                    desc = new UsbClassSpecificACInterfaceHeaderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 2:
                    /* INPUT_TERMINAL */
                    desc = new UsbInputTerminalDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 3:
                    /* OUTPUT_TERMINAL */
                    desc = new UsbOutputTerminalDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 4:
                    /* MIXER_UNIT */
                    desc = new UsbMixerUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 5:
                    /* SELECTOR_UNIT */
                    desc = new UsbSelectorUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 6:
                    /* FEATURE_UNIT */
                    desc = new UsbFeatureUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 7:
                    /* PROCESSING_UNIT */
                    desc = UsbProcessingUnitDescriptor::get(
                                interfaceDescriptor, descPos);
                break;
                case 8:
                    /* EXTENSION_UNIT */
                    desc = new UsbExtensionUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
                break;
            }

            return desc;
        }

        UsbClassSpecificACInterfaceHeaderDescriptor::UsbClassSpecificACInterfaceHeaderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bcdADC = *reinterpret_cast<const uint16_t *>(extra + 3);
            _wTotalLength = *reinterpret_cast<const uint16_t *>(extra + 5);
            _bInCollection = *(extra + 7);
            _baInterfaceNr.reserve(_bInCollection);
            for (int i = 8; i < 8 + _bInCollection; ++i)
                _baInterfaceNr.append(*(extra + i));
        }

        uint8_t UsbClassSpecificACInterfaceHeaderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbClassSpecificACInterfaceHeaderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbClassSpecificACInterfaceHeaderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint16_t UsbClassSpecificACInterfaceHeaderDescriptor::bcdADC() const
        {
            return _bcdADC;
        }

        uint16_t UsbClassSpecificACInterfaceHeaderDescriptor::wTotalLength() const
        {
            return _wTotalLength;
        }

        uint8_t UsbClassSpecificACInterfaceHeaderDescriptor::bInCollection() const
        {
            return _bInCollection;
        }

        uint8_t UsbClassSpecificACInterfaceHeaderDescriptor::baInterfaceNr(uint8_t index) const
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

        QString UsbClassSpecificACInterfaceHeaderDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Class-Specific AC Interface Header Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bcdADC", _bcdADC, "");
            ATTR("wTotalLength", _wTotalLength, _wTotalLength);
            ATTR("bInCollection", _bInCollection, _bInCollection);
            for (int i = 1; i <= _bInCollection; ++i)
                ATTR(QString("baInterfaceNr(%1)").arg(i), baInterfaceNr(i), baInterfaceNr(i));
            END;

            return html;
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

        uint8_t UsbInputTerminalDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint16_t UsbInputTerminalDescriptor::wChannelConfig() const
        {
            return _wChannelConfig;
        }

        uint8_t UsbInputTerminalDescriptor::iChannelNames() const
        {
            return _iChannelNames;
        }

        uint8_t UsbInputTerminalDescriptor::iTerminal() const
        {
            return _iTerminal;
        }

        QString UsbInputTerminalDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();
            QString html;
            START(tr("Input Terminal Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bTerminalID", _bTerminalID, _bTerminalID);
            ATTR("wTerminalType", _wTerminalType, strTerminalType(_wTerminalType));
            ATTR("bAssocTerminal", _bAssocTerminal, _bAssocTerminal);
            ATTR("bNrChannels", _bNrChannels, _bNrChannels);
            ATTR("wChannelConfig", _wChannelConfig, parseWChannelConfig(_wChannelConfig).join(NEWLINE));
            ATTRSTRDESC("iChannelNames", _iChannelNames, device);
            ATTRSTRDESC("iTerminal", _iTerminal, device);
            END;

            return html;
        }

        UsbInputTerminalDescriptor::UsbInputTerminalDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bTerminalID = *(extra + 3);
            _wTerminalType = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bAssocTerminal = *(extra + 6);
            _bNrChannels = *(extra + 7);
            _wChannelConfig = *reinterpret_cast<const uint16_t *>(extra + 8);
            _iChannelNames = *(extra + 10);
            _iTerminal = *(extra + 11);
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
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Output Terminal Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bTerminalID", _bTerminalID, _bTerminalID);
            ATTR("wTerminalType", _wTerminalType, strTerminalType(_wTerminalType));
            ATTR("bAssocTerminal", _bAssocTerminal, _bAssocTerminal);
            ATTR("bSourceID", _bSourceID, _bSourceID);
            ATTRSTRDESC("iTerminal", _iTerminal, device);
            END;

            return html;
        }

        UsbOutputTerminalDescriptor::UsbOutputTerminalDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
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

        UsbMixerUnitDescriptor::UsbMixerUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bNrInPins = *(extra + 4);
            _baSourceID.reserve(_bNrInPins);
            for (int i = 5; i < 5 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _bNrChannels = *(extra + 5 + _bNrInPins);
            _wChannelConfig = *reinterpret_cast<const uint16_t *>(extra + 6 + _bNrInPins);
            _iChannelNames = *(extra + 8 + _bNrInPins);
            const uint8_t N = _bLength - 10 - _bNrInPins;
            _bmControls.reserve(N);
            for (int i = 9 + _bNrInPins; i < 9 + _bNrInPins + N; ++i)
                _bmControls.append(*(extra + i));
            _iMixer = *(extra + 9 + _bNrInPins + N);
        }

        uint8_t UsbMixerUnitDescriptor::iMixer() const
        {
            return _iMixer;
        }

        QString UsbMixerUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Mixer Unit Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("bNrInPins", _bNrInPins, _bNrInPins);
            for (int i = 1; i <= _bNrInPins; ++i)
                ATTR(QString("baSourceID(%1)").arg(i), baSourceID(i), baSourceID(i));
            ATTR("bNrChannels", _bNrChannels, _bNrChannels);
            ATTR("wChannelConfig", _wChannelConfig, parseWChannelConfig(_wChannelConfig).join(NEWLINE));
            ATTRSTRDESC("iChannelNames", _iChannelNames, device);
            ATTRCUSTOM("bmControls", QString("0x") + _bmControls.toHex(), "");
            ATTRSTRDESC("iMixer", _iMixer, device);
            END;

            return html;
        }

        QByteArray UsbMixerUnitDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbMixerUnitDescriptor::iChannelNames() const
        {
            return _iChannelNames;
        }

        uint16_t UsbMixerUnitDescriptor::wChannelConfig() const
        {
            return _wChannelConfig;
        }

        uint8_t UsbMixerUnitDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint8_t UsbMixerUnitDescriptor::bNrInPins() const
        {
            return _bNrInPins;
        }

        uint8_t UsbMixerUnitDescriptor::baSourceID(uint8_t index) const
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

        uint8_t UsbMixerUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        uint8_t UsbMixerUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbMixerUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbMixerUnitDescriptor::bLength() const
        {
            return _bLength;
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

            QString html;
            START(tr("Selector Unit Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("bNrInPins", _bNrInPins, _bNrInPins);
            for (int i = 1; i <= _bNrInPins; ++i)
                ATTR(QString("baSourceID(%1)").arg(i), baSourceID(i), baSourceID(i));
            ATTRSTRDESC("iSelector", _iSelector, device);
            END;

            return html;
        }

        UsbSelectorUnitDescriptor::UsbSelectorUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bNrInPins = *(extra + 4);
            _baSourceID.reserve(_bNrInPins);
            for (int i = 5; i < 5 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _iSelector = *(extra + 5 + _bNrInPins);
        }

        uint8_t UsbFeatureUnitDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbFeatureUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbFeatureUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbFeatureUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        uint8_t UsbFeatureUnitDescriptor::bSourceID() const
        {
            return _bSourceID;
        }

        uint8_t UsbFeatureUnitDescriptor::bControlSize() const
        {
            return _bControlSize;
        }

        QByteArray UsbFeatureUnitDescriptor::bmaControls(uint8_t index) const
        {
            if (index <= _CHANNEL)
                return _bmaControls[index];
            else
            {
                LOGE(tr("Index %1 for bmaControls out of range, must be 0~%2.")
                     .arg(index).arg(_CHANNEL));
                return 0;
            }
        }

        uint8_t UsbFeatureUnitDescriptor::iFeature() const
        {
            return _iFeature;
        }

        QString UsbFeatureUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Feature Unit Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("bSourceID", _bSourceID, _bSourceID);
            ATTR("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize));
            for (int i = 0; i <= _CHANNEL; ++i)
                ATTRCUSTOM(QString("bmaControls(%1)").arg(i),
                           QString("0x") + bmaControls(i).toHex(),
                           __parseBmaControls(i));
            ATTRSTRDESC("iFeature", _iFeature, device);
            END;

            return html;
        }

        UsbFeatureUnitDescriptor::UsbFeatureUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bSourceID = *(extra + 4);
            _bControlSize = *(extra + 5);
            _CHANNEL = (_bLength - 7) / _bControlSize - 1;
            _bmaControls.reserve(_CHANNEL + 1);
            for (int i = 0; i <= _CHANNEL; ++i)
                _bmaControls.append(QByteArray(reinterpret_cast<const char *>(
                                                   extra + 6 + i * _bControlSize), _bControlSize));
            _iFeature = *(extra + 6 + (_CHANNEL + 1) * _bControlSize);
        }

        QString UsbFeatureUnitDescriptor::__parseBmaControls(uint8_t index) const
        {
            QByteArray control = _bmaControls[index];
            QStringList supports;
            if (control.length() >= 1)
            {
                if (BIT(control[0], 0))
                    supports.append(tr("Supports Mute Control"));
                if (BIT(control[0], 1))
                    supports.append(tr("Supports Volume Control"));
                if (BIT(control[0], 2))
                    supports.append(tr("Supports Bass Control"));
                if (BIT(control[0], 3))
                    supports.append(tr("Supports Mid Control"));
                if (BIT(control[0], 4))
                    supports.append(tr("Supports Treble Control"));
                if (BIT(control[0], 5))
                    supports.append(tr("Supports Graphic Equalizer Control"));
                if (BIT(control[0], 6))
                    supports.append(tr("Supports Automatic Gain Control"));
                if (BIT(control[0], 7))
                    supports.append(tr("Supports Delay Control"));
            }
            if (control.length() >= 2)
            {
                if (BIT(control[1], 0))
                    supports.append(tr("Supports Bass Boost Control"));
                if (BIT(control[2], 1))
                    supports.append(tr("Supports Loudness Control"));
            }

            return supports.join(NEWLINE);
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

        uint16_t UsbProcessingUnitDescriptor::wProcessType() const
        {
            return _wProcessType;
        }

        uint8_t UsbProcessingUnitDescriptor::bNrInPins() const
        {
            return _bNrInPins;
        }

        uint8_t UsbProcessingUnitDescriptor::baSourceID(uint8_t index) const
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

        uint8_t UsbProcessingUnitDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint16_t UsbProcessingUnitDescriptor::wChannelConfig() const
        {
            return _wChannelConfig;
        }

        uint8_t UsbProcessingUnitDescriptor::iChannelNames() const
        {
            return _iChannelNames;
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

        QString UsbProcessingUnitDescriptor::strWProcessType(uint16_t wProcessType)
        {
            static const QString processTypes[] = {
                "PROCESS_UNDEFINED",
                "UP/DOWNMIX_PROCESS",
                "DOLBY_PROLOGIC_PROCESS",
                "3D_STEREO_EXTENDER_PROCESS",
                "REVERBERATION_PROCESS",
                "CHORUS_PROCESS",
                "DYN_RANGE_COMP_PROCESS",
            };

            return processTypes[wProcessType < (sizeof(processTypes) / sizeof(QString)) ? wProcessType : 0];
        }

        UsbProcessingUnitDescriptor::UsbProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _wProcessType = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bNrInPins = *(extra + 6);
            _baSourceID.reserve(_bNrInPins);
            for (int i = 7; i < 7 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _bNrChannels = *(extra + 7 + _bNrInPins);
            _wChannelConfig =  *reinterpret_cast<const uint16_t *>(extra + 8 + _bNrInPins);
            _iChannelNames = *(extra + 10 + _bNrInPins);
            _bControlSize = *(extra + 11 + _bNrInPins);
            _bmControls.reserve(_bControlSize);
            for (int i = 12 + _bNrInPins; i < 12 + _bControlSize + _bNrInPins; ++i)
                _bmControls.append(*reinterpret_cast<const char *>(extra + i));
            _iProcessing = *(extra + 12 + _bNrInPins + _bControlSize);
        }

        void UsbProcessingUnitDescriptor::__parseBase(QString &html) const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("wProcessType", _wProcessType, strWProcessType(_wProcessType));
            ATTR("bNrInPins", _bNrInPins, _bNrInPins);
            if (_bNrInPins == 1)
                ATTR("bSourceID", baSourceID(1), baSourceID(1));
            else
                for (int i = 1; i <= _bNrInPins; ++i)
                    ATTR(QString("baSourceID(%1)").arg(i), baSourceID(i), baSourceID(i));
            ATTR("bNrChannels", _bNrChannels, _bNrChannels);
            ATTR("wChannelConfig", _wChannelConfig, parseWChannelConfig(_wChannelConfig).join(NEWLINE));
            ATTRSTRDESC("iChannelNames", _iChannelNames, device);
            ATTR("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize));
            ATTRCUSTOM("bmControls", QString("0x") + _bmControls.toHex(), __parseBmControls());
            ATTRSTRDESC("iProcessing", _iProcessing, device);
        }

        UsbProcessingUnitDescriptor *UsbProcessingUnitDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24 || extra[2] != 0x07)
                return nullptr;

            UsbProcessingUnitDescriptor *desc;
            switch (extra[4])
            {
                case 1:
                    /* UP/DOWNMIX_PROCESS */
                    desc = new UsbUpDownMixProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 2:
                    /* DOLBY_PROLOGIC_PROCESS */
                    desc = new UsbDolbyPrologicProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 3:
                    /* 3D_STEREO_EXTENDER_PROCESS */
                    desc = new Usb3DStereoExtenderProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 4:
                    /* REVERBERATION_PROCESS */
                    desc = new UsbReverberationProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 5:
                    /* CHORUS_PROCESS */
                    desc = new UsbChorusProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 6:
                    /* DYN_RANGE_COMP_PROCESS */
                    desc = new UsbDynamicRangeCompressorProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
                break;
            }

            return desc;
        }

        uint8_t UsbUpDownMixProcessingUnitDescriptor::bNrModes() const
        {
            return _bNrModes;
        }

        uint16_t UsbUpDownMixProcessingUnitDescriptor::waModes(uint8_t index) const
        {
            if (index > 0 && index <= _bNrModes)
                return _waModes[index - 1];
            else
            {
                LOGE(tr("Index %1 for waModes out of range, must be 1~%2.")
                     .arg(index).arg(_bNrModes));
                return 0;
            }
        }

        QString UsbUpDownMixProcessingUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Up/Down-mix Processing Unit Descriptor"));
            __parseBase(html);
            ATTR("bNrModes", _bNrModes, _bNrModes);
            for (int i = 1; i <= _bNrModes; ++i)
                ATTR(QString("waModes(%1)").arg(i), waModes(i), parseWChannelConfig(waModes(i)).join(NEWLINE));
            END;

            return html;
        }

        UsbUpDownMixProcessingUnitDescriptor::UsbUpDownMixProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbProcessingUnitDescriptor(parent, descPos)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bNrModes = *(extra + 14 + _bControlSize);
            _waModes.reserve(_bNrModes);
            for (int i = 0; i < _bNrModes; ++i)
                _waModes.append(*reinterpret_cast<const uint16_t *>(
                                    extra + 15 + _bControlSize + i * 2));
        }

        QString UsbUpDownMixProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList supports;
            if (BIT(_bmControls[0], 0))
                supports.append(tr("Supports Enable Processing Control"));
            if (BIT(_bmControls[0], 1))
                supports.append(tr("Supports Mode Select Control"));
            return supports.join(NEWLINE);
        }

        uint8_t UsbDolbyPrologicProcessingUnitDescriptor::bNrModes() const
        {
            return _bNrModes;
        }

        uint16_t UsbDolbyPrologicProcessingUnitDescriptor::waModes(uint8_t index) const
        {
            if (index > 0 && index <= _bNrModes)
                return _waModes[index - 1];
            else
            {
                LOGE(tr("Index %1 for waModes out of range, must be 1~%2.")
                     .arg(index).arg(_bNrModes));
                return 0;
            }
        }

        QString UsbDolbyPrologicProcessingUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Dolby Prologic Processing Unit Descriptor"));
            __parseBase(html);
            ATTR("bNrModes", _bNrModes, _bNrModes);
            for (int i = 1; i <= _bNrModes; ++i)
                ATTR(QString("waModes(%1)").arg(i), waModes(i), parseWChannelConfig(waModes(i)).join(NEWLINE));
            END;

            return html;
        }

        UsbDolbyPrologicProcessingUnitDescriptor::UsbDolbyPrologicProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbProcessingUnitDescriptor(parent, descPos)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bNrModes = *(extra + 14 + _bControlSize);
            _waModes.reserve(_bNrModes);
            for (int i = 0; i < _bNrModes; ++i)
                _waModes.append(*reinterpret_cast<const uint16_t *>(
                                    extra + 15 + _bControlSize + i * 2));
        }

        QString UsbDolbyPrologicProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList supports;
            if (BIT(_bmControls[0], 0))
                supports.append(tr("Supports Enable Processing Control"));
            if (BIT(_bmControls[0], 1))
                supports.append(tr("Supports Mode Select Control"));
            return supports.join(NEWLINE);
        }

        QString Usb3DStereoExtenderProcessingUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("3D-Stereo Extender Processing Unit Descriptor"));
            __parseBase(html);
            END;

            return html;
        }

        Usb3DStereoExtenderProcessingUnitDescriptor::Usb3DStereoExtenderProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbProcessingUnitDescriptor(parent, descPos)
        {

        }

        QString Usb3DStereoExtenderProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList supports;
            if (BIT(_bmControls[0], 0))
                supports.append(tr("Supports Enable Processing Control"));
            if (BIT(_bmControls[0], 1))
                supports.append(tr("Supports Spaciousness Control"));
            return supports.join(NEWLINE);
        }

        QString UsbReverberationProcessingUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Reverberation Processing Unit Descriptor"));
            __parseBase(html);
            END;

            return html;
        }

        UsbReverberationProcessingUnitDescriptor::UsbReverberationProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbProcessingUnitDescriptor(parent, descPos)
        {

        }

        QString UsbReverberationProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList supports;
            if (BIT(_bmControls[0], 0))
                supports.append(tr("Supports Enable Processing Control"));
            if (BIT(_bmControls[0], 1))
                supports.append(tr("Supports Reverb Type Control"));
            if (BIT(_bmControls[0], 2))
                supports.append(tr("Supports Reverb Level Control"));
            if (BIT(_bmControls[0], 3))
                supports.append(tr("Supports Reverb Time Control"));
            if (BIT(_bmControls[0], 4))
                supports.append(tr("Supports Reverb Delay Feedback Control"));
            return supports.join(NEWLINE);
        }

        QString UsbChorusProcessingUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Chorus Processing Unit Descriptor"));
            __parseBase(html);
            END;

            return html;
        }

        UsbChorusProcessingUnitDescriptor::UsbChorusProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbProcessingUnitDescriptor(parent, descPos)
        {

        }

        QString UsbChorusProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList supports;
            if (BIT(_bmControls[0], 0))
                supports.append(tr("Supports Enable Processing Control"));
            if (BIT(_bmControls[0], 1))
                supports.append(tr("Supports Chorus Level Control"));
            if (BIT(_bmControls[0], 2))
                supports.append(tr("Supports Chorus Modulation Rate Control"));
            if (BIT(_bmControls[0], 3))
                supports.append(tr("Supports Chorus Modulation Depth Control"));
            return supports.join(NEWLINE);
        }

        QString UsbDynamicRangeCompressorProcessingUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Dynamic Range Compressor Processing Unit Descriptor"));
            __parseBase(html);
            END;

            return html;
        }

        UsbDynamicRangeCompressorProcessingUnitDescriptor::UsbDynamicRangeCompressorProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbProcessingUnitDescriptor(parent, descPos)
        {

        }

        QString UsbDynamicRangeCompressorProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList supports;
            if (BIT(_bmControls[0], 0))
                supports.append(tr("Supports Enable Processing Control"));
            if (BIT(_bmControls[0], 1))
                supports.append(tr("Supports Compression Ratio Control"));
            if (BIT(_bmControls[0], 2))
                supports.append(tr("Supports MaxAmpl Control"));
            if (BIT(_bmControls[0], 3))
                supports.append(tr("Supports Threshold Control"));
            if (BIT(_bmControls[0], 4))
                supports.append(tr("Supports Attack time Control"));
            if (BIT(_bmControls[0], 5))
                supports.append(tr("Supports Release time Control"));
            return supports.join(NEWLINE);
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

        uint16_t UsbExtensionUnitDescriptor::wExtensionCode() const
        {
            return _wExtensionCode;
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

        uint8_t UsbExtensionUnitDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint16_t UsbExtensionUnitDescriptor::wChannelConfig() const
        {
            return _wChannelConfig;
        }

        uint8_t UsbExtensionUnitDescriptor::iChannelNames() const
        {
            return _iChannelNames;
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

            QString html;
            START(tr("Extension Unit Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("wExtensionCode", _wExtensionCode, _wExtensionCode);
            ATTR("bNrInPins", _bNrInPins, _bNrInPins);
            for (int i = 1; i <= _bNrInPins; ++i)
                ATTR(QString("baSourceID(%1)").arg(i), baSourceID(i), baSourceID(i));
            ATTR("bNrChannels", _bNrChannels, _bNrChannels);
            ATTR("wChannelConfig", _wChannelConfig, parseWChannelConfig(_wChannelConfig).join(NEWLINE));
            ATTRSTRDESC("iChannelNames", _iChannelNames, device);
            ATTR("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize));
            ATTRCUSTOM("bmControls", QString("0x") + _bmControls.toHex(), "");
            ATTRSTRDESC("iExtension", _iExtension, device);
            END;

            return html;
        }

        UsbExtensionUnitDescriptor::UsbExtensionUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _wExtensionCode = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bNrInPins = *(extra + 6);
            for (int i = 7; i < 7 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _bNrChannels = *(extra + 7 + _bNrInPins);
            _wChannelConfig = *reinterpret_cast<const uint16_t *>(extra + 8 + _bNrInPins);
            _iChannelNames = *(extra + 10 + _bNrInPins);
            _bControlSize = *(extra + 11 + _bNrInPins);
            _bmControls.reserve(_bControlSize);
            for (int i = 12 + _bNrInPins; i < 12 + _bControlSize + _bNrInPins; ++i)
                _bmControls.append(*reinterpret_cast<const char *>(extra + i));
            _iExtension = *(extra + 12 + _bNrInPins + _bControlSize);
        }
    } // namespace uac1

    namespace uac2 {
        UsbAudioControlInterfaceDescriptor::UsbAudioControlInterfaceDescriptor(
                UsbInterfaceDescriptor *parent):
            UsbAudioControlInterfaceDescriptorBasic(parent)
        {

        }

        uint8_t UsbAudioControlInterfaceDescriptor::specVersion() const
        {
            return 2;
        }

        QString UsbAudioControlInterfaceDescriptor::strSubtype(uint8_t subtype)
        {
            static const QString subtypes[] = {
                "AC_DESCRIPTOR_UNDEFINED",
                "HEADER",
                "INPUT_TERMINAL",
                "OUTPUT_TERMINAL",
                "MIXER_UNIT",
                "SELECTOR_UNIT",
                "FEATURE_UNIT",
                "EFFECT_UNIT",
                "PROCESSING_UNIT",
                "EXTENSION_UNIT",
                "CLOCK_SOURCE",
                "CLOCK_SELECTOR",
                "CLOCK_MULTIPLIER",
                "SAMPLE_RATE_CONVERTER",
            };

            return subtypes[subtype < (sizeof(subtypes) / sizeof(QString)) ? subtype : 0];
        }

        QStringList UsbAudioControlInterfaceDescriptor::parseBmChannelConfig(uint32_t bmChannelConfig)
        {
            QStringList locations;
            if (BIT(bmChannelConfig, 0))
                locations.append(tr("Front Left (FL)"));
            if (BIT(bmChannelConfig, 1))
                locations.append(tr("Front Right (FR)"));
            if (BIT(bmChannelConfig, 2))
                locations.append(tr("Front Center (FC)"));
            if (BIT(bmChannelConfig, 3))
                locations.append(tr("Low Frequency Enhancement (LFE)"));
            if (BIT(bmChannelConfig, 4))
                locations.append(tr("Back Left (BL)"));
            if (BIT(bmChannelConfig, 5))
                locations.append(tr("Back Right (BR)"));
            if (BIT(bmChannelConfig, 6))
                locations.append(tr("Front Left of Center (FLC)"));
            if (BIT(bmChannelConfig, 7))
                locations.append(tr("Front Right of Center (FRC)"));
            if (BIT(bmChannelConfig, 8))
                locations.append(tr("Back Center (BC)"));
            if (BIT(bmChannelConfig, 9))
                locations.append(tr("Side Left (SL)"));
            if (BIT(bmChannelConfig, 10))
                locations.append(tr("Side Right (SR)"));
            if (BIT(bmChannelConfig, 11))
                locations.append(tr("Top Center (TC)"));
            if (BIT(bmChannelConfig, 12))
                locations.append(tr("Top Front Left (TFL)"));
            if (BIT(bmChannelConfig, 13))
                locations.append(tr("Top Front Center(TFC)"));
            if (BIT(bmChannelConfig, 14))
                locations.append(tr("Top Front Right (TFR)"));
            if (BIT(bmChannelConfig, 15))
                locations.append(tr("Top Back Left (TBL)"));
            if (BIT(bmChannelConfig, 16))
                locations.append(tr("Top Back Center (TBC)"));
            if (BIT(bmChannelConfig, 17))
                locations.append(tr("Top Back Right (TBR)"));
            if (BIT(bmChannelConfig, 18))
                locations.append(tr("Top Front Left of Center (TFLC)"));
            if (BIT(bmChannelConfig, 19))
                locations.append(tr("Top Front Right of Center (TFRC)"));
            if (BIT(bmChannelConfig, 20))
                locations.append(tr("Left Low Frequency Effects (LLFE)"));
            if (BIT(bmChannelConfig, 21))
                locations.append(tr("Right Low Frequency Effects (RLFE)"));
            if (BIT(bmChannelConfig, 22))
                locations.append(tr("Top Side Left (TSL)"));
            if (BIT(bmChannelConfig, 23))
                locations.append(tr("Top Side Right (TSR)"));
            if (BIT(bmChannelConfig, 24))
                locations.append(tr("Bottom Center (BC)"));
            if (BIT(bmChannelConfig, 25))
                locations.append(tr("Back Left of Center (BLC)"));
            if (BIT(bmChannelConfig, 26))
                locations.append(tr("Back Right of Center (BRC)"));
            if (BIT(bmChannelConfig, 31))
                locations.append(tr("Raw Data (RD)"));

            return locations;
        }

        UsbAudioControlInterfaceDescriptor *UsbAudioControlInterfaceDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24)
                return nullptr;

            UsbAudioControlInterfaceDescriptor *desc;
            switch (extra[2])
            {
                case 0x1:
                    /* HEADER */
                    desc = new UsbClassSpecificACInterfaceHeaderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x2:
                    /* INPUT_TERMINAL */
                    desc = new UsbInputTerminalDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x3:
                    /* OUTPUT_TERMINAL */
                    desc = new UsbOutputTerminalDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x4:
                    /* MIXER_UNIT */
                    desc = new UsbMixerUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x5:
                    /* SELECTOR_UNIT */
                    desc = new UsbSelectorUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x6:
                    /* FEATURE_UNIT */
                    desc = new UsbFeatureUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x7:
                    /* EFFECT_UNIT */
                    desc = UsbEffectUnitDescriptor::get(
                                interfaceDescriptor, descPos);
                break;
                case 0x8:
                    /* PROCESSING_UNIT */
                    desc = UsbProcessingUnitDescriptor::get(
                                interfaceDescriptor, descPos);
                break;
                case 0x9:
                    /* EXTENSION_UNIT */
                    desc = new UsbExtensionUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0xA:
                    /* CLOCK_SOURCE */
                    desc = new UsbClockSourceDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0xB:
                    /* CLOCK_SELECTOR */
                    desc = new UsbClockSelectorDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0xC:
                    /* CLOCK_MULTIPLIER */
                    desc = new UsbClockMultiplierDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0xD:
                    /* SAMPLE_RATE_CONVERTER */
                    desc = new UsbSamplingRateConverterUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
                break;
            }

            return desc;
        }

        uint8_t UsbClassSpecificACInterfaceHeaderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbClassSpecificACInterfaceHeaderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbClassSpecificACInterfaceHeaderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint16_t UsbClassSpecificACInterfaceHeaderDescriptor::bcdADC() const
        {
            return _bcdADC;
        }

        uint8_t UsbClassSpecificACInterfaceHeaderDescriptor::bCategory() const
        {
            return _bCategory;
        }

        uint16_t UsbClassSpecificACInterfaceHeaderDescriptor::wTotalLength() const
        {
            return _wTotalLength;
        }

        uint16_t UsbClassSpecificACInterfaceHeaderDescriptor::bmControls() const
        {
            return _bmControls;
        }

        QString UsbClassSpecificACInterfaceHeaderDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Class-Specific AC Interface Header Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bcdADC", _bcdADC, "");
            ATTR("bCategory", _bCategory, __strBCategory());
            ATTR("wTotalLength", _wTotalLength, _wTotalLength);
            ATTR("bmControls", _bmControls, __parseBmControls());
            END;

            return html;
        }

        UsbClassSpecificACInterfaceHeaderDescriptor::UsbClassSpecificACInterfaceHeaderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bcdADC = *reinterpret_cast<const uint16_t *>(extra + 3);
            _bCategory = *(extra + 5);
            _wTotalLength = *reinterpret_cast<const uint16_t *>(extra + 6);
            _bmControls = *(extra + 8);
        }

        QString UsbClassSpecificACInterfaceHeaderDescriptor::__strBCategory() const
        {
            static const QString categories[] = {
                "FUNCTION_SUBCLASS_UNDEFINED",
                "DESKTOP_SPEAKER",
                "HOME_THEATER",
                "MICROPHONE",
                "HEADSET",
                "TELEPHONE",
                "CONVERTER",
                "VOICE/SOUND_RECORDER",
                "I/O_BOX",
                "MUSICAL_INSTRUMENT",
                "PRO-AUDIO",
                "AUDIO/VIDEO",
                "CONTROL_PANEL",
            };

            if (_bCategory == 0xFF)
                return QString("OTHER");

            return categories[_bCategory < (sizeof(categories) / sizeof(QString)) ? _bCategory : 0];
        }

        QString UsbClassSpecificACInterfaceHeaderDescriptor::__parseBmControls() const
        {
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                return tr("Latency Control (Read-only)");
                case 0b11:
                return tr("Latency Control (Host programmable)");
                default:
                return QString();
            }
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

        uint8_t UsbInputTerminalDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint32_t UsbInputTerminalDescriptor::bmChannelConfig() const
        {
            return _bmChannelConfig;
        }

        uint8_t UsbInputTerminalDescriptor::iChannelNames() const
        {
            return _iChannelNames;
        }

        uint8_t UsbInputTerminalDescriptor::iTerminal() const
        {
            return _iTerminal;
        }

        QString UsbInputTerminalDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();
            QString html;
            START(tr("Input Terminal Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bTerminalID", _bTerminalID, _bTerminalID);
            ATTR("wTerminalType", _wTerminalType, strTerminalType(_wTerminalType));
            ATTR("bAssocTerminal", _bAssocTerminal, _bAssocTerminal);
            ATTR("bCSourceID", _bCSourceID, _bCSourceID);
            ATTR("bNrChannels", _bNrChannels, _bNrChannels);
            ATTR("bmChannelConfig", _bmChannelConfig, parseBmChannelConfig(_bmChannelConfig).join(NEWLINE));
            ATTRSTRDESC("iChannelNames", _iChannelNames, device);
            ATTR("bmControls", _bmControls, __parseBmControls());
            ATTRSTRDESC("iTerminal", _iTerminal, device);
            END;

            return html;
        }

        UsbInputTerminalDescriptor::UsbInputTerminalDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bTerminalID = *(extra + 3);
            _wTerminalType = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bAssocTerminal = *(extra + 6);
            _bCSourceID = *(extra + 7);
            _bNrChannels = *(extra + 8);
            _bmChannelConfig = *reinterpret_cast<const uint16_t *>(extra + 9);
            _iChannelNames = *(extra + 13);
            _bmControls = *reinterpret_cast<const uint16_t *>(extra + 14);
            _iTerminal = *(extra + 16);
        }

        QString UsbInputTerminalDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Copy Protect Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Copy Protect Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Connector Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Connector Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Overload Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overload Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 6, 7))
            {
                case 0b01:
                    controls.append(tr("Cluster Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Cluster Control (Host programmable)"));
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

            return controls.join(NEWLINE);
        }

        uint16_t UsbInputTerminalDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbInputTerminalDescriptor::bCSourceID() const
        {
            return _bCSourceID;
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
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Output Terminal Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bTerminalID", _bTerminalID, _bTerminalID);
            ATTR("wTerminalType", _wTerminalType, strTerminalType(_wTerminalType));
            ATTR("bAssocTerminal", _bAssocTerminal, _bAssocTerminal);
            ATTR("bSourceID", _bSourceID, _bSourceID);
            ATTR("bCSourceID", _bCSourceID, _bCSourceID);
            ATTR("bmControls", _bmControls, __parseBmControls());
            ATTRSTRDESC("iTerminal", _iTerminal, device);
            END;

            return html;
        }

        UsbOutputTerminalDescriptor::UsbOutputTerminalDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bTerminalID = *(extra + 3);
            _wTerminalType = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bAssocTerminal = *(extra + 6);
            _bSourceID = *(extra + 7);
            _bCSourceID = *(extra + 8);
            _bmControls = *reinterpret_cast<const uint16_t *>(extra + 9);
            _iTerminal = *(extra + 11);
        }

        QString UsbOutputTerminalDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Copy Protect Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Copy Protect Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Connector Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Connector Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Overload Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overload Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 6, 7))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 8, 9))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        uint16_t UsbOutputTerminalDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbOutputTerminalDescriptor::bCSourceID() const
        {
            return _bCSourceID;
        }

        UsbMixerUnitDescriptor::UsbMixerUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bNrInPins = *(extra + 4);
            _baSourceID.reserve(_bNrInPins);
            for (int i = 5; i < 5 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _bNrChannels = *(extra + 5 + _bNrInPins);
            _bmChannelConfig = *reinterpret_cast<const uint32_t *>(extra + 6 + _bNrInPins);
            _iChannelNames = *(extra + 10 + _bNrInPins);
            const uint8_t N = _bLength - 13 - _bNrInPins;
            _bmMixerControls.reserve(N);
            for (int i = 11 + _bNrInPins; i < 11 + _bNrInPins + N; ++i)
                _bmMixerControls.append(*(extra + i));
            _bmControls = *(extra + 11 + _bNrInPins + N);
            _iMixer = *(extra + 12 + _bNrInPins + N);
        }

        QString UsbMixerUnitDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Cluster Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Cluster Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        QByteArray UsbMixerUnitDescriptor::bmMixerControls() const
        {
            return _bmMixerControls;
        }

        uint8_t UsbMixerUnitDescriptor::iMixer() const
        {
            return _iMixer;
        }

        QString UsbMixerUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Mixer Unit Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("bNrInPins", _bNrInPins, _bNrInPins);
            for (int i = 1; i <= _bNrInPins; ++i)
                ATTR(QString("baSourceID(%1)").arg(i), baSourceID(i), baSourceID(i));
            ATTR("bNrChannels", _bNrChannels, _bNrChannels);
            ATTR("bmChannelConfig", _bmChannelConfig, parseBmChannelConfig(_bmChannelConfig).join(NEWLINE));
            ATTRSTRDESC("iChannelNames", _iChannelNames, device);
            ATTRCUSTOM("bmMixerControls", QString("0x") + _bmMixerControls.toHex(), "");
            ATTR("bmControls", _bmControls, __parseBmControls());
            ATTRSTRDESC("iMixer", _iMixer, device);
            END;

            return html;
        }

        uint8_t UsbMixerUnitDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbMixerUnitDescriptor::iChannelNames() const
        {
            return _iChannelNames;
        }

        uint32_t UsbMixerUnitDescriptor::bmChannelConfig() const
        {
            return _bmChannelConfig;
        }

        uint8_t UsbMixerUnitDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint8_t UsbMixerUnitDescriptor::bNrInPins() const
        {
            return _bNrInPins;
        }

        uint8_t UsbMixerUnitDescriptor::baSourceID(uint8_t index) const
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

        uint8_t UsbMixerUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        uint8_t UsbMixerUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbMixerUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbMixerUnitDescriptor::bLength() const
        {
            return _bLength;
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

            QString html;
            START(tr("Selector Unit Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("bNrInPins", _bNrInPins, _bNrInPins);
            for (int i = 1; i <= _bNrInPins; ++i)
                ATTR(QString("baSourceID(%1)").arg(i), baSourceID(i), baSourceID(i));
            ATTR("bmControls", _bmControls, __parseBmControls());
            ATTRSTRDESC("iSelector", _iSelector, device);
            END;

            return html;
        }

        UsbSelectorUnitDescriptor::UsbSelectorUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bNrInPins = *(extra + 4);
            _baSourceID.reserve(_bNrInPins);
            for (int i = 5; i < 5 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _bmControls  = *(extra + 5 + _bNrInPins);
            _iSelector = *(extra + 6 + _bNrInPins);
        }

        QString UsbSelectorUnitDescriptor::__parseBmControls() const
        {
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                return tr("Selector Control (Read-only)");
                case 0b11:
                return tr("Selector Control (Host programmable)");
                default:
                return QString();
            }
        }

        uint8_t UsbSelectorUnitDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbFeatureUnitDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbFeatureUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbFeatureUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbFeatureUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        uint8_t UsbFeatureUnitDescriptor::bSourceID() const
        {
            return _bSourceID;
        }

        uint32_t UsbFeatureUnitDescriptor::bmaControls(uint8_t index) const
        {
            if (index <= _CHANNEL)
                return _bmaControls[index];
            else
            {
                LOGE(tr("Index %1 for bmaControls out of range, must be 0~%2.")
                     .arg(index).arg(_CHANNEL));
                return 0;
            }
        }

        uint8_t UsbFeatureUnitDescriptor::iFeature() const
        {
            return _iFeature;
        }

        QString UsbFeatureUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Feature Unit Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("bSourceID", _bSourceID, _bSourceID);
            for (int i = 0; i <= _CHANNEL; ++i)
                ATTR(QString("bmaControls(%1)").arg(i), bmaControls(i), __parseBmaControls(i));
            ATTRSTRDESC("iFeature", _iFeature, device);
            END;

            return html;
        }

        UsbFeatureUnitDescriptor::UsbFeatureUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bSourceID = *(extra + 4);
            _CHANNEL = (_bLength - 6) / 4 - 1;
            _bmaControls.reserve(_CHANNEL + 1);
            for (int i = 0; i <= _CHANNEL; ++i)
                _bmaControls.append(*reinterpret_cast<const uint32_t *>(
                                        extra + 5 + i * 4));
            _iFeature = *(extra + 5 + (_CHANNEL + 1) * 4);
        }

        QString UsbFeatureUnitDescriptor::__parseBmaControls(uint8_t index) const
        {
            uint32_t bmControls = _bmaControls[index];
            QStringList controls;
            switch (CUT(bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Mute Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Mute Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Volume Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Volume Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Bass Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Bass Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 6, 7))
            {
                case 0b01:
                    controls.append(tr("Mid Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Mid Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 8, 9))
            {
                case 0b01:
                    controls.append(tr("Treble Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Treble Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 10, 11))
            {
                case 0b01:
                    controls.append(tr("Graphic Equalizer Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Graphic Equalizer Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 12, 13))
            {
                case 0b01:
                    controls.append(tr("Automatic Gain Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Automatic Gain Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 14, 15))
            {
                case 0b01:
                    controls.append(tr("Delay Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Delay Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 16, 17))
            {
                case 0b01:
                    controls.append(tr("Bass Boost Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Bass Boost Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 18, 19))
            {
                case 0b01:
                    controls.append(tr("Loudness Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Loudness Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 20, 21))
            {
                case 0b01:
                    controls.append(tr("Input Gain Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Input Gain Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 22, 23))
            {
                case 0b01:
                    controls.append(tr("Input Gain Pad Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Input Gain Pad Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 24, 25))
            {
                case 0b01:
                    controls.append(tr("Phase Inverter Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Phase Inverter Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 26, 27))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(bmControls, 28, 29))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        uint8_t UsbEffectUnitDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbEffectUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbEffectUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbEffectUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        uint16_t UsbEffectUnitDescriptor::wEffectType() const
        {
            return _wEffectType;
        }

        uint8_t UsbEffectUnitDescriptor::bSourceID() const
        {
            return _bSourceID;
        }

        uint32_t UsbEffectUnitDescriptor::bmaControls(uint8_t index) const
        {
            if (index <= _CHANNEL)
                return _bmaControls[index];
            else
            {
                LOGE(tr("Index %1 for bmaControls out of range, must be 0~%2.")
                     .arg(index).arg(_CHANNEL));
                return 0;
            }
        }

        uint8_t UsbEffectUnitDescriptor::iEffects() const
        {
            return _iEffects;
        }

        QString UsbEffectUnitDescriptor::strWEffectType(uint16_t wEffectType)
        {
            static const QString effectTypes[] = {
                "EFFECT_UNDEFINED",
                "PARAM_EQ_SECTION_EFFECT",
                "REVERBERATION_EFFECT",
                "MOD_DELAY_EFFECT",
                "DYN_RANGE_COMP_EFFECT",
            };

            return effectTypes[wEffectType < (sizeof(effectTypes) / sizeof(QString)) ? wEffectType : 0];
        }

        UsbEffectUnitDescriptor::UsbEffectUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bSourceID = *(extra + 4);
            _CHANNEL = (_bLength - 16) / 4;
            _bmaControls.reserve(_CHANNEL + 1);
            for (int i = 0; i <= _CHANNEL; ++i)
                _bmaControls.append(*reinterpret_cast<const uint32_t *>(
                                        extra + 7 + i * 4));
            _iEffects = *(extra + 11 + (_CHANNEL + 1) * 4);
        }

        void UsbEffectUnitDescriptor::__parseBase(QString &html) const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("bSourceID", _bSourceID, _bSourceID);
            for (int i = 0; i <= _CHANNEL; ++i)
                ATTR(QString("bmaControls(%1)").arg(i), bmaControls(i), __parseBmaControls(i));
            ATTRSTRDESC("iEffects", _iEffects, device);
        }

        UsbEffectUnitDescriptor *UsbEffectUnitDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24 || extra[2] != 0x07)
                return nullptr;

            UsbEffectUnitDescriptor *desc;
            switch (extra[4])
            {
                case 1:
                    /* PARAM_EQ_SECTION_EFFECT */
                    desc = new UsbParametricEqualizerSectionEffectUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 2:
                    /* REVERBERATION_EFFECT */
                    desc = new UsbReverberationEffectUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 3:
                    /* MOD_DELAY_EFFECT */
                    desc = new UsbModulationDelayEffectUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 4:
                    /* DYN_RANGE_COMP_EFFECT */
                    desc = new UsbDynamicRangeCompressorEffectUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
                break;
            }

            return desc;
        }

        QString UsbParametricEqualizerSectionEffectUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Parametric Equalizer Section Effect Unit Descriptor"));
            __parseBase(html);
            END;

            return html;
        }

        UsbParametricEqualizerSectionEffectUnitDescriptor::UsbParametricEqualizerSectionEffectUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbEffectUnitDescriptor(parent, descPos)
        {

        }

        QString UsbParametricEqualizerSectionEffectUnitDescriptor::__parseBmaControls(uint8_t index) const
        {
            QStringList controls;
            switch (CUT(bmaControls(index), 0, 1))
            {
                case 0b01:
                    controls.append(tr("Enable Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Enable Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 2, 3))
            {
                case 0b01:
                    controls.append(tr("Center Frequency Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Center Frequency Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 4, 5))
            {
                case 0b01:
                    controls.append(tr("Q Factor Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Q Factor Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 6, 7))
            {
                case 0b01:
                    controls.append(tr("Gain Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Gain Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 8, 9))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 10, 11))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        QString UsbReverberationEffectUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Reverberation Effect Unit Descriptor"));
            __parseBase(html);
            END;

            return html;
        }

        UsbReverberationEffectUnitDescriptor::UsbReverberationEffectUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbEffectUnitDescriptor(parent, descPos)
        {

        }

        QString UsbReverberationEffectUnitDescriptor::__parseBmaControls(uint8_t index) const
        {
            QStringList controls;
            switch (CUT(bmaControls(index), 0, 1))
            {
                case 0b01:
                    controls.append(tr("Enable Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Enable Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 2, 3))
            {
                case 0b01:
                    controls.append(tr("Type Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Type Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 4, 5))
            {
                case 0b01:
                    controls.append(tr("Level Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Level Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 6, 7))
            {
                case 0b01:
                    controls.append(tr("Time Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Time Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 8, 9))
            {
                case 0b01:
                    controls.append(tr("Delay Feedback Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Delay Feedback Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 10, 11))
            {
                case 0b01:
                    controls.append(tr("Pre-Delay Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Pre-Delay Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 12, 13))
            {
                case 0b01:
                    controls.append(tr("Density Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Density Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 14, 15))
            {
                case 0b01:
                    controls.append(tr("Hi-Freq Roll-Off Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Hi-Freq Roll-Off Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 16, 17))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 18, 19))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        QString UsbModulationDelayEffectUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Modulation Delay Effect Unit Descriptor"));
            __parseBase(html);
            END;

            return html;
        }

        UsbModulationDelayEffectUnitDescriptor::UsbModulationDelayEffectUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbEffectUnitDescriptor(parent, descPos)
        {

        }

        QString UsbModulationDelayEffectUnitDescriptor::__parseBmaControls(uint8_t index) const
        {
            QStringList controls;
            switch (CUT(bmaControls(index), 0, 1))
            {
                case 0b01:
                    controls.append(tr("Enable Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Enable Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 2, 3))
            {
                case 0b01:
                    controls.append(tr("Balance Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Balance Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 4, 5))
            {
                case 0b01:
                    controls.append(tr("Rate Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Rate Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 6, 7))
            {
                case 0b01:
                    controls.append(tr("Depth Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Depth Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 8, 9))
            {
                case 0b01:
                    controls.append(tr("Time Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Time Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 10, 11))
            {
                case 0b01:
                    controls.append(tr("Feedback Level Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Feedback Level Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 12, 13))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 14, 15))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        QString UsbDynamicRangeCompressorEffectUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Dynamic Range Compressor Effect Unit Descriptor"));
            __parseBase(html);
            END;

            return html;
        }

        UsbDynamicRangeCompressorEffectUnitDescriptor::UsbDynamicRangeCompressorEffectUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbEffectUnitDescriptor(parent, descPos)
        {

        }

        QString UsbDynamicRangeCompressorEffectUnitDescriptor::__parseBmaControls(uint8_t index) const
        {
            QStringList controls;
            switch (CUT(bmaControls(index), 0, 1))
            {
                case 0b01:
                    controls.append(tr("Enable Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Enable Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 2, 3))
            {
                case 0b01:
                    controls.append(tr("Compression Ratio Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Compression Ratio Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 4, 5))
            {
                case 0b01:
                    controls.append(tr("MaxAmpl Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("MaxAmpl Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 6, 7))
            {
                case 0b01:
                    controls.append(tr("Threshold Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Threshold Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 8, 9))
            {
                case 0b01:
                    controls.append(tr("Attack Time Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Attack Time Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 10, 11))
            {
                case 0b01:
                    controls.append(tr("Release Time Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Release Time Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 12, 13))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(bmaControls(index), 14, 15))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
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

        uint16_t UsbProcessingUnitDescriptor::wProcessType() const
        {
            return _wProcessType;
        }

        uint8_t UsbProcessingUnitDescriptor::bNrInPins() const
        {
            return _bNrInPins;
        }

        uint8_t UsbProcessingUnitDescriptor::baSourceID(uint8_t index) const
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

        uint8_t UsbProcessingUnitDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint16_t UsbProcessingUnitDescriptor::bmChannelConfig() const
        {
            return _bmChannelConfig;
        }

        uint8_t UsbProcessingUnitDescriptor::iChannelNames() const
        {
            return _iChannelNames;
        }

        uint16_t UsbProcessingUnitDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbProcessingUnitDescriptor::iProcessing() const
        {
            return _iProcessing;
        }

        QString UsbProcessingUnitDescriptor::strWProcessType(uint16_t wProcessType)
        {
            static const QString processTypes[] = {
                "PROCESS_UNDEFINED",
                "UP/DOWNMIX_PROCESS",
                "DOLBY_PROLOGIC_PROCESS",
                "STEREO_EXTENDER_PROCESS",
            };

            return processTypes[wProcessType < (sizeof(processTypes) / sizeof(QString)) ? wProcessType : 0];
        }

        UsbProcessingUnitDescriptor::UsbProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _wProcessType = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bNrInPins = *(extra + 6);
            _baSourceID.reserve(_bNrInPins);
            for (int i = 7; i < 7 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _bNrChannels = *(extra + 7 + _bNrInPins);
            _bmChannelConfig =  *reinterpret_cast<const uint32_t *>(extra + 8 + _bNrInPins);
            _iChannelNames = *(extra + 12 + _bNrInPins);
            _bmControls = *reinterpret_cast<const uint16_t *>(extra + 13 + _bNrInPins);
            _iProcessing = *(extra + 15 + _bNrInPins);
        }

        void UsbProcessingUnitDescriptor::__parseBase(QString &html) const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("wProcessType", _wProcessType, strWProcessType(_wProcessType));
            ATTR("bNrInPins", _bNrInPins, _bNrInPins);
            if (_bNrInPins == 1)
                ATTR("bSourceID", baSourceID(1), baSourceID(1));
            else
                for (int i = 1; i <= _bNrInPins; ++i)
                    ATTR(QString("baSourceID(%1)").arg(i), baSourceID(i), baSourceID(i));
            ATTR("bNrChannels", _bNrChannels, _bNrChannels);
            ATTR("bmChannelConfig", _bmChannelConfig, parseBmChannelConfig(_bmChannelConfig).join(NEWLINE));
            ATTRSTRDESC("iChannelNames", _iChannelNames, device);
            ATTR("bmControls", _bmControls, __parseBmControls());
            ATTRSTRDESC("iProcessing", _iProcessing, device);
        }

        UsbProcessingUnitDescriptor *UsbProcessingUnitDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24 || extra[2] != 0x07)
                return nullptr;

            UsbProcessingUnitDescriptor *desc;
            switch (extra[4])
            {
                case 1:
                    /* UP/DOWNMIX_PROCESS */
                    desc = new UsbUpDownMixProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 2:
                    /* DOLBY_PROLOGIC_PROCESS */
                    desc = new UsbDolbyPrologicProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 3:
                    /* STEREO_EXTENDER_PROCESS */
                    desc = new StereoExtenderProcessingUnitDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
                break;
            }

            return desc;
        }

        uint8_t UsbUpDownMixProcessingUnitDescriptor::bNrModes() const
        {
            return _bNrModes;
        }

        uint32_t UsbUpDownMixProcessingUnitDescriptor::daModes(uint8_t index) const
        {
            if (index > 0 && index <= _bNrModes)
                return _daModes[index - 1];
            else
            {
                LOGE(tr("Index %1 for daModes out of range, must be 1~%2.")
                     .arg(index).arg(_bNrModes));
                return 0;
            }
        }

        QString UsbUpDownMixProcessingUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Up/Down-mix Processing Unit Descriptor"));
            __parseBase(html);
            ATTR("bNrModes", _bNrModes, _bNrModes);
            for (int i = 1; i <= _bNrModes; ++i)
                ATTR(QString("daModes(%1)").arg(i), daModes(i), parseBmChannelConfig(daModes(i)).join(NEWLINE));
            END;

            return html;
        }

        UsbUpDownMixProcessingUnitDescriptor::UsbUpDownMixProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbProcessingUnitDescriptor(parent, descPos)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bNrModes = *(extra + 17);
            _daModes.reserve(_bNrModes);
            for (int i = 0; i < _bNrModes; ++i)
                _daModes.append(*reinterpret_cast<const uint32_t *>(
                                    extra + 18 + i * 4));
        }

        QString UsbUpDownMixProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Enable Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Enable Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Mode Select Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Mode Select Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Cluster Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Cluster Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 6, 7))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 8, 9))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        uint8_t UsbDolbyPrologicProcessingUnitDescriptor::bNrModes() const
        {
            return _bNrModes;
        }

        uint32_t UsbDolbyPrologicProcessingUnitDescriptor::daModes(uint8_t index) const
        {
            if (index > 0 && index <= _bNrModes)
                return _daModes[index - 1];
            else
            {
                LOGE(tr("Index %1 for daModes out of range, must be 1~%2.")
                     .arg(index).arg(_bNrModes));
                return 0;
            }
        }

        QString UsbDolbyPrologicProcessingUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Dolby Prologic Processing Unit Descriptor"));
            __parseBase(html);
            ATTR("bNrModes", _bNrModes, _bNrModes);
            for (int i = 1; i <= _bNrModes; ++i)
                ATTR(QString("waModes(%1)").arg(i), daModes(i), parseBmChannelConfig(daModes(i)).join(NEWLINE));
            END;

            return html;
        }

        UsbDolbyPrologicProcessingUnitDescriptor::UsbDolbyPrologicProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbProcessingUnitDescriptor(parent, descPos)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bNrModes = *(extra + 17);
            _daModes.reserve(_bNrModes);
            for (int i = 0; i < _bNrModes; ++i)
                _daModes.append(*reinterpret_cast<const uint32_t *>(
                                    extra + 18 + i * 4));
        }

        QString UsbDolbyPrologicProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Enable Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Enable Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Mode Select Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Mode Select Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Cluster Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Cluster Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 6, 7))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 8, 9))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        QString StereoExtenderProcessingUnitDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Stereo Extender Processing Unit Descriptor"));
            __parseBase(html);
            END;

            return html;
        }

        StereoExtenderProcessingUnitDescriptor::StereoExtenderProcessingUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbProcessingUnitDescriptor(parent, descPos)
        {

        }

        QString StereoExtenderProcessingUnitDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Enable Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Enable Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Width Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Width Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Cluster Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Cluster Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 6, 7))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 8, 9))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
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

        uint16_t UsbExtensionUnitDescriptor::wExtensionCode() const
        {
            return _wExtensionCode;
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

        uint8_t UsbExtensionUnitDescriptor::bNrChannels() const
        {
            return _bNrChannels;
        }

        uint32_t UsbExtensionUnitDescriptor::bmChannelConfig() const
        {
            return _bmChannelConfig;
        }

        uint8_t UsbExtensionUnitDescriptor::iChannelNames() const
        {
            return _iChannelNames;
        }

        uint8_t UsbExtensionUnitDescriptor::bmControls() const
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

            QString html;
            START(tr("Extension Unit Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("wExtensionCode", _wExtensionCode, _wExtensionCode);
            ATTR("bNrInPins", _bNrInPins, _bNrInPins);
            for (int i = 1; i <= _bNrInPins; ++i)
                ATTR(QString("baSourceID(%1)").arg(i), baSourceID(i), baSourceID(i));
            ATTR("bNrChannels", _bNrChannels, _bNrChannels);
            ATTR("bmChannelConfig", _bmChannelConfig, parseBmChannelConfig(_bmChannelConfig).join(NEWLINE));
            ATTRSTRDESC("iChannelNames", _iChannelNames, device);
            ATTR("bmControls", _bmControls, __parseBmControls());
            ATTRSTRDESC("iExtension", _iExtension, device);
            END;

            return html;
        }

        UsbExtensionUnitDescriptor::UsbExtensionUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _wExtensionCode = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bNrInPins = *(extra + 6);
            for (int i = 7; i < 7 + _bNrInPins; ++i)
                _baSourceID.append(*(extra + i));
            _bNrChannels = *(extra + 7 + _bNrInPins);
            _bmChannelConfig = *reinterpret_cast<const uint32_t *>(extra + 8 + _bNrInPins);
            _iChannelNames = *(extra + 12 + _bNrInPins);
            _bmControls = *(extra + 13 + _bNrInPins);
            _iExtension = *(extra + 14 + _bNrInPins);
        }

        QString UsbExtensionUnitDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Enable Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Enable Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Cluster Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Cluster Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 4, 5))
            {
                case 0b01:
                    controls.append(tr("Underflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Underflow Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 6, 7))
            {
                case 0b01:
                    controls.append(tr("Overflow Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Overflow Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        uint8_t UsbClockSourceDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbClockSourceDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbClockSourceDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbClockSourceDescriptor::bClockID() const
        {
            return _bClockID;
        }

        uint8_t UsbClockSourceDescriptor::bmAttributes() const
        {
            return _bmAttributes;
        }

        uint8_t UsbClockSourceDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbClockSourceDescriptor::bAssocTerminal() const
        {
            return _bAssocTerminal;
        }

        uint8_t UsbClockSourceDescriptor::iClockSource() const
        {
            return _iClockSource;
        }

        QString UsbClockSourceDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Clock Source Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bClockID", _bClockID, _bClockID);
            ATTR("bmAttributes", _bmAttributes, __parseBmAttributes());
            ATTR("bmControls", _bmControls, __parseBmControls());
            ATTR("bAssocTerminal", _bAssocTerminal, _bAssocTerminal);
            ATTRSTRDESC("iClockSource", _iClockSource, device);
            END;

            return html;
        }

        UsbClockSourceDescriptor::UsbClockSourceDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bClockID = *(extra + 3);
            _bmAttributes = *(extra + 4);
            _bmControls = *(extra + 5);
            _bAssocTerminal = *(extra + 6);
            _iClockSource = *(extra + 7);
        }

        QString UsbClockSourceDescriptor::__parseBmAttributes() const
        {
            QStringList features;
            switch (CUT(_bmAttributes, 0, 1))
            {
                case 0b00:
                    features.append(tr("External Clock"));
                break;
                case 0b01:
                    features.append(tr("Internal fixed Clock"));
                break;
                case 0b10:
                    features.append(tr("Internal variable Clock"));
                break;
                case 0b11:
                    features.append(tr("Internal programmable Clock)"));
                break;
            }

            if (BIT(_bmAttributes, 2))
                features.append(tr("Clock synchronized to SOF"));

            return features.join(NEWLINE);
        }

        QString UsbClockSourceDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Clock Frequency Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Clock Frequency Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Clock Validity Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Clock Validity Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        uint8_t UsbClockSelectorDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbClockSelectorDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbClockSelectorDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbClockSelectorDescriptor::bClockID() const
        {
            return _bClockID;
        }

        uint8_t UsbClockSelectorDescriptor::bNrInPins() const
        {
            return _bNrInPins;
        }

        uint8_t UsbClockSelectorDescriptor::baCSourceID(uint8_t index) const
        {
            if (index > 0 && index <= _bNrInPins)
                return _baCSourceID[index - 1];
            else
            {
                LOGE(tr("Index %1 for baCSourceID out of range, must be 1~%2.")
                     .arg(index).arg(_bNrInPins));
                return 0;
            }
        }

        uint8_t UsbClockSelectorDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbClockSelectorDescriptor::iClockSelector() const
        {
            return _iClockSelector;
        }

        QString UsbClockSelectorDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Clock Selector Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bClockID", _bClockID, _bClockID);
            ATTR("bNrInPins", _bNrInPins, _bNrInPins);
            for (int i = 1; i <= _bNrInPins; ++i)
                ATTR(QString("baCSourceID(%1)").arg(i), baCSourceID(i), baCSourceID(i));
            ATTR("bmControls", _bmControls, __parseBmControls());
            ATTRSTRDESC("iClockSelector", _iClockSelector, device);
            END;

            return html;
        }

        UsbClockSelectorDescriptor::UsbClockSelectorDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bClockID = *(extra + 3);
            _bNrInPins = *(extra + 4);
            _baCSourceID.reserve(_bNrInPins);
            for (int i = 5; i < 5 + _bNrInPins; ++i)
                _baCSourceID.append(*(extra + i));
            _bmControls = *(extra + 5 + _bNrInPins);
            _iClockSelector = *(extra + 6 + _bNrInPins);
        }

        QString UsbClockSelectorDescriptor::__parseBmControls() const
        {
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                return tr("Clock Selector Control (Read-only)");
                case 0b11:
                return tr("Clock Selector Control (Host programmable)");
                default:
                return QString();
            }
        }

        uint8_t UsbClockMultiplierDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbClockMultiplierDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbClockMultiplierDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbClockMultiplierDescriptor::bClockID() const
        {
            return _bClockID;
        }

        uint8_t UsbClockMultiplierDescriptor::bCSourceID() const
        {
            return _bCSourceID;
        }

        uint8_t UsbClockMultiplierDescriptor::bmControls() const
        {
            return _bmControls;
        }

        uint8_t UsbClockMultiplierDescriptor::iClockMultiplier() const
        {
            return _iClockMultiplier;
        }

        QString UsbClockMultiplierDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Clock Multiplier Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bClockID", _bClockID, _bClockID);
            ATTR("bCSourceID", _bCSourceID, _bCSourceID);
            ATTR("bmControls", _bmControls, __parseBmControls());
            ATTRSTRDESC("iClockMultiplier", _iClockMultiplier, device);
            END;

            return html;
        }

        UsbClockMultiplierDescriptor::UsbClockMultiplierDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bClockID = *(extra + 3);
            _bCSourceID = *(extra + 4);
            _bmControls = *(extra + 5);
            _iClockMultiplier = *(extra + 6);
        }

        QString UsbClockMultiplierDescriptor::__parseBmControls() const
        {
            QStringList controls;
            switch (CUT(_bmControls, 0, 1))
            {
                case 0b01:
                    controls.append(tr("Clock Numerator Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Clock Numerator Control (Host programmable)"));
                break;
            }
            switch (CUT(_bmControls, 2, 3))
            {
                case 0b01:
                    controls.append(tr("Clock Denominator Control (Read-only)"));
                break;
                case 0b11:
                    controls.append(tr("Clock Denominator Control (Host programmable)"));
                break;
            }

            return controls.join(NEWLINE);
        }

        uint8_t UsbSamplingRateConverterUnitDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbSamplingRateConverterUnitDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbSamplingRateConverterUnitDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbSamplingRateConverterUnitDescriptor::bUnitID() const
        {
            return _bUnitID;
        }

        uint8_t UsbSamplingRateConverterUnitDescriptor::bSourceID() const
        {
            return _bSourceID;
        }

        uint8_t UsbSamplingRateConverterUnitDescriptor::bCSourceID() const
        {
            return _bCSourceID;
        }

        uint8_t UsbSamplingRateConverterUnitDescriptor::bCSourceOutID() const
        {
            return _bCSourceOutID;
        }

        uint8_t UsbSamplingRateConverterUnitDescriptor::iSRC() const
        {
            return _iSRC;
        }

        QString UsbSamplingRateConverterUnitDescriptor::infomationToHtml() const
        {
            UsbDevice *const device = interfaceDescriptor()->interface()->configurationDescriptor()->device();

            QString html;
            START(tr("Sampling Rate Converter Unit Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bUnitID", _bUnitID, _bUnitID);
            ATTR("bSourceID", _bSourceID, _bSourceID);
            ATTR("bCSourceID", _bCSourceID, _bCSourceID);
            ATTR("bCSourceOutID", _bCSourceOutID, _bCSourceOutID);
            ATTRSTRDESC("iSRC", _iSRC, device);
            END;

            return html;
        }

        UsbSamplingRateConverterUnitDescriptor::UsbSamplingRateConverterUnitDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbAudioControlInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bUnitID = *(extra + 3);
            _bSourceID = *(extra + 4);
            _bCSourceID = *(extra + 5);
            _bCSourceOutID = *(extra + 6);
            _iSRC = *(extra + 7);
        }
    } // namespace uac2
} // namespace usb
