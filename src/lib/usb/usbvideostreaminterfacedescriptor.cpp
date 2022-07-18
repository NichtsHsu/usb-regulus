#include "usbvideostreaminterfacedescriptor.h"
#include "usbendpointdescriptor.h"
#include "__usbmacro.h"

namespace usb {
    namespace uvc {
        UsbVideoStreamInterfaceDescriptor::UsbVideoStreamInterfaceDescriptor(
                UsbInterfaceDescriptor *parent):
            UsbInterfaceExtraDescriptor(parent)
        {

        }

        InterfaceExtraDescriptorType UsbVideoStreamInterfaceDescriptor::type() const
        {
            return InterfaceExtraDescriptorType::VS;
        }

        QString UsbVideoStreamInterfaceDescriptor::strSubtype(uint8_t subtype)
        {
            static const QMap<uint8_t, const char *> subtypes = {
                { 0x00, "VS_UNDEFINED" },
                { 0x01, "VS_INPUT_HEADER" },
                { 0x02, "VS_OUTPUT_HEADER" },
                { 0x03, "VS_STILL_IMAGE_FRAME" },
                { 0x04, "VS_FORMAT_UNCOMPRESSED" },
                { 0x05, "VS_FRAME_UNCOMPRESSED" },
                { 0x06, "VS_FORMAT_MJPEG" },
                { 0x07, "VS_FRAME_MJPEG" },
                { 0x0A, "VS_FORMAT_MPEG2TS" },
                { 0x0C, "VS_FORMAT_DV" },
                { 0x0D, "VS_COLORFORMAT" },
                { 0x10, "VS_FORMAT_FRAME_BASED" },
                { 0x11, "VS_FRAME_FRAME_BASED" },
                { 0x12, "VS_FORMAT_STREAM_BASED" },
                { 0x13, "VS_FORMAT_H264" },
                { 0x14, "VS_FRAME_H264" },
                { 0x15, "VS_FORMAT_H264_SIMULCAST" },
                { 0x16, "VS_FORMAT_VP8" },
                { 0x17, "VS_FRAME_VP8" },
                { 0x18, "VS_FORMAT_VP8_SIMULCAST" },
            };

            return subtypes.value(subtype, "VS_UNDEFINED");
        }

        UsbVideoStreamInterfaceDescriptor *UsbVideoStreamInterfaceDescriptor::get(
                UsbInterfaceDescriptor *interfaceDescriptor, uint8_t descPos)
        {
            const unsigned char *extra = interfaceDescriptor->extra() + descPos;
            if (extra[1] != 0x24)
                return nullptr;

            UsbVideoStreamInterfaceDescriptor *desc;
            switch (extra[2])
            {
                case 0x01:
                    /* VS_INPUT_HEADER */
                    desc = new UsbInputHeaderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x02:
                    /* VS_OUTPUT_HEADER */
                    desc = new UsbOutputHeaderDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x03:
                    /* VS_STILL_IMAGE_FRAME */
                    desc = new UsbStillImageFrameDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x04:
                    /* VS_FORMAT_UNCOMPRESSED */
                    desc = new UsbUncompressedVideoFormatDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x05:
                    /* VS_FRAME_UNCOMPRESSED */
                    desc = new UsbUncompressedVideoFrameDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x06:
                    /* VS_FORMAT_MJPEG */
                    desc = new UsbMotionJpegVideoFormatDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x07:
                    /* VS_FRAME_MJPEG */
                    desc = new UsbMotionJpegVideoFrameDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x0A:
                    /* VS_FORMAT_MPEG2TS */
                    desc = new UsbMpeg2TsFormatDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x0C:
                    /* VS_FORMAT_DV */
                    desc = new UsbDvFormatDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x0D:
                    /* VS_COLORFORMAT */
                    desc = new UsbColorMatchingDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x10:
                    /* VS_FORMAT_FRAME_BASED */
                    desc = new UsbFrameBasedPayloadVideoFormatDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x11:
                    /* VS_FRAME_FRAME_BASED */
                    desc = new UsbFrameBasedPayloadVideoFrameDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x12:
                    /* VS_FORMAT_STREAM_BASED */
                    desc = new UsbFrameBasedPayloadVideoFrameDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x13:
                    /* VS_FORMAT_H264 */
                case 0x15:
                    /* VS_FORMAT_H264_SIMULCAST */
                    desc = new UsbH264PayloadVideoFormatDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x14:
                    /* VS_FRAME_H264 */
                    desc = new UsbH264PayloadVideoFrameDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x16:
                    /* VS_FORMAT_VP8 */
                case 0x18:
                    /* VS_FORMAT_VP8_SIMULCAST */
                    desc = new UsbVp8PayloadVideoFormatDescriptor(
                                interfaceDescriptor, descPos);
                break;
                case 0x17:
                    /* VS_FRAME_VP8 */
                    desc = new UsbVp8PayloadVideoFrameDescriptor(
                                interfaceDescriptor, descPos);
                break;
                default:
                    desc = nullptr;
                break;
            }

            return desc;
        }

        uint8_t UsbInputHeaderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbInputHeaderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbInputHeaderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbInputHeaderDescriptor::bNumFormats() const
        {
            return _bNumFormats;
        }

        uint16_t UsbInputHeaderDescriptor::wTotalLength() const
        {
            return _wTotalLength;
        }

        uint8_t UsbInputHeaderDescriptor::bEndpointAddress() const
        {
            return _bEndpointAddress;
        }

        uint8_t UsbInputHeaderDescriptor::bmInfo() const
        {
            return _bmInfo;
        }

        uint8_t UsbInputHeaderDescriptor::bTerminalLink() const
        {
            return _bTerminalLink;
        }

        uint8_t UsbInputHeaderDescriptor::bStillCaptureMethod() const
        {
            return _bStillCaptureMethod;
        }

        uint8_t UsbInputHeaderDescriptor::bTriggerSupport() const
        {
            return _bTriggerSupport;
        }

        uint8_t UsbInputHeaderDescriptor::bTriggerUsage() const
        {
            return _bTriggerUsage;
        }

        uint8_t UsbInputHeaderDescriptor::bControlSize() const
        {
            return _bControlSize;
        }

        QByteArray UsbInputHeaderDescriptor::bmaControls(uint8_t index) const
        {
            if (index > 0 && index <= _bNumFormats)
                return _bmaControls[index - 1];
            else
            {
                LOGE(tr("Index %1 for bmaControls out of range, must be 1~%2.")
                     .arg(index).arg(_bNumFormats));
                return 0;
            }
        }

        QString UsbInputHeaderDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Input Header Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bNumFormats", _bNumFormats, _bNumFormats);
            ATTR("bEndpointAddress", _bEndpointAddress, tr("Endpoint %1 %2")
                 .arg(_bEndpointAddress & 0x0F)
                 .arg(strEndpointDirection(EndpointDirection(_bEndpointAddress & int(EndpointDirection::IN)))));
            ATTR("bmInfo", _bmInfo, _bmInfo ?
                     tr("Supports Dynamic Format Change"): QString());
            ATTR("bTerminalLink", _bTerminalLink, _bTerminalLink);
            ATTR("bStillCaptureMethod", _bStillCaptureMethod, __parseBStillCaptureMethod());
            ATTR("bTriggerSupport", _bTriggerSupport, _bTriggerSupport ?
                     tr("Supports hardware triggering") : QString());
            ATTR("bTriggerUsage", _bTriggerUsage, _bTriggerSupport ?
                     (_bTriggerUsage ?
                          tr("General purpose button event"):
                          tr("Initiate still image capture")):
                     QString());
            ATTR("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize));
            for (uint8_t i = 1; i <= _bNumFormats; ++i)
                ATTRCUSTOM(QString("bmaControls(%1)").arg(i),
                           QString("0x") + bmaControls(i).toHex(),
                           __parseBmaControls(i));
            END;

            return html;
        }

        UsbInputHeaderDescriptor::UsbInputHeaderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bNumFormats = *(extra + 3);
            _wTotalLength = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bEndpointAddress = *(extra + 6);
            _bmInfo = *(extra + 7);
            _bTerminalLink = *(extra + 8);
            _bStillCaptureMethod = *(extra + 9);
            _bTriggerSupport = *(extra + 10);
            _bTriggerUsage = *(extra + 11);
            _bControlSize = *(extra + 12);
            _bmaControls.reserve(_bNumFormats);
            for (uint8_t i = 0; i < _bNumFormats; ++i)
                _bmaControls.append(QByteArray(reinterpret_cast<const char *>(extra + 13 + i * _bControlSize),
                                               _bControlSize));
        }

        QString UsbInputHeaderDescriptor::__parseBStillCaptureMethod() const
        {
            switch (_bStillCaptureMethod)
            {
                case 1:
                return tr("Method 1");
                case 2:
                return tr("Method 2");
                case 3:
                return tr("Method 3");
                default:
                return tr("None");
            }
        }

        QString UsbInputHeaderDescriptor::__parseBmaControls(uint8_t index) const
        {
            QByteArray bmControls = bmaControls(index);
            QStringList supports;
            if (BIT(bmControls[0], 0))
                supports.append(tr("wKeyFrameRate field"));
            if (BIT(bmControls[0], 1))
                supports.append(tr("wPFrameRate field"));
            if (BIT(bmControls[0], 2))
                supports.append(tr("wCompQuality field"));
            if (BIT(bmControls[0], 3))
                supports.append(tr("wCompWindowSize field"));
            if (BIT(bmControls[0], 4))
                supports.append(tr("Generate Key Frame Control"));
            if (BIT(bmControls[0], 5))
                supports.append(tr("Update Frame Segment Control"));

            return supports.join(NEWLINE);
        }

        uint8_t UsbOutputHeaderDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbOutputHeaderDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbOutputHeaderDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbOutputHeaderDescriptor::bNumFormats() const
        {
            return _bNumFormats;
        }

        uint16_t UsbOutputHeaderDescriptor::wTotalLength() const
        {
            return _wTotalLength;
        }

        uint8_t UsbOutputHeaderDescriptor::bEndpointAddress() const
        {
            return _bEndpointAddress;
        }

        uint8_t UsbOutputHeaderDescriptor::bTerminalLink() const
        {
            return _bTerminalLink;
        }

        uint8_t UsbOutputHeaderDescriptor::bControlSize() const
        {
            return _bControlSize;
        }

        QByteArray UsbOutputHeaderDescriptor::bmaControls(uint8_t index) const
        {
            if (index > 0 && index <= _bNumFormats)
                return _bmaControls[index - 1];
            else
            {
                LOGE(tr("Index %1 for bmaControls out of range, must be 1~%2.")
                     .arg(index).arg(_bNumFormats));
                return 0;
            }
        }

        QString UsbOutputHeaderDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Output Header Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bNumFormats", _bNumFormats, _bNumFormats);
            ATTR("bEndpointAddress", _bEndpointAddress, tr("Endpoint %1 %2")
                 .arg(_bEndpointAddress & 0x0F)
                 .arg(strEndpointDirection(EndpointDirection(_bEndpointAddress & int(EndpointDirection::IN)))));
            ATTR("bTerminalLink", _bTerminalLink, _bTerminalLink);
            ATTR("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize));
            for (uint8_t i = 1; i <= _bNumFormats; ++i)
                ATTRCUSTOM(QString("bmaControls(%1)").arg(i),
                           QString("0x") + bmaControls(i).toHex(),
                           __parseBmaControls(i));
            END;

            return html;
        }

        UsbOutputHeaderDescriptor::UsbOutputHeaderDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bNumFormats = *(extra + 3);
            _wTotalLength = *reinterpret_cast<const uint16_t *>(extra + 4);
            _bEndpointAddress = *(extra + 6);
            _bTerminalLink = *(extra + 7);
            _bControlSize = *(extra + 8);
            _bmaControls.reserve(_bNumFormats);
            for (uint8_t i = 0; i < _bNumFormats; ++i)
                _bmaControls.append(QByteArray(reinterpret_cast<const char *>(extra + 9 + i * _bControlSize),
                                               _bControlSize));
        }

        QString UsbOutputHeaderDescriptor::__parseBmaControls(uint8_t index) const
        {
            QByteArray bmControls = bmaControls(index);
            QStringList supports;
            if (BIT(bmControls[0], 0))
                supports.append(tr("wKeyFrameRate field"));
            if (BIT(bmControls[0], 1))
                supports.append(tr("wPFrameRate field"));
            if (BIT(bmControls[0], 2))
                supports.append(tr("wCompQuality field"));
            if (BIT(bmControls[0], 3))
                supports.append(tr("wCompWindowSize field"));

            return supports.join(NEWLINE);
        }

        uint8_t UsbStillImageFrameDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbStillImageFrameDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbStillImageFrameDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbStillImageFrameDescriptor::bEndpointAddress() const
        {
            return _bEndpointAddress;
        }

        uint8_t UsbStillImageFrameDescriptor::bNumImageSizePatterns() const
        {
            return _bNumImageSizePatterns;
        }

        uint16_t UsbStillImageFrameDescriptor::wWidth(uint8_t index) const
        {
            if (index > 0 && index <= _bNumImageSizePatterns)
                return _wWidth[index - 1];
            else
            {
                LOGE(tr("Index %1 for wWidth out of range, must be 1~%2.")
                     .arg(index).arg(_bNumImageSizePatterns));
                return 0;
            }
        }

        uint16_t UsbStillImageFrameDescriptor::wHeight(uint8_t index) const
        {
            if (index > 0 && index <= _bNumImageSizePatterns)
                return _wHeight[index - 1];
            else
            {
                LOGE(tr("Index %1 for wHeight out of range, must be 1~%2.")
                     .arg(index).arg(_bNumImageSizePatterns));
                return 0;
            }
        }

        uint8_t UsbStillImageFrameDescriptor::bNumCompressionPattern() const
        {
            return _bNumCompressionPattern;
        }

        uint8_t UsbStillImageFrameDescriptor::bCompression(uint8_t index) const
        {
            if (index > 0 && index <= _bNumCompressionPattern)
                return _bCompression[index - 1];
            else
            {
                LOGE(tr("Index %1 for bCompression out of range, must be 1~%2.")
                     .arg(index).arg(_bNumCompressionPattern));
                return 0;
            }
        }

        QString UsbStillImageFrameDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Still Image Frame Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bEndpointAddress", _bEndpointAddress, tr("Endpoint %1 %2")
                 .arg(_bEndpointAddress & 0x0F)
                 .arg(strEndpointDirection(EndpointDirection(_bEndpointAddress & int(EndpointDirection::IN)))));
            ATTR("bNumImageSizePatterns", _bNumImageSizePatterns, _bNumImageSizePatterns);
            for (uint8_t i = 1; i <= _bNumImageSizePatterns; ++i)
            {
                ATTR(QString("wWidth(%1)").arg(i), wWidth(i), wWidth(i));
                ATTR(QString("wHeight(%1)").arg(i), wHeight(i), wHeight(i));
            }
            ATTR("bNumCompressionPattern", _bNumCompressionPattern, _bNumCompressionPattern);
            for (uint8_t i = 1; i <= _bNumCompressionPattern; ++i)
                ATTR(QString("bCompression(%1)").arg(i), bCompression(i), bCompression(i));
            END;

            return html;
        }

        UsbStillImageFrameDescriptor::UsbStillImageFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bEndpointAddress = *(extra + 3);
            _bNumImageSizePatterns = *(extra + 4);
            _wWidth.reserve(_bNumImageSizePatterns);
            _wHeight.reserve(_bNumImageSizePatterns);
            for (uint8_t i = 0; i < _bNumImageSizePatterns; ++i)
            {
                _wWidth.append(*reinterpret_cast<const uint16_t *>(extra + 5 + 4 * i));
                _wHeight.append(*reinterpret_cast<const uint16_t *>(extra + 7 + 4 * i));
            }
            _bNumCompressionPattern = *(extra + 5 + 4 * _bNumImageSizePatterns);
            _bCompression.reserve(_bNumCompressionPattern);
            for (uint8_t i = 0; i < _bNumCompressionPattern; ++i)
                _bCompression.append(*(extra + 6 + 4 * _bNumImageSizePatterns + i));
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bFormatIndex() const
        {
            return _bFormatIndex;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bNumFrameDescriptors() const
        {
            return _bNumFrameDescriptors;
        }

        QByteArray UsbUncompressedVideoFormatDescriptor::guidFormat() const
        {
            return _guidFormat;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bBitsPerPixel() const
        {
            return _bBitsPerPixel;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bDefaultFrameIndex() const
        {
            return _bDefaultFrameIndex;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bAspectRatioX() const
        {
            return _bAspectRatioX;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bAspectRatioY() const
        {
            return _bAspectRatioY;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bmInterlaceFlags() const
        {
            return _bmInterlaceFlags;
        }

        uint8_t UsbUncompressedVideoFormatDescriptor::bCopyProtect() const
        {
            return _bCopyProtect;
        }

        QString UsbUncompressedVideoFormatDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Uncompressed Video Format Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFormatIndex", _bFormatIndex, _bFormatIndex);
            ATTR("bNumFrameDescriptors", _bNumFrameDescriptors, _bNumFrameDescriptors);
            ATTRTEXT("guidFormat", hexUuid(_guidFormat));
            ATTR("bBitsPerPixel", _bBitsPerPixel, tr("%1 bit(s) per pixel").arg(_bBitsPerPixel));
            ATTR("bDefaultFrameIndex", _bDefaultFrameIndex, _bDefaultFrameIndex);
            ATTR("bAspectRatioX", _bAspectRatioX, _bAspectRatioX);
            ATTR("bAspectRatioY", _bAspectRatioY, _bAspectRatioY);
            ATTR("bmInterlaceFlags", _bmInterlaceFlags, __parseBmInterlaceFlags());
            ATTR("bCopyProtect", _bCopyProtect, _bCopyProtect ? tr("Restrict duplication") : "");
            END;

            return html;
        }

        UsbUncompressedVideoFormatDescriptor::UsbUncompressedVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatIndex = *(extra + 3);
            _bNumFrameDescriptors = *(extra + 4);
            _guidFormat.reserve(16);
            for (uint8_t i = 5; i < 21; ++i)
                _guidFormat.append(*(extra + i));
            _bBitsPerPixel = *(extra + 21);
            _bDefaultFrameIndex = *(extra + 22);
            _bAspectRatioX = *(extra + 23);
            _bAspectRatioY = *(extra + 24);
            _bmInterlaceFlags = *(extra + 25);
            _bCopyProtect = *(extra + 26);
        }

        QString UsbUncompressedVideoFormatDescriptor::__parseBmInterlaceFlags() const
        {
            QStringList infomations;
            if (BIT(_bmInterlaceFlags, 0))
                infomations.append(tr("Interlaced stream or variable"));
            if (BIT(_bmInterlaceFlags, 1))
                infomations.append(tr("1 field per frame"));
            else
                infomations.append(tr("2 fields per frame"));
            if (BIT(_bmInterlaceFlags, 2))
                infomations.append(tr("Field 1 first"));
            switch (CUT(_bmInterlaceFlags, 4, 5))
            {
                case 0b00:
                    infomations.append(tr("Field 1 only"));
                break;
                case 0b01:
                    infomations.append(tr("Field 2 only"));
                break;
                case 0b10:
                    infomations.append(tr("Regular pattern of fields 1 and 2"));
                break;
                case 0b11:
                    infomations.append(tr("Random pattern of fields 1 and 2"));
                break;
            }

            return infomations.join(NEWLINE);
        }

        uint8_t UsbUncompressedVideoFrameDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbUncompressedVideoFrameDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbUncompressedVideoFrameDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbUncompressedVideoFrameDescriptor::bFrameIndex() const
        {
            return _bFrameIndex;
        }

        uint8_t UsbUncompressedVideoFrameDescriptor::bmCapabilities() const
        {
            return _bmCapabilities;
        }

        uint16_t UsbUncompressedVideoFrameDescriptor::wWidth() const
        {
            return _wWidth;
        }

        uint16_t UsbUncompressedVideoFrameDescriptor::wHeight() const
        {
            return _wHeight;
        }

        uint32_t UsbUncompressedVideoFrameDescriptor::dwMinBitRate() const
        {
            return _dwMinBitRate;
        }

        uint32_t UsbUncompressedVideoFrameDescriptor::dwMaxBitRate() const
        {
            return _dwMaxBitRate;
        }

        uint32_t UsbUncompressedVideoFrameDescriptor::dwMaxVideoFrameBufferSize() const
        {
            return _dwMaxVideoFrameBufferSize;
        }

        uint32_t UsbUncompressedVideoFrameDescriptor::dwDefaultFrameInterval() const
        {
            return _dwDefaultFrameInterval;
        }

        uint8_t UsbUncompressedVideoFrameDescriptor::bFrameIntervalType() const
        {
            return _bFrameIntervalType;
        }

        uint32_t UsbUncompressedVideoFrameDescriptor::dwMinFrameInterval() const
        {
            if (_bFrameIntervalType == 0)
                return _dwFrameInterval[0];
            else
            {
                LOGE(tr("The dwMinFrameInterval field is only valid when the bFrameIntervalType field is 0."));
                return 0;
            }
        }

        uint32_t UsbUncompressedVideoFrameDescriptor::dwMaxFrameInterval() const
        {
            if (_bFrameIntervalType == 0)
                return _dwFrameInterval[1];
            else
            {
                LOGE(tr("The dwMinFrameInterval field is only valid when the bFrameIntervalType field is 0."));
                return 0;
            }
        }

        uint32_t UsbUncompressedVideoFrameDescriptor::dwFrameIntervalStep() const
        {
            if (_bFrameIntervalType == 0)
                return _dwFrameIntervalStep;
            else
            {
                LOGE(tr("The dwFrameIntervalStep field is only valid when the bFrameIntervalType field is 0."));
                return 0;
            }
        }

        uint32_t UsbUncompressedVideoFrameDescriptor::dwFrameInterval(uint8_t index) const
        {
            if (_bFrameIntervalType == 0)
            {
                LOGE(tr("The dwFrameInterval field is only valid when the bFrameIntervalType field is NOT 0."));
                return 0;
            }
            else if (index > 0 && index <= _bFrameIntervalType)
                return _dwFrameInterval[index - 1];
            else
            {
                LOGE(tr("Index %1 for dwFrameInterval out of range, must be 1~%2.")
                     .arg(index).arg(_bFrameIntervalType));
                return 0;
            }
        }

        QString UsbUncompressedVideoFrameDescriptor::__parseBmCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmCapabilities, 0))
                supports.append(tr("Still image supported"));
            if (BIT(_bmCapabilities, 1))
                supports.append(tr("Fixed frame-rate"));

            return supports.join(NEWLINE);
        }

        QString UsbUncompressedVideoFrameDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Uncompressed Video Frame Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFrameIndex", _bFrameIndex, _bFrameIndex);
            ATTR("bmCapabilities", _bmCapabilities, __parseBmCapabilities());
            ATTR("wWidth", _wWidth, _wWidth);
            ATTR("wHeight", _wHeight, _wHeight);
            ATTR("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate));
            ATTR("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate));
            ATTR("dwMaxVideoFrameBufferSize", _dwMaxVideoFrameBufferSize,
                 tr("%1 byte(s)").arg(_dwMaxVideoFrameBufferSize));
            ATTR("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                 tr("%1 * 100 ns").arg(_dwDefaultFrameInterval));
            ATTR("bFrameIntervalType", _bFrameIntervalType, _bFrameIntervalType ?
                     tr("%1 discrete frame intervals").arg(_bFrameIntervalType) :
                     tr("Continuous frame interval"));
            if (_bFrameIntervalType)
            {
                for (uint8_t i = 1; i <= _bFrameIntervalType; ++i)
                    ATTR(QString("dwFrameInterval(%1)").arg(i), dwFrameInterval(i),
                         tr("%1 * 100 ns").arg(dwFrameInterval(i)));
            }
            else
            {
                ATTR("dwMinFrameInterval", dwMinFrameInterval(),
                     tr("%1 * 100 ns").arg(dwMinFrameInterval()));
                ATTR("dwMaxFrameInterval", dwMaxFrameInterval(),
                     tr("%1 * 100 ns").arg(dwMaxFrameInterval()));
                ATTR("dwFrameIntervalStep", _dwFrameIntervalStep,
                     tr("%1 * 100 ns").arg(_dwFrameIntervalStep));
            }
            END;

            return html;
        }

        UsbUncompressedVideoFrameDescriptor::UsbUncompressedVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFrameIndex = *(extra + 3);
            _bmCapabilities = *(extra + 4);
            _wWidth = *reinterpret_cast<const uint16_t *>(extra + 5);
            _wHeight = *reinterpret_cast<const uint16_t *>(extra + 7);
            _dwMinBitRate = *reinterpret_cast<const uint32_t *>(extra + 9);
            _dwMaxBitRate = *reinterpret_cast<const uint32_t *>(extra + 13);
            _dwMaxVideoFrameBufferSize = *reinterpret_cast<const uint32_t *>(extra + 17);
            _dwDefaultFrameInterval = *reinterpret_cast<const uint32_t *>(extra + 21);
            _bFrameIntervalType = *(extra + 25);
            if (_bFrameIntervalType)
            {
                _dwFrameInterval.reserve(_bFrameIntervalType);
                for (uint8_t i = 0; i < _bFrameIntervalType; ++i)
                    _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 26 + i * 4));
            }
            else
            {
                _dwFrameInterval.reserve(2);
                _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 26));
                _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 30));
                _dwFrameIntervalStep = *reinterpret_cast<const uint32_t *>(extra + 34);
            }
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bFormatIndex() const
        {
            return _bFormatIndex;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bNumFrameDescriptors() const
        {
            return _bNumFrameDescriptors;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bmFlags() const
        {
            return _bmFlags;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bDefaultFrameIndex() const
        {
            return _bDefaultFrameIndex;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bAspectRatioX() const
        {
            return _bAspectRatioX;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bAspectRatioY() const
        {
            return _bAspectRatioY;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bmInterlaceFlags() const
        {
            return _bmInterlaceFlags;
        }

        uint8_t UsbMotionJpegVideoFormatDescriptor::bCopyProtect() const
        {
            return _bCopyProtect;
        }

        QString UsbMotionJpegVideoFormatDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Motion-JPEG Video Format Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFormatIndex", _bFormatIndex, _bFormatIndex);
            ATTR("bNumFrameDescriptors", _bNumFrameDescriptors, _bNumFrameDescriptors);
            ATTR("bmFlags", _bmFlags, BIT(_bmFlags, 0) ? tr("Fixed Size Samples"): "");
            ATTR("bDefaultFrameIndex", _bDefaultFrameIndex, _bDefaultFrameIndex);
            ATTR("bAspectRatioX", _bAspectRatioX, _bAspectRatioX);
            ATTR("bAspectRatioY", _bAspectRatioY, _bAspectRatioY);
            ATTR("bmInterlaceFlags", _bmInterlaceFlags, __parseBmInterlaceFlags());
            ATTR("bCopyProtect", _bCopyProtect, _bCopyProtect ? tr("Restrict duplication") : "");
            END;

            return html;
        }

        UsbMotionJpegVideoFormatDescriptor::UsbMotionJpegVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatIndex = *(extra + 3);
            _bNumFrameDescriptors = *(extra + 4);
            _bmFlags = *(extra + 5);
            _bDefaultFrameIndex = *(extra + 6);
            _bAspectRatioX = *(extra + 7);
            _bAspectRatioY = *(extra + 8);
            _bmInterlaceFlags = *(extra + 9);
            _bCopyProtect = *(extra + 10);
        }

        QString UsbMotionJpegVideoFormatDescriptor::__parseBmInterlaceFlags() const
        {
            QStringList infomations;
            if (BIT(_bmInterlaceFlags, 0))
                infomations.append(tr("Interlaced stream or variable"));
            if (BIT(_bmInterlaceFlags, 1))
                infomations.append(tr("1 field per frame"));
            else
                infomations.append(tr("2 fields per frame"));
            if (BIT(_bmInterlaceFlags, 2))
                infomations.append(tr("Field 1 first"));
            switch (CUT(_bmInterlaceFlags, 4, 5))
            {
                case 0b00:
                    infomations.append(tr("Field 1 only"));
                break;
                case 0b01:
                    infomations.append(tr("Field 2 only"));
                break;
                case 0b10:
                    infomations.append(tr("Regular pattern of fields 1 and 2"));
                break;
                case 0b11:
                    infomations.append(tr("Random pattern of fields 1 and 2"));
                break;
            }

            return infomations.join(NEWLINE);
        }

        uint8_t UsbMotionJpegVideoFrameDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbMotionJpegVideoFrameDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbMotionJpegVideoFrameDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbMotionJpegVideoFrameDescriptor::bFrameIndex() const
        {
            return _bFrameIndex;
        }

        uint8_t UsbMotionJpegVideoFrameDescriptor::bmCapabilities() const
        {
            return _bmCapabilities;
        }

        uint16_t UsbMotionJpegVideoFrameDescriptor::wWidth() const
        {
            return _wWidth;
        }

        uint16_t UsbMotionJpegVideoFrameDescriptor::wHeight() const
        {
            return _wHeight;
        }

        uint32_t UsbMotionJpegVideoFrameDescriptor::dwMinBitRate() const
        {
            return _dwMinBitRate;
        }

        uint32_t UsbMotionJpegVideoFrameDescriptor::dwMaxBitRate() const
        {
            return _dwMaxBitRate;
        }

        uint32_t UsbMotionJpegVideoFrameDescriptor::dwMaxVideoFrameBufferSize() const
        {
            return _dwMaxVideoFrameBufferSize;
        }

        uint32_t UsbMotionJpegVideoFrameDescriptor::dwDefaultFrameInterval() const
        {
            return _dwDefaultFrameInterval;
        }

        uint8_t UsbMotionJpegVideoFrameDescriptor::bFrameIntervalType() const
        {
            return _bFrameIntervalType;
        }

        uint32_t UsbMotionJpegVideoFrameDescriptor::dwMinFrameInterval() const
        {
            if (_bFrameIntervalType == 0)
                return _dwFrameInterval[0];
            else
            {
                LOGE(tr("The dwMinFrameInterval field is only valid when the bFrameIntervalType field is 0."));
                return 0;
            }
        }

        uint32_t UsbMotionJpegVideoFrameDescriptor::dwMaxFrameInterval() const
        {
            if (_bFrameIntervalType == 0)
                return _dwFrameInterval[1];
            else
            {
                LOGE(tr("The dwMinFrameInterval field is only valid when the bFrameIntervalType field is 0."));
                return 0;
            }
        }

        uint32_t UsbMotionJpegVideoFrameDescriptor::dwFrameIntervalStep() const
        {
            if (_bFrameIntervalType == 0)
                return _dwFrameIntervalStep;
            else
            {
                LOGE(tr("The dwFrameIntervalStep field is only valid when the bFrameIntervalType field is 0."));
                return 0;
            }
        }

        uint32_t UsbMotionJpegVideoFrameDescriptor::dwFrameInterval(uint8_t index) const
        {
            if (_bFrameIntervalType == 0)
            {
                LOGE(tr("The dwFrameInterval field is only valid when the bFrameIntervalType field is NOT 0."));
                return 0;
            }
            else if (index > 0 && index <= _bFrameIntervalType)
                return _dwFrameInterval[index - 1];
            else
            {
                LOGE(tr("Index %1 for dwFrameInterval out of range, must be 1~%2.")
                     .arg(index).arg(_bFrameIntervalType));
                return 0;
            }
        }

        QString UsbMotionJpegVideoFrameDescriptor::__parseBmCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmCapabilities, 0))
                supports.append(tr("Still image supported"));
            if (BIT(_bmCapabilities, 1))
                supports.append(tr("Fixed frame-rate"));

            return supports.join(NEWLINE);
        }

        QString UsbMotionJpegVideoFrameDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Motion-JPEG Video Frame Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFrameIndex", _bFrameIndex, _bFrameIndex);
            ATTR("bmCapabilities", _bmCapabilities, __parseBmCapabilities());
            ATTR("wWidth", _wWidth, _wWidth);
            ATTR("wHeight", _wHeight, _wHeight);
            ATTR("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate));
            ATTR("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate));
            ATTR("dwMaxVideoFrameBufferSize", _dwMaxVideoFrameBufferSize,
                 tr("%1 byte(s)").arg(_dwMaxVideoFrameBufferSize));
            ATTR("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                 tr("%1 * 100 ns").arg(_dwDefaultFrameInterval));
            ATTR("bFrameIntervalType", _bFrameIntervalType, _bFrameIntervalType ?
                     tr("%1 discrete frame intervals").arg(_bFrameIntervalType) :
                     tr("Continuous frame interval"));
            if (_bFrameIntervalType)
            {
                for (uint8_t i = 1; i <= _bFrameIntervalType; ++i)
                    ATTR(QString("dwFrameInterval(%1)").arg(i), dwFrameInterval(i),
                         tr("%1 * 100 ns").arg(dwFrameInterval(i)));
            }
            else
            {
                ATTR("dwMinFrameInterval", dwMinFrameInterval(),
                     tr("%1 * 100 ns").arg(dwMinFrameInterval()));
                ATTR("dwMaxFrameInterval", dwMaxFrameInterval(),
                     tr("%1 * 100 ns").arg(dwMaxFrameInterval()));
                ATTR("dwFrameIntervalStep", _dwFrameIntervalStep,
                     tr("%1 * 100 ns").arg(_dwFrameIntervalStep));
            }
            END;

            return html;
        }

        UsbMotionJpegVideoFrameDescriptor::UsbMotionJpegVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFrameIndex = *(extra + 3);
            _bmCapabilities = *(extra + 4);
            _wWidth = *reinterpret_cast<const uint16_t *>(extra + 5);
            _wHeight = *reinterpret_cast<const uint16_t *>(extra + 7);
            _dwMinBitRate = *reinterpret_cast<const uint32_t *>(extra + 9);
            _dwMaxBitRate = *reinterpret_cast<const uint32_t *>(extra + 13);
            _dwMaxVideoFrameBufferSize = *reinterpret_cast<const uint32_t *>(extra + 17);
            _dwDefaultFrameInterval = *reinterpret_cast<const uint32_t *>(extra + 21);
            _bFrameIntervalType = *(extra + 25);
            if (_bFrameIntervalType)
            {
                _dwFrameInterval.reserve(_bFrameIntervalType);
                for (uint8_t i = 0; i < _bFrameIntervalType; ++i)
                    _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 26 + i * 4));
            }
            else
            {
                _dwFrameInterval.reserve(2);
                _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 26));
                _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 30));
                _dwFrameIntervalStep = *reinterpret_cast<const uint32_t *>(extra + 34);
            }
        }

        uint8_t UsbMpeg2TsFormatDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbMpeg2TsFormatDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbMpeg2TsFormatDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbMpeg2TsFormatDescriptor::bFormatIndex() const
        {
            return _bFormatIndex;
        }

        uint8_t UsbMpeg2TsFormatDescriptor::bDataOffset() const
        {
            return _bDataOffset;
        }

        uint8_t UsbMpeg2TsFormatDescriptor::bPacketLength() const
        {
            return _bPacketLength;
        }

        uint8_t UsbMpeg2TsFormatDescriptor::bStrideLength() const
        {
            return _bStrideLength;
        }

        QByteArray UsbMpeg2TsFormatDescriptor::guidStrideFormat() const
        {
            return _guidStrideFormat;
        }

        QString UsbMpeg2TsFormatDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("MPEG-2 TS Format Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFormatIndex", _bFormatIndex, _bFormatIndex);
            ATTR("bDataOffset", _bDataOffset, _bDataOffset);
            ATTR("bPacketLength", _bPacketLength, _bPacketLength);
            ATTR("bStrideLength", _bStrideLength, _bStrideLength);
            ATTRCUSTOM("guidStrideFormat", "", hexUuid(_guidStrideFormat));
            END;

            return html;
        }

        UsbMpeg2TsFormatDescriptor::UsbMpeg2TsFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatIndex = *(extra + 3);
            _bDataOffset = *(extra + 4);
            _bPacketLength = *(extra + 5);
            _bStrideLength = *(extra + 6);
            _guidStrideFormat.reserve(16);
            for (int i = 7; i < 23; ++i)
                _guidStrideFormat.append(*(extra + i));
        }

        uint8_t UsbDvFormatDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbDvFormatDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbDvFormatDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbDvFormatDescriptor::bFormatIndex() const
        {
            return _bFormatIndex;
        }

        uint32_t UsbDvFormatDescriptor::dwMaxVideoFrameBufferSize() const
        {
            return _dwMaxVideoFrameBufferSize;
        }

        uint8_t UsbDvFormatDescriptor::bFormatType() const
        {
            return _bFormatType;
        }

        QString UsbDvFormatDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("DV Format Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFormatIndex", _bFormatIndex, _bFormatIndex);
            ATTR("dwMaxVideoFrameBufferSize", _dwMaxVideoFrameBufferSize,
                 tr("%1 byte(s)").arg(_dwMaxVideoFrameBufferSize));
            ATTR("bFormatType", _bFormatType, __parseBFormatType());
            END;

            return html;
        }

        UsbDvFormatDescriptor::UsbDvFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatIndex = *(extra + 3);
            _dwMaxVideoFrameBufferSize = *reinterpret_cast<const uint32_t *>(extra + 4);
            _bFormatType = *(extra + 8);
        }

        QString UsbDvFormatDescriptor::__parseBFormatType() const
        {
            QString type;
            if (BIT(_bFormatType, 7))
                type = tr("60 Hz");
            else
                type = tr("50 Hz");
            switch (CUT(_bFormatType, 0, 6))
            {
                case 0:
                    type += " SD-DV";
                break;
                case 1:
                    type += " SDL-DV";
                break;
                case 2:
                    type += " HD-DV";
                break;
            }

            return type;
        }

        uint8_t UsbColorMatchingDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbColorMatchingDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbColorMatchingDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbColorMatchingDescriptor::bColorPrimaries() const
        {
            return _bColorPrimaries;
        }

        uint8_t UsbColorMatchingDescriptor::bTransferCharacteristics() const
        {
            return _bTransferCharacteristics;
        }

        uint8_t UsbColorMatchingDescriptor::bMatrixCoefficients() const
        {
            return _bMatrixCoefficients;
        }

        QString UsbColorMatchingDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Color Matching Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bColorPrimaries", _bColorPrimaries, __strBColorPrimaries());
            ATTR("bTransferCharacteristics", _bTransferCharacteristics, __strBTransferCharacteristics());
            ATTR("bMatrixCoefficients", _bMatrixCoefficients, __strBMatrixCoefficients());
            END;

            return html;
        }

        UsbColorMatchingDescriptor::UsbColorMatchingDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bColorPrimaries = *(extra + 3);
            _bTransferCharacteristics = *(extra + 4);
            _bMatrixCoefficients = *(extra + 5);
        }

        QString UsbColorMatchingDescriptor::__strBColorPrimaries() const
        {
            switch (_bColorPrimaries)
            {
                case 0:
                return tr("Unspecified");
                case 1:
                return "BT.709, sRGB";
                case 2:
                return "BT.470-2 (M)";
                case 3:
                return "BT.470-2 (B, G)";
                case 4:
                return "SMPTE 170M";
                case 5:
                return "SMPTE 240M";
            }

            return QString();
        }

        QString UsbColorMatchingDescriptor::__strBTransferCharacteristics() const
        {
            switch (_bTransferCharacteristics)
            {
                case 0:
                return tr("Unspecified");
                case 1:
                return "BT.709";
                case 2:
                return "BT.470-2 M";
                case 3:
                return "BT.470-2 B, G";
                case 4:
                return "SMPTE 170M";
                case 5:
                return "SMPTE 240M";
                case 6:
                return "Linear";
                case 7:
                return "sRGB";
            }

            return QString();
        }

        QString UsbColorMatchingDescriptor::__strBMatrixCoefficients() const
        {
            switch (_bMatrixCoefficients)
            {
                case 0:
                return tr("Unspecified");
                case 1:
                return "BT.709";
                case 2:
                return "FCC";
                case 3:
                return "BT.470-2 B, G";
                case 4:
                return "SMPTE 170M";
                case 5:
                return "SMPTE 240M";
            }

            return QString();
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bFormatIndex() const
        {
            return _bFormatIndex;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bNumFrameDescriptors() const
        {
            return _bNumFrameDescriptors;
        }

        QByteArray UsbFrameBasedPayloadVideoFormatDescriptor::guidFormat() const
        {
            return _guidFormat;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bBitsPerPixel() const
        {
            return _bBitsPerPixel;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bDefaultFrameIndex() const
        {
            return _bDefaultFrameIndex;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bAspectRatioX() const
        {
            return _bAspectRatioX;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bAspectRatioY() const
        {
            return _bAspectRatioY;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bmInterlaceFlags() const
        {
            return _bmInterlaceFlags;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bCopyProtect() const
        {
            return _bCopyProtect;
        }

        uint8_t UsbFrameBasedPayloadVideoFormatDescriptor::bVariableSize() const
        {
            return _bVariableSize;
        }

        QString UsbFrameBasedPayloadVideoFormatDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Frame Based Payload Video Format Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFormatIndex", _bFormatIndex, _bFormatIndex);
            ATTR("bNumFrameDescriptors", _bNumFrameDescriptors, _bNumFrameDescriptors);
            ATTRTEXT("guidFormat", hexUuid(_guidFormat));
            ATTR("bBitsPerPixel", _bBitsPerPixel, tr("%1 bit(s) per pixel").arg(_bBitsPerPixel));
            ATTR("bDefaultFrameIndex", _bDefaultFrameIndex, _bDefaultFrameIndex);
            ATTR("bAspectRatioX", _bAspectRatioX, _bAspectRatioX);
            ATTR("bAspectRatioY", _bAspectRatioY, _bAspectRatioY);
            ATTR("bmInterlaceFlags", _bmInterlaceFlags, __parseBmInterlaceFlags());
            ATTR("bCopyProtect", _bCopyProtect, _bCopyProtect ? tr("Restrict duplication") : "");
            ATTR("bVariableSize", _bVariableSize, _bVariableSize ? tr("Variable Size") : tr("Fixed Size"));
            END;

            return html;
        }

        UsbFrameBasedPayloadVideoFormatDescriptor::UsbFrameBasedPayloadVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatIndex = *(extra + 3);
            _bNumFrameDescriptors = *(extra + 4);
            _guidFormat.reserve(16);
            for (uint8_t i = 5; i < 21; ++i)
                _guidFormat.append(*(extra + i));
            _bBitsPerPixel = *(extra + 21);
            _bDefaultFrameIndex = *(extra + 22);
            _bAspectRatioX = *(extra + 23);
            _bAspectRatioY = *(extra + 24);
            _bmInterlaceFlags = *(extra + 25);
            _bCopyProtect = *(extra + 26);
            _bVariableSize = *(extra + 27);
        }

        QString UsbFrameBasedPayloadVideoFormatDescriptor::__parseBmInterlaceFlags() const
        {
            QStringList infomations;
            if (BIT(_bmInterlaceFlags, 0))
                infomations.append(tr("Interlaced stream or variable"));
            if (BIT(_bmInterlaceFlags, 1))
                infomations.append(tr("1 field per frame"));
            else
                infomations.append(tr("2 fields per frame"));
            if (BIT(_bmInterlaceFlags, 2))
                infomations.append(tr("Field 1 first"));
            switch (CUT(_bmInterlaceFlags, 4, 5))
            {
                case 0b00:
                    infomations.append(tr("Field 1 only"));
                break;
                case 0b01:
                    infomations.append(tr("Field 2 only"));
                break;
                case 0b10:
                    infomations.append(tr("Regular pattern of fields 1 and 2"));
                break;
                case 0b11:
                    infomations.append(tr("Random pattern of fields 1 and 2"));
                break;
            }

            return infomations.join(NEWLINE);
        }

        uint8_t UsbFrameBasedPayloadVideoFrameDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbFrameBasedPayloadVideoFrameDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbFrameBasedPayloadVideoFrameDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbFrameBasedPayloadVideoFrameDescriptor::bFrameIndex() const
        {
            return _bFrameIndex;
        }

        uint8_t UsbFrameBasedPayloadVideoFrameDescriptor::bmCapabilities() const
        {
            return _bmCapabilities;
        }

        uint16_t UsbFrameBasedPayloadVideoFrameDescriptor::wWidth() const
        {
            return _wWidth;
        }

        uint16_t UsbFrameBasedPayloadVideoFrameDescriptor::wHeight() const
        {
            return _wHeight;
        }

        uint32_t UsbFrameBasedPayloadVideoFrameDescriptor::dwMinBitRate() const
        {
            return _dwMinBitRate;
        }

        uint32_t UsbFrameBasedPayloadVideoFrameDescriptor::dwMaxBitRate() const
        {
            return _dwMaxBitRate;
        }

        uint32_t UsbFrameBasedPayloadVideoFrameDescriptor::dwDefaultFrameInterval() const
        {
            return _dwDefaultFrameInterval;
        }

        uint8_t UsbFrameBasedPayloadVideoFrameDescriptor::bFrameIntervalType() const
        {
            return _bFrameIntervalType;
        }

        uint32_t UsbFrameBasedPayloadVideoFrameDescriptor::dwBytesPerLine() const
        {
            return _dwBytesPerLine;
        }

        uint32_t UsbFrameBasedPayloadVideoFrameDescriptor::dwMinFrameInterval() const
        {
            if (_bFrameIntervalType == 0)
                return _dwFrameInterval[0];
            else
            {
                LOGE(tr("The dwMinFrameInterval field is only valid when the bFrameIntervalType field is 0."));
                return 0;
            }
        }

        uint32_t UsbFrameBasedPayloadVideoFrameDescriptor::dwMaxFrameInterval() const
        {
            if (_bFrameIntervalType == 0)
                return _dwFrameInterval[1];
            else
            {
                LOGE(tr("The dwMinFrameInterval field is only valid when the bFrameIntervalType field is 0."));
                return 0;
            }
        }

        uint32_t UsbFrameBasedPayloadVideoFrameDescriptor::dwFrameIntervalStep() const
        {
            if (_bFrameIntervalType == 0)
                return _dwFrameIntervalStep;
            else
            {
                LOGE(tr("The dwFrameIntervalStep field is only valid when the bFrameIntervalType field is 0."));
                return 0;
            }
        }

        uint32_t UsbFrameBasedPayloadVideoFrameDescriptor::dwFrameInterval(uint8_t index) const
        {
            if (_bFrameIntervalType == 0)
            {
                LOGE(tr("The dwFrameInterval field is only valid when the bFrameIntervalType field is NOT 0."));
                return 0;
            }
            else if (index > 0 && index <= _bFrameIntervalType)
                return _dwFrameInterval[index - 1];
            else
            {
                LOGE(tr("Index %1 for dwFrameInterval out of range, must be 1~%2.")
                     .arg(index).arg(_bFrameIntervalType));
                return 0;
            }
        }

        QString UsbFrameBasedPayloadVideoFrameDescriptor::__parseBmCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmCapabilities, 0))
                supports.append(tr("Still image supported"));
            if (BIT(_bmCapabilities, 1))
                supports.append(tr("Fixed frame-rate"));

            return supports.join(NEWLINE);
        }

        QString UsbFrameBasedPayloadVideoFrameDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Frame Based Payload Video Frame Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFrameIndex", _bFrameIndex, _bFrameIndex);
            ATTR("bmCapabilities", _bmCapabilities, __parseBmCapabilities());
            ATTR("wWidth", _wWidth, _wWidth);
            ATTR("wHeight", _wHeight, _wHeight);
            ATTR("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate));
            ATTR("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate));
            ATTR("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                 tr("%1 * 100 ns").arg(_dwDefaultFrameInterval));
            ATTR("bFrameIntervalType", _bFrameIntervalType, _bFrameIntervalType ?
                     tr("%1 discrete frame intervals").arg(_bFrameIntervalType) :
                     tr("Continuous frame interval"));
            ATTR("dwBytesPerLine", _dwBytesPerLine, tr("%1 byte(s)").arg(_dwBytesPerLine));
            if (_bFrameIntervalType)
            {
                for (uint8_t i = 1; i <= _bFrameIntervalType; ++i)
                    ATTR(QString("dwFrameInterval(%1)").arg(i), dwFrameInterval(i),
                         tr("%1 * 100 ns").arg(dwFrameInterval(i)));
            }
            else
            {
                ATTR("dwMinFrameInterval", dwMinFrameInterval(),
                     tr("%1 * 100 ns").arg(dwMinFrameInterval()));
                ATTR("dwMaxFrameInterval", dwMaxFrameInterval(),
                     tr("%1 * 100 ns").arg(dwMaxFrameInterval()));
                ATTR("dwFrameIntervalStep", _dwFrameIntervalStep,
                     tr("%1 * 100 ns").arg(_dwFrameIntervalStep));
            }
            END;

            return html;
        }

        UsbFrameBasedPayloadVideoFrameDescriptor::UsbFrameBasedPayloadVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFrameIndex = *(extra + 3);
            _bmCapabilities = *(extra + 4);
            _wWidth = *reinterpret_cast<const uint16_t *>(extra + 5);
            _wHeight = *reinterpret_cast<const uint16_t *>(extra + 7);
            _dwMinBitRate = *reinterpret_cast<const uint32_t *>(extra + 9);
            _dwMaxBitRate = *reinterpret_cast<const uint32_t *>(extra + 13);
            _dwDefaultFrameInterval = *reinterpret_cast<const uint32_t *>(extra + 17);
            _bFrameIntervalType = *(extra + 21);
            _dwBytesPerLine = *reinterpret_cast<const uint32_t *>(extra + 22);
            if (_bFrameIntervalType)
            {
                _dwFrameInterval.reserve(_bFrameIntervalType);
                for (uint8_t i = 0; i < _bFrameIntervalType; ++i)
                    _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 26 + i * 4));
            }
            else
            {
                _dwFrameInterval.reserve(2);
                _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 26));
                _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 30));
                _dwFrameIntervalStep = *reinterpret_cast<const uint32_t *>(extra + 34);
            }
        }

        uint8_t UsbStreamBasedFormatDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbStreamBasedFormatDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbStreamBasedFormatDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbStreamBasedFormatDescriptor::bFormatIndex() const
        {
            return _bFormatIndex;
        }

        QByteArray UsbStreamBasedFormatDescriptor::guidFormat() const
        {
            return _guidFormat;
        }

        uint16_t UsbStreamBasedFormatDescriptor::dwPacketLength() const
        {
            return _dwPacketLength;
        }

        QString UsbStreamBasedFormatDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("Stream Based Format Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFormatIndex", _bFormatIndex, _bFormatIndex);
            ATTRTEXT("guidFormat", hexUuid(_guidFormat));
            ATTR("dwPacketLength", _dwPacketLength, _dwPacketLength);
            END;

            return html;
        }

        UsbStreamBasedFormatDescriptor::UsbStreamBasedFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatIndex = *(extra + 3);
            _guidFormat.reserve(16);
            for (uint8_t i = 4; i < 20; ++i)
                _guidFormat.append(*(extra + i));
            _dwPacketLength = *reinterpret_cast<const uint32_t *>(extra + 20);
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bFormatIndex() const
        {
            return _bFormatIndex;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bNumFrameDescriptors() const
        {
            return _bNumFrameDescriptors;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bDefaultFrameIndex() const
        {
            return _bDefaultFrameIndex;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bMaxCodecConfigDelay() const
        {
            return _bMaxCodecConfigDelay;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bmSupportedSliceModes() const
        {
            return _bmSupportedSliceModes;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bmSupportedSyncFrameTypes() const
        {
            return _bmSupportedSyncFrameTypes;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bResolutionScaling() const
        {
            return _bResolutionScaling;
        }

        uint8_t UsbH264PayloadVideoFormatDescriptor::bmSupportedRateControlModes() const
        {
            return _bmSupportedRateControlModes;
        }

        uint16_t UsbH264PayloadVideoFormatDescriptor::wMaxMBperSecResolutionNoScalability(uint8_t index) const
        {
            if (index > 0 && index <= 4)
                return _wMaxMBperSecResolutionNoScalability[index - 1];
            else
            {
                LOGE(tr("Index %1 for wMaxMBperSecResolutionNoScalability out of range, must be 1~4.")
                     .arg(index));
                return 0;
            }
        }

        uint16_t UsbH264PayloadVideoFormatDescriptor::wMaxMBperSecResolutionTemporalScalability(uint8_t index) const
        {
            if (index > 0 && index <= 4)
                return _wMaxMBperSecResolutionTemporalScalability[index - 1];
            else
            {
                LOGE(tr("Index %1 for wMaxMBperSecResolutionTemporalScalability out of range, must be 1~4.")
                     .arg(index));
                return 0;
            }
        }

        uint16_t UsbH264PayloadVideoFormatDescriptor::wMaxMBperSecResolutionTemporalQualityScalability(uint8_t index) const
        {
            if (index > 0 && index <= 4)
                return _wMaxMBperSecResolutionTemporalQualityScalability[index - 1];
            else
            {
                LOGE(tr("Index %1 for wMaxMBperSecResolutionTemporalQualityScalability out of range, must be 1~4.")
                     .arg(index));
                return 0;
            }
        }

        uint16_t UsbH264PayloadVideoFormatDescriptor::wMaxMBperSecResolutionsTemporalSpatialScalability(uint8_t index) const
        {
            if (index > 0 && index <= 4)
                return _wMaxMBperSecResolutionsTemporalSpatialScalability[index - 1];
            else
            {
                LOGE(tr("Index %1 for wMaxMBperSecResolutionsTemporalSpatialScalability out of range, must be 1~4.")
                     .arg(index));
                return 0;
            }
        }

        uint16_t UsbH264PayloadVideoFormatDescriptor::wMaxMBperSecResolutionFullScalability(uint8_t index) const
        {
            if (index > 0 && index <= 4)
                return _wMaxMBperSecResolutionFullScalability[index - 1];
            else
            {
                LOGE(tr("Index %1 for wMaxMBperSecResolutionFullScalability out of range, must be 1~4.")
                     .arg(index));
                return 0;
            }
        }

        QString UsbH264PayloadVideoFormatDescriptor::infomationToHtml() const
        {
            static constexpr const char *const order[] = {
                "One", "Two", "Three", "Four"
            };

            QString html;
            START(tr("H.264 Payload Video Format Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFormatIndex", _bFormatIndex, _bFormatIndex);
            ATTR("bNumFrameDescriptors", _bNumFrameDescriptors, _bNumFrameDescriptors);
            ATTR("bMaxCodecConfigDelay", _bMaxCodecConfigDelay, tr("%1 frame(s)").arg(_bMaxCodecConfigDelay));
            ATTR("bmSupportedSliceModes", _bmSupportedSliceModes, __parseBmSupportedSliceModes());
            ATTR("bmSupportedSyncFrameTypes", _bmSupportedSyncFrameTypes, __parseBmSupportedSyncFrameTypes());
            ATTR("bResolutionScaling", _bResolutionScaling, __strBResolutionScaling());
            ATTR("bmSupportedRateControlModes", _bmSupportedRateControlModes, __parseBmSupportedRateControlModes());
            for (uint8_t i = 0; i < 4; ++i)
                ATTR(QString("wMaxMBperSec%1ResolutionNoScalability").arg(order[i]),
                     _wMaxMBperSecResolutionNoScalability[i],
                     tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionNoScalability[i]));
            for (uint8_t i = 0; i < 4; ++i)
                ATTR(QString("wMaxMBperSec%1ResolutionTemporalScalability").arg(order[i]),
                     _wMaxMBperSecResolutionTemporalScalability[i],
                     tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionTemporalScalability[i]));
            for (uint8_t i = 0; i < 4; ++i)
                ATTR(QString("wMaxMBperSec%1ResolutionTemporalQualityScalability").arg(order[i]),
                     _wMaxMBperSecResolutionTemporalQualityScalability[i],
                     tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionTemporalQualityScalability[i]));
            for (uint8_t i = 0; i < 4; ++i)
                ATTR(QString("wMaxMBperSec%1ResolutionsTemporalSpatialScalability").arg(order[i]),
                     _wMaxMBperSecResolutionsTemporalSpatialScalability[i],
                     tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionsTemporalSpatialScalability[i]));
            for (uint8_t i = 0; i < 4; ++i)
                ATTR(QString("wMaxMBperSec%1ResolutionFullScalability").arg(order[i]),
                     _wMaxMBperSecResolutionFullScalability[i],
                     tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionFullScalability[i]));
            END;

            return html;
        }

        UsbH264PayloadVideoFormatDescriptor::UsbH264PayloadVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatIndex = *(extra + 3);
            _bNumFrameDescriptors = *(extra + 4);
            _bDefaultFrameIndex = *(extra + 5);
            _bMaxCodecConfigDelay = *(extra + 6);
            _bmSupportedSliceModes = *(extra + 7);
            _bmSupportedSyncFrameTypes = *(extra + 8);
            _bResolutionScaling = *(extra + 9);
            _bmSupportedRateControlModes = *(extra + 11);
            for (uint8_t i = 0; i < 4; ++i)
            {
                _wMaxMBperSecResolutionNoScalability[i] =
                        *reinterpret_cast<const uint16_t *>(extra + 12 + i * 2);
                _wMaxMBperSecResolutionTemporalScalability[i] =
                        *reinterpret_cast<const uint16_t *>(extra + 20 + i * 2);
                _wMaxMBperSecResolutionTemporalQualityScalability[i] =
                        *reinterpret_cast<const uint16_t *>(extra + 28 + i * 2);
                _wMaxMBperSecResolutionsTemporalSpatialScalability[i] =
                        *reinterpret_cast<const uint16_t *>(extra + 36 + i * 2);
                _wMaxMBperSecResolutionFullScalability[i] =
                        *reinterpret_cast<const uint16_t *>(extra + 44 + i * 2);
            }
        }

        QString UsbH264PayloadVideoFormatDescriptor::__parseBmSupportedSliceModes() const
        {
            if (_bmSupportedSliceModes == 0)
                return tr("Only one slice per frame mode");

            QStringList supports;
            if (BIT(_bmSupportedSliceModes, 0))
                supports.append(tr("Maximum number of MBs per slice mode"));
            if (BIT(_bmSupportedSliceModes, 1))
                supports.append(tr("Target compressed size per slice mode"));
            if (BIT(_bmSupportedSliceModes, 2))
                supports.append(tr("Number of slices per frame mode"));
            if (BIT(_bmSupportedSliceModes, 3))
                supports.append(tr("Number of Macroblock rows per slice mode"));

            return supports.join(NEWLINE);
        }

        QString UsbH264PayloadVideoFormatDescriptor::__parseBmSupportedSyncFrameTypes() const
        {
            QStringList supports;
            if (BIT(_bmSupportedSyncFrameTypes, 0))
                supports.append(tr("Reset"));
            if (BIT(_bmSupportedSyncFrameTypes, 1))
                supports.append(tr("IDR frame with SPS and PPS headers"));
            if (BIT(_bmSupportedSyncFrameTypes, 2))
                supports.append(tr("long term reference IDR frame"));
            if (BIT(_bmSupportedSyncFrameTypes, 3))
                supports.append(tr("Non-IDR random-access I frame"));
            if (BIT(_bmSupportedSyncFrameTypes, 4))
                supports.append(tr("Generate a long term reference random-access I frame"));
            if (BIT(_bmSupportedSyncFrameTypes, 5))
                supports.append(tr("long term reference P frame"));
            if (BIT(_bmSupportedSyncFrameTypes, 6))
                supports.append(tr("Gradual Decoder Refresh frames"));

            return supports.join(NEWLINE);
        }

        QString UsbH264PayloadVideoFormatDescriptor::__strBResolutionScaling() const
        {
            switch (_bResolutionScaling)
            {
                case 0:
                return tr("Not supported");
                case 1:
                return tr("1.5 or 2.0 scaling");
                case 2:
                return tr("1.0, 1.5 or 2.0 scaling");
                case 3:
                return tr("Resolutions by associated Frame Descriptors");
                case 4:
                return tr("Arbitrary scaling");
            }

            return QString();
        }

        QString UsbH264PayloadVideoFormatDescriptor::__parseBmSupportedRateControlModes() const
        {
            QStringList supports;
            if (BIT(_bmSupportedRateControlModes, 0))
                supports.append(tr("VBR with underflow allowed"));
            if (BIT(_bmSupportedRateControlModes, 1))
                supports.append(tr("CBR"));
            if (BIT(_bmSupportedRateControlModes, 2))
                supports.append(tr("Constant QP"));
            if (BIT(_bmSupportedRateControlModes, 3))
                supports.append(tr("Global VBR with underflow allowed"));
            if (BIT(_bmSupportedRateControlModes, 4))
                supports.append(tr("VBR without underflow"));
            if (BIT(_bmSupportedRateControlModes, 5))
                supports.append(tr("Global VBR without underflow"));

            return supports.join(NEWLINE);
        }

        uint8_t UsbH264PayloadVideoFrameDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbH264PayloadVideoFrameDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbH264PayloadVideoFrameDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbH264PayloadVideoFrameDescriptor::bFrameIndex() const
        {
            return _bFrameIndex;
        }

        uint16_t UsbH264PayloadVideoFrameDescriptor::wWidth() const
        {
            return _wWidth;
        }

        uint16_t UsbH264PayloadVideoFrameDescriptor::wHeight() const
        {
            return _wHeight;
        }

        uint16_t UsbH264PayloadVideoFrameDescriptor::wSARwidth() const
        {
            return _wSARwidth;
        }

        uint16_t UsbH264PayloadVideoFrameDescriptor::wSARheight() const
        {
            return _wSARheight;
        }

        uint16_t UsbH264PayloadVideoFrameDescriptor::wProfile() const
        {
            return _wProfile;
        }

        uint8_t UsbH264PayloadVideoFrameDescriptor::bLevelIDC() const
        {
            return _bLevelIDC;
        }

        uint16_t UsbH264PayloadVideoFrameDescriptor::wConstrainedToolset() const
        {
            return _wConstrainedToolset;
        }

        uint32_t UsbH264PayloadVideoFrameDescriptor::bmSupportedUsages() const
        {
            return _bmSupportedUsages;
        }

        uint16_t UsbH264PayloadVideoFrameDescriptor::bmCapabilities() const
        {
            return _bmCapabilities;
        }

        uint32_t UsbH264PayloadVideoFrameDescriptor::bmSVCCapabilities() const
        {
            return _bmSVCCapabilities;
        }

        uint32_t UsbH264PayloadVideoFrameDescriptor::bmMVCCapabilities() const
        {
            return _bmMVCCapabilities;
        }

        uint32_t UsbH264PayloadVideoFrameDescriptor::dwMinBitRate() const
        {
            return _dwMinBitRate;
        }

        uint32_t UsbH264PayloadVideoFrameDescriptor::dwMaxBitRate() const
        {
            return _dwMaxBitRate;
        }

        uint32_t UsbH264PayloadVideoFrameDescriptor::dwDefaultFrameInterval() const
        {
            return _dwDefaultFrameInterval;
        }

        uint8_t UsbH264PayloadVideoFrameDescriptor::bNumFrameIntervals() const
        {
            return _bNumFrameIntervals;
        }

        uint32_t UsbH264PayloadVideoFrameDescriptor::dwFrameInterval(uint8_t index) const
        {
            if (index > 0 && index <= _bNumFrameIntervals)
                return _dwFrameInterval[index - 1];
            else
            {
                LOGE(tr("Index %1 for dwFrameInterval out of range, must be 1~%2.")
                     .arg(index).arg(_bNumFrameIntervals));
                return 0;
            }
        }

        QString UsbH264PayloadVideoFrameDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("H.264 Payload Video Frame Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFrameIndex", _bFrameIndex, _bFrameIndex);
            ATTR("wWidth", _wWidth, _wWidth);
            ATTR("wHeight", _wHeight, _wHeight);
            ATTR("wSARwidth", _wSARwidth, _wSARwidth);
            ATTR("wSARheight", _wSARheight, _wSARheight);
            ATTR("wProfile", _wProfile, __strWProfile());
            ATTR("bLevelIDC", _bLevelIDC, __strBLevelIDC());
            ATTR("wConstrainedToolset", _wConstrainedToolset, _wConstrainedToolset);
            ATTR("bmSupportedUsages", _bmSupportedUsages, __parseBmSupportedUsages());
            ATTR("bmCapabilities", _bmCapabilities, __parseBmCapabilities());
            ATTR("bmSVCCapabilities", _bmSVCCapabilities, __parseBmSVCCapabilities());
            ATTR("bmMVCCapabilities", _bmMVCCapabilities, __parseBmMVCCapabilities());
            ATTR("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate));
            ATTR("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate));
            ATTR("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                 tr("%1 * 100 ns").arg(_dwDefaultFrameInterval));
            ATTR("bNumFrameIntervals", _bNumFrameIntervals, _bNumFrameIntervals);
            for (uint8_t i = 1; i <= _bNumFrameIntervals; ++i)
                ATTR(QString("dwFrameInterval(%1)").arg(i), dwFrameInterval(i),
                     tr("%1 * 100 ns").arg(dwFrameInterval(i)));
            END;

            return html;
        }

        UsbH264PayloadVideoFrameDescriptor::UsbH264PayloadVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFrameIndex = *(extra + 3);
            _wWidth = *reinterpret_cast<const uint16_t *>(extra + 4);
            _wHeight = *reinterpret_cast<const uint16_t *>(extra + 6);
            _wSARwidth = *reinterpret_cast<const uint16_t *>(extra + 8);
            _wSARheight = *reinterpret_cast<const uint16_t *>(extra + 10);
            _wProfile = *reinterpret_cast<const uint16_t *>(extra + 12);
            _bLevelIDC = *(extra + 14);
            _wConstrainedToolset = *reinterpret_cast<const uint16_t *>(extra + 15);
            _bmSupportedUsages = *reinterpret_cast<const uint32_t *>(extra + 17);
            _bmCapabilities = *reinterpret_cast<const uint16_t *>(extra + 21);
            _bmSVCCapabilities = *reinterpret_cast<const uint32_t *>(extra + 23);
            _bmMVCCapabilities = *reinterpret_cast<const uint32_t *>(extra + 27);
            _dwMinBitRate = *reinterpret_cast<const uint32_t *>(extra + 31);
            _dwMaxBitRate = *reinterpret_cast<const uint32_t *>(extra + 35);
            _dwDefaultFrameInterval = *reinterpret_cast<const uint32_t *>(extra + 39);
            _bNumFrameIntervals = *(extra + 43);
            _dwFrameInterval.reserve(_bNumFrameIntervals);
            for (uint8_t i = 0; i < _bNumFrameIntervals; ++i)
                _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 44 + i * 4));
        }

        QString UsbH264PayloadVideoFrameDescriptor::__strWProfile() const
        {
            const uint8_t profile_idc = (_wProfile >> 8) & 0xFF;
            const bool constraint_set_flag[6] = {
                BIT(_wProfile, 7),
                BIT(_wProfile, 6),
                BIT(_wProfile, 5),
                BIT(_wProfile, 4),
                BIT(_wProfile, 3),
                BIT(_wProfile, 2),
            };

            switch (profile_idc)
            {
                case 44:
                return tr("CAVLC 4:4:4 Intra profile");
                case 66:
                    if (constraint_set_flag[1])
                        return tr("Constrained Baseline profile");
                    else
                        return tr("Baseline profile");
                case 77:
                return tr("Main profile");
                case 88:
                return tr("Extended profile");
                case 100:
                    if (constraint_set_flag[4] && constraint_set_flag[5])
                        return tr("Constrained High profile");
                    else if (constraint_set_flag[4])
                        return tr("Progressive High profile");
                    else
                        return tr("High profile");
                case 110:
                    if (constraint_set_flag[4])
                        return tr("Progressive High 10 profile");
                    else if (constraint_set_flag[3])
                        return tr("High 10 Intra profile");
                    else
                        return tr("High 10 profile");
                case 122:
                    if (constraint_set_flag[3])
                        return tr("High 4:2:2 Intra profile");
                    else
                        return tr("High 4:2:2 profile");
                case 244:
                    if (constraint_set_flag[3])
                        return tr("High 4:4:4 Intra profile");
                    else
                        return tr("High 4:4:4 Predictive profile");
                default:
                return QString();
            }
        }

        QString UsbH264PayloadVideoFrameDescriptor::__strBLevelIDC() const
        {
            const uint8_t major = _bLevelIDC / 10;
            const uint8_t minor = _bLevelIDC % 10;
            return tr("Level %1.%2").arg(major).arg(minor);
        }

        QString UsbH264PayloadVideoFrameDescriptor::__parseBmSupportedUsages() const
        {
            QStringList supports;
            if (BIT(_bmSupportedUsages, 0))
                supports.append(tr("Real-time/UCConfig mode 0"));
            if (BIT(_bmSupportedUsages, 1))
                supports.append(tr("Real-time/UCConfig mode 1"));
            if (BIT(_bmSupportedUsages, 2))
                supports.append(tr("Real-time/UCConfig mode 2Q"));
            if (BIT(_bmSupportedUsages, 3))
                supports.append(tr("Real-time/UCConfig mode 2S"));
            if (BIT(_bmSupportedUsages, 4))
                supports.append(tr("Real-time/UCConfig mode 3"));
            supports.append(tr("Broadcast modes: %1")
                            .arg(CUT(_bmSupportedUsages, 8, 15)));
            if (BIT(_bmSupportedUsages, 16))
                supports.append(tr("File Storage mode with I and P slices"));
            if (BIT(_bmSupportedUsages, 17))
                supports.append(tr("File Storage mode with I, P, and B slices"));
            if (BIT(_bmSupportedUsages, 18))
                supports.append(tr("File storage all-I-frame mode"));
            if (BIT(_bmSupportedUsages, 24))
                supports.append(tr("MVC Stereo High Mode"));
            if (BIT(_bmSupportedUsages, 25))
                supports.append(tr("MVC Multiview Mode"));

            return supports.join(NEWLINE);
        }

        QString UsbH264PayloadVideoFrameDescriptor::__parseBmCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmCapabilities, 0))
                supports.append(tr("CAVLC only"));
            if (BIT(_bmCapabilities, 1))
                supports.append(tr("CABAC only"));
            if (BIT(_bmCapabilities, 2))
                supports.append(tr("Constant frame rate"));
            if (BIT(_bmCapabilities, 3))
                supports.append(tr("Separate QP for luma/chroma"));
            if (BIT(_bmCapabilities, 4))
                supports.append(tr("Separate QP for Cb/Cr"));
            if (BIT(_bmCapabilities, 5))
                supports.append(tr("No picture reordering"));
            if (BIT(_bmCapabilities, 6))
                supports.append(tr("Long Term Reference frame"));

            return supports.join(NEWLINE);
        }

        QString UsbH264PayloadVideoFrameDescriptor::__parseBmSVCCapabilities() const
        {
            QStringList supports;
            supports.append(tr("Maximum number of temporal layers: %1")
                            .arg(CUT(_bmSVCCapabilities, 0, 2) + 1));
            if (BIT(_bmSVCCapabilities, 3))
                supports.append(tr("Rewrite support"));
            supports.append(tr("Maximum number of CGS layers: %1")
                            .arg(CUT(_bmSVCCapabilities, 4, 6) + 1));
            supports.append(tr("Maximum number of MGS layers: %1")
                            .arg(CUT(_bmSVCCapabilities, 7, 9)));
            if (BIT(_bmSVCCapabilities, 10))
                supports.append(tr("Additional SNR scalability support"));
            supports.append(tr("Maximum number of spatial layers: %1")
                            .arg(CUT(_bmSVCCapabilities, 11, 13) + 1));

            return supports.join(NEWLINE);
        }

        QString UsbH264PayloadVideoFrameDescriptor::__parseBmMVCCapabilities() const
        {
            QStringList supports;
            supports.append(tr("Maximum number of temporal layers: %1")
                            .arg(CUT(_bmMVCCapabilities, 0, 2)));
            supports.append(tr("Maximum number of view components: %1")
                            .arg(CUT(_bmMVCCapabilities, 3, 10)));

            return supports.join(NEWLINE);
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bFormatIndex() const
        {
            return _bFormatIndex;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bNumFrameDescriptors() const
        {
            return _bNumFrameDescriptors;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bDefaultFrameIndex() const
        {
            return _bDefaultFrameIndex;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bMaxCodecConfigDelay() const
        {
            return _bMaxCodecConfigDelay;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bSupportedPartitionCount() const
        {
            return _bSupportedPartitionCount;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bmSupportedSyncFrameTypes() const
        {
            return _bmSupportedSyncFrameTypes;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bResolutionScaling() const
        {
            return _bResolutionScaling;
        }

        uint8_t UsbVp8PayloadVideoFormatDescriptor::bmSupportedRateControlModes() const
        {
            return _bmSupportedRateControlModes;
        }

        uint16_t UsbVp8PayloadVideoFormatDescriptor::wMaxMBperSec() const
        {
            return _wMaxMBperSec;
        }

        QString UsbVp8PayloadVideoFormatDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("VP8 Payload Video Format Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFormatIndex", _bFormatIndex, _bFormatIndex);
            ATTR("bNumFrameDescriptors", _bNumFrameDescriptors, _bNumFrameDescriptors);
            ATTR("bDefaultFrameIndex", _bDefaultFrameIndex, _bDefaultFrameIndex);
            ATTR("bMaxCodecConfigDelay", _bMaxCodecConfigDelay, _bMaxCodecConfigDelay);
            ATTR("bSupportedPartitionCount", _bSupportedPartitionCount, _bSupportedPartitionCount);
            ATTR("bmSupportedSyncFrameTypes", _bmSupportedSyncFrameTypes, __parseBmSupportedSyncFrameTypes());
            ATTR("bResolutionScaling", _bResolutionScaling, __strBResolutionScaling());
            ATTR("bmSupportedRateControlModes", _bmSupportedRateControlModes, __parseBmSupportedRateControlModes());
            ATTR("wMaxMBperSec", _wMaxMBperSec, tr("%1 MB/s").arg(_wMaxMBperSec));
            END;

            return html;
        }

        UsbVp8PayloadVideoFormatDescriptor::UsbVp8PayloadVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFormatIndex = *(extra + 3);
            _bNumFrameDescriptors = *(extra + 4);
            _bDefaultFrameIndex = *(extra + 5);
            _bMaxCodecConfigDelay = *(extra + 6);
            _bSupportedPartitionCount = *(extra + 7);
            _bmSupportedSyncFrameTypes = *(extra + 8);
            _bResolutionScaling = *(extra + 9);
            _bmSupportedRateControlModes = *(extra + 10);
            _wMaxMBperSec = *reinterpret_cast<const uint16_t *>(extra + 11);
        }

        QString UsbVp8PayloadVideoFormatDescriptor::__parseBmSupportedSyncFrameTypes() const
        {
            QStringList supports;
            if (BIT(_bmSupportedSyncFrameTypes, 0))
                supports.append(tr("Reset"));
            if (BIT(_bmSupportedSyncFrameTypes, 1))
                supports.append(tr("Intra frame"));
            if (BIT(_bmSupportedSyncFrameTypes, 2))
                supports.append(tr("Golden frame"));
            if (BIT(_bmSupportedSyncFrameTypes, 3))
                supports.append(tr("Alternate reference frame"));
            if (BIT(_bmSupportedSyncFrameTypes, 4))
                supports.append(tr("Gradual Decoder Refresh frames"));

            return supports.join(NEWLINE);
        }

        QString UsbVp8PayloadVideoFormatDescriptor::__strBResolutionScaling() const
        {
            switch (_bResolutionScaling)
            {
                case 0:
                return tr("Not supported");
                case 1:
                return tr("1.5 or 2.0 scaling");
                case 2:
                return tr("1.0, 1.5 or 2.0 scaling");
                case 3:
                return tr("Resolutions by associated Frame Descriptors");
                case 4:
                return tr("Arbitrary scaling");
            }

            return QString();
        }

        QString UsbVp8PayloadVideoFormatDescriptor::__parseBmSupportedRateControlModes() const
        {
            QStringList supports;
            if (BIT(_bmSupportedRateControlModes, 0))
                supports.append(tr("VBR"));
            if (BIT(_bmSupportedRateControlModes, 1))
                supports.append(tr("CBR"));
            if (BIT(_bmSupportedRateControlModes, 2))
                supports.append(tr("Constant QP"));
            if (BIT(_bmSupportedRateControlModes, 3))
                supports.append(tr("Global VBR"));

            return supports.join(NEWLINE);
        }

        uint8_t UsbVp8PayloadVideoFrameDescriptor::bLength() const
        {
            return _bLength;
        }

        uint8_t UsbVp8PayloadVideoFrameDescriptor::bDescriptorType() const
        {
            return _bDescriptorType;
        }

        uint8_t UsbVp8PayloadVideoFrameDescriptor::bDescriptorSubtype() const
        {
            return _bDescriptorSubtype;
        }

        uint8_t UsbVp8PayloadVideoFrameDescriptor::bFrameIndex() const
        {
            return _bFrameIndex;
        }

        uint16_t UsbVp8PayloadVideoFrameDescriptor::wWidth() const
        {
            return _wWidth;
        }

        uint16_t UsbVp8PayloadVideoFrameDescriptor::wHeight() const
        {
            return _wHeight;
        }

        uint32_t UsbVp8PayloadVideoFrameDescriptor::bmSupportedUsages() const
        {
            return _bmSupportedUsages;
        }

        uint16_t UsbVp8PayloadVideoFrameDescriptor::bmCapabilities() const
        {
            return _bmCapabilities;
        }

        uint32_t UsbVp8PayloadVideoFrameDescriptor::bmScalabilityCapabilities() const
        {
            return _bmScalabilityCapabilities;
        }

        uint32_t UsbVp8PayloadVideoFrameDescriptor::dwMinBitRate() const
        {
            return _dwMinBitRate;
        }

        uint32_t UsbVp8PayloadVideoFrameDescriptor::dwMaxBitRate() const
        {
            return _dwMaxBitRate;
        }

        uint32_t UsbVp8PayloadVideoFrameDescriptor::dwDefaultFrameInterval() const
        {
            return _dwDefaultFrameInterval;
        }

        uint8_t UsbVp8PayloadVideoFrameDescriptor::bNumFrameIntervals() const
        {
            return _bNumFrameIntervals;
        }

        uint32_t UsbVp8PayloadVideoFrameDescriptor::dwFrameInterval(uint8_t index) const
        {
            if (index > 0 && index <= _bNumFrameIntervals)
                return _dwFrameInterval[index - 1];
            else
            {
                LOGE(tr("Index %1 for dwFrameInterval out of range, must be 1~%2.")
                     .arg(index).arg(_bNumFrameIntervals));
                return 0;
            }
        }

        QString UsbVp8PayloadVideoFrameDescriptor::infomationToHtml() const
        {
            QString html;
            START(tr("VP8 Payload Video Frame Descriptor"));
            ATTR("bLength", _bLength, _bLength);
            ATTR("bDescriptorType", _bDescriptorType, _bDescriptorType);
            ATTR("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype));
            ATTR("bFrameIndex", _bFrameIndex, _bFrameIndex);
            ATTR("wWidth", _wWidth, _wWidth);
            ATTR("wHeight", _wHeight, _wHeight);
            ATTR("bmSupportedUsages", _bmSupportedUsages, __parseBmSupportedUsages());
            ATTR("bmCapabilities", _bmCapabilities, __parseBmCapabilities());
            ATTR("bmScalabilityCapabilities", _bmScalabilityCapabilities, __parseBmScalabilityCapabilities());
            ATTR("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate));
            ATTR("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate));
            ATTR("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                 tr("%1 * 100 ns").arg(_dwDefaultFrameInterval));
            ATTR("bNumFrameIntervals", _bNumFrameIntervals, _bNumFrameIntervals);
            for (uint8_t i = 1; i <= _bNumFrameIntervals; ++i)
                ATTR(QString("dwFrameInterval(%1)").arg(i), dwFrameInterval(i),
                     tr("%1 * 100 ns").arg(dwFrameInterval(i)));
            END;

            return html;
        }

        UsbVp8PayloadVideoFrameDescriptor::UsbVp8PayloadVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint8_t descPos):
            UsbVideoStreamInterfaceDescriptor(parent)
        {
            const unsigned char *extra = parent->extra() + descPos;

            _bLength = *extra;
            _bDescriptorType = *(extra + 1);
            _bDescriptorSubtype = *(extra + 2);
            _bFrameIndex = *(extra + 3);
            _wWidth = *reinterpret_cast<const uint16_t *>(extra + 4);
            _wHeight = *reinterpret_cast<const uint16_t *>(extra + 6);
            _bmSupportedUsages = *reinterpret_cast<const uint32_t *>(extra + 8);
            _bmCapabilities = *reinterpret_cast<const uint16_t *>(extra + 12);
            _bmScalabilityCapabilities = *reinterpret_cast<const uint32_t *>(extra + 14);
            _dwMinBitRate =  *reinterpret_cast<const uint32_t *>(extra + 18);
            _dwMaxBitRate =  *reinterpret_cast<const uint32_t *>(extra + 22);
            _dwDefaultFrameInterval = *reinterpret_cast<const uint32_t *>(extra + 26);
            _bNumFrameIntervals = *(extra + 30);
            _dwFrameInterval.reserve(_bNumFrameIntervals);
            for (uint8_t i = 0; i < _bNumFrameIntervals; ++i)
                _dwFrameInterval.append(*reinterpret_cast<const uint32_t *>(extra + 31 + i * 4));
        }

        QString UsbVp8PayloadVideoFrameDescriptor::__parseBmSupportedUsages() const
        {
            QStringList supports;
            if (BIT(_bmSupportedUsages, 1))
                supports.append(tr("Real-time"));
            if (BIT(_bmSupportedUsages, 2))
                supports.append(tr("Real-time with temporal layering structure"));
            if (BIT(_bmSupportedUsages, 16))
                supports.append(tr("File Storage mode with I and P slices"));
            if (BIT(_bmSupportedUsages, 18))
                supports.append(tr("File storage all-I-frame mode"));

            return supports.join(NEWLINE);
        }

        QString UsbVp8PayloadVideoFrameDescriptor::__parseBmCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmCapabilities, 2))
                supports.append(tr("Constant frame rate"));
            if (BIT(_bmCapabilities, 3))
                supports.append(tr("Separate QP for luma/chroma"));
            if (BIT(_bmCapabilities, 6))
                supports.append(tr("Golden frame"));
            if (BIT(_bmCapabilities, 7))
                supports.append(tr("Alternate reference frame"));

            return supports.join(NEWLINE);
        }

        QString UsbVp8PayloadVideoFrameDescriptor::__parseBmScalabilityCapabilities() const
        {
            return tr("maximum number of temporal enhancement layers: %1")
                    .arg(CUT(_bmScalabilityCapabilities, 0, 2) + 1);
        }
    } // namespace uvc
} // namespace usb
