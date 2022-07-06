#ifndef USBTERMINALTYPES_H
#define USBTERMINALTYPES_H

#include <QObject>
#include <QMap>
#include <QString>

namespace usb {
    QString strTerminalType(uint16_t wTerminalType);
}

#endif // USBTERMINALTYPES_H
