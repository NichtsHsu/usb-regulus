#ifndef USBHUBDESCRIPTOR_H
#define USBHUBDESCRIPTOR_H

#include <QObject>
#include <QByteArray>
#include "log/logger.h"

namespace usb {
    class UsbDeviceDescriptor;
}

namespace usb {
    /**
     * @brief The UsbHubDescriptor class
     * The Hub Descriptor C++ wrapper
     */
    class UsbHubDescriptor : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief tryGet
         * Try to send a request via control transfer to get the Hub Descriptor
         * @param deviceDesc
         * The device decriptor of hub device
         * @return pointer to hub descriptor, nullptr if failed or if not a hub device
         */
        static UsbHubDescriptor *tryGet(UsbDeviceDescriptor *deviceDesc);

        /**
         * @brief bDescriptorType
         * @return descriptor type, 0x29 if 2.0, 0x2A if 3.2
         */
        virtual uint8_t bDescriptorType() const = 0;

        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        virtual QString infomationToHtml() const = 0;

        /**
         * @brief deviceDescriptor
         * @return the Device Desciptor which current Hub Descriptor belongs to
         */
        UsbDeviceDescriptor *deviceDescriptor() const;

    protected:
        explicit UsbHubDescriptor(UsbDeviceDescriptor *deviceDesc);

        UsbDeviceDescriptor *_deviceDescriptor;
    };

    /**
     * @brief The Usb20HubDescriptor class
     * Hub Descriptor for USB 2.0
     */
    class Usb20HubDescriptor: public UsbHubDescriptor
    {
        Q_OBJECT
    public:
        /**
         * @brief bDescLength
         * @return descriptor length
         */
        uint8_t bDescLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type, 0x29 here
         */
        uint8_t bDescriptorType() const override;

        /**
         * @brief bNbrPorts
         * @return number of downstream facing ports that this hub supports
         */
        uint8_t bNbrPorts() const;

        /**
         * @brief wHubCharacteristics
         * @return bitmap of hub characteristics
         * @note
         * D0..1    Logical Power Switching Mode
         * D2       Identifies a Compound Device
         * D3..4    Over-current Protection Mode
         * D5..6    TT Think Time
         * D7       Port Indicators Supported
         * D8..15   Reserved
         */
        uint16_t wHubCharacteristics() const;

        /**
         * @brief bPwrOn2PwrGood
         * @return time (in 2 ms intervals) from the time the power-on sequence
         * begins on a port until power is good on that port.
         */
        uint8_t bPwrOn2PwrGood() const;

        /**
         * @brief bHubContrCurrent
         * @return maximum current requirements of the Hub Controller electronics in mA
         */
        uint8_t bHubContrCurrent() const;

        /**
         * @brief DeviceRemovable
         * @return bitmap to indicate if device is removable or not
         * @note
         * Bit 0 is reserved, Bit n indicates device in port n is removable or not.
         * Limit to 255 bit.
         */
        const QByteArray &DeviceRemovable() const;

        /**
         * @brief PortPwrCtrlMask
         * @return bitmap that all bits in this field should be set to 1.
         */
        const QByteArray &PortPwrCtrlMask() const;

        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        QString infomationToHtml() const override;

    private:
        explicit Usb20HubDescriptor(UsbDeviceDescriptor *deviceDesc);
        friend UsbHubDescriptor;
        inline QString __parseWHubCharacteristics() const;

        uint8_t _bDescLength, _bDescriptorType, _bNbrPorts, _bPwrOn2PwrGood, _bHubContrCurrent;
        uint16_t _wHubCharacteristics;
        QByteArray _DeviceRemovable, _PortPwrCtrlMask;
    };

    /**
     * @brief The UsbEnhancedSuperSpeedHubDescriptor class
     * Enhanced SuperSpeed Hub Descriptor for USB 3.2
     */
    class UsbEnhancedSuperSpeedHubDescriptor : public UsbHubDescriptor
    {
        Q_OBJECT
    public:
        /**
         * @brief bDescLength
         * @return descriptor length
         */
        uint8_t bDescLength() const;

        /**
         * @brief bDescriptorType
         * @return descriptor type, 0x2A here
         */
        uint8_t bDescriptorType() const override;

        /**
         * @brief bNbrPorts
         * @return number of downstream facing ports that this hub supports
         */
        uint8_t bNbrPorts() const;

        /**
         * @brief wHubCharacteristics
         * @return bitmap of hub characteristics
         * @note
         * D0..1    Logical Power Switching Mode
         * D2       Identifies a Compound Device
         * D3..4    Over-current Protection Mode
         * D5..15   Reserved
         */
        uint16_t wHubCharacteristics() const;

        /**
         * @brief bPwrOn2PwrGood
         * @return time (in 2 ms intervals) from the time the power-on sequence
         * begins on a port until power is good on that port.
         */
        uint8_t bPwrOn2PwrGood() const;

        /**
         * @brief bHubContrCurrent
         * @return maximum current requirements of the Hub Controller electronics
         * when the hub is operating on both USB 2.0 and Enhanced
         * SuperSpeed expressed in units of 4 mA.
         */
        uint8_t bHubContrCurrent() const;

        /**
         * @brief bHubHdrDecLat
         * @return Hub Packet Header Decode Latency
         */
        uint8_t bHubHdrDecLat() const;

        /**
         * @brief wHubDelay
         * @return maximum delay in nanoseconds a hub introduces while
         * forwarding packets in either direction.
         */
        uint16_t wHubDelay() const;

        /**
         * @brief DeviceRemovable
         * @return bitmap to indicate if device is removable or not
         * @note
         * Bit 0 is reserved, Bit n indicates device in port n is removable or not.
         * Limit to 15 bit.
         */
        uint16_t DeviceRemovable() const;

        /**
         * @brief infomationToHtml
         * @return HTML form informations
         */
        QString infomationToHtml() const override;

    private:
        explicit UsbEnhancedSuperSpeedHubDescriptor(UsbDeviceDescriptor *deviceDesc);
        friend UsbHubDescriptor;
        inline QString __parseWHubCharacteristics() const;

        uint8_t _bDescLength, _bDescriptorType, _bNbrPorts, _bPwrOn2PwrGood, _bHubContrCurrent, _bHubHdrDecLat;
        uint16_t _wHubCharacteristics, _wHubDelay, _DeviceRemovable;
    };
} // namespace usb

#ifndef USBDEVICEDESCRIPTOR_H
#include "usbdevicedescriptor.h"
#endif

#endif // USB_USBHUBDESCRIPTOR_H
