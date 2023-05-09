/*! C++ class wrapper of USB VideoStream Interface Descriptors

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

#ifndef USBVIDEOSTREAMINTERFACEDESCRIPTOR_H
#define USBVIDEOSTREAMINTERFACEDESCRIPTOR_H

#include <QObject>
#include "usbinterfaceextradescriptor.h"

namespace usb {
    namespace uvc {
        /**
         * @brief The UsbVideoStreamInterfaceDescriptor class
         * VideoStream Interface Descriptor
         */
        class UsbVideoStreamInterfaceDescriptor : public UsbInterfaceExtraDescriptor
        {
            Q_OBJECT
        public:
            UsbVideoStreamInterfaceDescriptor(UsbInterfaceDescriptor *parent);

            /**
             * @brief type
             * @return the interface extra descriptor type
             */
            InterfaceExtraDescriptorType type() const override;

            /**
             * @brief bDescriptorSubtype
             * @return descriptor subtype
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            virtual uint8_t bDescriptorSubtype() const = 0;

            /**
             * @brief strSubtype
             * @param subtype
             * @return string description of subtype
             */
            static QString strSubtype(uint8_t subtype);

            /**
             * @brief get
             * Get the VideoStream Interface Descriptor
             * @param interfaceDescriptor
             * Interface Descriptor with extra data
             * @param descPos
             * VideoStream Interface Descriptor's position at extra data
             * @return pointer to the instance of VideoStream Interface Descriptor,
             * nullptr if parse failed.
             */
            static UsbVideoStreamInterfaceDescriptor *get(UsbInterfaceDescriptor *interfaceDescriptor,
                                                          uint32_t descPos);
        };

        /**
         * @brief The UsbInputHeaderDescriptor class
         * Input Header Descripitor
         */
        class UsbInputHeaderDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bNumFormats
             * @return number of video payload Format descriptors following for this interface
             */
            uint8_t bNumFormats() const;

            /**
             * @brief wTotalLength
             * @return total number of bytes returned for the class-specific VideoStreaming
             * interface descriptors including this header descriptor.
             */
            uint16_t wTotalLength() const;

            /**
             * @brief bEndpointAddress
             * @return the address of the isochronous or bulk endpoint used for video data
             */
            uint8_t bEndpointAddress() const;

            /**
             * @brief bmInfo
             * @return bitmap indicating the capabilities of this VideoStreaming interface
             * @note
             * D1       Dynamic Format Change supported
             */
            uint8_t bmInfo() const;

            /**
             * @brief bTerminalLink
             * @return the terminal ID of the Output Terminal to which
             * the video endpoint of this interface is connected.
             */
            uint8_t bTerminalLink() const;

            /**
             * @brief bStillCaptureMethod
             * @return method of still image capture supported
             */
            uint8_t bStillCaptureMethod() const;

            /**
             * @brief bTriggerSupport
             * @return if hardware triggering is supported through this interface
             */
            uint8_t bTriggerSupport() const;

            /**
             * @brief bTriggerUsage
             * @return how the host software shall respond to
             * a hardware trigger interrupt event from this interface.
             */
            uint8_t bTriggerUsage() const;

            /**
             * @brief bControlSize
             * @return size of each bmaControls(x) field, in bytes
             */
            uint8_t bControlSize() const;

            /**
             * @brief bmaControls
             * @param index
             * @note
             * The Spec defines the first bmaControls is bmaControls(1),
             * therefore the parameter 'index' should also start with 1.
             * @return bitmap as following usage:
             * For bits D0..3, a bit set to 1 indicates that the named field is supported
             * by the Video Probe and Commit Control when bFormatIndex is index.
             * For bits D4..5, a bit set to 1 indicates that the named control is supported
             * by the device when bFormatIndex is index.
             * @note
             * D0       wKeyFrameRate
             * D1       wPFrameRate
             * D2       wCompQuality
             * D3       wCompWindowSize
             * D4       Generate Key Frame
             * D5       Update Frame Segment
             * D6..     Reserved
             */
            QByteArray bmaControls(uint8_t index) const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbInputHeaderDescriptor(UsbInterfaceDescriptor *parent,
                                              uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBStillCaptureMethod() const;
            inline QString __parseBmaControls(uint8_t index) const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bNumFormats,
            _bEndpointAddress, _bmInfo, _bTerminalLink, _bStillCaptureMethod,
            _bTriggerSupport, _bTriggerUsage, _bControlSize;
            uint16_t _wTotalLength;
            QVector<QByteArray> _bmaControls;
        };

        /**
         * @brief The UsbOutputHeaderDescriptor class
         * Output Header Descripitor
         */
        class UsbOutputHeaderDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bNumFormats
             * @return number of video payload Format descriptors following for this interface
             */
            uint8_t bNumFormats() const;

            /**
             * @brief wTotalLength
             * @return total number of bytes returned for the class-specific VideoStreaming
             * interface descriptors including this header descriptor.
             */
            uint16_t wTotalLength() const;

            /**
             * @brief bEndpointAddress
             * @return the address of the isochronous or bulk endpoint used for video data
             */
            uint8_t bEndpointAddress() const;

            /**
             * @brief bTerminalLink
             * @return the terminal ID of the Output Terminal to which
             * the video endpoint of this interface is connected.
             */
            uint8_t bTerminalLink() const;

            /**
             * @brief bControlSize
             * @return size of each bmaControls(x) field, in bytes
             */
            uint8_t bControlSize() const;

            /**
             * @brief bmaControls
             * @param index
             * @note
             * The Spec defines the first bmaControls is bmaControls(1),
             * therefore the parameter 'index' should also start with 1.
             * @return bitmap as following usage:
             * For bits D0..3, a bit set to 1 indicates that the named field is supported
             * by the Video Probe and Commit Control when bFormatIndex is index.
             * @note
             * D0       wKeyFrameRate
             * D1       wPFrameRate
             * D2       wCompQuality
             * D3       wCompWindowSize
             * D4..     Reserved
             */
            QByteArray bmaControls(uint8_t index) const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbOutputHeaderDescriptor(UsbInterfaceDescriptor *parent,
                                               uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmaControls(uint8_t index) const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bNumFormats,
            _bEndpointAddress, _bTerminalLink, _bControlSize;
            uint16_t _wTotalLength;
            QVector<QByteArray> _bmaControls;
        };

        /**
         * @brief The UsbStillImageFrameDescriptor class
         * Still Image Frame Descriptor C++ wrapper
         */
        class UsbStillImageFrameDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bEndpointAddress
             * @return the address of the isochronous or bulk endpoint used for still image capture
             * if method 3 of still image capture is used.
             */
            uint8_t bEndpointAddress() const;

            /**
             * @brief bNumImageSizePatterns
             * @return number of Image Size patterns of this format
             */
            uint8_t bNumImageSizePatterns() const;

            /**
             * @brief wWidth
             * @param index
             * @note
             * The Spec defines the first wWidth is wWidth(1),
             * therefore the parameter 'index' should also start with 1.
             * @return width of the still image in index-th pattern
             */
            uint16_t wWidth(uint8_t index) const;

            /**
             * @brief wHeight
             * @param index
             * @note
             * The Spec defines the first wHeight is wHeight(1),
             * therefore the parameter 'index' should also start with 1.
             * @return height of the still image in index-th pattern
             */
            uint16_t wHeight(uint8_t index) const;

            /**
             * @brief bNumCompressionPattern
             * @return number of Compression pattern of this format
             */
            uint8_t bNumCompressionPattern() const;

            /**
             * @brief bCompression
             * @param index
             * @note
             * The Spec defines the first bCompression is bCompression(1),
             * therefore the parameter 'index' should also start with 1.
             * @return compression of the still image in index-th pattern
             */
            uint8_t bCompression(uint8_t index) const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbStillImageFrameDescriptor(UsbInterfaceDescriptor *parent,
                                                  uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bEndpointAddress,
            _bNumImageSizePatterns, _bNumCompressionPattern;
            QVector<uint8_t> _bCompression;
            QVector<uint16_t> _wWidth, _wHeight;
        };

        /**
         * @brief The UsbUncompressedVideoFormatDescriptor class
         * Uncompressed Video Format Descriptor C++ wrapper
         */
        class UsbUncompressedVideoFormatDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatIndex
             * @return index of this format descriptor
             */
            uint8_t bFormatIndex() const;

            /**
             * @brief bNumFrameDescriptors
             * @return number of frame descriptors following that correspond to this format
             */
            uint8_t bNumFrameDescriptors() const;

            /**
             * @brief guidFormat
             * @return Globally Unique Identifier used to identify stream-encoding format
             */
            QByteArray guidFormat() const;

            /**
             * @brief bBitsPerPixel
             * @return number of bits per pixel used to specify color in the decoded video frame
             */
            uint8_t bBitsPerPixel() const;

            /**
             * @brief bDefaultFrameIndex
             * @return Optimum Frame Index for this stream
             */
            uint8_t bDefaultFrameIndex() const;

            /**
             * @brief bAspectRatioX
             * @return the X dimension of the picture aspect ratio
             */
            uint8_t bAspectRatioX() const;

            /**
             * @brief bAspectRatioY
             * @return the Y dimension of the picture aspect ratio
             */
            uint8_t bAspectRatioY() const;

            /**
             * @brief bmInterlaceFlags
             * @return bitmap that specifies interlace information
             * @note
             * D0       Interlaced stream or variable
             * D1       Fields per frame. 0: 2 fields, 1: 1 field
             * D2       Field 1 first
             * D3       Reserved
             * D4..5    Field pattern:
             *              00: Field 1 only
             *              01: Field 2 only
             *              10: Regular pattern of fields 1 and 2
             *              11: Random pattern of fields 1 and 2
             * D6..7    Reserved
             */
            uint8_t bmInterlaceFlags() const;

            /**
             * @brief bCopyProtect
             * @return whether duplication of the video stream is restricted
             */
            uint8_t bCopyProtect() const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbUncompressedVideoFormatDescriptor(UsbInterfaceDescriptor *parent,
                                                          uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmInterlaceFlags() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatIndex,
            _bNumFrameDescriptors, _bBitsPerPixel, _bDefaultFrameIndex,
            _bAspectRatioX, _bAspectRatioY, _bmInterlaceFlags, _bCopyProtect;
            QByteArray _guidFormat;
        };

        /**
         * @brief The UsbUncompressedVideoFrameDescriptor class
         * Uncompressed Video Frame Desciptor C++ wrapper
         */
        class UsbUncompressedVideoFrameDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFrameIndex
             * @return index of this frame descriptor
             */
            uint8_t bFrameIndex() const;

            /**
             * @brief bmCapabilities
             * @return bitmap indicating the capabilities are supported at this frame setting
             * @note
             * D0       Still image supported
             * D1       Fixed frame-rate
             * D2..7    Reserved
             */
            uint8_t bmCapabilities() const;

            /**
             * @brief wWidth
             * @return width of decoded bitmap frame in pixels
             */
            uint16_t wWidth() const;

            /**
             * @brief wHeight
             * @return height of decoded bitmap frame in pixels
             */
            uint16_t wHeight() const;

            /**
             * @brief dwMinBitRate
             * @return the minimum bit rate at the longest frame interval
             * in units of bps at which the data can be transmitted.
             */
            uint32_t dwMinBitRate() const;

            /**
             * @brief dwMaxBitRate
             * @return the maximum bit rate at the shortest frame interval
             * in units of bps at which the data can be transmitted.
             */
            uint32_t dwMaxBitRate() const;

            /**
             * @brief dwMaxVideoFrameBufferSize
             * @warning
             * Use of this field has been deprecated.
             * @return the maximum number of bytes that the compressor
             * will produce for a video frame or still image.
             */
            uint32_t dwMaxVideoFrameBufferSize() const;

            /**
             * @brief dwDefaultFrameInterval
             * @return the frame interval the device would like to indicate for use as a default
             */
            uint32_t dwDefaultFrameInterval() const;

            /**
             * @brief bFrameIntervalType
             * @return how the frame interval can be programmed
             */
            uint8_t bFrameIntervalType() const;

            /**
             * @brief dwMinFrameInterval
             * @warning
             * Only valid when bFrameIntervalType is 0
             * @return shortest frame interval supported (at highest frame rate), in 100 ns units
             */
            uint32_t dwMinFrameInterval() const;

            /**
             * @brief _dwMaxFrameInterval
             * @warning
             * Only valid when bFrameIntervalType is 0
             * @return longest frame interval supported (at lowest frame rate), in 100 ns units
             */
            uint32_t dwMaxFrameInterval() const;

            /**
             * @brief dwFrameIntervalStep
             * @warning
             * Only valid when bFrameIntervalType is 0
             * @return granularity of frame interval range, in 100 ns units
             */
            uint32_t dwFrameIntervalStep() const;

            /**
             * @brief dwFrameInterval
             * @warning
             * Only valid when bFrameIntervalType is NOT 0
             * @param index
             * @note
             * The Spec defines the first dwFrameInterval is dwFrameInterval(1),
             * therefore the parameter 'index' should also start with 1.
             * @return the index-th shortest frame interval supported (at index-th highest frame rate),
             * in 100 ns units.
             */
            uint32_t dwFrameInterval(uint8_t index) const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbUncompressedVideoFrameDescriptor(UsbInterfaceDescriptor *parent,
                                                         uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmCapabilities() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFrameIndex,
            _bmCapabilities, _bFrameIntervalType;
            uint16_t _wWidth, _wHeight;
            uint32_t _dwMinBitRate, _dwMaxBitRate, _dwMaxVideoFrameBufferSize,
            _dwDefaultFrameInterval, _dwFrameIntervalStep;
            QVector<uint32_t> _dwFrameInterval;
        };

        /**
         * @brief The UsbMotionJpegVideoFormatDescriptor class
         * Motion-JPEG Video Format Descriptor C++ wrapper
         */
        class UsbMotionJpegVideoFormatDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatIndex
             * @return index of this format descriptor
             */
            uint8_t bFormatIndex() const;

            /**
             * @brief bNumFrameDescriptors
             * @return number of frame descriptors following that correspond to this format
             */
            uint8_t bNumFrameDescriptors() const;

            /**
             * @brief bmFlags
             * @return characteristics of this format
             * @note
             * D0       Fixed Size Samples
             * D1..7    Reserved
             */
            uint8_t bmFlags() const;

            /**
             * @brief bDefaultFrameIndex
             * @return Optimum Frame Index for this stream
             */
            uint8_t bDefaultFrameIndex() const;

            /**
             * @brief bAspectRatioX
             * @return the X dimension of the picture aspect ratio
             */
            uint8_t bAspectRatioX() const;

            /**
             * @brief bAspectRatioY
             * @return the Y dimension of the picture aspect ratio
             */
            uint8_t bAspectRatioY() const;

            /**
             * @brief bmInterlaceFlags
             * @return bitmap that specifies interlace information
             * @note
             * D0       Interlaced stream or variable
             * D1       Fields per frame. 0: 2 fields, 1: 1 field
             * D2       Field 1 first
             * D3       Reserved
             * D4..5    Field pattern:
             *              00: Field 1 only
             *              01: Field 2 only
             *              10: Regular pattern of fields 1 and 2
             *              11: Random pattern of fields 1 and 2
             * D6..7    Reserved
             */
            uint8_t bmInterlaceFlags() const;

            /**
             * @brief bCopyProtect
             * @return whether duplication of the video stream is restricted
             */
            uint8_t bCopyProtect() const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbMotionJpegVideoFormatDescriptor(UsbInterfaceDescriptor *parent,
                                                        uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmInterlaceFlags() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatIndex,
            _bNumFrameDescriptors, _bmFlags, _bDefaultFrameIndex,
            _bAspectRatioX, _bAspectRatioY, _bmInterlaceFlags, _bCopyProtect;
        };

        /**
         * @brief The UsbMotionJpegVideoFrameDescriptor class
         * Motion-JPEG Video Frame Desciptor C++ wrapper
         */
        class UsbMotionJpegVideoFrameDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFrameIndex
             * @return index of this frame descriptor
             */
            uint8_t bFrameIndex() const;

            /**
             * @brief bmCapabilities
             * @return bitmap indicating the capabilities are supported at this frame setting
             * @note
             * D0       Still image supported
             * D1       Fixed frame-rate
             * D2..7    Reserved
             */
            uint8_t bmCapabilities() const;

            /**
             * @brief wWidth
             * @return width of decoded bitmap frame in pixels
             */
            uint16_t wWidth() const;

            /**
             * @brief wHeight
             * @return height of decoded bitmap frame in pixels
             */
            uint16_t wHeight() const;

            /**
             * @brief dwMinBitRate
             * @return the minimum bit rate at the longest frame interval
             * in units of bps at which the data can be transmitted.
             */
            uint32_t dwMinBitRate() const;

            /**
             * @brief dwMaxBitRate
             * @return the maximum bit rate at the shortest frame interval
             * in units of bps at which the data can be transmitted.
             */
            uint32_t dwMaxBitRate() const;

            /**
             * @brief dwMaxVideoFrameBufferSize
             * @warning
             * Use of this field has been deprecated.
             * @return the maximum number of bytes that the compressor
             * will produce for a video frame or still image.
             */
            uint32_t dwMaxVideoFrameBufferSize() const;

            /**
             * @brief dwDefaultFrameInterval
             * @return the frame interval the device would like to indicate for use as a default
             */
            uint32_t dwDefaultFrameInterval() const;

            /**
             * @brief bFrameIntervalType
             * @return how the frame interval can be programmed
             */
            uint8_t bFrameIntervalType() const;

            /**
             * @brief dwMinFrameInterval
             * @warning
             * Only valid when bFrameIntervalType is 0
             * @return shortest frame interval supported (at highest frame rate), in 100 ns units
             */
            uint32_t dwMinFrameInterval() const;

            /**
             * @brief _dwMaxFrameInterval
             * @warning
             * Only valid when bFrameIntervalType is 0
             * @return longest frame interval supported (at lowest frame rate), in 100 ns units
             */
            uint32_t dwMaxFrameInterval() const;

            /**
             * @brief dwFrameIntervalStep
             * @warning
             * Only valid when bFrameIntervalType is 0
             * @return granularity of frame interval range, in 100 ns units
             */
            uint32_t dwFrameIntervalStep() const;

            /**
             * @brief dwFrameInterval
             * @warning
             * Only valid when bFrameIntervalType is NOT 0
             * @param index
             * @note
             * The Spec defines the first bFrameIntervalType is bFrameIntervalType(1),
             * therefore the parameter 'index' should also start with 1.
             * @return the index-th shortest frame interval supported (at index-th highest frame rate),
             * in 100 ns units.
             */
            uint32_t dwFrameInterval(uint8_t index) const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbMotionJpegVideoFrameDescriptor(UsbInterfaceDescriptor *parent,
                                                       uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmCapabilities() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFrameIndex,
            _bmCapabilities, _bFrameIntervalType;
            uint16_t _wWidth, _wHeight;
            uint32_t _dwMinBitRate, _dwMaxBitRate, _dwMaxVideoFrameBufferSize,
            _dwDefaultFrameInterval, _dwFrameIntervalStep;
            QVector<uint32_t> _dwFrameInterval;
        };

        /**
         * @brief The UsbMpeg2TsFormatDescriptor class
         * MPEG-2 TS Format Descriptor C++ wrapper
         */
        class UsbMpeg2TsFormatDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatIndex
             * @return index of this Format Descriptor
             */
            uint8_t bFormatIndex() const;

            /**
             * @brief bDataOffset
             * @return offset to TSP packet within MPEG-2 TS transport stride, in bytes
             */
            uint8_t bDataOffset() const;

            /**
             * @brief bPacketLength
             * @return length of TSP packet, in bytes
             */
            uint8_t bPacketLength() const;

            /**
             * @brief bStrideLength
             * @return length of MPEG-2 TS transport stride
             */
            uint8_t bStrideLength() const;

            /**
             * @brief guidStrideFormat
             * @return Globally Unique Identifier indicating the format of the stride data
             */
            QByteArray guidStrideFormat() const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbMpeg2TsFormatDescriptor(UsbInterfaceDescriptor *parent,
                                                uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatIndex,
            _bDataOffset, _bPacketLength, _bStrideLength;
            QByteArray _guidStrideFormat;
        };

        /**
         * @brief The UsbDvFormatDescriptor class
         * DV Format Descriptor C++ wrapper
         */
        class UsbDvFormatDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatIndex
             * @return index of this Format Descriptor
             */
            uint8_t bFormatIndex() const;

            /**
             * @brief dwMaxVideoFrameBufferSize
             * @warning
             * Use of this field has been deprecated.
             * @return the maximum size of a single DV frame, in bytes
             */
            uint32_t dwMaxVideoFrameBufferSize() const;

            /**
             * @brief bFormatType
             * @return format type
             * @note
             * D0..6    0: SD-DV, 1: SDL-DV, 2: HD-DV
             * D7       0: 50 Hz, 1: 60 Hz
             */
            uint8_t bFormatType() const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbDvFormatDescriptor(UsbInterfaceDescriptor *parent,
                                           uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBFormatType() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatIndex, _bFormatType;
            uint32_t _dwMaxVideoFrameBufferSize;
        };

        class UsbColorMatchingDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bColorPrimaries
             * @return the color primaries and the reference white
             * @note
             * 0:       Unspecified
             * 1:       BT.709, sRGB
             * 2:       BT.470-2 (M)
             * 3:       BT.470-2 (B, G)
             * 4:       SMPTE 170M
             * 5:       SMPTE 240M
             * 6-255:   Reserved
             */
            uint8_t bColorPrimaries() const;

            /**
             * @brief bTransferCharacteristics
             * @return the optoelectronic transfer characteristic of the source
             * picture also called the gamma function.
             * @note
             * 0:       Unspecified
             * 1:       BT.709
             * 2:       BT.470-2 M
             * 3:       BT.470-2 B, G
             * 4:       SMPTE 170M
             * 5:       SMPTE 240M
             * 6:       Linear
             * 7:       sRGB
             * 8-255:   Reserved
             */
            uint8_t bTransferCharacteristics() const;

            /**
             * @brief bMatrixCoefficients
             * @return matrix used to compute luma and chroma values from the color primaries
             * @note
             * 0:       Unspecified
             * 1:       BT. 709
             * 2:       FCC
             * 3:       BT.470-2 B, G
             * 4:       SMPTE 170M
             * 5:       SMPTE 240M
             * 6-255:   Reserved
             */
            uint8_t bMatrixCoefficients() const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbColorMatchingDescriptor(UsbInterfaceDescriptor *parent,
                                                uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __strBColorPrimaries() const;
            inline QString __strBTransferCharacteristics() const;
            inline QString __strBMatrixCoefficients() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype,
            _bColorPrimaries, _bTransferCharacteristics, _bMatrixCoefficients;
        };

        /**
         * @brief The UsbFrameBasedPayloadVideoFormatDescriptor class
         * Frame Based Payload Video Format Descriptor C++ wrapper
         */
        class UsbFrameBasedPayloadVideoFormatDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @return descriptor subtype, 0x10 here
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatIndex
             * @return index of this format descriptor
             */
            uint8_t bFormatIndex() const;

            /**
             * @brief bNumFrameDescriptors
             * @return number of frame descriptors following that correspond to this format
             */
            uint8_t bNumFrameDescriptors() const;

            /**
             * @brief guidFormat
             * @return Globally Unique Identifier used to identify stream-encoding format
             */
            QByteArray guidFormat() const;

            /**
             * @brief bBitsPerPixel
             * @return number of bits per pixel used to specify color in the decoded video frame
             */
            uint8_t bBitsPerPixel() const;

            /**
             * @brief bDefaultFrameIndex
             * @return Optimum Frame Index for this stream
             */
            uint8_t bDefaultFrameIndex() const;

            /**
             * @brief bAspectRatioX
             * @return the X dimension of the picture aspect ratio
             */
            uint8_t bAspectRatioX() const;

            /**
             * @brief bAspectRatioY
             * @return the Y dimension of the picture aspect ratio
             */
            uint8_t bAspectRatioY() const;

            /**
             * @brief bmInterlaceFlags
             * @return bitmap that specifies interlace information
             * @note
             * D0       Interlaced stream or variable
             * D1       Fields per frame. 0: 2 fields, 1: 1 field
             * D2       Field 1 first
             * D3       Reserved
             * D4..5    Field pattern:
             *              00: Field 1 only
             *              01: Field 2 only
             *              10: Regular pattern of fields 1 and 2
             *              11: Random pattern of fields 1 and 2
             * D6..7    Reserved
             */
            uint8_t bmInterlaceFlags() const;

            /**
             * @brief bCopyProtect
             * @return whether duplication of the video stream is restricted
             */
            uint8_t bCopyProtect() const;

            /**
             * @brief bVariableSize
             * @return whether the data within the frame is of variable length from frame to frame
             */
            uint8_t bVariableSize() const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbFrameBasedPayloadVideoFormatDescriptor(UsbInterfaceDescriptor *parent,
                                                               uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmInterlaceFlags() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatIndex,
            _bNumFrameDescriptors, _bBitsPerPixel, _bDefaultFrameIndex,
            _bAspectRatioX, _bAspectRatioY, _bmInterlaceFlags, _bCopyProtect, _bVariableSize;
            QByteArray _guidFormat;
        };

        /**
         * @brief The UsbUncompressedVideoFrameDescriptor class
         * Frame Based Payload Video Frame Descriptor C++ wrapper
         */
        class UsbFrameBasedPayloadVideoFrameDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @return descriptor subtype, 0x11 here
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFrameIndex
             * @return index of this frame descriptor
             */
            uint8_t bFrameIndex() const;

            /**
             * @brief bmCapabilities
             * @return bitmap indicating the capabilities are supported at this frame setting
             * @note
             * D0       Still image supported
             * D1       Fixed frame-rate
             * D2..7    Reserved
             */
            uint8_t bmCapabilities() const;

            /**
             * @brief wWidth
             * @return width of decoded bitmap frame in pixels
             */
            uint16_t wWidth() const;

            /**
             * @brief wHeight
             * @return height of decoded bitmap frame in pixels
             */
            uint16_t wHeight() const;

            /**
             * @brief dwMinBitRate
             * @return the minimum bit rate at the longest frame interval
             * in units of bps at which the data can be transmitted.
             */
            uint32_t dwMinBitRate() const;

            /**
             * @brief dwMaxBitRate
             * @return the maximum bit rate at the shortest frame interval
             * in units of bps at which the data can be transmitted.
             */
            uint32_t dwMaxBitRate() const;

            /**
             * @brief dwDefaultFrameInterval
             * @return the frame interval the device would like to indicate for use as a default
             */
            uint32_t dwDefaultFrameInterval() const;

            /**
             * @brief bFrameIntervalType
             * @return how the frame interval can be programmed
             */
            uint8_t bFrameIntervalType() const;

            /**
             * @brief dwBytesPerLine
             * @return the number of bytes per line of video for packed fixed frame size formats,
             * allowing the receiver to perform stride alignment of the video.
             */
            uint32_t dwBytesPerLine() const;

            /**
             * @brief dwMinFrameInterval
             * @warning
             * Only valid when bFrameIntervalType is 0
             * @return shortest frame interval supported (at highest frame rate), in 100 ns units
             */
            uint32_t dwMinFrameInterval() const;

            /**
             * @brief _dwMaxFrameInterval
             * @warning
             * Only valid when bFrameIntervalType is 0
             * @return longest frame interval supported (at lowest frame rate), in 100 ns units
             */
            uint32_t dwMaxFrameInterval() const;

            /**
             * @brief dwFrameIntervalStep
             * @warning
             * Only valid when bFrameIntervalType is 0
             * @return granularity of frame interval range, in 100 ns units
             */
            uint32_t dwFrameIntervalStep() const;

            /**
             * @brief dwFrameInterval
             * @warning
             * Only valid when bFrameIntervalType is NOT 0
             * @param index
             * @note
             * The Spec defines the first bFrameIntervalType is bFrameIntervalType(1),
             * therefore the parameter 'index' should also start with 1.
             * @return the index-th shortest frame interval supported (at index-th highest frame rate),
             * in 100 ns units.
             */
            uint32_t dwFrameInterval(uint8_t index) const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbFrameBasedPayloadVideoFrameDescriptor(UsbInterfaceDescriptor *parent,
                                                              uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmCapabilities() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFrameIndex,
            _bmCapabilities, _bFrameIntervalType;
            uint16_t _wWidth, _wHeight;
            uint32_t _dwMinBitRate, _dwMaxBitRate, _dwDefaultFrameInterval,
            _dwFrameIntervalStep, _dwBytesPerLine;
            QVector<uint32_t> _dwFrameInterval;
        };

        /**
         * @brief The UsbStreamBasedFormatDescriptor class
         * Stream Based Format Descriptor C++ wrapper
         */
        class UsbStreamBasedFormatDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @return descriptor subtype, 0x12 here
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatIndex
             * @return index of this Format Descriptor
             */
            uint8_t bFormatIndex() const;

            /**
             * @brief guidFormat
             * @return Globally Unique Identifier used to identify stream-encoding format
             */
            QByteArray guidFormat() const;

            /**
             * @brief dwPacketLength
             * @return format-specific packet size in a Packet-Oriented stream
             */
            uint16_t dwPacketLength() const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbStreamBasedFormatDescriptor(UsbInterfaceDescriptor *parent,
                                                    uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatIndex;
            uint16_t _dwPacketLength;
            QByteArray _guidFormat;
        };

        /**
         * @brief The UsbH264PayloadVideoFormatDescriptor class
         * H.264 Payload Video Format Descriptor C++ wrapper
         */
        class UsbH264PayloadVideoFormatDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @return descriptor subtype, 0x13 or 0x15 here
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatIndex
             * @return index of this format descriptor
             */
            uint8_t bFormatIndex() const;

            /**
             * @brief bNumFrameDescriptors
             * @return number of Frame Descriptors following that correspond to this format
             */
            uint8_t bNumFrameDescriptors() const;

            /**
             * @brief bDefaultFrameIndex
             * @return default frame index
             */
            uint8_t bDefaultFrameIndex() const;

            /**
             * @brief bMaxCodecConfigDelay
             * @return Maximum number of frames the encoder takes to respond to a command
             */
            uint8_t bMaxCodecConfigDelay() const;

            /**
             * @brief bmSupportedSliceModes
             * @return bitmap that a bit set to 1 indicates that
             * the mentioned slice mode is supported.
             * @note
             * D0       Maximum number of MBs per slice mode
             * D1       Target compressed size per slice mode
             * D2       Number of slices per frame mode
             * D3       Number of Macroblock rows per slice mode
             * D4..7    Reserved
             * @note
             * Set all bits to 0 if only one slice per frame is supported
             */
            uint8_t bmSupportedSliceModes() const;

            /**
             * @brief bmSupportedSyncFrameTypes
             * @return bitmap that a bit set to 1 indicates that
             * the mentioned sync frame mode mode is supported.
             * @note
             * D0       Reset
             * D1       IDR frame with SPS and PPS headers
             * D2       IDR frame (with SPS and PPS headers) that is a long term reference frame
             * D3       Non-IDR random-access I frame (with SPS and PPS headers)
             * D4       Generate a random-access I frame (with SPS and PPS headers)
             *              that is not an IDR frame and it is a long-term reference frame
             * D5       P frame that is a long term reference frame
             * D6       Gradual Decoder Refresh frames
             * D7       Reserved
             */
            uint8_t bmSupportedSyncFrameTypes() const;

            /**
             * @brief bResolutionScaling
             * @return the support for resolution downsizing
             * @note
             * 0:       Not supported
             * 1:       Limited to 1.5 or 2.0 scaling in both directions,
             *              while maintaining the aspect ratio
             * 2:       Limited to 1.0, 1.5 or 2.0 scaling in either direction
             * 3:       Limited to resolutions reported by the associated Frame Descriptors
             * 4:       Arbitrary scaling
             * 5-255:   Reserved
             */
            uint8_t bResolutionScaling() const;

            /**
             * @brief bmSupportedRateControlModes
             * @return bitmatp indicating supported rate-control modes
             * @note
             * D0       Variable bit rate (VBR) with underflow allowed
             * D1       Constant bit rate (CBR)
             * D2       Constant QP
             * D3       Global VBR with underflow allowed
             * D4       VBR without underflow
             * D5       Global VBR without underflow
             * D6..7    Reserved
             */
            uint8_t bmSupportedRateControlModes() const;

            /**
             * @brief wMaxMBperSecResolutionNoScalability
             * @param index
             * Must be 1~4
             * @return maximum macroblock processing rate, in units of 1000 MB/s
             */
            uint16_t wMaxMBperSecResolutionNoScalability(uint8_t index) const;

            /**
             * @brief wMaxMBperSecResolutionTemporalScalability
             * @param index
             * Must be 1~4
             * @return maximum macroblock processing rate, in units of 1000 MB/s
             */
            uint16_t wMaxMBperSecResolutionTemporalScalability(uint8_t index) const;

            /**
             * @brief wMaxMBperSecResolutionTemporalQualityScalability
             * @param index
             * Must be 1~4
             * @return maximum macroblock processing rate, in units of 1000 MB/s
             */
            uint16_t wMaxMBperSecResolutionTemporalQualityScalability(uint8_t index) const;

            /**
             * @brief wMaxMBperSecResolutionsTemporalSpatialScalability
             * @param index
             * Must be 1~4
             * @return maximum macroblock processing rate, in units of 1000 MB/s
             */
            uint16_t wMaxMBperSecResolutionsTemporalSpatialScalability(uint8_t index) const;

            /**
             * @brief wMaxMBperSecResolutionFullScalability
             * @param index
             * Must be 1~4
             * @return maximum macroblock processing rate, in units of 1000 MB/s
             */
            uint16_t wMaxMBperSecResolutionFullScalability(uint8_t index) const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbH264PayloadVideoFormatDescriptor(UsbInterfaceDescriptor *parent,
                                                         uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmSupportedSliceModes() const;
            inline QString __parseBmSupportedSyncFrameTypes() const;
            inline QString __strBResolutionScaling() const;
            inline QString __parseBmSupportedRateControlModes() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatIndex,
            _bNumFrameDescriptors, _bDefaultFrameIndex, _bMaxCodecConfigDelay,
            _bmSupportedSliceModes, _bmSupportedSyncFrameTypes, _bResolutionScaling,
            _bmSupportedRateControlModes;
            uint16_t _wMaxMBperSecResolutionNoScalability[4],
            _wMaxMBperSecResolutionTemporalScalability[4],
            _wMaxMBperSecResolutionTemporalQualityScalability[4],
            _wMaxMBperSecResolutionsTemporalSpatialScalability[4],
            _wMaxMBperSecResolutionFullScalability[4];
        };

        /**
         * @brief The UsbH264PayloadVideoFrameDescriptor class
         * H.264 Payload Video Frame Descriptor C++ wrapper
         */
        class UsbH264PayloadVideoFrameDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @return descriptor subtype, 0x14 here
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFrameIndex
             * @return index of this frame descriptor
             */
            uint8_t bFrameIndex() const;

            /**
             * @brief wWidth
             * @return the width of pictures output from the decoding process, in pixels
             */
            uint16_t wWidth() const;

            /**
             * @brief wHeight
             * @return the height of pictures output from the decoding process, in pixels
             */
            uint16_t wHeight() const;

            /**
             * @brief wSARwidth
             * @return sample aspect ratio width
             */
            uint16_t wSARwidth() const;

            /**
             * @brief wSARheight
             * @return sample aspect ratio height
             */
            uint16_t wSARheight() const;

            /**
             * @brief wProfile
             * @return value specified by profile_idc and constraint flags in the H.264 specification
             */
            uint16_t wProfile() const;

            /**
             * @brief bLevelIDC
             * @return the level as specified by the level_idc flag
             */
            uint8_t bLevelIDC() const;

            /**
             * @brief wConstrainedToolset
             * @return zero, reserved
             */
            uint16_t wConstrainedToolset() const;

            /**
             * @brief bmSupportedUsages
             * @return bitmap that a bit set to 1 indicates that
             * the mentioned usage is supported.
             * @note
             * D0       Real-time/UCConfig mode 0
             * D1       Real-time/UCConfig mode 1
             * D2       Real-time/UCConfig mode 2Q
             * D3       Real-time/UCConfig mode 2S
             * D4       Real-time/UCConfig mode 3
             * D5..7    Reserved
             * D8..15   Broadcast modes
             * D16      File Storage mode with I and P slices
             * D17      File Storage mode with I, P, and B slices
             * D18      File storage all-I-frame mode
             * D19..23  Reserved
             * D24      MVC Stereo High Mode
             * D25      MVC Multiview Mode
             * D26..31  Reserved
             */
            uint32_t bmSupportedUsages() const;

            /**
             * @brief bmCapabilities
             * @return bitmap that a bit set to 1 indicates that
             * the mentioned Capability is supported.
             * @note
             * D0       CAVLC only
             * D1       CABAC only
             * D2       Constant frame rate
             * D3       Separate QP for luma/chroma
             * D4       Separate QP for Cb/Cr
             * D5       No picture reordering
             * D6       Long Term Reference frame
             * D7..15   Reserved
             */
            uint16_t bmCapabilities() const;

            /**
             * @brief bmSVCCapabilities
             * @return bitmap indicating the SVCC capabilities
             * @note
             * D0..2    Maximum number of temporal layers minus 1
             * D3       Rewrite support
             * D4..6    Maximum number of CGS layers minus 1
             * D7..9    Maximum number of MGS sublayers
             * D10      Additional SNR scalability support in spatial enhancement layers
             * D11..13  Maximum number of spatial layers minus 1
             * D14..31  Reserved
             */
            uint32_t bmSVCCapabilities() const;

            /**
             * @brief bmMVCCapabilities
             * @return bitmap indicating the MVCC capabilities
             * @note
             * D0..2    Maximum number of temporal layers minus 1
             * D3..10   Maximum number of view components minus 1
             * D11..31  Reserved
             */
            uint32_t bmMVCCapabilities() const;

            /**
             * @brief dwMinBitRate
             * @return minimum bit rate in units of bps
             */
            uint32_t dwMinBitRate() const;

            /**
             * @brief dwMaxBitRate
             * @return maximum bit rate in units of bps
             */
            uint32_t dwMaxBitRate() const;

            /**
             * @brief dwDefaultFrameInterval
             * @return the frame interval the device indicates for use as a default, in 100-ns units
             */
            uint32_t dwDefaultFrameInterval() const;

            /**
             * @brief bNumFrameIntervals
             * @return the number of frame intervals supported
             */
            uint8_t bNumFrameIntervals() const;

            /**
             * @brief dwFrameInterval
             * @param index
             * @note
             * The Spec defines the first dwFrameInterval is dwFrameInterval(1),
             * therefore the parameter 'index' should also start with 1.
             * @return the index-th shortest frame interval supported (at index-th highest frame rate),
             * in 100 ns units.
             */
            uint32_t dwFrameInterval(uint8_t index) const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbH264PayloadVideoFrameDescriptor(UsbInterfaceDescriptor *parent,
                                                        uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __strWProfile() const;
            inline QString __strBLevelIDC() const;
            inline QString __parseBmSupportedUsages() const;
            inline QString __parseBmCapabilities() const;
            inline QString __parseBmSVCCapabilities() const;
            inline QString __parseBmMVCCapabilities() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFrameIndex,
            _bLevelIDC, _bNumFrameIntervals;
            uint16_t _wWidth, _wHeight, _wSARwidth, _wSARheight, _wProfile,
            _wConstrainedToolset, _bmCapabilities;
            uint32_t _bmSupportedUsages, _bmSVCCapabilities, _bmMVCCapabilities,
            _dwMinBitRate, _dwMaxBitRate, _dwDefaultFrameInterval;
            QVector<uint32_t> _dwFrameInterval;
        };

        /**
         * @brief The UsbVp8PayloadVideoFormatDescriptor class
         * VP8 Payload Video Format Descriptor C++ wrapper
         */
        class UsbVp8PayloadVideoFormatDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @return descriptor subtype, 0x16 or 0x18 here
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFormatIndex
             * @return index of this format descriptor
             */
            uint8_t bFormatIndex() const;

            /**
             * @brief bNumFrameDescriptors
             * @return number of Frame Descriptors following that correspond to this format
             */
            uint8_t bNumFrameDescriptors() const;

            /**
             * @brief bDefaultFrameIndex
             * @return default frame index
             */
            uint8_t bDefaultFrameIndex() const;

            /**
             * @brief bMaxCodecConfigDelay
             * @return maximum number of frames the encoder takes to respond to a command
             */
            uint8_t bMaxCodecConfigDelay() const;

            /**
             * @brief bSupportedPartitionCount
             * @return maximum number of supported partitions per frame
             */
            uint8_t bSupportedPartitionCount() const;

            /**
             * @brief bmSupportedSyncFrameTypes
             * @return bitmap indicating supported sync frame types.
             * @note
             * D0       Reset
             * D1       Intra frame
             * D2       Golden frame
             * D3       Alternate reference frame
             * D4       Gradual Decoder Refresh frames
             * D5..7    Reserved
             */
            uint8_t bmSupportedSyncFrameTypes() const;

            /**
             * @brief bResolutionScaling
             * @return the support for resolution downsizing
             * @note
             * 0:       Not supported
             * 1:       Limited to 1.5 or 2.0 scaling in both directions,
             *              while maintaining the aspect ratio
             * 2:       Limited to 1.0, 1.5 or 2.0 scaling in either direction
             * 3:       Limited to resolutions reported by the associated Frame Descriptors
             * 4:       Arbitrary scaling
             * 5-255:   Reserved
             */
            uint8_t bResolutionScaling() const;

            /**
             * @brief bmSupportedRateControlModes
             * @return bitmatp indicating supported rate-control modes
             * @note
             * D0       Variable bit rate (VBR)
             * D1       Constant bit rate (CBR)
             * D2       Constant QP
             * D3       Global VBR
             * D4..7    Reserved
             */
            uint8_t bmSupportedRateControlModes() const;

            /**
             * @brief wMaxMBperSec
             * @return maximum macroblock processing rate, in units of MB/s
             */
            uint16_t wMaxMBperSec() const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbVp8PayloadVideoFormatDescriptor(UsbInterfaceDescriptor *parent,
                                                        uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmSupportedSyncFrameTypes() const;
            inline QString __strBResolutionScaling() const;
            inline QString __parseBmSupportedRateControlModes() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFormatIndex,
            _bNumFrameDescriptors, _bDefaultFrameIndex, _bMaxCodecConfigDelay,
            _bSupportedPartitionCount, _bmSupportedSyncFrameTypes,
            _bResolutionScaling, _bmSupportedRateControlModes;
            uint16_t _wMaxMBperSec;
        };

        /**
         * @brief The UsbVp8PayloadVideoFrameDescriptor class
         * VP8 Payload Video Frame Descriptor C++ wrapper
         */
        class UsbVp8PayloadVideoFrameDescriptor : public UsbVideoStreamInterfaceDescriptor
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
             * @return descriptor subtype, 0x17 here
             * @see UsbVideoStreamInterfaceDescriptor::strSubtype()
             */
            uint8_t bDescriptorSubtype() const override;

            /**
             * @brief bFrameIndex
             * @return index of this frame descriptor
             */
            uint8_t bFrameIndex() const;

            /**
             * @brief wWidth
             * @return the width of pictures output from the decoding process, in pixels
             */
            uint16_t wWidth() const;

            /**
             * @brief wHeight
             * @return the height of pictures output from the decoding process, in pixels
             */
            uint16_t wHeight() const;

            /**
             * @brief bmSupportedUsages
             * @return bitmap that a bit set to 1 indicates that
             * the mentioned usage is supported.
             * @note
             * D0       Reserved
             * D1       Real-time
             * D2       Real-time with temporal layering structure
             * D3..15   Reserved
             * D16      File Storage mode with I and P frames
             * D17      Reserved
             * D18      File storage all-I-frame mode
             * D19..31  Reserved
             */
            uint32_t bmSupportedUsages() const;

            /**
             * @brief bmCapabilities
             * @return bitmap that a bit set to 1 indicates that
             * the mentioned Capability is supported.
             * @note
             * D0..1    Reserved
             * D2       Constant frame rate
             * D3       Separate QP for luma/chroma
             * D4..5    Reserved
             * D6       Golden frame
             * D7       Alternate reference frame
             * D8..15   Reserved
             */
            uint16_t bmCapabilities() const;

            /**
             * @brief bmScalabilityCapabilities
             * @return bitmap indicating the scalability capabilities
             * @note
             * D0..2    maximum number of temporal enhancement layers minus 1
             */
            uint32_t bmScalabilityCapabilities() const;

            /**
             * @brief dwMinBitRate
             * @return minimum bit rate in units of bps
             */
            uint32_t dwMinBitRate() const;

            /**
             * @brief dwMaxBitRate
             * @return maximum bit rate in units of bps
             */
            uint32_t dwMaxBitRate() const;

            /**
             * @brief dwDefaultFrameInterval
             * @return the frame interval the device indicates for use as a default, in 100-ns units
             */
            uint32_t dwDefaultFrameInterval() const;

            /**
             * @brief bNumFrameIntervals
             * @return the number of frame intervals supported
             */
            uint8_t bNumFrameIntervals() const;

            /**
             * @brief dwFrameInterval
             * @param index
             * @note
             * The Spec defines the first dwFrameInterval is dwFrameInterval(1),
             * therefore the parameter 'index' should also start with 1.
             * @return the index-th shortest frame interval supported (at index-th highest frame rate),
             * in 100 ns units.
             */
            uint32_t dwFrameInterval(uint8_t index) const;

            /**
             * @brief getFieldNames
             * @return all field names in a string list
             */
            static const QStringList &getFieldNames();

            /**
             * @brief getFieldInformation
             * Get field information
             * @param field name
             * @return field information almost from USB specs
             */
            static QString getFieldInformation(const QString &field);

            /**
             * @brief infomationToHtml
             * @return HTML form informations
             */
            QString infomationToHtml() const override;

        private:
            explicit UsbVp8PayloadVideoFrameDescriptor(UsbInterfaceDescriptor *parent,
                                                       uint32_t descPos);
            friend UsbVideoStreamInterfaceDescriptor;
            inline QString __parseBmSupportedUsages() const;
            inline QString __parseBmCapabilities() const;
            inline QString __parseBmScalabilityCapabilities() const;

            uint8_t _bLength, _bDescriptorType, _bDescriptorSubtype, _bFrameIndex,
            _bLevelIDC, _bNumFrameIntervals;
            uint16_t _wWidth, _wHeight, _bmCapabilities;
            uint32_t _bmSupportedUsages, _bmScalabilityCapabilities,
            _dwMinBitRate, _dwMaxBitRate, _dwDefaultFrameInterval;
            QVector<uint32_t> _dwFrameInterval;
        };
    } // namespace uvc
} // namespace usb

#endif // USB_UVC_USBVIDEOSTREAMINTERFACEDESCRIPTOR_H
