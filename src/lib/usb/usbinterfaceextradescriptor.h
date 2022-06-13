#ifndef USBINTERFACEEXTRADESCRIPTOR_H
#define USBINTERFACEEXTRADESCRIPTOR_H

#include <QObject>

namespace usb {
    class UsbInterfaceDescriptor;
}

namespace usb {
    enum class InterfaceExtraDescriptorType {
        HID,            // Human Interface Device
        DFU,            // Device Firmware Upgrade
    };

    class UsbInterfaceExtraDescriptor: public QObject
    {
        Q_OBJECT
    public:
        UsbInterfaceExtraDescriptor(UsbInterfaceDescriptor *parent);

        /**
         * @brief interfaceDescriptor
         * @return the Interface Desciptor which current extra Descriptor belongs to
         */
        UsbInterfaceDescriptor *interfaceDescriptor() const;

        /**
         * @brief bLength
         * @return the total size of the descriptor
         */
        virtual uint8_t bLength() const = 0;

        /**
         * @brief bDescriptorType
         * @return descriptor type
         */
        virtual uint8_t bDescriptorType() const = 0;

        /**
         * @brief infomationToHtml
         * @return HTML form device informations
         */
        virtual QString infomationToHtml() const = 0;

        /**
         * @brief type
         * @return the interface extra descriptor type
         */
        virtual InterfaceExtraDescriptorType type() const = 0;

    private:
        UsbInterfaceDescriptor *_interfaceDescriptor;
    };
}

#ifndef USBINTERFACE_H
#include "usbinterfacedescriptor.h"
#endif

#endif // USBINTERFACEEXTRADESCRIPTOR_H
