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
         * @brief bmAttributes
         * @return bitmap encoding of supported device level features
         * @note
         * D0       Reserved
         * D1       P2P-DRD
         * D3..2    Beacon Behavior
         * D4       Power Indication
         * D7..5    Reserved
         */
        uint8_t bmAttributes() const;

        /**
         * @brief wPHYRates
         * @return bitmap describes the PHY-level signaling rate capabilities
         * of this device implementation represented as a bitmask
         * @note
         * Bit  Rate(Mbps)
         * D0   53.3    **required
         * D1   80
         * D2   106.7   **required
         * D3   160
         * D4   200     **required
         * D5   320
         * D6   400
         * D7   480
         * 15:8 Reserved. Must be zero.
         */
        uint16_t wPHYRates() const;

        /**
         * @brief bmTFITXPowerInfo
         * @return bitmap reports the PHY transmit power levels supported by
         * this device when transmitting on a TFI channel.
         * @note
         * D3..0    Power Level Steps
         * D7..4    Step Size (0~15 indicate 1.0~4.75)
         */
        uint8_t bmTFITXPowerInfo() const;

        /**
         * @brief bmFFITXPowerInfo
         * @return bitmap reports the PHY transmit power levels supported by
         * this device when transmitting on an FFI channel.
         * @note
         * D3..0    Power Level Steps
         * D7..4    Step Size (0~15 indicate 1.0~4.75)
         */
        uint8_t bmFFITXPowerInfo() const;

        /**
         * @brief bmBandGroup
         * @return bitmap reports which UWB band groups are supported by this implementation
         * @note
         * 1 at bit 0 means all bands in band group one are supported
         */
        uint16_t bmBandGroup() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        inline QString __parseBmAttributes() const;
        inline QStringList __parseWPHYRateList() const;
        inline QString __parsePowerInfo(uint8_t info) const;
        inline QStringList __parseBmBandGroupList() const;

        uint8_t _bmAttributes, _bmTFITXPowerInfo, _bmFFITXPowerInfo;
        uint16_t _wPHYRates, _bmBandGroup;
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
     * @brief The UsbPlatformDescriptor class
     * Libusb does not support USB Platform Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::PLATFORM
     */
    class UsbPlatformDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPlatformDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief platformCapabilityUUID
         * @return a 128-bit number that uniquely identifies
         * a platform specific capability of the device
         */
        const QByteArray &platformCapabilityUUID() const;

        /**
         * @brief capabilityData
         * @return a variable-length field containing data
         * associated with the platform specific capability.
         * This field may be zero bytes in length.
         */
        const QByteArray &capabilityData() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        QByteArray _platformCapabilityUUID, _capabilityData;
    };

    /**
     * @brief The UsbPowerDeliveryCapabilityDescriptor class
     * Libusb does not support USB Power Delivery Capability Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::POWER_DELIVERY
     */
    class UsbPowerDeliveryCapabilityDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPowerDeliveryCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief bmAttributes
         * @return bitmap encoding of supported device level features
         * @note
         * D0       Reversed
         * D1       Supports the Battery Charging Specification
         * D2       Supports the USB Power Delivery Specification
         * D3       Provider (device is capable of providing power)
         * D4       Consumer (device is a consumer of power)
         * D5       Supports the feature CHARGING_POLICY
         * D6       Supports power capabilities defined in the USB Type-C Specification
         * D7       Reversed
         * D8       Supports AC Supply Power Source
         * D9       Supports Battery Power Source
         * D10      Supports Other Power Source
         * D13..11  Number of batteries when D9 is set to one
         * D14      Uses VBUS
         * D31..15  Reversed
         * At least one of D8, D9 and D14 shall be set to one
         */
        uint32_t bmAttributes() const;

        /**
         * @brief bcdBCVersion
         * @return Battery Charging Specification Release Number in Binary-Coded Decimal
         * @note
         * Only be Valid if the device indicates that it supports
         * Battery Charging Specification in the bmAttributes field.
         */
        uint16_t bcdBCVersion() const;

        /**
         * @brief bcdPDVersion
         * @return USB Power Delivery Specification Release Number in Binary-Coded Decimal
         * @note
         * Only be Valid if the device indicates that it supports
         * USB Power Delivery Specification in the bmAttributes field.
         */
        uint16_t bcdPDVersion() const;

        /**
         * @brief bcdUSBTypeCVersion
         * @return USB Type-C Specification Release Number in Binary-Coded Decimal
         * @note
         * Only be Valid if the device indicates that it supports
         * USB Type-C Specification in the bmAttributes field.
         */
        uint16_t bcdUSBTypeCVersion() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        inline QString __parseBmAttributes() const;

        uint16_t _bcdBCVersion, _bcdPDVersion, _bcdUSBTypeCVersion;
        uint32_t _bmAttributes;
    };

    /**
     * @brief The UsbBatteryInfoCapabilityDescriptor class
     * Libusb does not support Battery Info Capability Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::BATTERY_INFO
     */
    class UsbBatteryInfoCapabilityDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbBatteryInfoCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief iBattery
         * @return index of string descriptor that contains the user-friendly name
         */
        uint8_t iBattery() const;

        /**
         * @brief iSerial
         * @return index of string descriptor that contains the Serial Number string
         */
        uint8_t iSerial() const;

        /**
         * @brief iManufacturer
         * @return index of string descriptor that contains the name of the Manufacturer
         */
        uint8_t iManufacturer() const;

        /**
         * @brief bBatteryId
         * @return value uniquely identify this Battery in status Messages
         */
        uint8_t bBatteryId() const;

        /**
         * @brief dwChargedThreshold
         * @return Battery Charge value that should be considered to be fully charged
         */
        uint32_t dwChargedThreshold() const;

        /**
         * @brief dwWeakThreshold
         * @return minimum charge level
         */
        uint32_t dwWeakThreshold() const;

        /**
         * @brief dwBatteryDesignCapacity
         * @return design capacity
         */
        uint32_t dwBatteryDesignCapacity() const;

        /**
         * @brief dwBatteryLastFullchargeCapacity
         * @return maximum capacity when fully charged
         */
        uint32_t dwBatteryLastFullchargeCapacity() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        uint8_t _iBattery, _iSerial, _iManufacturer, _bBatteryId;
        uint32_t _dwChargedThreshold, _dwWeakThreshold,
        _dwBatteryDesignCapacity, _dwBatteryLastFullchargeCapacity;
    };

    /**
     * @brief The UsbPowerDeliveryConsumerPortCapabilityDescriptor class
     * Libusb does not support PD Consumer Port Capability Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::PD_CONSUMER_PORT
     */
    class UsbPowerDeliveryConsumerPortCapabilityDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPowerDeliveryConsumerPortCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief bmCapabilities
         * @return bitmap indicate the specification the Consumer Port will operate
         * @note
         * D0       Battery Charging Specification
         * D1       USB Power Delivery Specification
         * D2       USB Type-C Current Specification
         * D15..3   Reserved
         */
        uint16_t bmCapabilities() const;

        /**
         * @brief wMinVoltage
         * @return minimum Voltage in 50mV units that this Consumer is capable of operating at
         */
        uint16_t wMinVoltage() const;

        /**
         * @brief wMaxVoltage
         * @return maximum Voltage in 50mV units that this Consumer is capable of operating at
         */
        uint16_t wMaxVoltage() const;

        /**
         * @brief dwMaxOperatingPower
         * @return maximum power in 10mW units this Consumer can draw
         * when it is in a steady state operating mode.
         */
        uint32_t dwMaxOperatingPower() const;

        /**
         * @brief dwMaxPeakPower
         * @return maximum power in 10mW units this Consumer can draw
         * for a short duration of time (dwMaxPeakPowerTime)
         * before it falls back into a steady state.
         */
        uint32_t dwMaxPeakPower() const;

        /**
         * @brief dwMaxPeakPowerTime
         * @return time in 100ms units that this Consumer can draw peak current
         */
        uint32_t dwMaxPeakPowerTime() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        inline QString __parseBmCapabilities() const;

        uint16_t _bmCapabilities, _wMinVoltage, _wMaxVoltage;
        uint32_t _dwMaxOperatingPower, _dwMaxPeakPower, _dwMaxPeakPowerTime;
    };

    /**
     * @brief The UsbPowerDeliveryProviderPortCapabilityDescriptor class
     * Libusb does not support PD Provider Port Capability Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::PD_PROVIDER_PORT
     */
    class UsbPowerDeliveryProviderPortCapabilityDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPowerDeliveryProviderPortCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief bmCapabilities
         * @return bitmap indicate the specification the Provider Port will operate
         * @note
         * D0       Battery Charging Specification
         * D1       USB Power Delivery Specification
         * D2       USB Type-C Current Specification
         * D15..3   Reserved
         */
        uint16_t bmCapabilities() const;

        /**
         * @brief bNumOfPDObjects
         * @return the number of Power Data Objects
         */
        uint8_t bNumOfPDObjects() const;

        /**
         * @brief wPowerDataObject
         * @param index
         * @return the index-th Power Data Object supported by this Provider Port
         */
        uint32_t wPowerDataObject(size_t index) const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        inline QString __parseBmCapabilities() const;
        inline QString __parsePowerDataObject(uint32_t object) const;
        inline QString __parseFixedSupplyPDO(uint32_t object) const;
        inline QString __parseBatterySupplyPDO(uint32_t object) const;
        inline QString __parseVariableSupplyPDO(uint32_t object) const;
        inline QString __parseSPRProgrammablePowerSupply(uint32_t object) const;
        inline QString __parseEPRAdjustableVoltageSupply(uint32_t object) const;

        uint8_t _bNumOfPDObjects;
        uint16_t _bmCapabilities;
        const uint32_t *_wPowerDataObject;
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

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        inline QString __parseBmAttributes() const;
        inline QString __parseFunctionalitySupport() const;
        inline QString __parseSublinkSpeedAttr(size_t index) const;

        uint16_t _wFunctionalitySupport;
        uint32_t _bmAttributes;
        const uint32_t *_bmSublinkSpeedAttr;
    };

    /**
     * @brief The UsbPrecisionTimeMeasurementCapabilityDescriptor class
     * The Precision Time Measurement Capability Descriptor contains no additional field
     * besides bLength, bDescriptorType and bDevCapabilityType.
     * @see DeviceCapabilityType::PRECISION_TIME_MEASUREMENT
     */
    class UsbPrecisionTimeMeasurementCapabilityDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPrecisionTimeMeasurementCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;
    };

    /**
     * @brief The UsbExtendedWirelessDeviceCapabilityDescriptor class
     * Libusb does not support Extended Wireless USB Device Capability Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::WIRELESS_EXT
     */
    class UsbExtendedWirelessDeviceCapabilityDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbExtendedWirelessDeviceCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief wIdleTimeout
         * @return time interval in units of milliseconds, from the instant
         * there are no active transfers to a device until the host
         * decides to send the Device Sleep IE to the device.
         */
        uint16_t wIdleTimeout() const;

        /**
         * @brief wWakeUpLatency
         * @return time interval in units of milliseconds, during which the device
         * prefers to remain in the Sleep state, if directed to sleep by the host.
         */
        uint16_t wWakeUpLatency() const;

        /**
         * @brief bmControl
         * @return bitmap about control
         * @note
         * D0       strictly(if 0) use the device provided values of wIdleTimeout and wWakeUpLatency
         *          free(if 1) to choose idle timeout and wake up letency
         * D7..1    Reversed
         */
        uint8_t bmControl() const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        uint8_t _bmControl;
        uint16_t _wIdleTimeout, _wWakeUpLatency;
    };

    /**
     * @brief The UsbConfigurationSummaryDescriptor class
     * Libusb does not support Configuration Summary Descriptor Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::CONFIGURATION_SUMMARY
     */
    class UsbConfigurationSummaryDescriptor: public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbConfigurationSummaryDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent = nullptr);

        /**
         * @brief bcdVersion
         * @return the revision of the Configuration Summary Descriptor with specification
         */
        uint16_t bcdVersion() const;

        /**
         * @brief bClass
         * @return class code of the function
         */
        uint8_t bClass() const;

        /**
         * @brief bSubClass
         * @return subclass code of the function
         */
        uint8_t bSubClass() const;

        /**
         * @brief bProtocol
         * @return protocol of the function
         */
        uint8_t bProtocol() const;

        /**
         * @brief bConfigurationCount
         * @return Number of configurations that include this class/subclass/protocol
         */
        uint8_t bConfigurationCount() const;

        /**
         * @brief bConfigurationIndex
         * @param index
         * @return index-th configuration descriptor index for
         * a configuration containing this class/subclass/protocol
         */
        uint8_t bConfigurationIndex(size_t index) const;

        /**
         * @brief infomationToHtml
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        uint8_t _bClass, _bSubClass, _bProtocol, _bConfigurationCount;
        uint16_t _bcdVersion;
        const uint8_t *_bConfigurationIndex;
    };
}

#endif // USBDEVICECAPABILITYDESCRIPTOR_H
