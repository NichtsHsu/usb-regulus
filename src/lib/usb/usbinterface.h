#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <libusb.h>
#include <usbutils/names.h>
#include <QObject>
#include <QVector>

namespace usb {
    class UsbInterfaceDescriptor;
    class UsbConfigurationDescriptor;
}
#ifndef USBCONFIGURATIONDESCRIPTOR_H
#include "usbconfigurationdescriptor.h"
#endif
#ifndef USBINTERFACEDESCRIPTOR_H
#include "usbinterfacedescriptor.h"
#endif
#include "log/logger.h"

namespace usb {

    /**
     * @brief The UsbInterface class
     * C++ class wrapper of libusb_interface
     */
    class UsbInterface : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbInterface(const libusb_interface *interface, UsbConfigurationDescriptor *parent = nullptr);

        /**
         * @brief num_altsetting
         * @return the number of alternate settings that belong to this interface
         */
        int num_altsetting() const;

        /**
         * @brief altsetting
         * @param index
         * Usually you are expected to use index 0,
         * unless you have manually set the alternate setting.
         * @return get the specified alternate setting
         */
        UsbInterfaceDescriptor *altsetting(int index) const;

        /**
         * @brief displayName
         * @return the name displayed in device tree view.
         * Prefer the iInterface; if iIterface is 0, use protocol;
         * if protocol value can not be translated to string,
         * use class+subclass as the display name.
         */
        const QString &displayName() const;

        /**
         * @brief configDescriptor
         * @return get the config descriptor which this interface is belong to
         */
        UsbConfigurationDescriptor *configDescriptor() const;

    signals:

    private:
        QVector<UsbInterfaceDescriptor *> _altsetting;
        int _num_altsetting;
        UsbConfigurationDescriptor *_configDescriptor;

        QString _displayName;
    };
}
#endif // USBINTERFACE_H
