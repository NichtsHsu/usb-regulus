#include "usbinterfaceextradescriptor.h"

namespace usb {
    UsbInterfaceExtraDescriptor::UsbInterfaceExtraDescriptor(UsbInterfaceDescriptor *parent):
        QObject(parent), _interfaceDescriptor(parent)
    {

    }

    UsbInterfaceDescriptor *UsbInterfaceExtraDescriptor::interfaceDescriptor() const
    {
        return _interfaceDescriptor;
    }
}
