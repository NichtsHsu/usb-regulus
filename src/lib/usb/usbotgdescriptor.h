#ifndef USBOTGDESCRIPTOR_H
#define USBOTGDESCRIPTOR_H

#include "usbconfigurationextradescriptor.h"
#include <QObject>

namespace usb {
    /**
     * @brief The UsbOtgDescriptor class
     * The OTG (On-The-Go) Descriptor C++ wrapper
     */
    class UsbOtgDescriptor : public UsbConfigurationExtraDescriptor
    {
        Q_OBJECT
    public:
        /**
         * @brief bLength
         * @return the total size of the interface association descriptor
         */
        uint8_t bLength() const override;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         */
        uint8_t bDescriptorType() const override;

        /**
         * @brief type
         * @return ConfigurationExtraDescriptorType::OTG
         */
        ConfigurationExtraDescriptorType type() const override;

        /**
         * @brief bmAttributes
         * @return bitmap of attributes
         * @note
         * D0       SRP support
         * D1       HNP support
         * D2       ADP support
         * D3       RSP support
         * D7..4    Reserved
         */
        uint8_t bmAttributes() const;

        /**
         * @brief bcdOTG
         * @return  BCD version number that identifies the release of the OTG and EH supplement
         */
        uint16_t bcdOTG() const;

        /**
         * @brief infomationToHtml
         * @return HTML form device informations
         */
        QString infomationToHtml() const override;

    private:
        explicit UsbOtgDescriptor(UsbConfigurationDescriptor *configDesc);
        friend UsbConfigurationExtraDescriptor;
        void __initWithBuffer(const uint8_t *buffer) override;
        inline QString __parseBmAttributes() const;

        uint8_t _bLength, _bDescriptorType, _bmAttributes;
        uint16_t _bcdOTG;
    };
} // namespace usb

#endif // USB_USBOTGDESCRIPTOR_H
