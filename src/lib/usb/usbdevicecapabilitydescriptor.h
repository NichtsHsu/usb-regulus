/*! C++ class wrapper of USB Device Capability Descriptors

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

#ifndef USBDEVICECAPABILITYDESCRIPTOR_H
#define USBDEVICECAPABILITYDESCRIPTOR_H

#include <libusb.h>
#include <QObject>
#include <QByteArray>
#include <QVector>

namespace usb {
    class UsbBosDescriptor;
};

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
        BILLBOARD_AUM = 0x0F,
        CONFIGURATION_SUMMARY = 0x10,
    };

    /**
     * @brief The UsbDeviceCapabilityDescriptor class
     * The C++ wrapper of libusb_bos_dev_capability_descriptor.
     */
    class UsbDeviceCapabilityDescriptor : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbDeviceCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *desc,
                UsbBosDescriptor *parent);

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
         * @return the device capability descriptor type
         * @see DeviceCapabilityType
         */
        DeviceCapabilityType type() const;

        /**
         * @brief bosDescriptor
         * @return the parent UsbBosDescriptor
         */
        UsbBosDescriptor *bosDescriptor() const;

        static QString strType(uint8_t bDescriptorType);
        static QString strType(DeviceCapabilityType type);

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
    class UsbWirelessDeviceCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbWirelessDeviceCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
    class Usb20ExtensionDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit Usb20ExtensionDescriptor(
                const libusb_usb_2_0_extension_descriptor *desc,
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
    class UsbSuperSpeedDeviceCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbSuperSpeedDeviceCapabilityDescriptor(
                const libusb_ss_usb_device_capability_descriptor *desc,
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
        uint16_t _wSpeedsSupported, _bU2DevExitLat;
    };

    /**
     * @brief The UsbContainerIdDescriptor class
     * The C++ wrapper of libusb_container_id_descriptor
     * @see DeviceCapabilityType::CONTAINER_ID
     */
    class UsbContainerIdDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbContainerIdDescriptor(
                const libusb_container_id_descriptor *desc,
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
    class UsbPlatformDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPlatformDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
    class UsbPowerDeliveryCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPowerDeliveryCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

        /**
         * @brief bmAttributes
         * @return bitmap encoding of supported device level features
         * @note
         * D0       Reserved
         * D1       Supports the Battery Charging Specification
         * D2       Supports the USB Power Delivery Specification
         * D3       Provider (device is capable of providing power)
         * D4       Consumer (device is a consumer of power)
         * D5       Supports the feature CHARGING_POLICY
         * D6       Supports power capabilities defined in the USB Type-C Specification
         * D7       Reserved
         * D8       Supports AC Supply Power Source
         * D9       Supports Battery Power Source
         * D10      Supports Other Power Source
         * D13..11  Number of batteries when D9 is set to one
         * D14      Uses VBUS
         * D31..15  Reserved
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
    class UsbBatteryInfoCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbBatteryInfoCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
    class UsbPowerDeliveryConsumerPortCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPowerDeliveryConsumerPortCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
    class UsbPowerDeliveryProviderPortCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPowerDeliveryProviderPortCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
    class UsbSuperSpeedPlusDeviceCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbSuperSpeedPlusDeviceCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
    class UsbPrecisionTimeMeasurementCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbPrecisionTimeMeasurementCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
    class UsbExtendedWirelessDeviceCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbExtendedWirelessDeviceCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
         * D7..1    Reserved
         */
        uint8_t bmControl() const;

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
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        uint8_t _bmControl;
        uint16_t _wIdleTimeout, _wWakeUpLatency;
    };

    /**
     * @brief The UsbBillboardCapabilityDescriptor class
     * Libusb does not support Billboard Capability Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::BILLBOARD
     */
    class UsbBillboardCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbBillboardCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

        /**
         * @brief iAddtionalInfoURL
         * @return string descriptor providing a URL where the user can go to
         * get more detailed information about the product and the various AUM it supports.
         */
        uint8_t iAddtionalInfoURL() const;

        /**
         * @brief bNumberOfAlternateOrUSB4Modes
         * @return number of AUM supported
         */
        uint8_t bNumberOfAlternateOrUSB4Modes() const;

        /**
         * @brief bPreferredAlternateOrUSB4Mode
         * @return index of the preferred AUM
         */
        uint8_t bPreferredAlternateOrUSB4Mode() const;

        /**
         * @brief VCONNPower
         * @return bitmapt indicating the information about VCONN Power
         * @note
         * D0..2    VCONN Power needed by the adapter for full functionality
         * D3..14   Reserved
         * D15      The adapter does not require any VCONN Power
         */
        uint16_t VCONNPower() const;

        /**
         * @brief bmConfigured
         * @return bitmap that each bit pair in this field indicates
         * the state of the Alternate Modes identified by the
         * combination of wSVID[i] and bAlternateOrUSB4Mode[i].
         * @note
         * D0..1                First AUM defined
         * D(n-1)*2..(n-1)*2+1  Nth AUM defined
         * Value:
         * 00b:     Unspecified Error
         * 01b:     AUM configuration not attempted or exited
         * 10b:     AUM configuration attempted but unsuccessful and not entered
         * 11b:     AUM configuration successful
         */
        QByteArray bmConfigured() const;

        /**
         * @brief bcdVersion
         * @return Billboard Capability version number in Binary-Coded Decimal
         */
        uint16_t bcdVersion() const;

        /**
         * @brief bAdditionalFailureInfo
         * @return bitmap indicating additional failure infomations
         * @note
         * D0       lack of power
         * D1       no USB-PD communication
         * D2..7    Reversed
         */
        uint8_t bAdditionalFailureInfo() const;

        /**
         * @brief wSVID
         * @param index
         * @return Standard or Vendor ID of index-th AUM
         */
        uint16_t wSVID(uint8_t index) const;

        /**
         * @brief bAlternateOrUSB4Mode
         * @param index
         * @return Index of the index-th AUM within the SVID as returned
         * in response to a Discover Modes command.
         */
        uint8_t bAlternateOrUSB4Mode(uint8_t index) const;

        /**
         * @brief iAlternateOrUSB4ModeString
         * @param index
         * @return string descriptor describing protocol of index-th AUM
         */
        uint8_t iAlternateOrUSB4ModeString(uint8_t index) const;

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
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        inline QString __parseVCONNPower() const;
        inline QString __parseBmConfigured() const;
        inline QString __parseBmConfigured(uint8_t index) const;
        inline QString __parseBAdditionalFailureInfo() const;
#ifdef Q_OS_UNIX
        inline QString __getSVID(uint8_t index) const;
#endif

        uint8_t _iAddtionalInfoURL, _bNumberOfAlternateOrUSB4Modes,
        _bPreferredAlternateOrUSB4Mode, _bAdditionalFailureInfo;
        uint16_t _VCONNPower, _bcdVersion;
        QByteArray _bmConfigured;
        QVector<uint16_t> _wSVID;
        QVector<uint8_t> _bAlternateOrUSB4Mode, _iAlternateOrUSB4ModeString;
    };

    /**
     * @brief The UsbAuthenticationCapabilityDescriptor class
     * Libusb does not support Authentication Capability Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::AUTHENTICATION
     */
    class UsbAuthenticationCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbAuthenticationCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

        /**
         * @brief bmAttributes
         * @return bitmap encoding of supported features
         * @note
         * D0       Firmware can be updated
         * D1       Device changes interfaces when updated
         * D3..7    Reserved
         */
        uint8_t bmAttributes() const;

        /**
         * @brief bcdProtocolVersion
         * @return USB Type-C Authentication Protocol Version
         */
        uint8_t bcdProtocolVersion() const;

        /**
         * @brief bcdCapability
         * @return same value as Param1 in a DIGESTS Authentication Response
         */
        uint8_t bcdCapability() const;

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
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        inline QString __parseBmAttributes() const;
        uint8_t _bmAttributes, _bcdProtocolVersion, _bcdCapability;
    };

    /**
     * @brief The UsbBillboardAumCapabilityDescriptor class
     * Billboard AUM Capability Descriptor C++ wrapper
     */
    class UsbBillboardAumCapabilityDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbBillboardAumCapabilityDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

        /**
         * @brief bIndex
         * @return index at which the AUM appears in the array of Alternate Modes
         * described in the Billboard Capability Descriptor.
         */
        uint8_t bIndex() const;

        /**
         * @brief dwAlternateModeVdo
         * @return contents of the Mode VDO
         */
        uint32_t dwAlternateModeVdo() const;

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
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        uint8_t _bIndex;
        uint32_t _dwAlternateModeVdo;
    };

    /**
     * @brief The UsbConfigurationSummaryDescriptor class
     * Libusb does not support Configuration Summary Descriptor.
     * Therefore we parse raw data from libusb_bos_dev_capability_descriptor.
     * @see DeviceCapabilityType::CONFIGURATION_SUMMARY
     */
    class UsbConfigurationSummaryDescriptor : public UsbDeviceCapabilityDescriptor
    {
        Q_OBJECT
    public:
        explicit UsbConfigurationSummaryDescriptor(
                const libusb_bos_dev_capability_descriptor *base,
                UsbBosDescriptor *parent);

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
         * @return HTML form descriptor informations
         */
        virtual QString infomationToHtml() const override;

    private:
        uint8_t _bClass, _bSubClass, _bProtocol, _bConfigurationCount;
        uint16_t _bcdVersion;
        const uint8_t *_bConfigurationIndex;
    };
}

#ifndef USBBOSDESCRIPTOR_H
#include "usbbosdescriptor.h"
#endif

#endif // USBDEVICECAPABILITYDESCRIPTOR_H
