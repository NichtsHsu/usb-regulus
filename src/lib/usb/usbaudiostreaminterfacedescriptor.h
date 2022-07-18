/*! C++ class wrapper of USB AudioStream Interface Desciptors

 * Copyright (C) 2022 Nichts Hsu

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

#ifndef USBAUDIOSTREAMINTERFACEDESCRIPTOR_H
#define USBAUDIOSTREAMINTERFACEDESCRIPTOR_H

#include <QObject>
#include <QVector>
#include <QByteArray>
#include "usbinterfaceextradescriptor.h"

namespace usb {
    class UsbInterfaceDescriptor;
}

namespace usb {
    /**
     * @brief The UsbAudioStreamInterfaceDescriptorBasic class
     * AudioStream Interface Descriptor basic class, to indicate the Audio spec version
     */
    class UsbAudioStreamInterfaceDescriptorBasic : public usb::UsbInterfaceExtraDescriptor
    {
        Q_OBJECT
    public:
        UsbAudioStreamInterfaceDescriptorBasic(UsbInterfaceDescriptor *parent);

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
         * @brief The UsbAudioStreamInterfaceDescriptor class
         * AudioStream Interface Descriptor C++ wrapper
         */
        class UsbAudioStreamInterfaceDescriptor : public UsbAudioStreamInterfaceDescriptorBasic
        {
            Q_OBJECT
        public:
            explicit UsbAudioStreamInterfaceDescriptor(UsbInterfaceDescriptor *parent);

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            virtual uint8_t bDescriptorSubtype() const = 0;

            /**
             * @brief specVersion
             * @return the version of Audio Spec. Always be 1 here.
             */
            uint8_t specVersion() const override;

            /**
             * @brief strSubtype
             * @param subtype
             * @return string description of subtype
             */
            static QString strSubtype(uint8_t subtype);

            /**
             * @brief strWFormatTag
             * @param wFormatTag
             * @return string description of wFormatTag
             */
            static QString strWFormatTag(uint16_t wFormatTag);

            /**
             * @brief get
             * Get the AudioStream Interface Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * AudioStream Interface Descriptor's position at extra data
             * @return pointer to the instance of AudioStream Interface Descriptor,
             * nullptr if parse failed.
             */
            static UsbAudioStreamInterfaceDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                          uint8_t descPos);
        };

        /**
         * @brief The UsbClassSpecificASInterfaceDescriptor class
         * Class-Specific AS Interface Descriptor C++ wrapper
         */
        class UsbClassSpecificASInterfaceDescriptor : public UsbAudioStreamInterfaceDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bTerminalLink
             * @return the Terminal ID of the Terminal to which the endpoint of this interface is connected
             */
            uint8_t bTerminalLink() const;

            /**
             * @brief bDelay
             * @return Delay introduced by the data path
             */
            uint8_t bDelay() const;

            /**
             * @brief wFormatTag
             * @return the Audio Data Format that has to be used to communicate with this interface
             * @see UsbAudioStreamInterfaceDescriptor::strAudioDataFormatType()
             */
            uint16_t wFormatTag() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbClassSpecificASInterfaceDescriptor(UsbInterfaceDescriptor *parent,
                                                  uint8_t descPos);
            friend UsbAudioStreamInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bTerminalLink, _bDelay;
            uint16_t _wFormatTag;
        };

        /**
         * @brief The UsbFormatTypeDescriptor class
         * Format Type Descriptor C++ wrapper
         */
        class UsbFormatTypeDescriptor : public UsbAudioStreamInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             * @see UsbFormatTypeDescriptor::strBFormatType()
             */
            virtual uint8_t bFormatType() const = 0;

            /**
             * @brief strBFormatType
             * @param bFormatType
             * @return string description of bFormatType
             */
            static QString strBFormatType(uint8_t bFormatType);

        protected:
            UsbFormatTypeDescriptor(UsbInterfaceDescriptor *parent);

        private:
            friend UsbAudioStreamInterfaceDescriptor;

            /**
             * @brief get
             * Get the Format Type Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * Format Type Descriptor's position at extra data
             * @return pointer to the instance of Format Type Descriptor,
             * nullptr if parse failed.
             */
            static UsbFormatTypeDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                uint8_t descPos);
        };

        /**
         * @brief The UsbTypeIFormatTypeDescriptor class
         * Type I Format Type Descriptor
         */
        class UsbTypeIFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief bNrChannels
             * @return the number of physical channels in the audio data stream
             */
            uint8_t bNrChannels() const;

            /**
             * @brief bSubframeSize
             * @return the number of bytes occupied by one audio subframe
             */
            uint8_t bSubframeSize() const;

            /**
             * @brief bBitResolution
             * @return the number of effectively used bits from the available bits in an audio subframe
             */
            uint8_t bBitResolution() const;

            /**
             * @brief bSamFreqType
             * @return how the sampling frequency can be programmed
             */
            uint8_t bSamFreqType() const;

            /**
             * @brief tLowerSamFreq
             * @warning
             * Only valid when bSamFreqType is 0
             * @return lower bound in Hz of the sampling frequency range for this isochronous data endpoint
             */
            QByteArray tLowerSamFreq() const;

            /**
             * @brief tUpperSamFreq
             * @warning
             * Only valid when bSamFreqType is 0
             * @return upper bound in Hz of the sampling frequency range for this isochronous data endpoint
             */
            QByteArray tUpperSamFreq() const;

            /**
             * @brief tSamFreq
             * @warning
             * Only valid when bSamFreqType is NOT 0
             * @param index
             * @note
             * The Spec defines the first tSamFreq is tSamFreq(1),
             * therefore the parameter 'index' should also start with 1.
             * @return index-th sampling frequency in Hz for this isochronous data endpoint
             */
            QByteArray tSamFreq(uint8_t index) const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbTypeIFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                         uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType,
            _bNrChannels, _bSubframeSize, _bBitResolution, _bSamFreqType;
            QVector<QByteArray> _tSamFreq;
        };

        /**
         * @brief The UsbTypeIIFormatTypeDescriptor class
         * Type II Format Type Descriptor
         */
        class UsbTypeIIFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief wMaxBitRate
             * @return the maximum number of bits per second this interface can handle
             */
            uint16_t wMaxBitRate() const;

            /**
             * @brief wSamplesPerFrame
             * @return the number of PCM audio samples contained in one encoded audio frame
             */
            uint16_t wSamplesPerFrame() const;

            /**
             * @brief bSamFreqType
             * @return how the sampling frequency can be programmed
             */
            uint8_t bSamFreqType() const;

            /**
             * @brief tLowerSamFreq
             * @warning
             * Only valid when bSamFreqType is 0
             * @return lower bound in Hz of the sampling frequency range for this isochronous data endpoint
             */
            QByteArray tLowerSamFreq() const;

            /**
             * @brief tUpperSamFreq
             * @warning
             * Only valid when bSamFreqType is 0
             * @return upper bound in Hz of the sampling frequency range for this isochronous data endpoint
             */
            QByteArray tUpperSamFreq() const;

            /**
             * @brief tSamFreq
             * @warning
             * Only valid when bSamFreqType is NOT 0
             * @param index
             * @note
             * The Spec defines the first tSamFreq is tSamFreq(1),
             * therefore the parameter 'index' should also start with 1.
             * @return index-th sampling frequency in Hz for this isochronous data endpoint
             */
            QByteArray tSamFreq(uint8_t index) const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbTypeIIFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                          uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType, _bSamFreqType;
            uint16_t _wMaxBitRate, _wSamplesPerFrame;
            QVector<QByteArray> _tSamFreq;
        };

        /**
         * @brief The UsbTypeIIIFormatTypeDescriptor class
         * Type III Format Type Descriptor
         */
        class UsbTypeIIIFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief bNrChannels
             * @return the number of physical channels in the audio data stream
             */
            uint8_t bNrChannels() const;

            /**
             * @brief bSubframeSize
             * @return the number of bytes occupied by one audio subframe
             */
            uint8_t bSubframeSize() const;

            /**
             * @brief bBitResolution
             * @return the number of effectively used bits from the available bits in an audio subframe
             */
            uint8_t bBitResolution() const;

            /**
             * @brief bSamFreqType
             * @return how the sampling frequency can be programmed
             */
            uint8_t bSamFreqType() const;

            /**
             * @brief tLowerSamFreq
             * @warning
             * Only valid when bSamFreqType is 0
             * @return lower bound in Hz of the sampling frequency range for this isochronous data endpoint
             */
            QByteArray tLowerSamFreq() const;

            /**
             * @brief tUpperSamFreq
             * @warning
             * Only valid when bSamFreqType is 0
             * @return upper bound in Hz of the sampling frequency range for this isochronous data endpoint
             */
            QByteArray tUpperSamFreq() const;

            /**
             * @brief tSamFreq
             * @warning
             * Only valid when bSamFreqType is NOT 0
             * @param index
             * @note
             * The Spec defines the first tSamFreq is tSamFreq(1),
             * therefore the parameter 'index' should also start with 1.
             * @return index-th sampling frequency in Hz for this isochronous data endpoint
             */
            QByteArray tSamFreq(uint8_t index) const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbTypeIIIFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                           uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType,
            _bNrChannels, _bSubframeSize, _bBitResolution, _bSamFreqType;
            QVector<QByteArray> _tSamFreq;
        };

        /**
         * @brief The UsbFormatSpecificDescriptor class
         * Format-Specific Descriptor C++ wrapper
         */
        class UsbFormatSpecificDescriptor : public UsbAudioStreamInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief wFormatTag
             * @return constant identifying the precise format the AudioStreaming interface is using
             * @see UsbAudioStreamInterfaceDescriptor::strWFormatTag()
             */
            virtual uint16_t wFormatTag() const = 0;

        protected:
            UsbFormatSpecificDescriptor(UsbInterfaceDescriptor *parent);

        private:
            friend UsbAudioStreamInterfaceDescriptor;

            /**
             * @brief get
             * Get the Format-Specific Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * Format-Specific Descriptor's position at extra data
             * @return pointer to the instance of Format Type Descriptor,
             * nullptr if parse failed.
             */
            static UsbFormatSpecificDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                    uint8_t descPos);
        };

        /**
         * @brief The UsbMpegFormatSpecificDescriptor class
         * MPEG Format-Specific Descriptor C++ wrapper
         */
        class UsbMpegFormatSpecificDescriptor : public UsbFormatSpecificDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief wFormatTag
             * @return constant identifying the precise format the AudioStreaming interface is using
             * @see UsbAudioStreamInterfaceDescriptor::strWFormatTag()
             */
            uint16_t wFormatTag() const override;

            /**
             * @brief bmMPEGCapabilities
             * @return bitmap identifying the MPEG capabilities of the decoder
             * @note
             * D0..2    Layer support
             * D3       MPEG-1 only
             * D4       MPEG-1 dual-channel
             * D5       MPEG-2 second stereo
             * D6       MPEG-2 7.1 channel augmentation
             * D7       Adaptive multi-channel prediction
             * D8..9    MPEG-2 multilingual support
             * D10..15  Reserved
             */
            uint16_t bmMPEGCapabilities() const;

            /**
             * @brief bmMPEGFeatures
             * @return bitmap identifying the features the decoder supports
             * @note
             * D0..3    Reserved
             * D4..5    Internal Dynamic Range Control
             * D6..7    Reserved
             */
            uint8_t bmMPEGFeatures() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbMpegFormatSpecificDescriptor(UsbInterfaceDescriptor *parent,
                                            uint8_t descPos);
            friend UsbFormatSpecificDescriptor;
            inline QString __parseBmMPEGCapabilities() const;
            inline QString __parseBmMPEGFeatures() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bmMPEGFeatures;
            uint16_t _wFormatTag, _bmMPEGCapabilities;
        };

        class UsbAc3FormatSpecificDescriptor : public UsbFormatSpecificDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief wFormatTag
             * @return constant identifying the precise format the AudioStreaming interface is using
             * @see UsbAudioStreamInterfaceDescriptor::strWFormatTag()
             */
            uint16_t wFormatTag() const override;

            /**
             * @brief bmBSID
             * @return bitmap that a bit set to 1 indicates that the corresponding BSID mode is supported
             */
            uint32_t bmBSID() const;

            /**
             * @brief bmAC3Features
             * @return bitmap that a bit set to 1 indicates that the mentioned feature is supported
             * @note
             * D0       RF mode
             * D1       Line mode
             * D2       Custom0 mode
             * D3       Custom1 mode
             * D4..5    Internal Dynamic Range Control
             * D6..7    Reserved
             */
            uint8_t bmAC3Features() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbAc3FormatSpecificDescriptor(UsbInterfaceDescriptor *parent,
                                           uint8_t descPos);
            friend UsbFormatSpecificDescriptor;
            inline QString __parseBmAC3Features() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bmAC3Features;
            uint16_t _wFormatTag;
            uint32_t _bmBSID;
        };
    } // namespace uac1

    /* For Audio 2.0 spec */
    namespace uac2 {
        /**
         * @brief The UsbAudioStreamInterfaceDescriptor class
         * AudioStream Interface Descriptor C++ wrapper
         */
        class UsbAudioStreamInterfaceDescriptor : public UsbAudioStreamInterfaceDescriptorBasic
        {
            Q_OBJECT
        public:
            explicit UsbAudioStreamInterfaceDescriptor(UsbInterfaceDescriptor *parent);

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            virtual uint8_t bDescriptorSubtype() const = 0;

            /**
             * @brief specVersion
             * @return the version of Audio Spec. Always be 2 here.
             */
            uint8_t specVersion() const override;

            /**
             * @brief strSubtype
             * @param subtype
             * @return string description of subtype
             */
            static QString strSubtype(uint8_t subtype);

            /**
             * @brief strBFormatType
             * @param bFormatType
             * @return string description of bFormatType
             */
            static QString strBFormatType(uint8_t bFormatType);

            /**
             * @brief parseBmChannelConfig
             * A wrapper of uac2::UsbAudioControlInterfaceDescriptor::parseBmChannelConfig()
             * @see uac2::UsbAudioControlInterfaceDescriptor::parseBmChannelConfig()
             * @param bmChannelConfig
             * @return stringlist of spatial locations
             */
            static inline QStringList parseBmChannelConfig(uint32_t bmChannelConfig);

            /**
             * @brief parseBmFormats
             * @param bFormatType
             * @param bmFormats
             * @return stringlist of supported formats
             */
            static inline QStringList parseBmFormats(uint8_t bFormatType, uint32_t bmFormats);

            /**
             * @brief get
             * Get the AudioStream Interface Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * AudioStream Interface Descriptor's position at extra data
             * @return pointer to the instance of AudioStream Interface Descriptor,
             * nullptr if parse failed.
             */
            static UsbAudioStreamInterfaceDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                          uint8_t descPos);
        };

        /**
         * @brief The UsbClassSpecificASInterfaceDescriptor class
         * Class-Specific AS Interface Descriptor C++ wrapper
         */
        class UsbClassSpecificASInterfaceDescriptor : public UsbAudioStreamInterfaceDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bTerminalLink
             * @return the Terminal ID of the Terminal to which this interface is connected
             */
            uint8_t bTerminalLink() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates which Control are present and
             * what their capabilities are for index-th channel
             * @note
             * D0..1    Active Alternate Setting Control
             * D2..3    Valid Alternate Settings Control
             * D4..7    Reserved
             */
            uint8_t bmControls() const;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             * @see UsbAudioStreamInterfaceDescriptor::strBFormatType()
             */
            uint8_t bFormatType() const;

            /**
             * @brief bmFormats
             * @return the Audio Data Format(s) that can be used to communicate with this interface
             * @see UsbAudioStreamInterfaceDescriptor::parseBmFormats()
             */
            uint32_t bmFormats() const;

            /**
             * @brief bNrChannels
             * @return number of physical channels in the AS Interface audio channel cluster
             */
            uint8_t bNrChannels() const;

            /**
             * @brief bmChannelConfig
             * @return bitmap describing the spatial location of the physical channels
             * @see UsbAudioStreamInterfaceDescriptor::parseBmChannelConfig()
             */
            uint32_t bmChannelConfig() const;

            /**
             * @brief iChannelNames
             * @return string descriptor describing the name of the first physical channel
             */
            uint8_t iChannelNames() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbClassSpecificASInterfaceDescriptor(UsbInterfaceDescriptor *parent,
                                                  uint8_t descPos);
            friend UsbAudioStreamInterfaceDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bTerminalLink,
            _bmControls, _bFormatType, _bNrChannels, _iChannelNames;
            uint32_t _bmFormats, _bmChannelConfig;
        };


        /**
         * @brief The UsbFormatTypeDescriptor class
         * Format Type Descriptor C++ wrapper
         */
        class UsbFormatTypeDescriptor : public UsbAudioStreamInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             * @see UsbAudioStreamInterfaceDescriptor::strBFormatType()
             */
            virtual uint8_t bFormatType() const = 0;

            /**
             * @brief strBSideBandProtocol
             * @param bSideBandProtocol
             * @return string description of bSideBandProtocol
             */
            static QString strBSideBandProtocol(uint8_t bSideBandProtocol);

        protected:
            UsbFormatTypeDescriptor(UsbInterfaceDescriptor *parent);

        private:
            friend UsbAudioStreamInterfaceDescriptor;

            /**
             * @brief get
             * Get the Format Type Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * Format Type Descriptor's position at extra data
             * @return pointer to the instance of Format Type Descriptor,
             * nullptr if parse failed.
             */
            static UsbFormatTypeDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                uint8_t descPos);
        };

        /**
         * @brief The UsbTypeIFormatTypeDescriptor class
         * Type I Format Type Descriptor
         */
        class UsbTypeIFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief bSubslotSize
             * @return the number of bytes occupied by one audio subslot
             */
            uint8_t bSubslotSize() const;

            /**
             * @brief bBitResolution
             * @return the number of effectively used bits from the available bits in an audio subslot
             */
            uint8_t bBitResolution() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbTypeIFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                         uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType,
            _bSubslotSize, _bBitResolution;
        };

        /**
         * @brief The UsbTypeIIFormatTypeDescriptor class
         * Type II Format Type Descriptor
         */
        class UsbTypeIIFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief wMaxBitRate
             * @return the maximum number of bits per second this interface can handle
             */
            uint16_t wMaxBitRate() const;

            /**
             * @brief wSlotsPerFrame
             * @return the number of PCM audio slots contained in one encoded audio frame
             */
            uint16_t wSlotsPerFrame() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbTypeIIFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                          uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType;
            uint16_t _wMaxBitRate, _wSlotsPerFrame;
        };

        /**
         * @brief The UsbTypeIIIFormatTypeDescriptor class
         * Type III Format Type Descriptor
         */
        class UsbTypeIIIFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief bSubslotSize
             * @return the number of bytes occupied by one audio subslot
             */
            uint8_t bSubslotSize() const;

            /**
             * @brief bBitResolution
             * @return the number of effectively used bits from the available bits in an audio subslot
             */
            uint8_t bBitResolution() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbTypeIIIFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                           uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType,
            _bSubslotSize, _bBitResolution;
        };

        /**
         * @brief The UsbTypeIVFormatTypeDescriptor class
         * Type IV Format Type Descriptor
         */
        class UsbTypeIVFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbTypeIVFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                          uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType;
        };

        /**
         * @brief The UsbExtendedTypeIFormatTypeDescriptor class
         * Extended Type I Format Type Descriptor
         */
        class UsbExtendedTypeIFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief bSubslotSize
             * @return the number of bytes occupied by one audio subslot
             */
            uint8_t bSubslotSize() const;

            /**
             * @brief bBitResolution
             * @return the number of effectively used bits from the available bits in an audio subslot
             */
            uint8_t bBitResolution() const;

            /**
             * @brief bHeaderLength
             * @return size of the Packet Header
             */
            uint8_t bHeaderLength() const;

            /**
             * @brief bControlSize
             * @return size of the Control Channel Words
             */
            uint8_t bControlSize() const;

            /**
             * @brief bSideBandProtocol
             * @return constant identifying the Side Band Protocol used for
             * the Packet Header and Control Channel content.
             * @see UsbFormatTypeDescriptor::strBSideBandProtocol()
             */
            uint8_t bSideBandProtocol() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbExtendedTypeIFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                                 uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType,
            _bSubslotSize, _bBitResolution, _bHeaderLength, _bControlSize, _bSideBandProtocol;
        };

        /**
         * @brief The UsbExtendedTypeIIFormatTypeDescriptor class
         * Extended Type II Format Type Descriptor
         */
        class UsbExtendedTypeIIFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief wMaxBitRate
             * @return the maximum number of bits per second this interface can handle
             */
            uint16_t wMaxBitRate() const;

            /**
             * @brief wSamplesPerFrame
             * @return the number of PCM audio samples contained in one encoded audio frame
             */
            uint16_t wSamplesPerFrame() const;

            /**
             * @brief bHeaderLength
             * @return size of the Packet Header
             */
            uint8_t bHeaderLength() const;

            /**
             * @brief bSideBandProtocol
             * @return constant identifying the Side Band Protocol used for the Packet Header content
             */
            uint8_t bSideBandProtocol() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbExtendedTypeIIFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                                  uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType,
            _bHeaderLength, _bSideBandProtocol;
            uint16_t _wMaxBitRate, _wSamplesPerFrame;
        };

        /**
         * @brief The UsbExtendedTypeIIIFormatTypeDescriptor class
         * Extended Type III Format Type Descriptor
         */
        class UsbExtendedTypeIIIFormatTypeDescriptor : public UsbFormatTypeDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatType
             * @return constant identifying the Format Type the AudioStreaming interface is using
             */
            uint8_t bFormatType() const override;

            /**
             * @brief bSubslotSize
             * @return the number of bytes occupied by one audio subslot
             */
            uint8_t bSubslotSize() const;

            /**
             * @brief bBitResolution
             * @return the number of effectively used bits from the available bits in an audio subslot
             */
            uint8_t bBitResolution() const;

            /**
             * @brief bHeaderLength
             * @return size of the Packet Header
             */
            uint8_t bHeaderLength() const;

            /**
             * @brief bSideBandProtocol
             * @return constant identifying the Side Band Protocol used for the Packet Header content
             */
            uint8_t bSideBandProtocol() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbExtendedTypeIIIFormatTypeDescriptor(UsbInterfaceDescriptor *parent,
                                                   uint8_t descPos);
            friend UsbFormatTypeDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatType,
            _bSubslotSize, _bBitResolution, _bHeaderLength, _bSideBandProtocol;
        };

        class UsbEncoderDescriptor : public UsbAudioStreamInterfaceDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bEncoderID
             * @return constant uniquely identifying the encoder within the interface
             */
            uint8_t bEncoderID() const;

            /**
             * @brief bEncoder
             * @return constant identifying the encoder
             * @see UsbEncoderDescriptor::strBEncoder()
             */
            uint8_t bEncoder() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates which Control are present and
             * what their capabilities are for index-th channel
             * @note
             * D0..1    Bit Rate Control
             * D2..3    Quality Control
             * D4..5    VBR Control
             * D6..7    Type Control
             * D8..9    Underflow Control
             * D10..11  Overflow Control
             * D12..13  Encoder Error Control
             * D14..15  Param1 Control
             * D16..17  Param2 Control
             * D18..19  Param3 Control
             * D20..21  Param4 Control
             * D22..23  Param5 Control
             * D24..25  Param6 Control
             * D26..27  Param7 Control
             * D28..29  Param8 Control
             * D30..31  Reserved
             */
            uint32_t bmControls() const;

            /**
             * @brief iParam1
             * @return string descriptor describing the purpose of Param1
             */
            uint8_t iParam1() const;

            /**
             * @brief iParam2
             * @return string descriptor describing the purpose of Param2
             */
            uint8_t iParam2() const;

            /**
             * @brief iParam3
             * @return string descriptor describing the purpose of Param3
             */
            uint8_t iParam3() const;

            /**
             * @brief iParam4
             * @return string descriptor describing the purpose of Param4
             */
            uint8_t iParam4() const;

            /**
             * @brief iParam5
             * @return string descriptor describing the purpose of Param5
             */
            uint8_t iParam5() const;

            /**
             * @brief iParam6
             * @return string descriptor describing the purpose of Param6
             */
            uint8_t iParam6() const;

            /**
             * @brief iParam7
             * @return string descriptor describing the purpose of Param7
             */
            uint8_t iParam7() const;

            /**
             * @brief iParam8
             * @return string descriptor describing the purpose of Param8
             */
            uint8_t iParam8() const;

            /**
             * @brief iEncoder
             * @return string descriptor describing the encoder
             */
            uint8_t iEncoder() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

            /**
             * @brief strBEncoder
             * @param bEncoder
             * @return string description of bEncoder
             */
            static QString strBEncoder(uint8_t bEncoder);

        private:
            UsbEncoderDescriptor(UsbInterfaceDescriptor *parent,
                                 uint8_t descPos);
            friend UsbAudioStreamInterfaceDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bEncoderID,
            _bEncoder, _iParam1, _iParam2, _iParam3, _iParam4, _iParam5,
            _iParam6, _iParam7, _iParam8, _iEncoder;
            uint32_t _bmControls;
        };

        /**
         * @brief The UsbDecoderDescriptor class
         * Decoder Descriptor C++ wrapper
         */
        class UsbDecoderDescriptor : public UsbAudioStreamInterfaceDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief bDecoderID
             * @return constant uniquely identifying the decoder within the interface
             */
            virtual uint8_t bDecoderID() const = 0;

            /**
             * @brief bDecoder
             * @return constant identifying the decoder
             * @see UsbDecoderDescriptor::strBDecoder();
             */
            virtual uint8_t bDecoder() const = 0;

            /**
             * @brief strBDecoder
             * @param bDecoder
             * @return string description of bDecoder
             */
            static QString strBDecoder(uint8_t bDecoder);

        protected:
            UsbDecoderDescriptor(UsbInterfaceDescriptor *parent);
        private:
            friend UsbAudioStreamInterfaceDescriptor;

            /**
             * @brief get
             * Get the Decoder Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * Decoder Descriptor's position at extra data
             * @return pointer to the instance of Decoder Descriptor,
             * nullptr if parse failed.
             */
            static UsbDecoderDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                             uint8_t descPos);
        };

        /**
         * @brief The UsbMpegDecoderDescriptor class
         * MPEG Decoder Descriptor
         */
        class UsbMpegDecoderDescriptor : public UsbDecoderDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bDecoderID
             * @return constant uniquely identifying the decoder within the interface
             */
            uint8_t bDecoderID() const override;

            /**
             * @brief bDecoder
             * @return constant identifying the decoder
             * @see UsbDecoderDescriptor::strBDecoder();
             */
            uint8_t bDecoder() const override;

            /**
             * @brief bmMPEGCapabilities
             * @return bitmap identifying the MPEG capabilities of the decoder
             * @note
             * D0..2    Layer support
             * D3       MPEG-1 only
             * D4       MPEG-1 dual-channel
             * D5       MPEG-2 second stereo
             * D6       MPEG-2 7.1 channel augmentation
             * D7       Adaptive multi-channel prediction
             * D8..9    MPEG-2 multilingual support
             * D10..15  Reserved
             */
            uint16_t bmMPEGCapabilities() const;

            /**
             * @brief bmMPEGFeatures
             * @return bitmap identifying the features the decoder supports
             * @note
             * D0..3    Reserved
             * D4..5    Internal Dynamic Range Control
             * D6..7    Reserved
             */
            uint8_t bmMPEGFeatures() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates which Control are present and
             * what their capabilities are for index-th channel
             * @note
             * D0..1    Underflow Control
             * D2..3    Overflow Control
             * D4..5    Decoder Error Control
             * D6..7    Reserved
             */
            uint8_t bmControls() const;

            /**
             * @brief iDecoder
             * @return string descriptor describing the decoder
             */
            uint8_t iDecoder() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbMpegDecoderDescriptor(UsbInterfaceDescriptor *parent,
                                     uint8_t descPos);
            friend UsbDecoderDescriptor;
            inline QString __parseBmMPEGCapabilities() const;
            inline QString __parseBmMPEGFeatures() const;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bDecoderID,
            _bDecoder, _bmMPEGFeatures, _bmControls, _iDecoder;
            uint16_t _bmMPEGCapabilities;
        };

        /**
         * @brief The UsbAc3DecoderDescriptor class
         * AC-3 Decoder Descriptor C++ wrapper
         */
        class UsbAc3DecoderDescriptor : public UsbDecoderDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bDecoderID
             * @return constant uniquely identifying the decoder within the interface
             */
            uint8_t bDecoderID() const override;

            /**
             * @brief bDecoder
             * @return constant identifying the decoder
             * @see UsbDecoderDescriptor::strBDecoder();
             */
            uint8_t bDecoder() const override;

            /**
             * @brief bmBSID
             * @return bitmap that a bit set to 1 indicates that the corresponding BSID mode is supported
             */
            uint32_t bmBSID() const;

            /**
             * @brief bmAC3Features
             * @return bitmap that a bit set to 1 indicates that the mentioned feature is supported
             * @note
             * D0       RF mode
             * D1       Line mode
             * D2       Custom0 mode
             * D3       Custom1 mode
             * D4..5    Internal Dynamic Range Control
             * D6..7    Reserved
             */
            uint8_t bmAC3Features() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates which Control are present and
             * what their capabilities are for index-th channel
             * @note
             * D0..1    Underflow Control
             * D2..3    Overflow Control
             * D4..5    Decoder Error Control
             * D6..7    Reserved
             */
            uint8_t bmControls() const;

            /**
             * @brief iDecoder
             * @return string descriptor describing the decoder
             */
            uint8_t iDecoder() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbAc3DecoderDescriptor(UsbInterfaceDescriptor *parent,
                                    uint8_t descPos);
            friend UsbDecoderDescriptor;
            inline QString __parseBmAC3Features() const;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bDecoderID,
            _bDecoder, _bmAC3Features, _bmControls, _iDecoder;
            uint32_t _bmBSID;
        };

        /**
         * @brief The UsbWmaDecoderDescriptor class
         * WMA Decoder Descriptor C++ Wrapper
         */
        class UsbWmaDecoderDescriptor : public UsbDecoderDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bDecoderID
             * @return constant uniquely identifying the decoder within the interface
             */
            uint8_t bDecoderID() const override;

            /**
             * @brief bDecoder
             * @return constant identifying the decoder
             * @see UsbDecoderDescriptor::strBDecoder();
             */
            uint8_t bDecoder() const override;

            /**
             * @brief bmWMAProfile
             * @return bitmap that a bit set to 1 indicates that the mentioned feature is supported
             * @note
             * D0       WMA Profile 1, L1
             * D1       WMA Profile 2, L2
             * D2       WMA Profile 3, L3
             * D3       WMA Profile Other, L
             * D4       WMA Speech 1, S1
             * D5       WMA Speech 2, S2
             * D6       WMAPro Profile 1, M1
             * D7       WMAPro Profile 2, M2
             * D8       WMAPro Profile 3, M3
             * D9       WMAPro Profile Other, M
             * D10      WMA lossless decoding
             * D11..15  Reserved
             */
            uint16_t bmWMAProfile() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates which Control are present and
             * what their capabilities are for index-th channel
             * @note
             * D0..1    Underflow Control
             * D2..3    Overflow Control
             * D4..5    Decoder Error Control
             * D6..7    Reserved
             */
            uint8_t bmControls() const;

            /**
             * @brief iDecoder
             * @return string descriptor describing the decoder
             */
            uint8_t iDecoder() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbWmaDecoderDescriptor(UsbInterfaceDescriptor *parent,
                                    uint8_t descPos);
            friend UsbDecoderDescriptor;
            inline QString __parseBmWMAProfile() const;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bDecoderID,
            _bDecoder, _bmControls, _iDecoder;
            uint16_t _bmWMAProfile;
        };

        class UsbDtsDecoderDescriptor : public UsbDecoderDescriptor
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
             * @see UsbAudioStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bDecoderID
             * @return constant uniquely identifying the decoder within the interface
             */
            uint8_t bDecoderID() const override;

            /**
             * @brief bDecoder
             * @return constant identifying the decoder
             * @see UsbDecoderDescriptor::strBDecoder();
             */
            uint8_t bDecoder() const override;

            /**
             * @brief bmCapabilities
             * @return bitmap that a bit set to 1 indicates that the mentioned feature is supported
             * @note
             * D0       Core
             * D1       Lossless
             * D2       LBR
             * D3       MultipleStreamMixing
             * D4       DualDecode
             * D5..7    Reserved
             */
            uint8_t bmCapabilities() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates which Control are present and
             * what their capabilities are for index-th channel
             * @note
             * D0..1    Underflow Control
             * D2..3    Overflow Control
             * D4..5    Decoder Error Control
             * D6..7    Reserved
             */
            uint8_t bmControls() const;

            /**
             * @brief iDecoder
             * @return string descriptor describing the decoder
             */
            uint8_t iDecoder() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            UsbDtsDecoderDescriptor(UsbInterfaceDescriptor *parent,
                                    uint8_t descPos);
            friend UsbDecoderDescriptor;
            inline QString __parseBmCapabilities() const;
            inline QString __parseBmControls() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bDecoderID,
            _bDecoder, _bmCapabilities, _bmControls, _iDecoder;
        };
    } // namespace uac2
} // namespace usb

#ifndef USBINTERFACEDESCRIPTOR_H
#include "usbinterfacedescriptor.h"
#endif

#endif // USB_USBAUDIOSTREAMINTERFACEDESCRIPTORBASIC_H
