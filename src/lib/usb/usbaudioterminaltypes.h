#ifndef USBAUDIOTERMINALTYPES_H
#define USBAUDIOTERMINALTYPES_H

#include <QObject>
#include <QMap>
#include <QString>

namespace usb {
    namespace uac1 {
        QString strTerminalType(uint16_t wTerminalType);
    }

    namespace uac2 {
        using uac1::strTerminalType;
    }
}

#endif // USBAUDIOTERMINALTYPES_H
