/*! C++ class wrapper of USB VideoControl Interface Descriptors

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

#ifndef USBVIDEOCONTROLINTERFACEDESCRIPTOR_H
#define USBVIDEOCONTROLINTERFACEDESCRIPTOR_H

#include <QObject>
#include <QMap>
#include <QVector>

#include "usbinterfaceextradescriptor.h"
#include "usbhtmlbuilder.h"

namespace usb {
    namespace uvc {
        /**
         * @brief The UsbVideoControlInterfaceDescriptor class
         * VideoControl Interface Descriptor C++ wrapper
         */
        class UsbVideoControlInterfaceDescriptor : public UsbInterfaceExtraDescriptor
        {
            Q_OBJECT
        public:
            UsbVideoControlInterfaceDescriptor(UsbInterfaceDescriptor *parent);

            /**
             * @brief type
             * @return the interface extra descriptor type
             */
            InterfaceExtraDescriptorType type() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype
             * @see UsbVideoControlInterfaceDescriptor::strSubtype()
             */
            virtual uint8_t bDescriptorSubtype() const = 0;

            /**
             * @brief strSubtype
             * @param subtype
             * @return string description of subtype
             */
            static QString strSubtype(uint8_t subtype);

            /**
             * @brief strWTerminalType
             * @param wTerminal
             * @return string description of wTerminalType
             */
            static QString strWTerminalType(uint16_t wTerminalType);

            /**
             * @brief get
             * Get the VideoControl Interface Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * VideoControl Interface Descriptor's position at extra data
             * @return pointer to the instance of VideoControl Interface Descriptor,
             * nullptr if parse failed.
             */
            static UsbVideoControlInterfaceDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                           uint8_t descPos);
        };

        /**
         * @brief The UsbClassSpecificVCInterfaceHeaderDescriptor class
         * Class-Specific VC Interface Header Descriptor C++ wrapper
         */
        class UsbClassSpecificVCInterfaceHeaderDescriptor : public UsbVideoControlInterfaceDescriptor
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
             * @see UsbVideoControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bcdUVC
             * @return video Device Class Specification release number in binary-coded decimal
             */
            uint16_t bcdUVC() const;

            /**
             * @brief wTotalLength
             * @return total number of bytes returned for the class-specific VideoControl interface descriptor
             */
            uint16_t wTotalLength() const;

            /**
             * @brief dwClockFrequency
             * @warning
             * Use of this field has been deprecated
             * @return the device clock frequency in Hz
             */
            uint32_t dwClockFrequency() const;

            /**
             * @brief bInCollection
             * @return the number of VideoStreaming interfaces in the Video Interface
             * Collection to which this VideoControl interface belongs.
             */
            uint8_t bInCollection() const;

            /**
             * @brief baInterfaceNr
             * @param index
             * @note
             * The Spec defines the first baInterfaceNr is baInterfaceNr(1),
             * therefore the parameter 'index' should also start with 1.
             * @return interface number of the index-th VideoStreaming in the Collection
             */
            uint8_t baInterfaceNr(uint8_t index) const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbClassSpecificVCInterfaceHeaderDescriptor(UsbInterfaceDescriptor *parent,
                                                                 uint8_t descPos);
            friend UsbVideoControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bInCollection;
            uint16_t _bcdUVC, _wTotalLength;
            uint32_t _dwClockFrequency;
            QVector<uint8_t> _baInterfaceNr;
        };

        /**
         * @brief The UsbInputTerminalDescriptor class
         * Input Terminal Descriptor C++ wrapper
         */
        class UsbInputTerminalDescriptor : public UsbVideoControlInterfaceDescriptor
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
             * @see UsbVideoControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bTerminalID
             * @return
             */
            uint8_t bTerminalID() const;

            /**
             * @brief wTerminalType
             * @return constant that characterizes the type of Terminal
             * @see UsbVideoControlInterfaceDescriptor::strWTerminalType()
             */
            uint16_t wTerminalType() const;

            /**
             * @brief bAssocTerminal
             * @return ID of the Output Terminal to which this Input Terminal is associated,
             * or zero (0) if no such association exists.
             */
            uint8_t bAssocTerminal() const;

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

        protected:
            explicit UsbInputTerminalDescriptor(UsbInterfaceDescriptor *parent,
                                                uint8_t descPos);
            inline void __parseBase(UsbHtmlBuilder &builder) const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bTerminalID,
            _bAssocTerminal, _iTerminal;
            uint16_t _wTerminalType;

        private:
            friend UsbVideoControlInterfaceDescriptor;

            /**
             * @brief get
             * Get the Input Terminal Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * Input Terminal Descriptor's position at extra data
             * @return pointer to the instance of Input Terminal Descriptor,
             * nullptr if parse failed.
             */
            static UsbInputTerminalDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                   uint8_t descPos);
        };

        /**
         * @brief The UsbOutputTerminalDescriptor class
         * Output Terminal Descriptor C++ wrapper
         */
        class UsbOutputTerminalDescriptor : public UsbVideoControlInterfaceDescriptor
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
             * @see UsbVideoControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bTerminalID
             * @return
             */
            uint8_t bTerminalID() const;

            /**
             * @brief wTerminalType
             * @return constant that characterizes the type of Terminal
             * @see UsbVideoControlInterfaceDescriptor::strWTerminalType()
             */
            uint16_t wTerminalType() const;

            /**
             * @brief bAssocTerminal
             * @return ID of the Output Terminal to which this Input Terminal is associated,
             * or zero (0) if no such association exists.
             */
            uint8_t bAssocTerminal() const;

            /**
             * @brief bSourceID
             * @return ID of the Unit or Terminal to which this Terminal is connected
             */
            uint8_t bSourceID() const;

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

        protected:
            explicit UsbOutputTerminalDescriptor(UsbInterfaceDescriptor *parent,
                                                 uint8_t descPos);
            inline void __parseBase(UsbHtmlBuilder &builder) const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bTerminalID,
            _bAssocTerminal, _bSourceID, _iTerminal;
            uint16_t _wTerminalType;

        private:
            friend UsbVideoControlInterfaceDescriptor;

            /**
             * @brief get
             * Get the Output Terminal Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * Output Terminal Descriptor's position at extra data
             * @return pointer to the instance of Output Terminal Descriptor,
             * nullptr if parse failed.
             */
            static UsbOutputTerminalDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                    uint8_t descPos);
        };

        /**
         * @brief The UsbCameraTerminalDescriptor class
         * Camera Terminal Descriptor C++ wrapper
         */
        class UsbCameraTerminalDescriptor : public UsbInputTerminalDescriptor
        {
            Q_OBJECT
        public:
            /**
             * @brief wObjectiveFocalLengthMin
             * @return the value of Lmin If Optical Zoom is not supported;
             * this field shall be set to 0.
             */
            uint16_t wObjectiveFocalLengthMin() const;

            /**
             * @brief wObjectiveFocalLengthMax
             * @return the value of Lmax If Optical Zoom is not supported;
             * this field shall be set to 0.
             */
            uint16_t wObjectiveFocalLengthMax() const;

            /**
             * @brief wOcularFocalLength
             * @return the value of Locular If Optical Zoom is not supported;
             * this field shall be set to 0.
             */
            uint16_t wOcularFocalLength() const;

            /**
             * @brief bControlSize
             * @return size in bytes of the bmControls field, must be 3
             */
            uint8_t bControlSize() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates that the
             * mentioned Control is supported for the video stream.
             * @note
             * D0       Scanning Mode
             * D1       Auto-Exposure Mode
             * D2       Auto-Exposure Priority
             * D3       Exposure Time (Absolute)
             * D4       Exposure Time (Relative)
             * D5       Focus (Absolute)
             * D6       Focus (Relative)
             * D7       Iris (Absolute)
             * D8       Iris (Relative)
             * D9       Zoom (Absolute)
             * D10      Zoom (Relative)
             * D11      PanTilt (Absolute)
             * D12      PanTilt (Relative)
             * D13      Roll (Absolute)
             * D14      Roll (Relative)
             * D15..16  Reserved
             * D17      Focus, Auto
             * D18      Privacy
             * D19      Focus, Simple
             * D20      Window
             * D21      Region of Interest
             * D22..23  Reserved
             */
            QByteArray bmControls() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbCameraTerminalDescriptor(UsbInterfaceDescriptor *parent,
                                                 uint8_t descPos);
            friend UsbInputTerminalDescriptor;
            inline QString __parseBmControls() const;

            uint8_t _bControlSize;
            uint16_t _wObjectiveFocalLengthMin, _wObjectiveFocalLengthMax, _wOcularFocalLength;
            QByteArray _bmControls;
        };

        /**
         * @brief The UsbSelectorUnitDescriptor class
         * Selector Unit Descriptor
         */
        class UsbSelectorUnitDescriptor : public UsbVideoControlInterfaceDescriptor
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
             * @see UsbVideoControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant that uniquely identifies the Unit within the video function
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
            friend UsbVideoControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNrInPins, _iSelector;
            QVector<uint8_t> _baSourceID;
        };

        /**
         * @brief The UsbProcessingUnitDescriptor class
         * Processing Unit Descriptor
         */
        class UsbProcessingUnitDescriptor : public UsbVideoControlInterfaceDescriptor
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
             * @see UsbVideoControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant that uniquely identifies the Unit within the video function
             */
            uint8_t bUnitID() const;

            /**
             * @brief bSourceID
             * @return ID of the Unit or Terminal to which this Unit is connected
             */
            uint8_t bSourceID() const;

            /**
             * @brief wMaxMultiplier
             * @return maximum digital magnification, multiplied by 100
             */
            uint16_t wMaxMultiplier() const;

            /**
             * @brief bControlSize
             * @return size of the bmControls field, in bytes, must be 3
             */
            uint8_t bControlSize() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates that the mentioned
             * Control is supported for the video stream.
             * @note
             * D0       Brightness
             * D1       Contrast
             * D2       Hue
             * D3       Saturation
             * D4       Sharpness
             * D5       Gamma
             * D6       White Balance Temperature
             * D7       White Balance Component
             * D8       Backlight Compensation
             * D9       Gain
             * D10      Power Line Frequency
             * D11      Hue, Auto
             * D12      White Balance Temperature, Auto
             * D13      White Balance Component, Auto
             * D14      Digital Multiplier
             * D15      Digital Multiplier Limit
             * D16      Analog Video Standard
             * D17      Analog Video Lock Status
             * D18      Contrast, Auto
             * D19..23  Reserved
             */
            QByteArray bmControls() const;

            /**
             * @brief iProcessing
             * @return string descriptor describing this processing unit
             */
            uint8_t iProcessing() const;

            /**
             * @brief bmVideoStandards
             * @return bitmap of all analog video standards supported by the Processing Unit
             * @note
             * D0       None
             * D1       NTSC - 525/60
             * D2       PAL - 625/50
             * D3       SECAM - 625/50
             * D4       NTSC - 625/50
             * D5       PAL - 525/60
             * D6..7    Reserved
             */
            uint8_t bmVideoStandards() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbProcessingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                                 uint8_t descPos);
            friend UsbVideoControlInterfaceDescriptor;
            inline QString __parseBmControls() const;
            inline QString __parseBmVideoStandards() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bSourceID, _bControlSize, _iProcessing, _bmVideoStandards;
            uint16_t _wMaxMultiplier;
            QByteArray _bmControls;
        };

        /**
         * @brief The UsbExtensionUnitDescriptor class
         * Extension Unit Descriptor
         */
        class UsbExtensionUnitDescriptor : public UsbVideoControlInterfaceDescriptor
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
             * @see UsbVideoControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return  constant that uniquely identifies the Unit within the video function
             */
            uint8_t bUnitID() const;

            /**
             * @brief guidExtensionCode
             * @return vendor-specific code identifying the Extension Unit
             */
            QByteArray guidExtensionCode() const;

            /**
             * @brief bNumControls
             * @return number of controls in this extension unit
             */
            uint8_t bNumControls() const;

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
             * @brief bControlSize
             * @return size of the bmControls field in bytes
             */
            uint8_t bControlSize() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates that the mentioned
             * Control is supported for the video stream.
             * @note
             * D..      Vendor-specific
             */
            QByteArray bmControls() const;

            /**
             * @brief iExtension
             * @return string descriptor that describing this extension unit
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
            friend UsbVideoControlInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bNumControls, _bNrInPins, _bControlSize, _iExtension;
            QVector<uint8_t> _baSourceID;
            QByteArray _guidExtensionCode, _bmControls;
        };

        /**
         * @brief The UsbEncodingUnitDescriptor class
         * Encoding Unit Descriptor C++ wrapper
         */
        class UsbEncodingUnitDescriptor : public UsbVideoControlInterfaceDescriptor
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
             * @see UsbVideoControlInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bUnitID
             * @return constant that uniquely identifies the Unit within the video function
             */
            uint8_t bUnitID() const;

            /**
             * @brief bSourceID
             * @return ID of the Unit or Terminal to which this Unit is connected
             */
            uint8_t bSourceID() const;

            /**
             * @brief iEncoding
             * @return  string descriptor that describing this encoding unit
             */
            uint8_t iEncoding() const;

            /**
             * @brief bControlSize
             * @return size of the bmControls and bmControlsRuntime fields, in bytes
             */
            uint8_t bControlSize() const;

            /**
             * @brief bmControls
             * @return bitmap that a bit set to 1 indicates that the mentioned
             * Control is supported for the video stream.
             * @note
             * D0       Select Layer
             * D1       Profile and Toolset
             * D2       Video Resolution
             * D3       Minimum Frame Interval
             * D4       Slice Mode
             * D5       Rate Control Mode
             * D6       Average Bit Rate
             * D7       CPB Size
             * D8       Peak Bit Rate
             * D9       Quantization Parameter
             * D10      Synchronization and Long-Term Reference Frame
             * D11      Long-Term Buffer
             * D12      Picture Long-Term Reference
             * D13      LTR Validation
             * D14      Level IDC
             * D15      SEI Message
             * D16      QP Range
             * D17      Priority ID
             * D18      Start or Stop Layer/View
             * D19      Error Resiliency
             * D20..23  Reserved
             */
            QByteArray bmControls() const;

            /**
             * @brief bmControlsRuntime
             * @return bitmap that a bit set to 1 indicates that the mentioned
             * Control is supported for the video stream.
             * @note
             * D0       Select Layer
             * D1       Profile and Toolset
             * D2       Video Resolution
             * D3       Minimum Frame Interval
             * D4       Slice Mode
             * D5       Rate Control Mode
             * D6       Average Bit Rate
             * D7       CPB Size
             * D8       Peak Bit Rate
             * D9       Quantization Parameter
             * D10      Synchronization and Long-Term Reference Frame
             * D11      Long-Term Buffer
             * D12      Picture Long-Term Reference
             * D13      LTR Validation
             * D14      Level IDC
             * D15      SEI Message
             * D16      QP Range
             * D17      Priority ID
             * D18      Start or Stop Layer/View
             * D19      Error Resiliency
             * D20..23  Reserved
             */
            QByteArray bmControlsRuntime() const;

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbEncodingUnitDescriptor(UsbInterfaceDescriptor *parent,
                                               uint8_t descPos);
            friend UsbVideoControlInterfaceDescriptor;
            inline QString __parseBmControls(const QByteArray &bmControls) const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bUnitID,
            _bSourceID, _iEncoding, _bControlSize;
            QByteArray _bmControls, _bmControlsRuntime;
        };
    } // namespace uvc
} // namespace usb

#endif // USB_USBVIDEOCONTROLINTERFACEDESCRIPTOR_H
