/*! String description of Terminal Type for USB Audio specification

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
