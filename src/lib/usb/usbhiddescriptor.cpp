#include "usbhiddescriptor.h"
#include "__usbmacro.h"

namespace usb {
    constexpr const char *const UsbHidDescriptor::_countryCodeMap[36] = {
        "Not Supported",                    // 00
        "Arabic",                           // 01
        "Belgian",                          // 02
        "Canadian-Bilingual",               // 03
        "Canadian-French",                  // 04
        "Czech Republic",                   // 05
        "Danish",                           // 06
        "Finnish",                          // 07
        "French",                           // 08
        "German",                           // 09
        "Greek",                            // 10
        "Hebrew",                           // 11
        "Hungary",                          // 12
        "International (ISO)",              // 13
        "Italian",                          // 14
        "Japan (Katakana)",                 // 15
        "Korean",                           // 16
        "Latin American",                   // 17
        "Netherlands/Dutch",                // 18
        "Norwegian",                        // 19
        "Persian (Farsi)",                  // 20
        "Poland",                           // 21
        "Portuguese",                       // 22
        "Russia",                           // 23
        "Slovakia",                         // 24
        "Spanish",                          // 25
        "Swedish",                          // 26
        "Swiss/French",                     // 27
        "Swiss/German",                     // 28
        "Switzerland",                      // 29
        "Taiwan",                           // 30
        "Turkish-Q",                        // 31
        "UK",                               // 32
        "US",                               // 33
        "Yugoslavia",                       // 34
        "Turkish-F"                         // 35
    };

    UsbHidDescriptor::UsbHidDescriptor(UsbInterfaceDescriptor *interfaceDescriptor):
        QObject(interfaceDescriptor)
    {
        const unsigned char *extra = interfaceDescriptor->extra();

        _bLength = *extra;
        _bDescriptorType = *(extra + 1);
        _bcdHID = *reinterpret_cast<const uint16_t *>(extra + 2);
        _bCountryCode = *(extra + 4);
        _bNumDescriptors = *(extra + 5);
        _interfaceDescriptor = interfaceDescriptor;

        _hidReportDescriptor = new UsbHidReportDescriptor(this);
        _hidReportDescriptor->_bDescriptorType = *(extra + 6);
        _hidReportDescriptor->_wDescriptorLength = *reinterpret_cast<const uint16_t *>(extra + 7);

        interfaceDescriptor->interface()->claim();

        UsbDevice *device = _interfaceDescriptor->interface()->configDescriptor()->device();
        QByteArray buffer('\0', _hidReportDescriptor->_wDescriptorLength);
        int ret = device->controlTransfer(0x81,
                                LIBUSB_REQUEST_GET_DESCRIPTOR,
                                LIBUSB_DT_REPORT << 8,
                                interfaceDescriptor->bInterfaceNumber(),
                                buffer,
                                TRANSFER_TIMEOUT);
        interfaceDescriptor->interface()->release();
        if (ret < LIBUSB_SUCCESS)
        {
            delete _hidReportDescriptor;
            _hidReportDescriptor = nullptr;
            LOGE(tr("Failed to request HID report descriptor, libusb reports: %1.").arg(libusb_error_name(ret)));
            return;
        }

        _hidReportDescriptor->_rawDescriptor = buffer;
    }

    uint8_t UsbHidDescriptor::bLength() const
    {
        return _bLength;
    }

    uint8_t UsbHidDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint16_t UsbHidDescriptor::bcdHID() const
    {
        return _bcdHID;
    }

    uint8_t UsbHidDescriptor::bCountryCode() const
    {
        return _bCountryCode;
    }

    uint8_t UsbHidDescriptor::bNumDescriptors() const
    {
        return _bNumDescriptors;
    }

    QString UsbHidDescriptor::country() const
    {
        // Dummy code, just for triggering Linguist
#if 0
        tr("Not Supported");tr("Arabic");tr("Belgian");tr("Canadian-Bilingual");
        tr("Canadian-French");tr("Czech Republic");tr("Danish");tr("Finnish");
        tr("French");tr("German");tr("Greek");tr("Hebrew");
        tr("Hungary");tr("International (ISO)");tr("Italian");tr("Japan (Katakana)");
        tr("Korean");tr("Latin American");tr("Netherlands/Dutch");tr("Norwegian");
        tr("Persian (Farsi)");tr("Poland");tr("Portuguese");tr("Russia");
        tr("Slovakia");tr("Spanish");tr("Swedish");tr("Swiss/French");
        tr("Swiss/German");tr("Switzerland");tr("Taiwan");tr("Turkish-Q");
        tr("UK");tr("US");tr("Yugoslavia");tr("Turkish-F");
#endif

        return tr(_countryCodeMap[_bCountryCode]);
    }

    UsbHidReportDescriptor *UsbHidDescriptor::hidReportDescriptor() const
    {
        return _hidReportDescriptor;
    }

    QString UsbHidDescriptor::infomationToHtml() const
    {
        QString html;
        START("HID Descriptor");
        ATTR("bLength", _bLength, _bLength);
        ATTR("bcdHID", _bcdHID, "");
        ATTR("bCountryCode", _bCountryCode, country());
        ATTR("bNumDescriptors", _bNumDescriptors, _bNumDescriptors);
        END;

        return html;
    }

    UsbInterfaceDescriptor *UsbHidDescriptor::interfaceDescriptor() const
    {
        return _interfaceDescriptor;
    }

    UsbHidReportDescriptor::UsbHidReportDescriptor(UsbHidDescriptor *parent):
        QObject(parent)
    {

    }

    uint8_t UsbHidReportDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint16_t UsbHidReportDescriptor::wDescriptorLength() const
    {
        return _wDescriptorLength;
    }

    const QByteArray &UsbHidReportDescriptor::rawDescriptor() const
    {
        return _rawDescriptor;
    }
}
