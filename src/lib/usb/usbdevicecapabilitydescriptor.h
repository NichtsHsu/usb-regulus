#ifndef USBDEVICECAPABILITYDESCRIPTOR_H
#define USBDEVICECAPABILITYDESCRIPTOR_H

#include <libusb.h>
#include <QObject>
#include <QByteArray>

namespace usb {
    class UsbBosDescriptor;
};

#ifndef USBBOSDESCRIPTOR_H
#include "usbbosdescriptor.h"
#endif

namespace usb {
    enum class DeviceCapabilityType {
        WIRELESS = 0x01,
        USB_2_0_EXTENSION = 0x02,
        SUPERSPEED = 0x03,
        CONTAINER_ID = 0x04,
        PLATFORM = 0x05,
        POWER_DELIVERY = 0x06,
        BATTERY_INFO = 0x07,
        PD_CONSUMER_PORT = 0x08,
        PD_PROVIDER_PORT = 0x09,
        SUPERSPEED_PLUS = 0x0A,
        PRECISION_TIME_MEASUREMENT = 0x0B,
        WIRELESS_EXT = 0x0C,
        BILLBOARD = 0X0D,
        AUTHENTICATION = 0x0E,
        BILLBOARD_EX = 0x0F,
        CONFIGURATION_SUMMARY = 0x10,
    };

    /**
     * @brief The UsbDeviceCapabilityDescriptor class
     * The C++ wrapper of libusb_bos_dev_capability_descriptor.
     * As the base class for Usb20ExtensionDescriptor, UsbSuperSpeedDeviceCapabilityDescriptor,
     * UsbContainerIdDescriptor and UsbWirelessDeviceCapabilityDescriptor,
     * so that we can just keep `UsbDeviceCapabilityDescriptor *` in UsbBosDescriptor.
     * Can be converted to the corresponding derived class according to `type()` function:
     *      WIRELESS -> UsbWirelessDeviceCapabilityDescriptor
     *      USB_2_0 -> Usb20ExtensionDescriptor
     *      SUPERSPEED -> UsbSuperSpeedDeviceCapabilityDescriptor
     *      CONTAINER_ID -> UsbContainerIdDescriptor
     */
    class UsbDeviceCapabilityDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbDeviceCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *desc,
                UsbBosDescriptor *parent = nullptr);

        virtual ~UsbDeviceCapabilityDescriptor() = default;

        /**
         * @brief bLength
         * @return size of this descriptor
         */
        virtual uint8_t bLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         * libusb_descriptor_type::LIBUSB_DT_DEVICE_CAPABILITY in this context
         */
        uint8_t bDescriptorType() const;

        /**
         * @brief type
         * @return the real device capability descriptor type
         *      WIRELESS -> UsbWirelessDeviceCapabilityDescriptor
         *      USB_2_0 -> Usb20ExtensionDescriptor
         *      SUPERSPEED -> UsbSuperSpeedDeviceCapabilityDescriptor
         *      CONTAINER_ID -> UsbContainerIdDescriptor
         */
        DeviceCapabilityType type() const;

        /**
         * @brief bosDescriptor
         * @return the parent UsbBosDescriptor
         */
        UsbBosDescriptor *bosDescriptor() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const = 0;

    protected:
        uint8_t _bLength, _bDevCapabilityType, _bDescriptorType;
        UsbBosDescriptor *_bosDescriptor;
    };

    /**
     * @brief The UsbWirelessDeviceCapabilityDescriptor class
     * Libusb does not support Wireless Device Capability Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::WIRELESS
     */
    class UsbWirelessDeviceCapabilityDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbWirelessDeviceCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:

    };

    /**
     * @brief The Usb20ExtensionDescriptor class
     * The C++ wrapper of libusb_usb_2_0_extension_descriptor
     * @see DeviceCapabilityType::USB_2_0_EXTENSION
     */
    class Usb20ExtensionDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit Usb20ExtensionDescriptor(
                const libusb_usb_2_0_extension_descriptor *desc,
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief bLength
         * @return size of this descriptor
         */
        virtual uint8_t bLength() const override;

        /**
         * @brief bmAttributes
         * @return bitmap encoding of supported device level features
         * @see libusb_usb_2_0_extension_attributes
         * @see Usb20ExtensionDescriptor::supportLPM()
         */
        uint32_t bmAttributes() const;

        /**
         * @brief supportLPM
         * Equivalent to `Usb20ExtensionDescriptor::bmAttributes() & LIBUSB_BM_LPM_SUPPORT`
         * @return if LPM(Link Power Management) is supported
         */
        bool supportLPM() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        uint8_t _bLength;
        uint32_t _bmAttributes;
    };

    /**
     * @brief The UsbSuperSpeedDeviceCapabilityDescriptor class
     * The C++ wrapper of libusb_ss_usb_device_capability_descriptor
     * @see DeviceCapabilityType::SUPERSPEED
     */
    class UsbSuperSpeedDeviceCapabilityDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbSuperSpeedDeviceCapabilityDescriptor(
                const libusb_ss_usb_device_capability_descriptor *desc,
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief bLength
         * @return size of this descriptor
         */
        virtual uint8_t bLength() const override;

        /**
         * @brief bmAttributes
         * @return bitmap encoding of supported device level features
         * @see libusb_ss_usb_device_capability_attributes
         * @see UsbSuperSpeedDeviceCapabilityDescriptor::supportLTM()
         */
        uint8_t bmAttributes() const;

        /**
         * @brief bFunctionalitySupport
         * @return if the lowest speed at which all the functionality
         * that the device supports is available to the user
         */
        uint8_t bFunctionalitySupport() const;

        /**
         * @brief bU1DevExitLat
         * @return U1 Device Exit Latency
         */
        uint8_t bU1DevExitLat() const;

        /**
         * @brief wSpeedSupported
         * @return bitmap encoding of the speed supported by this device
         * when operating in SuperSpeed mode
         * @see libusb_supported_speed
         * @see UsbSuperSpeedDeviceCapabilityDescriptor::speedSupportedList()
         */
        uint16_t wSpeedSupported() const;

        /**
         * @brief bU2DevExitLat
         * @return U2 Device Exit Latency
         */
        uint16_t bU2DevExitLat() const;

        /**
         * @brief supportLTM
         * Equivalent to `UsbSuperSpeedDeviceCapabilityDescriptor::bmAttributes() & LIBUSB_BM_LTM_SUPPORT`
         * @return if LTM(Latency Tolerance Messages) is supported
         */
        bool supportLTM() const;

        /**
         * @brief speedSupportedList
         * @return a list of supported speeds
         */
        QStringList speedSupportedList() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        uint8_t _bLength, _bmAttributes, _bFunctionalitySupport, _bU1DevExitLat;
        uint16_t _wSpeedSupported, _bU2DevExitLat;
    };

    /**
     * @brief The UsbContainerIdDescriptor class
     * The C++ wrapper of libusb_container_id_descriptor
     * @see DeviceCapabilityType::CONTAINER_ID
     */
    class UsbContainerIdDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbContainerIdDescriptor(
                const libusb_container_id_descriptor *desc,
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief bLength
         * @return size of this descriptor
         */
        uint8_t bLength() const override;

        /**
         * @brief containerID
         * @return 128 bit UUID
         */
        const QByteArray &containerID() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        uint8_t _bLength;
        QByteArray _containerID;
    };

    /**
     * @brief The UsbSuperSpeedPlusDeviceCapabilityDescriptor class
     * Libusb does not support USB SuperSpeed Plus Device Capability Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::SUPERSPEED_PLUS
     */
    class UsbSuperSpeedPlusDeviceCapabilityDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbSuperSpeedPlusDeviceCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

        /**
         * @brief bmAttributes
         * @return bitmap encoding of supported SuperSpeedPlus
         * @note
         * D4..0    Sublink Speed Attribute Count
         * D8..5    Sublink Speed ID Count
         * D31..9   Reserved
         */
        uint32_t bmAttributes() const;

        /**
         * @brief wFunctionalitySupport
         * The device shall support full functionality at all
         * reported bandwidths at or above the minimum
         * bandwidth described via this field.
         * @note
         * D3..0    Sublink Speed Attribute ID
         * D7..4    Reserved
         * D11..8   Min Rx Lane Count
         * D15..12  Min Tx Lane Count
         */
        uint16_t wFunctionalitySupport() const;

        /**
         * @brief sublinkSpeedAttr
         * @param index
         * Index must be less than (Sublink Speed Attribute Count + 1)
         * @return bitmap encoding of the specified Sublink’s characteristics
         * @note
         * D3..0    Sublink Speed Attribute ID
         * D5..4    Lane Speed Exponent
         * D7..6    Sublink Type
         * D13..8   Reserved
         * D15..14  Link Protocol
         * D31..16  Lane Speed Mantissa
         */
        uint32_t sublinkSpeedAttr(size_t index) const;

    private:
        inline QString __parseBmAttributes() const;
        inline QString __parseFunctionalitySupport() const;
        inline QString __parseSublinkSpeedAttr(size_t index) const;

        uint16_t _wFunctionalitySupport;
        uint32_t _bmAttributes;
        const uint32_t *_bmSublinkSpeedAttr;
    };
}

#endif // USBDEVICECAPABILITYDESCRIPTOR_H
