﻿#ifndef USBHIDDESCRIPTOR_H
#define USBHIDDESCRIPTOR_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <log/logger.h>

namespace usb {
    class UsbInterfaceDescriptor;
    class UsbHidDescriptor;
    class UsbHidReportDescriptor;
}

#ifndef USBINTERFACE_H
#include "usbinterfacedescriptor.h"
#endif

namespace usb {
    /**
     * @brief The UsbHidDescriptor class
     * USB HID Descriptor C++ class.
     * Parse it from the extra_data of UsbInterfaceDescriptor
     */
    class UsbHidDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbHidDescriptor(UsbInterfaceDescriptor *interfaceDescriptor = nullptr);

        /**
         * @brief interfaceDescriptor
         * @return the Interface Desciptor which current HID Descriptor belongs to
         */
        UsbInterfaceDescriptor *interfaceDescriptor() const;

        /**
         * @brief bLength
         * @return the total size of the HID descriptor
         */
        uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_HID in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief bcdHID
         * @return the HID Class Specification release in binary-coded decimal
         */
        uint16_t bcdHID() const;

        /**
         * @brief bCountryCode
         * @return country code of the localized hardware
         * @note
         * Most hardware is not localized and thus this value would be zero.
         * Usualy used in keyboard devices.
         * @see UsbHidDescriptor::country()
         */
        uint8_t bCountryCode() const;

        /**
         * @brief bNumDescriptors
         * @return the number of additional class specific descriptors present
         * @note
         * Must be at least one (1) as a Report descriptor will always be present.
         * @see UsbHidDescriptor::hidReportDescriptor()
         */
        uint8_t bNumDescriptors() const;

        /**
         * @brief country
         * @return country name of bCountryCode
         */
        QString country() const;

        /**
         * @brief hidReportDescriptor
         * @return HID Report Descriptor
         */
        UsbHidReportDescriptor *hidReportDescriptor() const;

        /**
         * @brief infomationToHtml
         * @return HTML form device informations
         */
        QString infomationToHtml() const;

    private:
        uint8_t _bLength, _bDescriptorType, _bCountryCode, _bNumDescriptors;
        uint16_t _bcdHID;
        UsbInterfaceDescriptor *_interfaceDescriptor;
        UsbHidReportDescriptor *_hidReportDescriptor;
        bool _mouseProtected, _keyboardProtected;

        static const char *const _countryCodeMap[36];
    };

    /**
     * @brief The UsbHidReportDescriptor class
     * The USB HID Report Descriptor
     * @note
     * This descriptor should be generate from UsbHidDescriptor,
     * so make its constructor private.
     * @see UsbHidDescriptor::hidReportDescriptor()
     */
    class UsbHidReportDescriptor: public QObject
    {
    private:
        explicit UsbHidReportDescriptor(UsbHidDescriptor *parent = nullptr);
        friend UsbHidDescriptor;

    public:
        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_REPORT in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief wDescriptorLength
         * @return the total size of the Report descriptor
         */
        uint16_t wDescriptorLength() const;

        /**
         * @brief rawDescriptor
         * @return binary raw descriptor data
         */
        const QByteArray &rawDescriptor() const;

        /**
         * @brief humanReadableDescriptor
         * @return human readable descriptor as specification defined
         */
        QString humanReadableDescriptor() const;

        /**
         * @brief infomationToHtml
         * @return HTML form device informations
         */
        QString infomationToHtml() const;

    private:
        uint8_t _bDescriptorType;
        uint16_t _wDescriptorLength;
        QByteArray _rawDescriptor;
    };

}
#endif // USBHIDDESCRIPTOR_H
