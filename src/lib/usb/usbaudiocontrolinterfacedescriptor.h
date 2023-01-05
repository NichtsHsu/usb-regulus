/*! C++ class wrapper of USB AudioControl Interface Desciptors

 * Copyright (C) 2022-2023 Nichts Hsu

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef USBAUDIOCONTROLINTERFACEDESCRIPTOR_H
#define USBAUDIOCONTROLINTERFACEDESCRIPTOR_H

#include <QObject>
#include <QByteArray>
#include <QVector>
#include "usbinterfaceextradescriptor.h"
#include "usbhtmlbuilder.h"

namespace usb {
    class UsbInterfaceDescriptor;
}

namespace usb {
    /**
     * @brief The UsbAudioControlInterfaceDescriptorBasic class
     * AudioControl Interface Descriptor basic class, to indicate the Audio spec version
     */
    class UsbAudioControlInterfaceDescriptorBasic : public UsbInterfaceExtraDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbAudioControlInterfaceDescriptorBasic(UsbInterfaceDescriptor *parent);

        /**
         * @brief type
         * @return the interface extra descriptor type
         */
        InterfaceExtraDescriptorType type() const override;

        /**
         * @brief specVersion
         * @return audio version, 1, 2 or 3
         */
        virtual uint8_t specVersion() const = 0;
    };

    /* For Audio 1.0 spec */
    namespace uac1 {
        /**
         * @brief The UsbAudioControlInterfaceDescriptor class
         * AudioControl Interface Descriptor C++ wrapper
         */
        class UsbAudioControlInterfaceDescriptor : public UsbAudioControlInterfaceDescriptorBasic
        {
            Q_OBJECT
        public:
            explicit UsbAudioControlInterfaceDescriptor(UsbInterfaceDescriptor *parent);

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            virtual uint8_t bDescriptorSubtype() const = 0;

            /**
             * @brief specVersion
             * @return the version of Audio Spec. Always be 1 here.
             */
            uint8_t specVersion() const override;

            /**
             * @brief strSubtype
             * @return string description of subtype
             */
            static QString strSubtype(uint8_t subtype);

            /**
             * @brief parseWChannelConfig
             * @param wChannelConfig
             * The bit field that indicates which spatial locations are present in the cluster.
             * @return stringlist of spatial locations
             * @note
             * D0       Left Front (L)
             * D1       Right Front (R)
             * D2       Center Front (C)
             * D3       Low Frequency Enhancement (LFE)
             * D4       Left Surround (LS)
             * D5       Right Surround (RS)
             * D6       Left of Center (LC)
             * D7       Right of Center (RC)
             * D8       Surround (S)
             * D9       Side Left (SL)
             * D10      Side Right (SR)
             * D11      Top (T)
             * D12..15  Reserved
             */
            static QStringList parseWChannelConfig(uint16_t wChannelConfig);

            /**
             * @brief get
             * Get the AudioControl Interface Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * AudioControl Interface Descriptor's position at extra data
             * @return pointer to the instance of AudioControl Interface Descriptor,
             * nullptr if parse failed.
             */
            static UsbAudioControlInterfaceDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                           uint8_t descPos);
        };

        /**
         * @brief The UsbClassSpecificACInterfaceHeaderDescriptor class
         * Class-Specific AC Interface Header Descriptor C++ wrapper
         */
        class UsbClassSpecificACInterfaceHeaderDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x1 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bcdADC
             * @return Audio Device Class Specification Release Number in Binary-Coded Decimal
             */
            uint16_t bcdADC() const;

            /**
             * @brief wTotalLength
             * @return total number of bytes returned for the
             * class-specific AudioControl interface descriptor.
             */
            uint16_t wTotalLength() const;

            /**
             * @brief bInCollection
             * @return the number of AudioStreaming and MIDIStreaming interfaces
             * in the Audio Interface Collection to which this AudioControl interface belongs
             */
            uint8_t bInCollection() const;

            /**
             * @brief baInterfaceNr
             * @param index
             * @note
             * The Spec defines the first baInterfaceNr is baInterfaceNr(1),
             * therefore the parameter 'index' should also start with 1.
             * @return interface number of the index-th AudioStreaming
             * or MIDIStreaming interface in the Collection.
             */
            uint8_t baInterfaceNr(uint8_t index) const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbClassSpecificACInterfaceHeaderDescriptor(UsbInterfaceDescriptor *parent,
                                                                 uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bInCollection;
            uint16_t _bcdADC, _wTotalLength;
            QVector<uint8_t> _baInterfaceNr;
        };

        /**
         * @brief The UsbInputTerminalDescriptor class
         * Input Terminal Descriptor C++ wrapper
         */
        class UsbInputTerminalDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x2 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bTerminalID
             * @return constant uniquely identifying the Terminal within the audio function
             */
            uint8_t bTerminalID() const;

            /**
             * @brief wTerminalType
             * @return constant characterizing the type of Terminal
             * @see strTerminalType()
             */
            uint16_t wTerminalType() const;

            /**
             * @brief bAssocTerminal
             * @return ID of the Output Terminal to which this Input Terminal is associated
             */
            uint8_t bAssocTerminal() const;

            /**
             * @brief bNrChannels
             * @return number of logical output channels in the Terminal’s output audio channel cluster
             */
            uint8_t bNrChannels() const;

            /**
             * @brief wChannelConfig
             * @return bitmap describing the spatial location of the logical channels
             * @see UsbAudioControlInterfaceDescriptor::parseWChannelConfig()
             */
            uint16_t wChannelConfig() const;

            /**
             * @brief iChannelNames
             * @return string descriptor describing the name of the first logical channel
             */
            uint8_t iChannelNames() const;

            /**
             * @brief iTerminal
             * @return string descriptor describing the Input Terminal
             */
            uint8_t iTerminal() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;


        private:
            explicit UsbInputTerminalDescriptor(UsbInterfaceDescriptor *parent,
                                                uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bTerminalID,
            _bAssocTerminal, _bNrChannels, _iChannelNames, _iTerminal;
            uint16_t _wTerminalType, _wChannelConfig;
        };

        /**
         * @brief The UsbOutputTerminalDescriptor class
         * Output Terminal Descriptor C++ wrapper
         */
        class UsbOutputTerminalDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x3 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bTerminalID
             * @return constant uniquely identifying the Terminal within the audio function
             */
            uint8_t bTerminalID() const;

            /**
             * @brief wTerminalType
             * @return constant characterizing the type of Terminal
             * @see strTerminalType()
             */
            uint16_t wTerminalType() const;

            /**
             * @brief bAssocTerminal
             * @return ID of the Input Terminal to which this Output Terminal is associated
             */
            uint8_t bAssocTerminal() const;

            /**
             * @brief bSourceID
             * @return ID of the Unit or Terminal to which this Terminal is connected
             */
            uint8_t bSourceID() const;

            /**
             * @brief iTerminal
             * @return string descriptor describing the Output Terminal
             */
            uint8_t iTerminal() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbOutputTerminalDescriptor(UsbInterfaceDescriptor *parent,
                                                 uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bTerminalID,
            _bAssocTerminal, _bSourceID, _iTerminal;
            uint16_t _wTerminalType;
        };

        /**
         * @brief The UsbMixerUnitDescriptor class
         * Mixer Unit Descriptor C++ wrapper
         */
        class UsbMixerUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x4 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief bNrInPins
             * @return number of Input Pins of this Unit
             */
            uint8_t bNrInPins() const;

            /**
             * @brief baSourceID
             * @param index
             * @note
             * The Spec defines the first baSourceID is baSourceID(1),
             * therefore the parameter 'index' should also start with 1.
             * @return ID of the Unit or Terminal to which the index-th Input Pin
             * of this Mixer Unit is connected.
             */
            uint8_t baSourceID(uint8_t index) const;

            /**
             * @brief bNrChannels
             * @return number of logical output channels in the Mixer’s output audio channel cluster
             */
            uint8_t bNrChannels() const;

            /**
             * @brief wChannelConfig
             * @return bitmap describing the spatial location of the logical channels
             * @see UsbAudioControlInterfaceDescriptor::parseWChannelConfig()
             */
            uint16_t wChannelConfig() const;

            /**
             * @brief iChannelNames
             * @return string descriptor describing the name of the first logical channel
             */
            uint8_t iChannelNames() const;

            /**
             * @brief bmControls
             * @return bitmap indicating which mixing Controls are programmable
             */
            QByteArray bmControls() const;

            /**
             * @brief iMixer
             * @return string descriptor describing the Mixer Unit
             */
            uint8_t iMixer() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbMixerUnitDescriptor(UsbInterfaceDescriptor *parent,
                                            uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNrInPins, _bNrChannels, _iChannelNames, _iMixer;
            uint16_t _wChannelConfig;
            QVector<uint8_t> _baSourceID;
            QByteArray _bmControls;
        };

        /**
         * @brief The UsbSelectorUnitDescriptor class
         * Selector Unit Descriptor C++ wrapper
         */
        class UsbSelectorUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x5 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief bNrInPins
             * @return number of Input Pins of this Unit
             */
            uint8_t bNrInPins() const;

            /**
             * @brief baSourceID
             * @param index
             * @note
             * The Spec defines the first baSourceID is baSourceID(1),
             * therefore the parameter 'index' should also start with 1.
             * @return ID of the Unit or Terminal to which the index-th Input Pin
             * of this Selector Unit is connected.
             */
            uint8_t baSourceID(uint8_t index) const;

            /**
             * @brief iSelector
             * @return string descriptor describing the Selector Unit
             */
            uint8_t iSelector() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbSelectorUnitDescriptor(UsbInterfaceDescriptor *parent,
                                               uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNrInPins, _iSelector;
            QByteArray _baSourceID;
        };

        /**
         * @brief The UsbFeatureUnitDescriptor class
         * Feature Unit Descriptor C++ wrapper
         */
        class UsbFeatureUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x6 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief bSourceID
             * @return ID of the Unit or Terminal to which this Feature Unit is connected
             */
            uint8_t bSourceID() const;

            /**
             * @brief bControlSize
             * @return size in bytes of an element of the bmaControls array
             */
            uint8_t bControlSize() const;

            /**
             * @brief bmaControls
             * @param index
             * @return bitmap that a bit set to 1 indicates that
             * the mentioned Control is supported for index-th channel
             * @note
             * D0       Mute
             * D1       Volume
             * D2       Bass
             * D3       Mid
             * D4       Treble
             * D5       Graphic Equalizer
             * D6       Automatic Gain
             * D7       Delay
             * D8       Bass Boost
             * D9       Loudness
             * D10..    Reserved
             */
            QByteArray bmaControls(uint8_t index) const;

            /**
             * @brief iFeature
             * @return string descriptor describing this Feature Unit
             */
            uint8_t iFeature() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbFeatureUnitDescriptor(UsbInterfaceDescriptor *parent,
                                              uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmaControls(uint8_t index) const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bSourceID, _bControlSize, _iFeature;
            QVector<QByteArray> _bmaControls;
            uint8_t _CHANNEL;
        };

        /**
         * @brief The UsbProcessingUnitDescriptor class
         * Processing Unit Descriptor C++ wrapper
         */
        class UsbProcessingUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x7 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief wProcessType
             * @return constant identifying the type of processing this Unit is performing
             * @see UsbProcessingUnitDescriptor::strWProcessType()
             */
            uint16_t wProcessType() const;

            /**
             * @brief bNrInPins
             * @return number of Input Pins of this Unit
             */
            uint8_t bNrInPins() const;

            /**
             * @brief baSourceID
             * @param index
             * @return ID of the Unit or Terminal to which the index-th Input Pin
             * of this Processing Unit is connected.
             */
            uint8_t baSourceID(uint8_t index) const;

            /**
             * @brief bNrChannels
             * @return number of logical output channels in the
             * audio channel cluster of the Processing Unit
             */
            uint8_t bNrChannels() const;

            /**
             * @brief wChannelConfig
             * @return bitmap describing the spatial location of the logical channels
             * in the audio channel cluster of the Processing Unit.
             * @see UsbAudioControlInterfaceDescriptor::parseWChannelConfig()
             */
            uint16_t wChannelConfig() const;

            /**
             * @brief iChannelNames
             * @return string descriptor describing the name of the first logical channel
             * in the audio channel cluster of the Processing Unit.
             */
            uint8_t iChannelNames() const;

            /**
             * @brief bControlSize
             * @return size in bytes of the bmControls field
             */
            uint8_t bControlSize() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates that the mentioned Control is supported
             * @note
             * D0       Supports Enable Processing
             * D1..     process-specific allocation
             * @see __parseBmControls() of derived class
             */
            QByteArray bmControls() const;

            /**
             * @brief iProcessing
             * @return string descriptor describing this Processing Unit
             */
            uint8_t iProcessing() const;

            /**
             * @brief strWProcessType
             * @param wProcessType
             * @return string description of wProcessType
             */
            static QString strWProcessType(uint16_t wProcessType);

        protected:
            explicit UsbProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                 uint8_t descPos);
            inline void __parseBase(UsbHtmlBuilder &builder) const;

            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             */
            virtual QString __parseBmControls() const = 0;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNrInPins, _bNrChannels, _iChannelNames, _bControlSize, _iProcessing;
            uint16_t _wProcessType, _wChannelConfig;
            QVector<uint8_t> _baSourceID;
            QByteArray _bmControls;

        private:
            /**
             * @brief get
             * Get the Processing Unit Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * Processing Unit Descriptor's position at extra data
             * @return pointer to the instance of Processing Unit Descriptor,
             * nullptr if parse failed.
             */
            static UsbProcessingUnitDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                    uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
        };

        /**
         * @brief The UsbUpDownMixProcessingUnitDescriptor class
         * Up/Down-mix Processing Unit Descriptor C++ wrapper
         */
        class UsbUpDownMixProcessingUnitDescriptor : public UsbProcessingUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bNrModes
             * @return number of modes supported by this Processing Unit
             */
            uint8_t bNrModes() const;

            /**
             * @brief waModes
             * @param index
             * @note
             * The Spec defines the first waModes is waModes(1),
             * therefore the parameter 'index' should also start with 1.
             * @return bitmap describing the active logical channels in index-th mode
             * @note waModes is the subset of wChannelConfig
             * @see UsbProcessingUnitDescriptor::wChannelConfig()
             * @see UsbAudioControlInterfaceDescriptor::parseWChannelConfig()
             */
            uint16_t waModes(uint8_t index) const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbUpDownMixProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                          uint8_t descPos);
            friend UsbProcessingUnitDescriptor;

            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             * @note
             * D0       Enable Processing
             * D1       Mode Select
             * D2..     Reserved
             */
            QString __parseBmControls() const override;

            uint8_t _bNrModes;
            QVector<uint16_t> _waModes;
        };

        /**
         * @brief The UsbDolbyPrologicProcessingUnitDescriptor class
         * Dolby Prologic Processing Unit Descriptor C++ wrapper
         */
        class UsbDolbyPrologicProcessingUnitDescriptor : public UsbProcessingUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bNrModes
             * @return number of modes supported by this Processing Unit
             */
            uint8_t bNrModes() const;

            /**
             * @brief waModes
             * @param index
             * @note
             * The Spec defines the first waModes is waModes(1),
             * therefore the parameter 'index' should also start with 1.
             * @return bitmap describing the active logical channels in index-th mode
             * @note waModes is the subset of wChannelConfig
             * @see UsbProcessingUnitDescriptor::wChannelConfig()
             * @see UsbAudioControlInterfaceDescriptor::parseWChannelConfig()
             */
            uint16_t waModes(uint8_t index) const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbDolbyPrologicProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                              uint8_t descPos);
            friend UsbProcessingUnitDescriptor;

            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             * @note
             * D0       Enable Processing
             * D1       Mode Select
             * D2..     Reserved
             */
            QString __parseBmControls() const override;

            uint8_t _bNrModes;
            QVector<uint16_t> _waModes;
        };

        /**
         * @brief The Usb3DStereoExtenderProcessingUnitDescriptor class
         * 3D-Stereo Extender Processing Unit Descriptor C++ wrapper
         */
        class Usb3DStereoExtenderProcessingUnitDescriptor : public UsbProcessingUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit Usb3DStereoExtenderProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                                 uint8_t descPos);
            friend UsbProcessingUnitDescriptor;

            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             * @note
             * D0       Enable Processing
             * D1       Spaciousness
             * D2..     Reserved
             */
            QString __parseBmControls() const override;
        };

        /**
         * @brief The UsbReverberationProcessingUnitDescriptor class
         * Reverberation Processing Unit Descriptor C++ wrapper
         */
        class UsbReverberationProcessingUnitDescriptor : public UsbProcessingUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbReverberationProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                              uint8_t descPos);
            friend UsbProcessingUnitDescriptor;

            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             * @note
             * D0       Enable Processing
             * D1       Reverb Type
             * D2       Reverb Level
             * D3       Reverb Time
             * D4       Reverb Delay Feedback
             * D5..     Reserved
             */
            QString __parseBmControls() const override;
        };

        class UsbChorusProcessingUnitDescriptor : public UsbProcessingUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbChorusProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                       uint8_t descPos);
            friend UsbProcessingUnitDescriptor;

            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             * @note
             * D0       Enable Processing
             * D1       Chorus Level
             * D2       Chorus Modulation Rate
             * D3       Chorus Modulation Depth
             * D4..     Reserved
             */
            QString __parseBmControls() const override;
        };

        /**
         * @brief The UsbDynamicRangeCompressorProcessingUnitDescriptor class
         * Dynamic Range Compressor Processing Unit Descriptor C++ wrapper
         */
        class UsbDynamicRangeCompressorProcessingUnitDescriptor : public UsbProcessingUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbDynamicRangeCompressorProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                                       uint8_t descPos);
            friend UsbProcessingUnitDescriptor;

            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             * @note
             * D0       Enable Processing
             * D1       Compression Ratio
             * D2       MaxAmpl
             * D3       Threshold
             * D4       Attack time
             * D5       Release time
             * D6..     Reserved
             */
            QString __parseBmControls() const override;
        };

        /**
         * @brief The UsbExtensionUnitDescriptor class
         * Extension Unit Descriptor C++ wrapper
         */
        class UsbExtensionUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x8 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief wExtensionCode
             * @return vendor-specific code identifying the Extension Unit
             */
            uint16_t wExtensionCode() const;

            /**
             * @brief bNrInPins
             * @return number of Input Pins of this Unit
             */
            uint8_t bNrInPins() const;

            /**
             * @brief baSourceID
             * @param index
             * @note
             * The Spec defines the first baSourceID is baSourceID(1),
             * therefore the parameter 'index' should also start with 1.
             * @return ID of the Unit or Terminal to which the index-th Input Pin
             * of this Extension Unit is connected.
             */
            uint8_t baSourceID(uint8_t index) const;

            /**
             * @brief bNrChannels
             * @return number of logical output channels in the
             * audio channel cluster of the Extension Unit.
             */
            uint8_t bNrChannels() const;

            /**
             * @brief wChannelConfig
             * @return bitmap describing the spatial location of the logical channels
             * in the audio channel cluster of the Extension Unit.
             */
            uint16_t wChannelConfig() const;

            /**
             * @brief iChannelNames
             * @return string descriptor describing the name of the first logical channel
             * in the audio channel cluster of the Extension Unit.
             */
            uint8_t iChannelNames() const;

            /**
             * @brief bControlSize
             * @return size of the bmControls field in bytes
             */
            uint8_t bControlSize() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates that the mentioned Control is supported
             * @note
             * D0       Enable Processing
             * D1..     Reserved
             */
            QByteArray bmControls() const;

            /**
             * @brief iExtension
             * @return string descriptor describing this Extension Unit
             */
            uint8_t iExtension() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbExtensionUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNrInPins, _bNrChannels, _iChannelNames, _bControlSize, _iExtension;
            uint16_t _wExtensionCode, _wChannelConfig;
            QVector<uint8_t> _baSourceID;
            QByteArray _bmControls;
        };
    } // namespace uac1

    /* For Audio 2.0 spec */
    namespace uac2 {
        /**
         * @brief The UsbAudioControlInterfaceDescriptor class
         * AudioControl Interface Descriptor C++ wrapper
         */
        class UsbAudioControlInterfaceDescriptor : public UsbAudioControlInterfaceDescriptorBasic
        {
            Q_OBJECT
        public:
            explicit UsbAudioControlInterfaceDescriptor(UsbInterfaceDescriptor *parent);

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            virtual uint8_t bDescriptorSubtype() const = 0;

            /**
             * @brief specVersion
             * @return the version of Audio Spec. Always be 2 here.
             */
            uint8_t specVersion() const override;

            /**
             * @brief strSubtype
             * @return string description of subtype
             */
            static QString strSubtype(uint8_t subtype);

            /**
             * @brief parseWChannelConfig
             * @param wChannelConfig
             * The bit field that indicates which spatial locations are present in the cluster.
             * @return stringlist of spatial locations
             * @note
             * D0       Front Left (FL)
             * D1       Front Right (FR)
             * D2       Front Center (FC)
             * D3       Low Frequency Enhancement (LFE)
             * D4       Back Left (BL)
             * D5       Back Right (BR)
             * D6       Front Left of Center (FLC)
             * D7       Front Right of Center (FRC)
             * D8       Back Center (BC)
             * D9       Side Left (SL)
             * D10      Side Right (SR)
             * D11      Top Center (TC)
             * D12      Top Front Left (TFL)
             * D13      Top Front Center(TFC)
             * D14      Top Front Right (TFR)
             * D15      Top Back Left (TBL)
             * D16      Top Back Center (TBC)
             * D17      Top Back Right (TBR)
             * D18      Top Front Left of Center (TFLC)
             * D19      Top Front Right of Center (TFRC)
             * D20      Left Low Frequency Effects (LLFE)
             * D21      Right Low Frequency Effects (RLFE)
             * D22      Top Side Left (TSL)
             * D23      Top Side Right (TSR)
             * D24      Bottom Center (BC)
             * D25      Back Left of Center (BLC)
             * D26      Back Right of Center (BRC)
             * D27..30  Reserved
             * D31      Raw Data (RD)
             */
            static QStringList parseBmChannelConfig(uint32_t bmChannelConfig);

            /**
             * @brief get
             * Get the AudioControl Interface Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * AudioControl Interface Descriptor's position at extra data
             * @return pointer to the instance of AudioControl Interface Descriptor,
             * nullptr if parse failed.
             */
            static UsbAudioControlInterfaceDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                           uint8_t descPos);
        };

        /**
         * @brief The UsbClassSpecificACInterfaceHeaderDescriptor class
         * Class-Specific AC Interface Header Descriptor C++ wrapper
         */
        class UsbClassSpecificACInterfaceHeaderDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x1 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bcdADC
             * @return Audio Device Class Specification Release Number in Binary-Coded Decimal
             */
            uint16_t bcdADC() const;

            /**
             * @brief bCategory
             * @return constant indicating the primary use of this audio function,
             * as intended by the manufacturer.
             */
            uint8_t bCategory() const;

            /**
             * @brief wTotalLength
             * @return total number of bytes returned for the
             * class-specific AudioControl interface descriptor.
             */
            uint16_t wTotalLength() const;

            /**
             * @brief bmControls
             * @return bitmap indicating which Controls are present at the
             * audio function level and what their capabilities are.
             * @note
             * D0..1    Latency Control
             * D2..15   Reserved
             */
            uint16_t bmControls() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;


        private:
            explicit UsbClassSpecificACInterfaceHeaderDescriptor(UsbInterfaceDescriptor *parent,
                                                                 uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __strBCategory() const;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bCategory;
            uint16_t _bcdADC, _wTotalLength, _bmControls;
        };

        /**
         * @brief The UsbInputTerminalDescriptor class
         * Input Terminal Descriptor C++ wrapper
         */
        class UsbInputTerminalDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x2 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bTerminalID
             * @return constant uniquely identifying the Terminal within the audio function
             */
            uint8_t bTerminalID() const;

            /**
             * @brief wTerminalType
             * @return constant characterizing the type of Terminal
             * @see strTerminalType()
             */
            uint16_t wTerminalType() const;

            /**
             * @brief bAssocTerminal
             * @return ID of the Output Terminal to which this Input Terminal is associated
             */
            uint8_t bAssocTerminal() const;

            /**
             * @brief bCSourceID
             * @return ID of the Clock Entity to which this Input Terminal is connected
             */
            uint8_t bCSourceID() const;

            /**
             * @brief bNrChannels
             * @return number of logical output channels in the Terminal’s output audio channel cluster
             */
            uint8_t bNrChannels() const;

            /**
             * @brief bmChannelConfig
             * @return bitmap describing the spatial location of the logical channels
             * @see UsbAudioControlInterfaceDescriptor::parseBmChannelConfig()
             */
            uint32_t bmChannelConfig() const;

            /**
             * @brief iChannelNames
             * @return string descriptor describing the name of the first logical channel
             */
            uint8_t iChannelNames() const;

            /**
             * @brief bmControls
             * @return bitmap indicating which Controls are present and what their capabilities are
             * @note
             * D0..1    Copy Protect Control
             * D2..3    Connector Control
             * D4..5    Overload Control
             * D6..7    Cluster Control
             * D8..9    Underflow Control
             * D10..11  Overflow Control
             * D12..15  Reserved
             */
            uint16_t bmControls() const;

            /**
             * @brief iTerminal
             * @return string descriptor describing the Input Terminal
             */
            uint8_t iTerminal() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbInputTerminalDescriptor(UsbInterfaceDescriptor *parent,
                                                uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bTerminalID,
            _bAssocTerminal, _bCSourceID, _bNrChannels, _iChannelNames, _iTerminal;
            uint16_t _wTerminalType, _bmControls;
            uint32_t _bmChannelConfig;
        };

        /**
         * @brief The UsbOutputTerminalDescriptor class
         * Output Terminal Descriptor C++ wrapper
         */
        class UsbOutputTerminalDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x3 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bTerminalID
             * @return constant uniquely identifying the Terminal within the audio function
             */
            uint8_t bTerminalID() const;

            /**
             * @brief wTerminalType
             * @return constant characterizing the type of Terminal
             * @see strTerminalType()
             */
            uint16_t wTerminalType() const;

            /**
             * @brief bAssocTerminal
             * @return ID of the Input Terminal to which this Output Terminal is associated
             */
            uint8_t bAssocTerminal() const;

            /**
             * @brief bSourceID
             * @return ID of the Unit or Terminal to which this Terminal is connected
             */
            uint8_t bSourceID() const;

            /**
             * @brief bCSourceID
             * @return ID of the Clock Entity to which this Output Terminal is connected
             */
            uint8_t bCSourceID() const;

            /**
             * @brief bmControls
             * @return bitmap indicating which Controls are present and what their capabilities are
             * @note
             * D0..1    Copy Protect Control
             * D2..3    Connector Control
             * D4..5    Overload Control
             * D6..7    Underflow Control
             * D8..9    Overflow Control
             * D10..15  Reserved
             */
            uint16_t bmControls() const;

            /**
             * @brief iTerminal
             * @return string descriptor describing the Output Terminal
             */
            uint8_t iTerminal() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbOutputTerminalDescriptor(UsbInterfaceDescriptor *parent,
                                                 uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bTerminalID,
            _bAssocTerminal, _bSourceID, _bCSourceID, _iTerminal;
            uint16_t _wTerminalType, _bmControls;
        };

        /**
         * @brief The UsbMixerUnitDescriptor class
         * Mixer Unit Descriptor C++ wrapper
         */
        class UsbMixerUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x4 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief bNrInPins
             * @return number of Input Pins of this Unit
             */
            uint8_t bNrInPins() const;

            /**
             * @brief baSourceID
             * @param index
             * @note
             * The Spec defines the first baSourceID is baSourceID(1),
             * therefore the parameter 'index' should also start with 1.
             * @return ID of the Unit or Terminal to which the index-th Input Pin
             * of this Mixer Unit is connected.
             */
            uint8_t baSourceID(uint8_t index) const;

            /**
             * @brief bNrChannels
             * @return number of logical output channels in the Mixer’s output audio channel cluster
             */
            uint8_t bNrChannels() const;

            /**
             * @brief bmChannelConfig
             * @return bitmap describing the spatial location of the logical channels
             * @see UsbAudioControlInterfaceDescriptor::parseBmChannelConfig()
             */
            uint32_t bmChannelConfig() const;

            /**
             * @brief iChannelNames
             * @return string descriptor describing the name of the first logical channel
             */
            uint8_t iChannelNames() const;

            /**
             * @brief bmMixerControls
             * @return bitmap indicating which Mixer Controls are programmable
             */
            QByteArray bmMixerControls() const;

            /**
             * @brief bmControls
             * @return bitmap indicating which Controls are present and what their capabilities are
             * @note
             * D0..1    Cluster Control
             * D2..3    Underflow Control
             * D4..5    Overflow Control
             * D6..7    Reserved
             */
            uint8_t bmControls() const;

            /**
             * @brief iMixer
             * @return string descriptor describing the Mixer Unit
             */
            uint8_t iMixer() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;


        private:
            explicit UsbMixerUnitDescriptor(UsbInterfaceDescriptor *parent,
                                            uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNrInPins, _bNrChannels, _iChannelNames, _bmControls, _iMixer;
            uint32_t _bmChannelConfig;
            QVector<uint8_t> _baSourceID;
            QByteArray _bmMixerControls;
        };

        /**
         * @brief The UsbSelectorUnitDescriptor class
         * Selector Unit Descriptor C++ wrapper
         */
        class UsbSelectorUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x5 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief bNrInPins
             * @return number of Input Pins of this Unit
             */
            uint8_t bNrInPins() const;

            /**
             * @brief baSourceID
             * @param index
             * @note
             * The Spec defines the first baSourceID is baSourceID(1),
             * therefore the parameter 'index' should also start with 1.
             * @return ID of the Unit or Terminal to which the index-th Input Pin
             * of this Selector Unit is connected.
             */
            uint8_t baSourceID(uint8_t index) const;

            /**
             * @brief bmControls
             * @return bitmap indicating which Controls are present and what their capabilities are
             * @note
             * D0..1    Selector Control
             * D2..7    Reserved
             */
            uint8_t bmControls() const;

            /**
             * @brief iSelector
             * @return string descriptor describing the Selector Unit
             */
            uint8_t iSelector() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;


        private:
            explicit UsbSelectorUnitDescriptor(UsbInterfaceDescriptor *parent,
                                               uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNrInPins, _bmControls, _iSelector;
            QByteArray _baSourceID;
        };

        /**
         * @brief The UsbFeatureUnitDescriptor class
         * Feature Unit Descriptor C++ wrapper
         */
        class UsbFeatureUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x6 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief bSourceID
             * @return ID of the Unit or Terminal to which this Feature Unit is connected
             */
            uint8_t bSourceID() const;

            /**
             * @brief bmaControls
             * @param index
             * @return bitmap that a bit set to 1 indicates which Control are present and
             * what their capabilities are for index-th channel
             * @note
             * D0..1    Mute Control
             * D2..3    Volume Control
             * D4..5    Bass Control
             * D6..7    Mid Control
             * D8..9    Treble Control
             * D10..11  Graphic Equalizer Control
             * D12..13  Automatic Gain Control
             * D14..15  Delay Control
             * D16..17  Bass Boost Control
             * D18..19  Loudness Control
             * D20..21  Input Gain Control
             * D22..23  Input Gain Pad Control
             * D24..25  Phase Inverter Control
             * D26..27  Underflow Control
             * D28..29  Overfow Control
             * D30..31  Reserved
             */
            uint32_t bmaControls(uint8_t index) const;

            /**
             * @brief iFeature
             * @return string descriptor describing this Feature Unit
             */
            uint8_t iFeature() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbFeatureUnitDescriptor(UsbInterfaceDescriptor *parent,
                                              uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmaControls(uint8_t index) const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bSourceID, _iFeature;
            QVector<uint32_t> _bmaControls;
            uint8_t _CHANNEL;
        };

        /**
         * @brief The UsbEffectUnitDescriptor class
         * Effect Unit Descriptor C++ wrapper
         */
        class UsbEffectUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x7 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief wEffectType
             * @return constant identifying the type of effect this Unit is performing
             * @see UsbEffectUnitDescriptor::strWEffectType()
             */
            uint16_t wEffectType() const;

            /**
             * @brief bSourceID
             * @return ID of the Unit or Terminal to which this Effect Unit is connected
             */
            uint8_t bSourceID() const;

            /**
             * @brief bmControls
             * @param index
             * @return bitmap that a bit set to 1 indicates which Control are present and
             * what their capabilities are for index-th channel
             * @note
             * D0       Supports Enable Processing
             * D1..     effect-specific allocation
             * @see __parseBmaControls() of derived class
             */
            uint32_t bmaControls(uint8_t index) const;

            /**
             * @brief iProcessing
             * @return string descriptor describing this Effect Unit
             */
            uint8_t iEffects() const;

            /**
             * @brief strWEffectType
             * @param wEffectType
             * @return string description of wEffectType
             */
            static QString strWEffectType(uint16_t wEffectType);

        protected:
            explicit UsbEffectUnitDescriptor(UsbInterfaceDescriptor *parent,
                                             uint8_t descPos);
            inline void __parseBase(UsbHtmlBuilder &builder) const;

            /**
             * @brief __parseBmaControls
             * @return string description of bmaControls
             */
            virtual QString __parseBmaControls(uint8_t index) const = 0;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bSourceID, _iEffects;
            uint16_t _wEffectType;
            QVector<uint32_t> _bmaControls;

        private:
            /**
             * @brief get
             * Get the Processing Unit Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * Processing Unit Descriptor's position at extra data
             * @return pointer to the instance of Processing Unit Descriptor,
             * nullptr if parse failed.
             */
            static UsbEffectUnitDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;

            uint8_t _CHANNEL;
        };

        /**
         * @brief The UsbParametricEqualizerSectionEffectUnitDescriptor class
         * Parametric Equalizer Section Effect Unit Descriptor C++ wrapper
         */
        class UsbParametricEqualizerSectionEffectUnitDescriptor : public UsbEffectUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbParametricEqualizerSectionEffectUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                                       uint8_t descPos);
            friend UsbEffectUnitDescriptor;

            /**
             * @brief __parseBmaControls
             * @return string description of bmControls
             * @note
             * D0..1    Enable Control
             * D2..3    Center Frequency Control
             * D4..5    Q Factor Control
             * D6..7    Gain Control
             * D8..9    Underflow Control
             * D10..11  Overflow Control
             * D12..31  Reversed
             */
            QString __parseBmaControls(uint8_t index) const override;
        };

        /**
         * @brief The UsbReverberationEffectUnitDescriptor class
         * Reverberation Effect Unit Descriptor C++ Wrapper
         */
        class UsbReverberationEffectUnitDescriptor : public UsbEffectUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbReverberationEffectUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                          uint8_t descPos);
            friend UsbEffectUnitDescriptor;

            /**
             * @brief __parseBmaControls
             * @return string description of bmControls
             * @note
             * D0..1    Enable Control
             * D2..3    Type Control
             * D4..5    Level Control
             * D6..7    Time Control
             * D8..9    Delay Feedback Control
             * D10..11  Pre-Delay Control
             * D12..13  Density Control
             * D14..15  Hi-Freq Roll-Off Control
             * D16..17  Underflow Control
             * D18..19  Overflow Control
             * D20..31  Reversed
             */
            QString __parseBmaControls(uint8_t index) const override;
        };

        /**
         * @brief The UsbModulationDelayEffectUnitDescriptor class
         * Modulation Delay Effect Unit Descriptor C++ Wrapper
         */
        class UsbModulationDelayEffectUnitDescriptor : public UsbEffectUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbModulationDelayEffectUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                            uint8_t descPos);
            friend UsbEffectUnitDescriptor;

            /**
             * @brief __parseBmaControls
             * @return string description of bmControls
             * @note
             * D0..1    Enable Control
             * D2..3    Balance Control
             * D4..5    Rate Control
             * D6..7    Depth Control
             * D8..9    Time Control
             * D10..11  Feedback Level Control
             * D12..13  Underflow Control
             * D14..15  Overflow Control
             * D16..31  Reversed
             */
            QString __parseBmaControls(uint8_t index) const override;
        };

        /**
         * @brief The UsbDynamicRangeCompressorEffectUnitDescriptor class
         * Dynamic Range Compressor Effect Unit Descriptor C++ Wrapper
         */
        class UsbDynamicRangeCompressorEffectUnitDescriptor : public UsbEffectUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbDynamicRangeCompressorEffectUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                                   uint8_t descPos);
            friend UsbEffectUnitDescriptor;

            /**
             * @brief __parseBmaControls
             * @return string description of bmControls
             * @note
             * D0..1    Enable Control
             * D2..3    Compression Ratio Control
             * D4..5    MaxAmpl Control
             * D6..7    Threshold Control
             * D8..9    Attack Time Control
             * D10..11  Release Time Control
             * D12..13  Underflow Control
             * D14..15  Overflow Control
             * D16..31  Reversed
             */
            QString __parseBmaControls(uint8_t index) const override;
        };

        /**
         * @brief The UsbProcessingUnitDescriptor class
         * Processing Unit Descriptor C++ wrapper
         */
        class UsbProcessingUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x8 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief wProcessType
             * @return constant identifying the type of processing this Unit is performing
             * @see UsbProcessingUnitDescriptor::strWProcessType()
             */
            uint16_t wProcessType() const;

            /**
             * @brief bNrInPins
             * @return number of Input Pins of this Unit
             */
            uint8_t bNrInPins() const;

            /**
             * @brief baSourceID
             * @param index
             * @return ID of the Unit or Terminal to which the index-th Input Pin
             * of this Processing Unit is connected.
             */
            uint8_t baSourceID(uint8_t index) const;

            /**
             * @brief bNrChannels
             * @return number of logical output channels in the
             * audio channel cluster of the Processing Unit
             */
            uint8_t bNrChannels() const;

            /**
             * @brief bmChannelConfig
             * @return bitmap describing the spatial location of the logical channels
             * in the audio channel cluster of the Processing Unit.
             * @see UsbAudioControlInterfaceDescriptor::parseBmChannelConfig()
             */
            uint16_t bmChannelConfig() const;

            /**
             * @brief iChannelNames
             * @return string descriptor describing the name of the first logical channel
             * in the audio channel cluster of the Processing Unit.
             */
            uint8_t iChannelNames() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates which Control are present and
             * what their capabilities are for index-th channel
             * @note
             * D0..1    Enable Control
             * D2..15   Process-specific allocation
             * @see __parseBmControls() of derived class
             */
            uint16_t bmControls() const;

            /**
             * @brief iProcessing
             * @return string descriptor describing this Processing Unit
             */
            uint8_t iProcessing() const;

            /**
             * @brief strWProcessType
             * @param wProcessType
             * @return string description of wProcessType
             */
            static QString strWProcessType(uint16_t wProcessType);

        protected:
            explicit UsbProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                 uint8_t descPos);
            inline void __parseBase(UsbHtmlBuilder &builder) const;

            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             */
            virtual QString __parseBmControls() const = 0;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNrInPins, _bNrChannels, _iChannelNames, _iProcessing;
            uint16_t _wProcessType, _bmControls;
            uint32_t _bmChannelConfig;
            QVector<uint8_t> _baSourceID;

        private:
            /**
             * @brief get
             * Get the Processing Unit Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * Processing Unit Descriptor's position at extra data
             * @return pointer to the instance of Processing Unit Descriptor,
             * nullptr if parse failed.
             */
            static UsbProcessingUnitDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                    uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
        };

        /**
         * @brief The UsbUpDownMixProcessingUnitDescriptor class
         * Up/Down-mix Processing Unit Descriptor C++ wrapper
         */
        class UsbUpDownMixProcessingUnitDescriptor : public UsbProcessingUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bNrModes
             * @return number of modes supported by this Processing Unit
             */
            uint8_t bNrModes() const;

            /**
             * @brief daModes
             * @param index
             * @note
             * The Spec defines the first daModes is daModes(1),
             * therefore the parameter 'index' should also start with 1.
             * @return bitmap describing the active logical channels in index-th mode
             * @note daModes is the subset of bmChannelConfig
             * @see UsbProcessingUnitDescriptor::bmChannelConfig()
             * @see UsbAudioControlInterfaceDescriptor::parseBmChannelConfig()
             */
            uint32_t daModes(uint8_t index) const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbUpDownMixProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                          uint8_t descPos);
            friend UsbProcessingUnitDescriptor;

            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             * @note
             * D0..1    Enable Control
             * D2..3    Mode Select Control
             * D4..5    Cluster Control
             * D6..7    Underflow Control
             * D8..9    Overflow Control
             * D10..15  Reserved
             */
            QString __parseBmControls() const override;

            uint8_t _bNrModes;
            QVector<uint32_t> _daModes;
        };

        /**
         * @brief The UsbDolbyPrologicProcessingUnitDescriptor class
         * Dolby Prologic Processing Unit Descriptor C++ wrapper
         */
        class UsbDolbyPrologicProcessingUnitDescriptor : public UsbProcessingUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bNrModes
             * @return number of modes supported by this Processing Unit
             */
            uint8_t bNrModes() const;

            /**
             * @brief daModes
             * @param index
             * @note
             * The Spec defines the first daModes is daModes(1),
             * therefore the parameter 'index' should also start with 1.
             * @return bitmap describing the active logical channels in index-th mode
             * @note daModes is the subset of bmChannelConfig
             * @see UsbProcessingUnitDescriptor::bmChannelConfig()
             * @see UsbAudioControlInterfaceDescriptor::parseBmChannelConfig()
             */
            uint32_t daModes(uint8_t index) const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbDolbyPrologicProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                              uint8_t descPos);
            friend UsbProcessingUnitDescriptor;


            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             * @note
             * D0..1    Enable Control
             * D2..3    Mode Select Control
             * D4..5    Cluster Control
             * D6..7    Underflow Control
             * D8..9    Overflow Control
             * D10..15  Reserved
             */
            QString __parseBmControls() const override;

            uint8_t _bNrModes;
            QVector<uint32_t> _daModes;
        };

        /**
         * @brief The StereoExtenderProcessingUnitDescriptor class
         *  Stereo Extender Processing Unit Descriptor C++ wrapper
         */
        class  StereoExtenderProcessingUnitDescriptor : public UsbProcessingUnitDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit StereoExtenderProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                            uint8_t descPos);
            friend UsbProcessingUnitDescriptor;


            /**
             * @brief __parseBmControls
             * @return string description of bmControls
             * @note
             * D0..1    Enable Control
             * D2..3    Width Control
             * D4..5    Cluster Control
             * D6..7    Underflow Control
             * D8..9    Overflow Control
             * D10..15  Reserved
             */
            QString __parseBmControls() const override;
        };

        /**
         * @brief The UsbExtensionUnitDescriptor class
         * Extension Unit Descriptor C++ wrapper
         */
        class UsbExtensionUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0x9 here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief wExtensionCode
             * @return vendor-specific code identifying the Extension Unit
             */
            uint16_t wExtensionCode() const;

            /**
             * @brief bNrInPins
             * @return number of Input Pins of this Unit
             */
            uint8_t bNrInPins() const;

            /**
             * @brief baSourceID
             * @param index
             * @note
             * The Spec defines the first baSourceID is baSourceID(1),
             * therefore the parameter 'index' should also start with 1.
             * @return ID of the Unit or Terminal to which the index-th Input Pin
             * of this Extension Unit is connected.
             */
            uint8_t baSourceID(uint8_t index) const;

            /**
             * @brief bNrChannels
             * @return number of logical output channels in the
             * audio channel cluster of the Extension Unit.
             */
            uint8_t bNrChannels() const;

            /**
             * @brief bmChannelConfig
             * @return bitmap describing the spatial location of the logical channels
             * in the audio channel cluster of the Processing Unit.
             * @see UsbAudioControlInterfaceDescriptor::parseBmChannelConfig()
             */
            uint32_t bmChannelConfig() const;

            /**
             * @brief iChannelNames
             * @return string descriptor describing the name of the first logical channel
             * in the audio channel cluster of the Extension Unit.
             */
            uint8_t iChannelNames() const;

            /**
             * @brief bmControls
             * @return bitmap indicating which Controls are present and what their capabilities are
             * @note
             * D0..1    Enable Control
             * D2..3    Cluster Control
             * D4..5    Underflow Control
             * D6..7    Overflow Control
             */
            uint8_t bmControls() const;

            /**
             * @brief iExtension
             * @return string descriptor describing this Extension Unit
             */
            uint8_t iExtension() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbExtensionUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNrInPins, _bNrChannels, _iChannelNames, _bmControls, _iExtension;
            uint16_t _wExtensionCode;
            QVector<uint8_t> _baSourceID;
            uint32_t _bmChannelConfig;
        };

        /**
         * @brief The UsbClockSourceDescriptor class
         * Clock Source Descriptor C++ wrapper
         */
        class UsbClockSourceDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0xA here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bClockID
             * @return constant uniquely identifying the Clock Source Entity
             * within the audio function.
             */
            uint8_t bClockID() const;

            /**
             * @brief bmAttributes
             * @return bitmap indicating the Clock Type and whether an internal clock
             * is free running or synchronized to the Start of Frame.
             * @note
             * D0..1    Clock Type
             * D2       Clock synchronized to SOF
             * D3..7    Reserved
             */
            uint8_t bmAttributes() const;

            /**
             * @brief bmControls
             * @return bitmap indicating which Controls are present and what their capabilities are
             * @note
             * D0..1    Clock Frequency Control
             * D2..3    Clock Validity Control
             * D4..7    Reversed
             */
            uint8_t bmControls() const;

            /**
             * @brief bAssocTerminal
             * @return Terminal ID of the Terminal that is associated with this Clock Source
             */
            uint8_t bAssocTerminal() const;

            /**
             * @brief iClockSource
             * @return string descriptor describing the Clock Source Entity
             */
            uint8_t iClockSource() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbClockSourceDescriptor(UsbInterfaceDescriptor *parent,
                                              uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmAttributes() const;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bClockID,
            _bmAttributes, _bmControls, _bAssocTerminal, _iClockSource;
        };

        /**
         * @brief The UsbClockSelectorDescriptor class
         * Clock Selector Descriptor C++ wrapper
         */
        class UsbClockSelectorDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0xB here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bClockID
             * @return constant uniquely identifying the Clock Source Entity
             * within the audio function.
             */
            uint8_t bClockID() const;

            /**
             * @brief bNrInPins
             * @return number of Input Pins of this Unit
             */
            uint8_t bNrInPins() const;

            /**
             * @brief baCSourceID
             * @param index
             * @note
             * The Spec defines the first baCSourceID is baCSourceID(1),
             * therefore the parameter 'index' should also start with 1.
             * @return ID of the Clock Entity to which the index-th Clock Input Pin
             * of this Clock Selector Entity is connected.
             */
            uint8_t baCSourceID(uint8_t index) const;

            /**
             * @brief bmControls
             * @return bitmap indicating which Controls are present and what their capabilities are
             * @note
             * D0..1    Clock Selector Control
             * D2..7    Reversed
             */
            uint8_t bmControls() const;

            /**
             * @brief iClockSelector
             * @return string descriptor describing the Clock Selector Entity
             */
            uint8_t iClockSelector() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;


        private:
            explicit UsbClockSelectorDescriptor(UsbInterfaceDescriptor *parent,
                                                uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bClockID,
            _bNrInPins, _bmControls, _iClockSelector;
            QVector<uint8_t> _baCSourceID;
        };

        /**
         * @brief The UsbClockMultiplierDescriptor class
         * Clock Multiplier Descriptor C++ wrapper
         */
        class UsbClockMultiplierDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0xC here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bClockID
             * @return constant uniquely identifying the Clock Source Entity
             * within the audio function.
             */
            uint8_t bClockID() const;

            /**
             * @brief bCSourceID
             * @return ID of the Clock Entity to which the Clock Input Pin
             * of this Clock Selector Entity is connected.
             */
            uint8_t bCSourceID() const;

            /**
             * @brief bmControls
             * @return bitmap indicating which Controls are present and what their capabilities are
             * @note
             * D0..1    Clock Numerator Control
             * D2..3    Clock Denominator Control
             * D4..7    Reversed
             */
            uint8_t bmControls() const;

            /**
             * @brief iClockMultiplier
             * @return string descriptor describing the Clock Selector Entity
             */
            uint8_t iClockMultiplier() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;


        private:
            explicit UsbClockMultiplierDescriptor(UsbInterfaceDescriptor *parent,
                                                  uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bClockID,
            _bCSourceID, _bmControls, _iClockMultiplier;
        };

        /**
         * @brief The UsbSamplingRateConverterUnitDescriptor class
         * Sampling Rate Converter Unit Descriptor C++ wrapper
         */
        class UsbSamplingRateConverterUnitDescriptor : public UsbAudioControlInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bLength
             * @return the total size of the descriptor
             */
            uint8_t bLength() const override;

            /**
             * @brief bDescriptorType
             * @return descriptor type, 0x24 here
             */
            uint8_t bDescriptorType() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype, 0xD here
             * @see UsbAudioControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant uniquely identifying the Unit within the audio function
             */
            uint8_t bUnitID() const;

            /**
             * @brief bSourceID
             * @return ID of the Unit or Terminal to which this SRC Unit is connected
             */
            uint8_t bSourceID() const;

            /**
             * @brief bCSourceID
             * @return ID of the Clock Entity to which this SRC Unit input section is connected
             */
            uint8_t bCSourceID() const;

            /**
             * @brief bCSourceOutID
             * @return ID of the Clock Entity to which this SRC Unit output section is connected
             */
            uint8_t bCSourceOutID() const;

            /**
             * @brief iSRC
             * @return string descriptor describing the SRC Unit
             */
            uint8_t iSRC() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbSamplingRateConverterUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                            uint8_t descPos);
            friend UsbAudioControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bSourceID, _bCSourceID, _bCSourceOutID, _iSRC;
        };
    } // namespace uac2
} // namespace usb

#ifndef USBINTERFACEDESCRIPTOR_H
#include "usbinterfacedescriptor.h"
#endif

#endif // USBAUDIOCONTROLINTERFACEDESCRIPTOR_H
