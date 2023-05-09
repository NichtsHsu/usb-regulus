#include "usbvideostreaminterfacedescriptor.h"
#include "usbendpointdescriptor.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

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
                UsbInterfaceDescriptor *interfaceDescriptor, uint32_t descPos)
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

        const QStringList &UsbInputHeaderDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bNumFormats",
                "wTotalLength",
                "bEndpointAddress",
                "bmInfo",
                "bTerminalLink",
                "bStillCaptureMethod",
                "bTriggerSupport",
                "bTriggerUsage",
                "bControlSize",
                "bmaControls",
            };

            return fields;
        }

        QString UsbInputHeaderDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_INPUT_HEADER descriptor subtype"},
                {"bNumFormats", "Number of video payload Format "
                 "descriptors following for this interface"
                 "(excluding video Frame descriptors)."},
                {"wTotalLength", "Total number of bytes returned for the "
                 "class-specific VideoStreaming interface "
                 "descriptors including this header descriptor."},
                {"bEndpointAddress", "<p>The address of the isochronous or bulk "
                 "endpoint used for video data. The address "
                 "is encoded as follows:</p>"
                 "<table><tr><td>D7:</td><td><p>Direction</p><p>1 = IN endpoint</p></td></tr>"
                 "<tr><td>D6..4:</td><td>Reserved, set to zero.</td></tr>"
                 "<tr><td>D3..0:</td><td>The endpoint number, determined by the designer.</td></tr></table>"},
                {"bmInfo", "<p>Indicates the capabilities of this VideoStreaming interface:</p>"
                 "<table><tr><td>D0:</td><td>Dynamic Format Change supported</td></tr>"
                 "<tr><td>D7..1:</td><td>Reserved, set to zero.</td></tr></table>"},
                {"bTerminalLink", "The terminal ID of the Output Terminal to "
                 "which the video endpoint of this interface is connected."},
                {"bStillCaptureMethod", "<p>Method of still image capture supported as "
                 "described in section 2.4.2.4, \"Still Image Capture\":</p>"
                 "<table><tr><td>0:</td><td>None (Host software will not support any form of still image capture)</td></tr>"
                 "<tr><td>1:</td><td>Method 1</td></tr>"
                 "<tr><td>2:</td><td>Method 2</td></tr>"
                 "<tr><td>3:</td><td>Method 3</td></tr></table>"},
                {"bTriggerSupport", "<p>Specifies if hardware triggering is supported through this interface.</p>"
                 "<table><tr><td>0:</td><td>Not supported</td></tr>"
                 "<tr><td>1:</td><td>Supported</td></tr></table>"},
                {"bTriggerUsage", "<p>Specifies how the host software shall "
                 "respond to a hardware trigger interrupt "
                 "event from this interface. This is ignored if "
                 "the <i>bTriggerSupport</i> field is zero.</p>"
                 "<table><tr><td>0:</td><td>Initiate still image capture</td></tr>"
                 "<tr><td>1:</td><td>General purpose button event. Host "
                 "driver will notify client application of "
                 "button press and button release events</td></tr></table>"},
                {"bControlSize", "Size of each bmaControls[x] field, in bytes."},
                {"bmaControls", "<p>For bits D3..0, a bit set to 1 indicates that "
                 "the named field is supported by the Video "
                 "Probe and Commit Control when "
                 "<i>bFormatIndex</i> is x (for bmaControls[x]):</p>"
                 "<table><tr><td>D0:</td><td>wKeyFrameRate</td></tr>"
                 "<tr><td>D1:</td><td>wPFrameRate</td></tr>"
                 "<tr><td>D2:</td><td>wCompQuality</td></tr>"
                 "<tr><td>D3:</td><td>wCompWindowSize</td></tr></table>"
                 "<p>For bits D5..4, a bit set to 1 indicates that "
                 "the named control is supported by the "
                 "device when <i>bFormatIndex</i> is x (for bmaControls[x]):</p>"
                 "<table><tr><td>D4:</td><td>Generate Key Frame</td></tr>"
                 "<tr><td>D5:</td><td>Update Frame Segment</td></tr>"
                 "<tr><td>D6..(n*8-1):</td><td>Reserved, set to zero</td></tr></table>"
                 "<p><b>*Note*</b> going forward from version 1.5 the "
                 "proper means to detect whether a field is "
                 "supported by Probe & Commit is to issue a "
                 "VS_PROBE_CONTROL(GET_CUR).</p>"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbInputHeaderDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Input Header Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bNumFormats", _bNumFormats)
                    .attr("bEndpointAddress", _bEndpointAddress, tr("Endpoint %1 %2")
                          .arg(_bEndpointAddress & 0x0F)
                          .arg(strEndpointDirection(EndpointDirection(_bEndpointAddress & int(EndpointDirection::IN)))))
                    .attr("bmInfo", _bmInfo, _bmInfo ?
                              tr("Supports Dynamic Format Change"): QString())
                    .attr("bTerminalLink", _bTerminalLink)
                    .attr("bStillCaptureMethod", _bStillCaptureMethod, __parseBStillCaptureMethod())
                    .attr("bTriggerSupport", _bTriggerSupport, _bTriggerSupport ?
                              tr("Supports hardware triggering") : QString())
                    .attr("bTriggerUsage", _bTriggerUsage, _bTriggerSupport ?
                              (_bTriggerUsage ?
                                   tr("General purpose button event"):
                                   tr("Initiate still image capture")):
                              QString())
                    .attr("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize));
            for (uint8_t i = 1; i <= _bNumFormats; ++i)
                builder.attr("bmaControls",
                             QString("0x") + bmaControls(i).toHex(),
                             __parseBmaControls(i),
                             i);

            return builder.end().build();
        }

        UsbInputHeaderDescriptor::UsbInputHeaderDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

        const QStringList &UsbOutputHeaderDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bNumFormats",
                "wTotalLength",
                "bEndpointAddress",
                "bTerminalLink",
                "bControlSize",
                "bmaControls",
            };

            return fields;
        }

        QString UsbOutputHeaderDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_OUTPUT_HEADER descriptor subtype"},
                {"bNumFormats", "Number of video payload Format "
                 "descriptors following for this interface"
                 "(excluding video Frame descriptors)."},
                {"wTotalLength", "Total number of bytes returned for the "
                 "class-specific VideoStreaming interface "
                 "descriptors including this header descriptor."},
                {"bEndpointAddress", "<p>The address of the isochronous or bulk "
                 "endpoint used for video data. The address "
                 "is encoded as follows:</p>"
                 "<table><tr><td>D7:</td><td><p>Direction</p><p>0 = OUT endpoint</p></td></tr>"
                 "<tr><td>D6..4:</td><td>Reserved, set to zero.</td></tr>"
                 "<tr><td>D3..0:</td><td>The endpoint number, determined by the designer.</td></tr></table>"},
                {"bTerminalLink", "The terminal ID of the Input Terminal to "
                 "which the video endpoint of this interface is connected."},
                {"bControlSize", "Size of each bmaControls[x] field, in bytes."},
                {"bmaControls", "<p>For bits D3..0, a bit set to 1 indicates that "
                 "the named field is supported by the Video "
                 "Probe and Commit Control when "
                 "<i>bFormatIndex</i> is x (for bmaControls[x]):</p>"
                 "<table><tr><td>D0:</td><td>wKeyFrameRate</td></tr>"
                 "<tr><td>D1:</td><td>wPFrameRate</td></tr>"
                 "<tr><td>D2:</td><td>wCompQuality</td></tr>"
                 "<tr><td>D3:</td><td>wCompWindowSize</td></tr>"
                 "<tr><td>D4..(n*8-1):</td><td>Reserved, set to zero</td></tr></table>"
                 "<p><b>*Note*</b> D0-D3 are deprecated. Going "
                 "forward from version 1.5, the proper "
                 "means to detect whether a field is "
                 "supported by Probe & Commit is to issue "
                 "a VS_PROBE_CONTROL(GET_CUR).</p>"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbOutputHeaderDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Output Header Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bNumFormats", _bNumFormats)
                    .attr("bEndpointAddress", _bEndpointAddress, tr("Endpoint %1 %2")
                          .arg(_bEndpointAddress & 0x0F)
                          .arg(strEndpointDirection(EndpointDirection(_bEndpointAddress & int(EndpointDirection::IN)))))
                    .attr("bTerminalLink", _bTerminalLink)
                    .attr("bControlSize", _bControlSize, tr("%1 byte(s)").arg(_bControlSize));
            for (uint8_t i = 1; i <= _bNumFormats; ++i)
                builder.attr("bmaControls",
                             QString("0x") + bmaControls(i).toHex(),
                             __parseBmaControls(i),
                             i);

            return builder.end().build();
        }

        UsbOutputHeaderDescriptor::UsbOutputHeaderDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

        const QStringList &UsbStillImageFrameDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bEndpointAddress",
                "bNumImageSizePatterns",
                "wWidth",
                "wHeight",
                "bNumCompressionPattern",
                "bCompression",
            };

            return fields;
        }

        QString UsbStillImageFrameDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_STILL_IMAGE_FRAME descriptor subtype"},
                {"bEndpointAddress", "<p>If method 3 of still image capture is used, "
                 "this contains the address of the bulk endpoint used for still image capture. The "
                 "address is encoded as follows:</p>"
                 "<table><tr><td>D7:</td><td>Direction (set to 1 = IN endpoint)</td></tr>"
                 "<tr><td>D6..4:</td><td>Reserved, set to zero.</td></tr>"
                 "<tr><td>D3..0:</td><td>The endpoint number, determined by the designer.</td></tr></table>"},
                {"bNumImageSizePatterns", "Number of Image Size patterns of this format"},
                {"wWidth", "Width of the still image in nth pattern"},
                {"wHeight", "Height of the still image in nth pattern"},
                {"bNumCompressionPattern", "Number of Compression pattern of this format"},
                {"bCompression", "Compression of the still image in nth pattern"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbStillImageFrameDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Still Image Frame Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bEndpointAddress", _bEndpointAddress, tr("Endpoint %1 %2")
                          .arg(_bEndpointAddress & 0x0F)
                          .arg(strEndpointDirection(EndpointDirection(_bEndpointAddress & int(EndpointDirection::IN)))))
                    .attr("bNumImageSizePatterns", _bNumImageSizePatterns);
            for (uint8_t i = 1; i <= _bNumImageSizePatterns; ++i)
                builder.attr("wWidth", wWidth(i), i)
                        .attr("wHeight", wHeight(i), i);
            builder.attr("bNumCompressionPattern", _bNumCompressionPattern);
            for (uint8_t i = 1; i <= _bNumCompressionPattern; ++i)
                builder.attr("bCompression", bCompression(i), i);

            return builder.end().build();
        }

        UsbStillImageFrameDescriptor::UsbStillImageFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

        const QStringList &UsbUncompressedVideoFormatDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFormatIndex",
                "bNumFrameDescriptors",
                "guidFormat",
                "bBitsPerPixel",
                "bDefaultFrameIndex",
                "bAspectRatioX",
                "bAspectRatioY",
                "bmInterlaceFlags",
                "bCopyProtect",
            };

            return fields;
        }

        QString UsbUncompressedVideoFormatDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FORMAT_UNCOMPRESSED descriptor subtype"},
                {"bFormatIndex", "Index of this format descriptor"},
                {"bNumFrameDescriptors", "Number of frame descriptors following "
                 "that correspond to this format"},
                {"guidFormat", "Globally Unique Identifier used to "
                 "identify stream-encoding format"},
                {"bBitsPerPixel", "Number of bits per pixel used to "
                 "specify color in the decoded video frame"},
                {"bDefaultFrameIndex", "Optimum Frame Index (used to select "
                 "resolution) for this stream"},
                {"bAspectRatioX", "The X dimension of the picture aspect ratio"},
                {"bAspectRatioY", "The Y dimension of the picture aspect ratio"},
                {"bmInterlaceFlags", "<p>Specifies interlace information. If the "
                 "scanning mode control in the Camera "
                 "Terminal is supported for this stream, "
                 "this field shall reflect the field format "
                 "used in interlaced mode."
                 "(Top field in PAL is field 1, top field in "
                 "NTSC is field 2.):</p>"
                 "<table><tr><td>D0:</td><td><p>Interlaced stream or variable.</p><p>1 = Yes</p></td></tr>"
                 "<tr><td>D1:</td><td><p>Fields per frame.</p><p>0 = 2 fields</p><p>1 = 1 field</p></td></tr>"
                 "<tr><td>D2:</td><td><p>Field 1 first.</p><p>1 = Yes</p></td></tr>"
                 "<tr><td>D3:</td><td>Reserved</td></tr>"
                 "<tr><td>D5..4:</td><td><p>Field pattern</p><p>00 = Field 1 only</p><p>01 = Field 2 only</p>"
                 "<p>10 = Regular pattern of fields 1 and 2</p>"
                 "<p>11 = Random pattern of fields 1 and 2</p></td></tr>"
                 "<tr><td>D7..6:</td><td>Reserved. Do not use.</td></tr></table>"},
                {"bCopyProtect", "<p>Specifies whether duplication of the "
                 "video stream is restricted:</p>"
                 "<table><tr><td>0:</td><td>No restrictions</td></tr>"
                 "<tr><td>1:</td><td>Restrict duplication</td></tr></table>"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbUncompressedVideoFormatDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Uncompressed Video Format Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatIndex", _bFormatIndex)
                    .attr("bNumFrameDescriptors", _bNumFrameDescriptors)
                    .attr("guidFormat", "", hexUuid(_guidFormat))
                    .attr("bBitsPerPixel", _bBitsPerPixel, tr("%1 bit(s) per pixel").arg(_bBitsPerPixel))
                    .attr("bDefaultFrameIndex", _bDefaultFrameIndex)
                    .attr("bAspectRatioX", _bAspectRatioX)
                    .attr("bAspectRatioY", _bAspectRatioY)
                    .attr("bmInterlaceFlags", _bmInterlaceFlags, __parseBmInterlaceFlags())
                    .attr("bCopyProtect", _bCopyProtect, _bCopyProtect ? tr("Restrict duplication") : "")
                    .end()
                    .build();
        }

        UsbUncompressedVideoFormatDescriptor::UsbUncompressedVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

            return infomations.join(UsbHtmlBuilder::NEWLINE);
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

        const QStringList &UsbUncompressedVideoFrameDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFrameIndex",
                "bmCapabilities",
                "wWidth",
                "wHeight",
                "dwMinBitRate",
                "dwMaxBitRate",
                "dwMaxVideoFrameBufferSize",
                "dwDefaultFrameInterval",
                "bFrameIntervalType",
                "dwMinFrameInterval",
                "dwMaxFrameInterval",
                "dwFrameIntervalStep",
                "dwFrameInterval",
            };

            return fields;
        }

        QString UsbUncompressedVideoFrameDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FRAME_UNCOMPRESSED descriptor subtype"},
                {"bFrameIndex", "Index of this frame descriptor"},
                {"bmCapabilities", "<p>D0: Still image supported</p>"
                 "<p>Specifies whether still images are "
                 "supported at this frame setting. This is "
                 "only applicable for VS interfaces with "
                 "an IN video endpoint using Still "
                 "Image Capture Method 1, and should "
                 "be set to 0 in all other cases.</p>"
                 "<p>D1: Fixed frame-rate</p>"
                 "<p>Specifies whether the device provides "
                 "a fixed frame rate on a stream "
                 "associated with this frame descriptor. "
                 "Set to 1 if fixed rate is enabled; "
                 "otherwise, set to 0.</p>"
                 "<p>D7..2: Reserved, set to 0.</p>"},
                {"wWidth", "Width of decoded bitmap frame in pixels"},
                {"wHeight", "Height of decoded bitmap frame in pixels"},
                {"dwMinBitRate", "Specifies the minimum bit rate at the "
                 "longest frame interval in units of bps "
                 "at which the data can be transmitted."},
                {"dwMaxBitRate", "Specifies the maximum bit rate at the "
                 "shortest frame interval in units of bps "
                 "at which the data can be transmitted."},
                {"dwMaxVideoFrameBufferSize", "<p>Use of this field has been deprecated.</p>"
                 "<p>Specifies the maximum number of "
                 "bytes that the compressor will produce "
                 "for a video frame or still image.</p>"
                 "<p>The <i>dwMaxVideoFrameSize</i> field of "
                 "the Video Probe and Commit control "
                 "replaces this descriptor field. A value "
                 "for this field shall be chosen for "
                 "compatibility with host software that "
                 "implements an earlier version of this "
                 "specification.</p>"},
                {"dwDefaultFrameInterval", "<p>Specifies the frame interval the device "
                 "would like to indicate for use as a "
                 "default. This must be a valid frame "
                 "interval described in the fields below.</p>"},
                {"bFrameIntervalType", "Indicates how the frame interval can be programmed:"
                 "<table><tr><td>0:</td><td>Continuous frame interval</td></tr>"
                 "<tr><td>1..255:</td><td>The number of discrete frame intervals supported</td></tr></table>"},
                {"dwMinFrameInterval", "Shortest frame interval supported (at "
                 "highest frame rate), in 100 ns units."},
                {"dwMaxFrameInterval", "Longest frame interval supported (at "
                 "lowest frame rate), in 100 ns units."},
                {"dwFrameIntervalStep", "Indicates granularity of frame interval "
                 "range, in 100 ns units."},
                {"dwFrameInterval", "Nth shortest frame interval supported (at "
                 "nth highest frame rate), in 100 ns units."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbUncompressedVideoFrameDescriptor::__parseBmCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmCapabilities, 0))
                supports.append(tr("Still image supported"));
            if (BIT(_bmCapabilities, 1))
                supports.append(tr("Fixed frame-rate"));

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbUncompressedVideoFrameDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Uncompressed Video Frame Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFrameIndex", _bFrameIndex)
                    .attr("bmCapabilities", _bmCapabilities, __parseBmCapabilities())
                    .attr("wWidth", _wWidth)
                    .attr("wHeight", _wHeight)
                    .attr("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate))
                    .attr("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate))
                    .attr("dwMaxVideoFrameBufferSize", _dwMaxVideoFrameBufferSize,
                          tr("%1 byte(s)").arg(_dwMaxVideoFrameBufferSize))
                    .attr("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                          tr("%1 * 100 ns").arg(_dwDefaultFrameInterval))
                    .attr("bFrameIntervalType", _bFrameIntervalType, _bFrameIntervalType ?
                              tr("%1 discrete frame intervals").arg(_bFrameIntervalType) :
                              tr("Continuous frame interval"));
            if (_bFrameIntervalType)
                for (uint8_t i = 1; i <= _bFrameIntervalType; ++i)
                    builder.attr("dwFrameInterval",
                                 dwFrameInterval(i),
                                 tr("%1 * 100 ns").arg(dwFrameInterval(i)),
                                 i);
            else
                builder.attr("dwMinFrameInterval", dwMinFrameInterval(),
                             tr("%1 * 100 ns").arg(dwMinFrameInterval()))
                        .attr("dwMaxFrameInterval", dwMaxFrameInterval(),
                              tr("%1 * 100 ns").arg(dwMaxFrameInterval()))
                        .attr("dwFrameIntervalStep", _dwFrameIntervalStep,
                              tr("%1 * 100 ns").arg(_dwFrameIntervalStep));

            return builder.end().build();
        }

        UsbUncompressedVideoFrameDescriptor::UsbUncompressedVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

        const QStringList &UsbMotionJpegVideoFormatDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFormatIndex",
                "bNumFrameDescriptors",
                "bmFlags",
                "bDefaultFrameIndex",
                "bAspectRatioX",
                "bAspectRatioY",
                "bmInterlaceFlags",
                "bCopyProtect",
            };

            return fields;
        }

        QString UsbMotionJpegVideoFormatDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FORMAT_MJPEG descriptor subtype"},
                {"bFormatIndex", "Index of this format descriptor"},
                {"bNumFrameDescriptors", "Number of frame descriptors following "
                 "that correspond to this format"},
                {"bmFlags", "<p>Specifies characteristics of this format:</p>"
                 "<p>D0: FixedSizeSamples. 1 = Yes</p>"
                 "<p>All other bits are reserved for future "
                 "use and shall be reset to zero.</p>"},
                {"bDefaultFrameIndex", "Optimum Frame Index (used to select "
                 "resolution) for this stream"},
                {"bAspectRatioX", "The X dimension of the picture aspect ratio"},
                {"bAspectRatioY", "The Y dimension of the picture aspect ratio"},
                {"bmInterlaceFlags", "<p>Specifies interlace information. If the "
                 "scanning mode control in the Camera "
                 "Terminal is supported for this stream, "
                 "this field shall reflect the field format "
                 "used in interlaced mode."
                 "(Top field in PAL is field 1, top field in "
                 "NTSC is field 2.):</p>"
                 "<table><tr><td>D0:</td><td><p>Interlaced stream or variable.</p><p>1 = Yes</p></td></tr>"
                 "<tr><td>D1:</td><td><p>Fields per frame.</p><p>0 = 2 fields</p><p>1 = 1 field</p></td></tr>"
                 "<tr><td>D2:</td><td><p>Field 1 first.</p><p>1 = Yes</p></td></tr>"
                 "<tr><td>D3:</td><td>Reserved</td></tr>"
                 "<tr><td>D5..4:</td><td><p>Field pattern</p><p>00 = Field 1 only</p><p>01 = Field 2 only</p>"
                 "<p>10 = Regular pattern of fields 1 and 2</p>"
                 "<p>11 = Random pattern of fields 1 and 2</p></td></tr>"
                 "<tr><td>D7..6:</td><td>Reserved. Do not use.</td></tr></table>"},
                {"bCopyProtect", "<p>Specifies whether duplication of the "
                 "video stream is restricted:</p>"
                 "<table><tr><td>0:</td><td>No restrictions</td></tr>"
                 "<tr><td>1:</td><td>Restrict duplication</td></tr></table>"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbMotionJpegVideoFormatDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Motion-JPEG Video Format Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatIndex", _bFormatIndex)
                    .attr("bNumFrameDescriptors", _bNumFrameDescriptors)
                    .attr("bmFlags", _bmFlags, BIT(_bmFlags, 0) ? tr("Fixed Size Samples"): "")
                    .attr("bDefaultFrameIndex", _bDefaultFrameIndex)
                    .attr("bAspectRatioX", _bAspectRatioX)
                    .attr("bAspectRatioY", _bAspectRatioY)
                    .attr("bmInterlaceFlags", _bmInterlaceFlags, __parseBmInterlaceFlags())
                    .attr("bCopyProtect", _bCopyProtect, _bCopyProtect ? tr("Restrict duplication") : "")
                    .end()
                    .build();
        }

        UsbMotionJpegVideoFormatDescriptor::UsbMotionJpegVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

            return infomations.join(UsbHtmlBuilder::NEWLINE);
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

        const QStringList &UsbMotionJpegVideoFrameDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFrameIndex",
                "bmCapabilities",
                "wWidth",
                "wHeight",
                "dwMinBitRate",
                "dwMaxBitRate",
                "dwMaxVideoFrameBufferSize",
                "dwDefaultFrameInterval",
                "bFrameIntervalType",
                "dwMinFrameInterval",
                "dwMaxFrameInterval",
                "dwFrameIntervalStep",
                "dwFrameInterval",
            };

            return fields;
        }

        QString UsbMotionJpegVideoFrameDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FRAME_MJPEG descriptor subtype"},
                {"bFrameIndex", "Index of this frame descriptor"},
                {"bmCapabilities", "<p>D0: Still image supported</p>"
                 "<p>Specifies whether still images are "
                 "supported at this frame setting. This is "
                 "only applicable for VS interfaces with "
                 "an IN video endpoint using Still "
                 "Image Capture Method 1, and should "
                 "be set to 0 in all other cases.</p>"
                 "<p>D1: Fixed frame-rate</p>"
                 "<p>Specifies whether the device provides "
                 "a fixed frame rate on a stream "
                 "associated with this frame descriptor. "
                 "Set to 1 if fixed rate is enabled; "
                 "otherwise, set to 0.</p>"
                 "<p>D7..2: Reserved, set to 0.</p>"},
                {"wWidth", "Width of decoded bitmap frame in pixels"},
                {"wHeight", "Height of decoded bitmap frame in pixels"},
                {"dwMinBitRate", "Specifies the minimum bit rate at the "
                 "longest frame interval in units of bps "
                 "at which the data can be transmitted."},
                {"dwMaxBitRate", "Specifies the maximum bit rate at the "
                 "shortest frame interval in units of bps "
                 "at which the data can be transmitted."},
                {"dwMaxVideoFrameBufferSize", "<p>Use of this field has been deprecated.</p>"
                 "<p>Specifies the maximum number of "
                 "bytes that the compressor will produce "
                 "for a video frame or still image.</p>"
                 "<p>The <i>dwMaxVideoFrameSize</i> field of "
                 "the Video Probe and Commit control "
                 "replaces this descriptor field. A value "
                 "for this field shall be chosen for "
                 "compatibility with host software that "
                 "implements an earlier version of this "
                 "specification.</p>"},
                {"dwDefaultFrameInterval", "<p>Specifies the frame interval the device "
                 "would like to indicate for use as a "
                 "default. This must be a valid frame "
                 "interval described in the fields below.</p>"},
                {"bFrameIntervalType", "Indicates how the frame interval can be programmed:"
                 "<table><tr><td>0:</td><td>Continuous frame interval</td></tr>"
                 "<tr><td>1..255:</td><td>The number of discrete frame intervals supported</td></tr></table>"},
                {"dwMinFrameInterval", "Shortest frame interval supported (at "
                 "highest frame rate), in 100 ns units."},
                {"dwMaxFrameInterval", "Longest frame interval supported (at "
                 "lowest frame rate), in 100 ns units."},
                {"dwFrameIntervalStep", "Indicates granularity of frame interval "
                 "range, in 100 ns units."},
                {"dwFrameInterval", "Nth shortest frame interval supported (at "
                 "nth highest frame rate), in 100 ns units."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbMotionJpegVideoFrameDescriptor::__parseBmCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmCapabilities, 0))
                supports.append(tr("Still image supported"));
            if (BIT(_bmCapabilities, 1))
                supports.append(tr("Fixed frame-rate"));

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbMotionJpegVideoFrameDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Motion-JPEG Video Frame Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFrameIndex", _bFrameIndex)
                    .attr("bmCapabilities", _bmCapabilities, __parseBmCapabilities())
                    .attr("wWidth", _wWidth)
                    .attr("wHeight", _wHeight)
                    .attr("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate))
                    .attr("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate))
                    .attr("dwMaxVideoFrameBufferSize", _dwMaxVideoFrameBufferSize,
                          tr("%1 byte(s)").arg(_dwMaxVideoFrameBufferSize))
                    .attr("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                          tr("%1 * 100 ns").arg(_dwDefaultFrameInterval))
                    .attr("bFrameIntervalType", _bFrameIntervalType, _bFrameIntervalType ?
                              tr("%1 discrete frame intervals").arg(_bFrameIntervalType) :
                              tr("Continuous frame interval"));
            if (_bFrameIntervalType)
                for (uint8_t i = 1; i <= _bFrameIntervalType; ++i)
                    builder.attr("dwFrameInterval",
                                 dwFrameInterval(i),
                                 tr("%1 * 100 ns").arg(dwFrameInterval(i)),
                                 i);
            else
                builder.attr("dwMinFrameInterval", dwMinFrameInterval(),
                             tr("%1 * 100 ns").arg(dwMinFrameInterval()))
                        .attr("dwMaxFrameInterval", dwMaxFrameInterval(),
                              tr("%1 * 100 ns").arg(dwMaxFrameInterval()))
                        .attr("dwFrameIntervalStep", _dwFrameIntervalStep,
                              tr("%1 * 100 ns").arg(_dwFrameIntervalStep));

            return builder.end().build();
        }

        UsbMotionJpegVideoFrameDescriptor::UsbMotionJpegVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

        const QStringList &UsbMpeg2TsFormatDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFormatIndex",
                "bDataOffset",
                "bPacketLength",
                "bStrideLength",
                "guidStrideFormat",
            };

            return fields;
        }

        QString UsbMpeg2TsFormatDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FORMAT_MPEG2TS descriptor subtype"},
                {"bFormatIndex", "Index of this format descriptor"},
                {"bDataOffset", "Offset to TSP packet within MPEG-2 "
                 "TS transport stride, in bytes."},
                {"bPacketLength", "Length of TSP packet, in bytes (typically 188)."},
                {"bStrideLength", "Length of MPEG-2 TS transport stride."},
                {"guidStrideFormat", "A Globally Unique Identifier "
                 "indicating the format of the stride data "
                 "(if any). Set to zeros if there is no "
                 "Stride Data, or if the Stride Data is to "
                 "be ignored by the application."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbMpeg2TsFormatDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("MPEG-2 TS Format Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatIndex", _bFormatIndex)
                    .attr("bDataOffset", _bDataOffset)
                    .attr("bPacketLength", _bPacketLength)
                    .attr("bStrideLength", _bStrideLength)
                    .attr("guidStrideFormat", "", hexUuid(_guidStrideFormat))
                    .end()
                    .build();
        }

        UsbMpeg2TsFormatDescriptor::UsbMpeg2TsFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

        const QStringList &UsbDvFormatDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFormatIndex",
                "dwMaxVideoFrameBufferSize",
                "bFormatType",
            };

            return fields;
        }

        QString UsbDvFormatDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FORMAT_DV descriptor subtype"},
                {"bFormatIndex", "Index of this format descriptor"},
                {"dwMaxVideoFrameBufferSize", "<p>Use of this field has been deprecated.</p>"
                 "<p>Specifies the maximum size of a single DV frame, in bytes. This "
                 "will vary according to the format (SD/SDL/HD) and standard (NTSC/PAL).</p>"
                 "<p>The <i>dwMaxVideoFrameSize</i> field of "
                 "the Video Probe and Commit control "
                 "replaces this descriptor field. A value "
                 "for this field shall be chosen for "
                 "compatibility with host software that "
                 "implements an earlier version of this "
                 "specification.</p>"},
                {"bFormatType", "<table><tr><td>D7:</td><td><p>0 = 50 Hz</p><p>1 = 60 Hz</p></td></tr>"
                 "<tr><td>D6..0:</td><td><p>0: SD-DV</p><p>1: SDL-DV</p><p>2: HD-DV</p></td></tr></table>"
                 "<p>All other values are reserved.</p>"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbDvFormatDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("DV Format Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatIndex", _bFormatIndex)
                    .attr("dwMaxVideoFrameBufferSize", _dwMaxVideoFrameBufferSize,
                          tr("%1 byte(s)").arg(_dwMaxVideoFrameBufferSize))
                    .attr("bFormatType", _bFormatType, __parseBFormatType())
                    .end()
                    .build();
        }

        UsbDvFormatDescriptor::UsbDvFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

        const QStringList &UsbColorMatchingDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bColorPrimaries",
                "bTransferCharacteristics",
                "bMatrixCoefficients",
            };

            return fields;
        }

        QString UsbColorMatchingDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_COLORFORMAT descriptor subtype"},
                {"bColorPrimaries", "<p>This defines the color primaries and the reference white.</p>"
                 "<table><tr><td>0:</td><td>Unspecified (Image characteristics unknown)</td></tr>"
                 "<tr><td>1:</td><td>BT.709, sRGB (default)</td></tr>"
                 "<tr><td>2:</td><td>BT.470-2 (M)</td></tr>"
                 "<tr><td>3:</td><td>BT.470-2 (B, G)</td></tr>"
                 "<tr><td>4:</td><td>SMPTE 170M</td></tr>"
                 "<tr><td>5:</td><td>SMPTE 240M</td></tr>"
                 "<tr><td>6-255:</td><td>Reserved</td></tr></table>"},
                {"bTransferCharacteristics", "<p>This field defines the optoelectronic transfer characteristic of "
                 "the source picture also called the gamma function.</p>"
                 "<table><tr><td>0:</td><td>Unspecified (Image characteristics unknown)</td></tr>"
                 "<tr><td>1:</td><td>BT.709 (default)</td></tr>"
                 "<tr><td>2:</td><td>BT.470-2 M</td></tr>"
                 "<tr><td>3:</td><td>BT.470-2 B, G</td></tr>"
                 "<tr><td>4:</td><td>SMPTE 170M</td></tr>"
                 "<tr><td>5:</td><td>SMPTE 240M</td></tr>"
                 "<tr><td>6:</td><td>Linear (V = Lc)</td></tr>"
                 "<tr><td>7:</td><td>sRGB (very similar to BT.709)</td></tr>"
                 "<tr><td>8-255:</td><td>Reserved</td></tr></table>"},
                {"bMatrixCoefficients", "<p>Matrix used to compute luma and "
                 "chroma values from the color primaries.</p>"
                 "<table><tr><td>0:</td><td>Unspecified (Image characteristics unknown)</td></tr>"
                 "<tr><td>1:</td><td>BT. 709</td></tr>"
                 "<tr><td>2:</td><td>FCC</td></tr>"
                 "<tr><td>3:</td><td>BT.470-2 B, G</td></tr>"
                 "<tr><td>4:</td><td>SMPTE 170M (BT.601, default)</td></tr>"
                 "<tr><td>5:</td><td>SMPTE 240M</td></tr>"
                 "<tr><td>6-255:</td><td>Reserved</td></tr></table>"},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbColorMatchingDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Color Matching Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bColorPrimaries", _bColorPrimaries, __strBColorPrimaries())
                    .attr("bTransferCharacteristics", _bTransferCharacteristics, __strBTransferCharacteristics())
                    .attr("bMatrixCoefficients", _bMatrixCoefficients, __strBMatrixCoefficients())
                    .end()
                    .build();
        }

        UsbColorMatchingDescriptor::UsbColorMatchingDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

        const QStringList &UsbFrameBasedPayloadVideoFormatDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFormatIndex",
                "bNumFrameDescriptors",
                "guidFormat",
                "bBitsPerPixel",
                "bDefaultFrameIndex",
                "bAspectRatioX",
                "bAspectRatioY",
                "bmInterlaceFlags",
                "bCopyProtect",
                "bVariableSize",
            };

            return fields;
        }

        QString UsbFrameBasedPayloadVideoFormatDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FORMAT_FRAME_BASED descriptor subtype"},
                {"bFormatIndex", "Index of this format descriptor"},
                {"bNumFrameDescriptors", "Number of frame descriptors following "
                 "that correspond to this format"},
                {"guidFormat", "Globally Unique Identifier used to "
                 "identify stream-encoding format"},
                {"bBitsPerPixel", "Number of bits per pixel used to "
                 "specify color in the decoded video frame"},
                {"bDefaultFrameIndex", "Optimum Frame Index (used to select "
                 "resolution) for this stream"},
                {"bAspectRatioX", "The X dimension of the picture aspect ratio"},
                {"bAspectRatioY", "The Y dimension of the picture aspect ratio"},
                {"bmInterlaceFlags", "<p>Specifies interlace information. If the "
                 "scanning mode control in the Camera "
                 "Terminal is supported for this stream, "
                 "this field shall reflect the field format "
                 "used in interlaced mode."
                 "(Top field in PAL is field 1, top field in "
                 "NTSC is field 2.):</p>"
                 "<table><tr><td>D0:</td><td><p>Interlaced stream or variable.</p><p>1 = Yes</p></td></tr>"
                 "<tr><td>D1:</td><td><p>Fields per frame.</p><p>0 = 2 fields</p><p>1 = 1 field</p></td></tr>"
                 "<tr><td>D2:</td><td><p>Field 1 first.</p><p>1 = Yes</p></td></tr>"
                 "<tr><td>D3:</td><td>Reserved</td></tr>"
                 "<tr><td>D5..4:</td><td><p>Field pattern</p><p>00 = Field 1 only</p><p>01 = Field 2 only</p>"
                 "<p>10 = Regular pattern of fields 1 and 2</p>"
                 "<p>11 = Random pattern of fields 1 and 2</p></td></tr>"
                 "<tr><td>D7..6:</td><td>Reserved. Do not use.</td></tr></table>"},
                {"bCopyProtect", "<p>Specifies whether duplication of the "
                 "video stream is restricted:</p>"
                 "<table><tr><td>FALSE (0):</td><td>No restrictions</td></tr>"
                 "<tr><td>TRUE (1):</td><td>Restrict duplication</td></tr></table>"},
                {"bVariableSize", ""}
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbFrameBasedPayloadVideoFormatDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Frame Based Payload Video Format Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatIndex", _bFormatIndex)
                    .attr("bNumFrameDescriptors", _bNumFrameDescriptors)
                    .attr("guidFormat", "", hexUuid(_guidFormat))
                    .attr("bBitsPerPixel", _bBitsPerPixel, tr("%1 bit(s) per pixel").arg(_bBitsPerPixel))
                    .attr("bDefaultFrameIndex", _bDefaultFrameIndex)
                    .attr("bAspectRatioX", _bAspectRatioX)
                    .attr("bAspectRatioY", _bAspectRatioY)
                    .attr("bmInterlaceFlags", _bmInterlaceFlags, __parseBmInterlaceFlags())
                    .attr("bCopyProtect", _bCopyProtect, _bCopyProtect ? tr("Restrict duplication") : "")
                    .attr("bVariableSize", _bVariableSize, _bVariableSize ? tr("Variable Size") : tr("Fixed Size"))
                    .end()
                    .build();
        }

        UsbFrameBasedPayloadVideoFormatDescriptor::UsbFrameBasedPayloadVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

            return infomations.join(UsbHtmlBuilder::NEWLINE);
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

        const QStringList &UsbFrameBasedPayloadVideoFrameDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFrameIndex",
                "bmCapabilities",
                "wWidth",
                "wHeight",
                "dwMinBitRate",
                "dwMaxBitRate",
                "dwDefaultFrameInterval",
                "bFrameIntervalType",
                "dwBytesPerLine",
                "dwMinFrameInterval",
                "dwMaxFrameInterval",
                "dwFrameIntervalStep",
                "dwFrameInterval",
            };

            return fields;
        }

        QString UsbFrameBasedPayloadVideoFrameDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FRAME_FRAME_BASED descriptor subtype"},
                {"bFrameIndex", "Index of this frame descriptor"},
                {"bmCapabilities", "<p>D0: Still image supported</p>"
                 "<p>Specifies whether still images are "
                 "supported at this frame setting. This is "
                 "only applicable for VS interfaces with "
                 "an IN video endpoint using Still "
                 "Image Capture Method 1, and should "
                 "be set to 0 in all other cases.</p>"
                 "<p>D1: Fixed frame-rate</p>"
                 "<p>Specifies whether the device provides "
                 "a fixed frame rate on a stream "
                 "associated with this frame descriptor. "
                 "Set to 1 if fixed rate is enabled; "
                 "otherwise, set to 0.</p>"
                 "<p>D7..2: Reserved, set to 0.</p>"},
                {"wWidth", "Width of decoded bitmap frame in pixels"},
                {"wHeight", "Height of decoded bitmap frame in pixels"},
                {"dwMinBitRate", "Specifies the minimum bit rate at the "
                 "longest frame interval in units of bps "
                 "at which the data can be transmitted."},
                {"dwMaxBitRate", "Specifies the maximum bit rate at the "
                 "shortest frame interval in units of bps "
                 "at which the data can be transmitted."},
                {"dwDefaultFrameInterval", "<p>Specifies the frame interval the device "
                 "would like to indicate for use as a "
                 "default. This must be a valid frame "
                 "interval described in the fields below.</p>"},
                {"bFrameIntervalType", "Indicates how the frame interval can be programmed:"
                 "<table><tr><td>0:</td><td>Continuous frame interval</td></tr>"
                 "<tr><td>1..255:</td><td>The number of discrete frame intervals supported</td></tr></table>"},
                {"dwBytesPerLine", "<p>Specifies the number of bytes per line "
                 "of video for packed fixed frame size "
                 "formats, allowing the receiver to "
                 "perform stride alignment of the video.</p>"
                 "<p>If the bVariableSize value (above) is "
                 "TRUE (1), or if the format does not "
                 "permit such alignment, this value shall "
                 "be set to zero (0).</p>"},
                {"dwMinFrameInterval", "Shortest frame interval supported (at "
                 "highest frame rate), in 100 ns units."},
                {"dwMaxFrameInterval", "Longest frame interval supported (at "
                 "lowest frame rate), in 100 ns units."},
                {"dwFrameIntervalStep", "Indicates granularity of frame interval "
                 "range, in 100 ns units."},
                {"dwFrameInterval", "Nth shortest frame interval supported (at "
                 "nth highest frame rate), in 100 ns units."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbFrameBasedPayloadVideoFrameDescriptor::__parseBmCapabilities() const
        {
            QStringList supports;
            if (BIT(_bmCapabilities, 0))
                supports.append(tr("Still image supported"));
            if (BIT(_bmCapabilities, 1))
                supports.append(tr("Fixed frame-rate"));

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbFrameBasedPayloadVideoFrameDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("Frame Based Payload Video Frame Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFrameIndex", _bFrameIndex)
                    .attr("bmCapabilities", _bmCapabilities, __parseBmCapabilities())
                    .attr("wWidth", _wWidth)
                    .attr("wHeight", _wHeight)
                    .attr("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate))
                    .attr("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate))
                    .attr("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                          tr("%1 * 100 ns").arg(_dwDefaultFrameInterval))
                    .attr("bFrameIntervalType", _bFrameIntervalType, _bFrameIntervalType ?
                              tr("%1 discrete frame intervals").arg(_bFrameIntervalType) :
                              tr("Continuous frame interval"))
                    .attr("dwBytesPerLine", _dwBytesPerLine, tr("%1 byte(s)").arg(_dwBytesPerLine));
            if (_bFrameIntervalType)
                for (uint8_t i = 1; i <= _bFrameIntervalType; ++i)
                    builder.attr("dwFrameInterval",
                                 dwFrameInterval(i),
                                 tr("%1 * 100 ns").arg(dwFrameInterval(i)),
                                 i);
            else
                builder.attr("dwMinFrameInterval", dwMinFrameInterval(),
                             tr("%1 * 100 ns").arg(dwMinFrameInterval()))
                        .attr("dwMaxFrameInterval", dwMaxFrameInterval(),
                              tr("%1 * 100 ns").arg(dwMaxFrameInterval()))
                        .attr("dwFrameIntervalStep", _dwFrameIntervalStep,
                              tr("%1 * 100 ns").arg(_dwFrameIntervalStep));

            return builder.end().build();
        }

        UsbFrameBasedPayloadVideoFrameDescriptor::UsbFrameBasedPayloadVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

        const QStringList &UsbStreamBasedFormatDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFormatIndex",
                "guidFormat",
                "dwPacketLength",
            };

            return fields;
        }

        QString UsbStreamBasedFormatDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FORMAT_STREAM_BASED descriptor subtype"},
                {"bFormatIndex", "Index of this format descriptor"},
                {"guidFormat", "Globally Unique Identifier used to "
                 "identify stream-encoding format"},
                {"dwPacketLength", "If non-zero, indicates a format-specific "
                 "packet size in a Packet-Oriented stream. If zero, indicates that the "
                 "format-specific data is either Byte-Oriented, or consists of variable size "
                 "format-specific packets. See section 2.2, \"Payload Data\" of the "
                 "specification for more details."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbStreamBasedFormatDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("Stream Based Format Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatIndex", _bFormatIndex)
                    .attr("guidFormat", "", hexUuid(_guidFormat))
                    .attr("dwPacketLength", _dwPacketLength)
                    .end()
                    .build();
        }

        UsbStreamBasedFormatDescriptor::UsbStreamBasedFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

        const QStringList &UsbH264PayloadVideoFormatDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFormatIndex",
                "bNumFrameDescriptors",
                "bDefaultFrameIndex",
                "bMaxCodecConfigDelay",
                "bmSupportedSliceModes",
                "bmSupportedSyncFrameTypes",
                "bResolutionScaling",
                "bmSupportedRateControlModes",
                "wMaxMBperSecOneResolutionNoScalability",
                "wMaxMBperSecTwoResolutionsNoScalability",
                "wMaxMBperSecThreeResolutionsNoScalability",
                "wMaxMBperSecFourResolutionsNoScalability",
                "wMaxMBperSecOneResolutionTemporalScalability",
                "wMaxMBperSecTwoResolutionTemporalScalability",
                "wMaxMBperSecThreeResolutionTemporalScalability",
                "wMaxMBperSecFourResolutionTemporalScalability",
                "wMaxMBperSecOneResolutionTemporalQualityScalability",
                "wMaxMBperSecTwoResolutionTemporalQualityScalability",
                "wMaxMBperSecThreeResolutionTemporalQualityScalability",
                "wMaxMBperSecFourResolutionTemporalQualityScalability",
                "wMaxMBperSecOneResolutionsTemporalSpatialScalability",
                "wMaxMBperSecTwoResolutionsTemporalSpatialScalability",
                "wMaxMBperSecThreeResolutionsTemporalSpatialScalability",
                "wMaxMBperSecFourResolutionsTemporalSpatialScalability",
                "wMaxMBperSecOneResolutionFullScalability",
                "wMaxMBperSecTwoResolutionFullScalability",
                "wMaxMBperSecThreeResolutionFullScalability",
                "wMaxMBperSecFourResolutionFullScalability",
            };

            return fields;
        }

        QString UsbH264PayloadVideoFormatDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "<p>VS_FORMAT_H264 or VS_FORMAT_H264_SIMULCAST "
                 "descriptor subtype (defined as 0x13 or 0x15).</p>"
                 "<p>For devices that support simulcast transport, the device should create a "
                 "video format descriptor with this field set to VS_FORMAT_H264_SIMULCAST. "
                 "All the video frame descriptors that support simulcast shall be under this format.</p>"
                 "<p>Video frame descriptors that do not support simulcast transport must be "
                 "under a video format descriptor with this field set to VS_FORMAT_H264.</p>"},
                {"bFormatIndex", "Index of this format descriptor. The "
                 "index must be unique from other "
                 "format descriptors in the same video interface."},
                {"bNumFrameDescriptors", "Number of Frame Descriptors "
                 "following that correspond to this format"},
                {"bDefaultFrameIndex", "Default frame index."},
                {"bMaxCodecConfigDelay", "Maximum number of frames the "
                 "encoder takes to respond to a command."},
                {"bmSupportedSliceModes", "<p>Slice mode:</p>"
                 "<table><tr><td>D0:</td><td>Maximum number of MBs per slice mode</td></tr>"
                 "<tr><td>D1:</td><td>Target compressed size per slice mode</td></tr>"
                 "<tr><td>D2:</td><td>Number of slices per frame mode</td></tr>"
                 "<tr><td>D3:</td><td>Number of Macroblock rows per slice mode</td></tr>"
                 "<tr><td>D7-4:</td><td>Reserved, set to 0</td></tr></table>"
                 "<p>Set everything to 0 if only one slice per frame is supported.</p>"},
                {"bmSupportedSyncFrameTypes", "<table><tr><td>D0:</td><td>Reset</td></tr>"
                 "<tr><td>D1:</td><td>IDR frame with SPS and PPS headers.</td></tr>"
                 "<tr><td>D2:</td><td>IDR frame (with SPS and PPS "
                 "headers) that is a long term reference frame.</td></tr>"
                 "<tr><td>D3:</td><td>Non-IDR random-access I frame (with SPS and PPS headers).</td></tr>"
                 "<tr><td>D4:</td><td>Generate a random-access I frame (with SPS and PPS headers) that is not "
                 "an IDR frame and it is a long-term reference frame.</td></tr>"
                 "<tr><td>D5:</td><td>P frame that is a long term reference frame.</td></tr>"
                 "<tr><td>D6:</td><td>Gradual Decoder Refresh frames</td></tr>"
                 "<tr><td>D7:</td><td>Reserved, set to 0</td></tr></table>"},
                {"bResolutionScaling", "<p>Specifies the support for resolution downsizing.</p>"
                 "<table><tr><td>0:</td><td>Not supported.</td></tr>"
                 "<tr><td>1:</td><td>Limited to 1.5 or 2.0 scaling in both "
                 "directions, while maintaining the aspect ratio.</td></tr>"
                 "<tr><td>2:</td><td>Limited to 1.0, 1.5 or 2.0 scaling in either direction.</td></tr>"
                 "<tr><td>3:</td><td>Limited to resolutions reported by "
                 "the associated Frame Descriptors</td></tr>"
                 "<tr><td>4:</td><td>Arbitrary scaling.</td></tr>"
                 "<tr><td>5 to 255:</td><td>Reserved</td></tr></table>"
                 "<p>Resolution scaling is implemented using the Video Resolution Encoding "
                 "Unit, and cannot set the resolution above that specified in the currently "
                 "selected frame descriptor</p>"},
                {"bmSupportedRateControlModes", "<p>Supported rate-control modes.</p>"
                 "<table><tr><td>D0:</td><td>Variable bit rate (VBR) with "
                 "underflow allowed (H.264 low_delay_hrd_flag = 1)</td></tr>"
                 "<tr><td>D1:</td><td>Constant bit rate (CBR) (H.264 low_delay_hrd_flag = 0)</td></tr>"
                 "<tr><td>D2:</td><td>Constant QP</td></tr>"
                 "<tr><td>D3:</td><td>Global VBR with underflow allowed (H.264 low_delay_hrd_flag = 1)</td></tr>"
                 "<tr><td>D4:</td><td>VBR without underflow (H.264 low_delay_hrd_flag = 0)</td></tr>"
                 "<tr><td>D5:</td><td>Global VBR without underflow (H.264 low_delay_hrd_flag = 0)</td></tr>"
                 "<tr><td>D7-6:</td><td>Reserved, set to 0</td></tr></table>"},
                {"wMaxMBperSecOneResolutionNoScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for a single AVC stream. See Section "
                 "3.3 for details."},
                {"wMaxMBperSecTwoResolutionsNoScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for two AVC stream. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecThreeResolutionsNoScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for three AVC stream. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecFourResolutionsNoScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for four AVC stream. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecOneResolutionTemporalScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal scalable SVC, summing "
                 "up across all layers when all layers have the same resolution. See Section "
                 "3.3 for details."},
                {"wMaxMBperSecTwoResolutionTemporalScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal scalable SVC, summing "
                 "up across all layers when all layers consist of two different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecThreeResolutionTemporalScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal scalable SVC, summing "
                 "up across all layers when all layers consist of three different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecFourResolutionTemporalScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal scalable SVC, summing "
                 "up across all layers when all layers consist of four different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecOneResolutionTemporalQualityScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal and quality scalable SVC streams, summing "
                 "up across all layers when all layers have the same resolution. See Section "
                 "3.3 for details."},
                {"wMaxMBperSecTwoResolutionTemporalQualityScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal and quality scalable SVC streams, summing "
                 "up across all layers when all layers consist of two different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecThreeResolutionTemporalQualityScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal and quality scalable SVC streams, summing "
                 "up across all layers when all layers consist of three different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecFourResolutionTemporalQualityScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal and quality scalable SVC streams, summing "
                 "up across all layers when all layers consist of four different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecOneResolutionsTemporalSpatialScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal and spatial scalable SVC streams, summing "
                 "up across all layers when all layers have the same resolution. See Section "
                 "3.3 for details."},
                {"wMaxMBperSecTwoResolutionsTemporalSpatialScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal and spatial scalable SVC streams, summing "
                 "up across all layers when all layers consist of two different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecThreeResolutionsTemporalSpatialScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal and spatial scalable SVC streams, summing "
                 "up across all layers when all layers consist of three different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecFourResolutionsTemporalSpatialScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for temporal and spatial scalable SVC streams, summing "
                 "up across all layers when all layers consist of four different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecOneResolutionFullScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for fully scalable streams, summing "
                 "up across all layers when all layers have the same resolution. See Section "
                 "3.3 for details."},
                {"wMaxMBperSecTwoResolutionFullScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for fully scalable streams, summing "
                 "up across all layers when all layers consist of two different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecThreeResolutionFullScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for fully scalable streams, summing "
                 "up across all layers when all layers consist of three different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
                {"wMaxMBperSecFourResolutionFullScalability", "Maximum macroblock processing "
                 "rate, in units of 1000 MB/s, allowed for fully scalable streams, summing "
                 "up across all layers when all layers consist of four different resolution. See Section "
                 "3.3 for details. Zero for devices that do not support simulcast."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbH264PayloadVideoFormatDescriptor::infomationToHtml() const
        {
            static constexpr const char *const order[] = {
                "One", "Two", "Three", "Four"
            };

            UsbHtmlBuilder builder;
            builder.start(tr("H.264 Payload Video Format Descriptor"), true, "UVC1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatIndex", _bFormatIndex)
                    .attr("bNumFrameDescriptors", _bNumFrameDescriptors)
                    .attr("bMaxCodecConfigDelay", _bMaxCodecConfigDelay, tr("%1 frame(s)").arg(_bMaxCodecConfigDelay))
                    .attr("bmSupportedSliceModes", _bmSupportedSliceModes, __parseBmSupportedSliceModes())
                    .attr("bmSupportedSyncFrameTypes", _bmSupportedSyncFrameTypes, __parseBmSupportedSyncFrameTypes())
                    .attr("bResolutionScaling", _bResolutionScaling, __strBResolutionScaling())
                    .attr("bmSupportedRateControlModes", _bmSupportedRateControlModes, __parseBmSupportedRateControlModes());
            for (uint8_t i = 0; i < 4; ++i)
                builder.attr(QString("wMaxMBperSec%1ResolutionNoScalability").arg(order[i]),
                             _wMaxMBperSecResolutionNoScalability[i],
                             tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionNoScalability[i]));
            for (uint8_t i = 0; i < 4; ++i)
                builder.attr(QString("wMaxMBperSec%1ResolutionTemporalScalability").arg(order[i]),
                             _wMaxMBperSecResolutionTemporalScalability[i],
                             tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionTemporalScalability[i]));
            for (uint8_t i = 0; i < 4; ++i)
                builder.attr(QString("wMaxMBperSec%1ResolutionTemporalQualityScalability").arg(order[i]),
                             _wMaxMBperSecResolutionTemporalQualityScalability[i],
                             tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionTemporalQualityScalability[i]));
            for (uint8_t i = 0; i < 4; ++i)
                builder.attr(QString("wMaxMBperSec%1ResolutionsTemporalSpatialScalability").arg(order[i]),
                             _wMaxMBperSecResolutionsTemporalSpatialScalability[i],
                             tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionsTemporalSpatialScalability[i]));
            for (uint8_t i = 0; i < 4; ++i)
                builder.attr(QString("wMaxMBperSec%1ResolutionFullScalability").arg(order[i]),
                             _wMaxMBperSecResolutionFullScalability[i],
                             tr("%1 * 1000 MB/s").arg(_wMaxMBperSecResolutionFullScalability[i]));

            return builder.end().build();
        }

        UsbH264PayloadVideoFormatDescriptor::UsbH264PayloadVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

        const QStringList &UsbH264PayloadVideoFrameDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFrameIndex",
                "wWidth",
                "wHeight",
                "wSARwidth",
                "wSARheight",
                "wProfile",
                "bLevelIDC",
                "wConstrainedToolset",
                "bmSupportedUsages",
                "bmCapabilities",
                "bmSVCCapabilities",
                "bmMVCCapabilities",
                "dwMinBitRate",
                "dwMaxBitRate",
                "dwDefaultFrameInterval",
                "bNumFrameIntervals",
                "dwFrameInterval",
            };

            return fields;
        }

        QString UsbH264PayloadVideoFrameDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FRAME_H264 descriptor subtype"},
                {"bFrameIndex", "Index of this Frame descriptor"},
                {"wWidth", "The width, in pixels, of pictures "
                 "output from the decoding process. Must be a multiple of 2. Does not "
                 "need to be an integer multiple of 16, and can be specified using a "
                 "frame cropping rectangle in the active SPS."},
                {"wHeight", "The height, in pixels, of pictures "
                 "output from the decoding process. Must be a multiple of 2. When "
                 "field coding or frame/field adaptive coding is used, shall be a "
                 "multiple of 4. Does not need to be an integer multiple of 16, and can "
                 "be specified using a frame cropping rectangle in the active SPS."},
                {"wSARwidth", "Sample aspect ratio width (as "
                 "defined in H.264 Annex E); shall be relatively prime with respect to "
                 "<i>bSARheight</i>."},
                {"wSARheight", "Sample aspect ratio height (as "
                 "defined in H.264 Annex E); shall be relatively prime with respect to "
                 "<i>bSARwidth</i>."},
                {"wProfile", "<p>The first two bytes of the sequence parameter set, specified "
                 "by profile_idc and constraint flags in the H.264 specification, to "
                 "indicate the profile and applicable constraints to be used. For example:</p>"
                 "<table><tr><td>0x4240:</td><td>Constrained Baseline Profile</td></tr>"
                 "<tr><td>0x4200:</td><td>Baseline Profile</td></tr>"
                 "<tr><td>0x4D00:</td><td>Main Profile</td></tr>"
                 "<tr><td>0x640C:</td><td>Constrained High Profile</td></tr>"
                 "<tr><td>0x6400:</td><td>High Profile</td></tr>"
                 "<tr><td>0x5304:</td><td>Scalable Constrained Baseline Profile</td></tr>"
                 "<tr><td>0x5300:</td><td>Scalable Baseline Profile</td></tr>"
                 "<tr><td>0x5604:</td><td>Scalable Constrained High Profile</td></tr>"
                 "<tr><td>0x5600:</td><td>Scalable High Profile</td></tr>"
                 "<tr><td>0x7600:</td><td>Multiview High Profile</td></tr>"
                 "<tr><td>0x8000:</td><td>Stereo High Profile</td></tr></table>"},
                {"bLevelIDC", "The level, as specified by the "
                 "level_idc flag (9, 10, 11, 12, 13, 20, 21, 22, 30, 31, 32, 40, 41, 42, "
                 "etc). For example:"
                 "<table><tr><td>0x1F:</td><td>Level 3.1.</td></tr>"
                 "<tr><td>0x28:</td><td>Level 4.0.</td></tr></table>"},
                {"wConstrainedToolset", "Reserved, set to zero"},
                {"bmSupportedUsages", "<table><tr><td>D0:</td><td>Real-time/UCConfig mode 0.</td></tr>"
                 "<tr><td>D1:</td><td>Real-time/UCConfig mode 1.</td></tr>"
                 "<tr><td>D2:</td><td>Real-time/UCConfig mode 2Q.</td></tr>"
                 "<tr><td>D3:</td><td>Real-time/UCConfig mode 2S.</td></tr>"
                 "<tr><td>D4:</td><td>Real-time/UCConfig mode 3.</td></tr>"
                 "<tr><td>D7-5:</td><td>Reserved; set to 0.</td></tr>"
                 "<tr><td>D15-8:</td><td>Broadcast modes.</td></tr>"
                 "<tr><td>D16:</td><td>File Storage mode with I slices (e.g. IPPP). Must be set to 1.</td></tr>"
                 "<tr><td>D17:</td><td>File Storage mode with I, P, and B slices (e.g. IB...BP).</td></tr>"
                 "<tr><td>D18:</td><td>File storage all-I-frame mode.</td></tr>"
                 "<tr><td>D23-19:</td><td>Reserved; set to 0.</td></tr>"
                 "<tr><td>D24:</td><td>MVC Stereo High Mode.</td></tr>"
                 "<tr><td>D25:</td><td>MVC Multiview Mode</td></tr>"
                 "<tr><td>D31-D26:</td><td>Reserved; set to 0.</td></tr></table>"
                 "<p>Devices must support bmSupportedUsages(D16) \"File Storage I, P, P\"</p>"},
                {"bmCapabilities", "<table><tr><td>D0:</td><td>CAVLC only.</td></tr>"
                 "<tr><td>D1:</td><td>CABAC only.</td></tr>"
                 "<tr><td>D2:</td><td>Constant frame rate.</td></tr>"
                 "<tr><td>D3:</td><td>Separate QP for luma/chroma.</td></tr>"
                 "<tr><td>D4:</td><td>Separate QP for Cb/Cr.</td></tr>"
                 "<tr><td>D5:</td><td>No picture reordering.</td></tr>"
                 "<tr><td>D6:</td><td>Long Term Reference frame.</td></tr>"
                 "<tr><td>D15-D7:</td><td>Reserved; set to 0.</td></tr></table>"
                 "<p>Note when D4 is 1, then D3 must be 1.</p>"},
                {"bmSVCCapabilities", "<table><tr><td>D2-0:</td><td>Maximum number of "
                 "temporal layers minus 1.</td></tr>"
                 "<tr><td>D3:</td><td>Rewrite support.</td></tr>"
                 "<tr><td>D6-4:</td><td>Maximum number of CGS layers minus 1.</td></tr>"
                 "<tr><td>D9-7:</td><td>Maximum number of MGS sublayers.</td></tr>"
                 "<tr><td>D10:</td><td>Additional SNR scalability "
                 "support in spatial enhancement layers.</td></tr>"
                 "<tr><td>D13-11:</td><td>Maximum number of spatial layers minus 1.</td></tr>"
                 "<tr><td>D31-14:</td><td>Reserved. Set to zero.</td></tr></table>"
                 "<p>See Section 3.3.2 for details.</p>"},
                {"bmMVCCapabilities", "<table><tr><td>D2-0:</td><td>Maximum number of "
                 "temporal layers minus 1.</td></tr>"
                 "<tr><td>D10-3:</td><td>Maximum number of view components minus 1.</td></tr>"
                 "<tr><td>D31-11:</td><td>Reserved. Set to zero.</td></tr></table>"
                 "<p>See Section 3.4 for details.</p>"},
                {"dwMinBitRate", "Specifies the minimum bit rate, at "
                 "maximum compression and longest frame interval, in units of "
                 "bps, at which the data can be transmitted."},
                {"dwMaxBitRate", "Specifies the maximum bit rate, at "
                 "minimum compression and shortest frame interval, in units of "
                 "bps, at which the data can be transmitted."},
                {"dwDefaultFrameInterval", "Specifies the frame interval the "
                 "device indicates for use as a default, in 100-ns units."},
                {"bNumFrameIntervals", "Specifies the number of frame "
                 "intervals supported."},
                {"dwFrameInterval", "Nth shortest frame interval supported "
                 "(at the nth highest frame rate), in 100-ns units."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbH264PayloadVideoFrameDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder builder;
            builder.start(tr("H.264 Payload Video Frame Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFrameIndex", _bFrameIndex)
                    .attr("wWidth", _wWidth)
                    .attr("wHeight", _wHeight)
                    .attr("wSARwidth", _wSARwidth)
                    .attr("wSARheight", _wSARheight)
                    .attr("wProfile", _wProfile, __strWProfile())
                    .attr("bLevelIDC", _bLevelIDC, __strBLevelIDC())
                    .attr("wConstrainedToolset", _wConstrainedToolset)
                    .attr("bmSupportedUsages", _bmSupportedUsages, __parseBmSupportedUsages())
                    .attr("bmCapabilities", _bmCapabilities, __parseBmCapabilities())
                    .attr("bmSVCCapabilities", _bmSVCCapabilities, __parseBmSVCCapabilities())
                    .attr("bmMVCCapabilities", _bmMVCCapabilities, __parseBmMVCCapabilities())
                    .attr("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate))
                    .attr("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate))
                    .attr("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                          tr("%1 * 100 ns").arg(_dwDefaultFrameInterval))
                    .attr("bNumFrameIntervals", _bNumFrameIntervals);
            for (uint8_t i = 1; i <= _bNumFrameIntervals; ++i)
                builder.attr("dwFrameInterval",
                             dwFrameInterval(i),
                             tr("%1 * 100 ns").arg(dwFrameInterval(i)),
                             i);

            return builder.end().build();
        }

        UsbH264PayloadVideoFrameDescriptor::UsbH264PayloadVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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
                case 83:
                    if (constraint_set_flag[5])
                        return tr("Scalable Constrained Baseline profile");
                    else
                        return tr("Scalable Baseline profile");
                case 86:
                return tr("Scalable High profile");
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
                case 118:
                return tr("Multiview High profile");
                case 122:
                    if (constraint_set_flag[3])
                        return tr("High 4:2:2 Intra profile");
                    else
                        return tr("High 4:2:2 profile");
                case 128:
                return tr("Stereo High profile");
                case 134:
                return tr("MFC High profile");
                case 135:
                return tr("MFC Depth High profile");
                case 138:
                return tr("Multiview Depth High profile");
                case 139:
                return tr("Enhanced Multiview Depth High profile");
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbH264PayloadVideoFrameDescriptor::__parseBmMVCCapabilities() const
        {
            QStringList supports;
            supports.append(tr("Maximum number of temporal layers: %1")
                            .arg(CUT(_bmMVCCapabilities, 0, 2)));
            supports.append(tr("Maximum number of view components: %1")
                            .arg(CUT(_bmMVCCapabilities, 3, 10)));

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

        const QStringList &UsbVp8PayloadVideoFormatDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFormatIndex",
                "bNumFrameDescriptors",
                "bDefaultFrameIndex",
                "bMaxCodecConfigDelay",
                "bSupportedPartitionCount",
                "bmSupportedSyncFrameTypes",
                "bResolutionScaling",
                "bmSupportedRateControlModes",
                "wMaxMBperSec",
            };

            return fields;
        }

        QString UsbVp8PayloadVideoFormatDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FORMAT_VP8 or VS_FORMAT_VP8_SIMULCAST descriptor subtype"},
                {"bFormatIndex", "Index of this format descriptor"},
                {"bNumFrameDescriptors", "Number of Frame Descriptors "
                 "following that correspond to this format."},
                {"bDefaultFrameIndex", "Default frame index."},
                {"bMaxCodecConfigDelay", "Maximum number of frames the "
                 "encoder takes to respond to a command."},
                {"bSupportedPartitionCount", "Maximum number of supported "
                 "partitions per frame. Number must be equal to some power of 2."},
                {"bmSupportedSyncFrameTypes", "<table><tr><td>D0:</td><td>Reset</td></tr>"
                 "<tr><td>D1:</td><td>Intra frame</td></tr>"
                 "<tr><td>D2:</td><td>Golden frame</td></tr>"
                 "<tr><td>D3:</td><td>Alternate reference frame</td></tr>"
                 "<tr><td>D4:</td><td>Gradual Decoder Refresh frames</td></tr>"
                 "<tr><td>D7-D5:</td><td>Reserved, set to 0</td></tr></table>"},
                {"bResolutionScaling", "<p>Specifies the support for resolution downsizing.</p>"
                 "<table><tr><td>0:</td><td>Not supported.</td></tr>"
                 "<tr><td>1:</td><td>Limited to 1.5 or 2.0 scaling in "
                 "both directions, while maintaining the aspect ratio.</td></tr>"
                 "<tr><td>2:</td><td>Limited to 1.0, 1.5 or 2.0 scaling in either direction.</td></tr>"
                 "<tr><td>3:</td><td>Limited to resolutions reported "
                 "by the associated Frame.</td></tr>"
                 "<tr><td>4:</td><td>Arbitrary scaling.</td></tr>"
                 "<tr><td>5 to 255:</td><td>Reserved</td></tr></table>"
                 "<p>Resolution scaling is implemented using the Video "
                 "Resolution Encoding Unit, and cannot set the resolution above "
                 "that specified in the currently selected frame descriptor</p>"},
                {"bmSupportedRateControlModes", "<p>Supported rate-control modes.</p>"
                 "<table><tr><td>D0:</td><td>Variable bit rate (VBR)</td></tr>"
                 "<tr><td>D1:</td><td>Constant bit rate (CBR)</td></tr>"
                 "<tr><td>D2:</td><td>Constant QP</td></tr>"
                 "<tr><td>D3:</td><td>Global VBR</td></tr>"
                 "<tr><td>D7-D4:</td><td>Reserved, set to 0</td></tr></table>"},
                {"wMaxMBperSec", "Maximum macroblock processing "
                 "rate, in units of MB/s, allowed for all VP8 streams by the device."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbVp8PayloadVideoFormatDescriptor::infomationToHtml() const
        {
            return UsbHtmlBuilder()
                    .start(tr("VP8 Payload Video Format Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFormatIndex", _bFormatIndex)
                    .attr("bNumFrameDescriptors", _bNumFrameDescriptors)
                    .attr("bDefaultFrameIndex", _bDefaultFrameIndex)
                    .attr("bMaxCodecConfigDelay", _bMaxCodecConfigDelay)
                    .attr("bSupportedPartitionCount", _bSupportedPartitionCount)
                    .attr("bmSupportedSyncFrameTypes", _bmSupportedSyncFrameTypes, __parseBmSupportedSyncFrameTypes())
                    .attr("bResolutionScaling", _bResolutionScaling, __strBResolutionScaling())
                    .attr("bmSupportedRateControlModes", _bmSupportedRateControlModes, __parseBmSupportedRateControlModes())
                    .attr("wMaxMBperSec", _wMaxMBperSec, tr("%1 MB/s").arg(_wMaxMBperSec))
                    .end()
                    .build();
        }

        UsbVp8PayloadVideoFormatDescriptor::UsbVp8PayloadVideoFormatDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

        const QStringList &UsbVp8PayloadVideoFrameDescriptor::getFieldNames()
        {
            static const QStringList fields = {
                "bLength",
                "bDescriptorType",
                "bDescriptorSubtype",
                "bFrameIndex",
                "wWidth",
                "wHeight",
                "bmSupportedUsages",
                "bmCapabilities",
                "bmScalabilityCapabilities",
                "dwMinBitRate",
                "dwMaxBitRate",
                "dwDefaultFrameInterval",
                "bNumFrameIntervals",
                "dwFrameInterval",
            };

            return fields;
        }

        QString UsbVp8PayloadVideoFrameDescriptor::getFieldInformation(const QString &field)
        {
            static const QMap<QString, QString> fieldDescription = {
                {"bLength", "Size of this descriptor, in bytes"},
                {"bDescriptorType", "CS_INTERFACE descriptor type"},
                {"bDescriptorSubtype", "VS_FRAME_VP8 descriptor subtype"},
                {"bFrameIndex", "Index of this frame descriptor"},
                {"wWidth", "The width, in pixels, of valid "
                 "picture area from the decoding process. Must be a multiple of 2. "
                 "Regardless of <i>wWidth</i> setting coded width is always the next multiple of 16."},
                {"wHeight", "The height, in pixels, of valid "
                 "picture area from the decoding process. Must be a multiple of 2. "
                 "Regardless of <i>wHeight</i> setting coded height is always the next multiple of 16."},
                {"bmSupportedUsages", "<table><tr><td>D0:</td><td>Reserved; set to 0</td></tr>"
                 "<tr><td>D1:</td><td>Real-time.</td></tr>"
                 "<tr><td>D2:</td><td>Real-time with temporal "
                 "layering structure as specified in <i>bmLayoutPerStream</i> field.</td></tr>"
                 "<tr><td>D15-3:</td><td>Reserved; set to 0.</td></tr>"
                 "<tr><td>D16:</td><td>File Storage mode with I and P frames (e.g. IPPP).</td></tr>"
                 "<tr><td>D17:</td><td>Reserved; set to 0.</td></tr>"
                 "<tr><td>D18:</td><td>File storage all-I-frame mode.</td></tr>"
                 "<tr><td>D31-D19:</td><td>Reserved; set to 0.</td></tr></table>"},
                {"bmCapabilities", "<table><tr><td>D1-0:</td><td>Reserved; set to 0</td></tr>"
                 "<tr><td>D2:</td><td>Constant frame rate.</td></tr>"
                 "<tr><td>D3:</td><td>Separate QP for luma/chroma.</td></tr>"
                 "<tr><td>D5-4:</td><td>Reserved; set to 0.</td></tr>"
                 "<tr><td>D6:</td><td>Golden frame.</td></tr>"
                 "<tr><td>D7:</td><td>Alternate reference frame.</td></tr>"
                 "<tr><td>D15-D8:</td><td>Reserved; set to 0.</td></tr></table>"},
                {"bmScalabilityCapabilities", "<table><tr><td>D2-0:</td><td>Maximum number of "
                 "temporal enhancement layers minus 1.</td></tr>"
                 "<tr><td>D31-D3:</td><td>Reserved.</td></tr></table>"},
                {"dwMinBitRate", "Specifies the minimum bit rate, "
                 "at maximum compression and longest frame interval, in units of "
                 "bps, at which the data can be transmitted."},
                {"dwMaxBitRate", "Specifies the maximum bit rate, "
                 "at minimum compression and shortest frame interval, in units "
                 "of bps, at which the data can be transmitted."},
                {"dwDefaultFrameInterval", "Specifies the frame interval the "
                 "device indicates for use as a default, in 100-ns units."},
                {"bNumFrameIntervals", "Specifies the number of frame intervals supported."},
                {"dwFrameInterval", "Nth shortest frame interval supported (at "
                 "nth highest frame rate), in 100 ns units."},
            };

            if (fieldDescription.contains(field))
                return fieldDescription[field];
            else
                return QString();
        }

        QString UsbVp8PayloadVideoFrameDescriptor::infomationToHtml() const
        {
            UsbHtmlBuilder bulider;
            bulider.start(tr("VP8 Payload Video Frame Descriptor"), true, "UVC 1.5")
                    .attr("bLength", _bLength)
                    .attr("bDescriptorType", _bDescriptorType, "CS_INTERFACE")
                    .attr("bDescriptorSubtype", _bDescriptorSubtype, strSubtype(_bDescriptorSubtype))
                    .attr("bFrameIndex", _bFrameIndex)
                    .attr("wWidth", _wWidth)
                    .attr("wHeight", _wHeight)
                    .attr("bmSupportedUsages", _bmSupportedUsages, __parseBmSupportedUsages())
                    .attr("bmCapabilities", _bmCapabilities, __parseBmCapabilities())
                    .attr("bmScalabilityCapabilities", _bmScalabilityCapabilities, __parseBmScalabilityCapabilities())
                    .attr("dwMinBitRate", _dwMinBitRate, tr("%1 bps").arg(_dwMinBitRate))
                    .attr("dwMaxBitRate", _dwMaxBitRate, tr("%1 bps").arg(_dwMaxBitRate))
                    .attr("dwDefaultFrameInterval", _dwDefaultFrameInterval,
                          tr("%1 * 100 ns").arg(_dwDefaultFrameInterval))
                    .attr("bNumFrameIntervals", _bNumFrameIntervals);
            for (uint8_t i = 1; i <= _bNumFrameIntervals; ++i)
                bulider.attr("dwFrameInterval",
                             dwFrameInterval(i),
                             tr("%1 * 100 ns").arg(dwFrameInterval(i)),
                             i);

            return bulider.end().build();
        }

        UsbVp8PayloadVideoFrameDescriptor::UsbVp8PayloadVideoFrameDescriptor(
                UsbInterfaceDescriptor *parent, uint32_t descPos):
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
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

            return supports.join(UsbHtmlBuilder::NEWLINE);
        }

        QString UsbVp8PayloadVideoFrameDescriptor::__parseBmScalabilityCapabilities() const
        {
            return tr("maximum number of temporal enhancement layers: %1")
                    .arg(CUT(_bmScalabilityCapabilities, 0, 2) + 1);
        }
    } // namespace uvc
} // namespace usb
