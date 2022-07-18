/*! Macros definition about USB

 * Copyright (C) 2022 Nichts Hsu

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

#ifndef __USBMACRO_H
#define __USBMACRO_H

/*! This file is suggested to be included in *.cpp files but not in *.h files.
 */

#include <libusb.h>
#include <QString>

#define TRANSFER_TIMEOUT 250
#define MAX_STRDESC_LENGTH 128

#define ERROR_PROTECT_MOUSE -101
#define ERROR_PROTECT_KEYBOARD -102
#define ERROR_OUT_OF_RANGE -103
#define ERROR_DUPLICATE_REQUEST -104

#define BIT(v, n) bool((v) & (1 << (n)))
#define CUT(v, b, e) (((v) & (((1ull << ((e) - (b) + 1)) - 1) << (b))) >> (b))

#define DEVICE do { html += QString("<h1 align='center'>%1</h1>").arg(_deviceDescriptor->description()); } while(0)
#define INTERFACE do { html += QString("<h1 align='center'>%1</h1>").arg(_displayName); } while(0)
#define START(_title) do { html += QString("<h2 align='center'>%1</h2><table width=\"100%\">").arg(_title); } while (0)
#define ATTR(_name, _hex, _text) do { \
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>") \
    .arg(_name).arg(_hex, sizeof(_hex) * 2, 16, QChar('0')).arg(_text); \
    } while(0)
#define ATTRTEXT(_name, _text) do { \
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\"></td><td>%2</td></tr>").arg(_name).arg(_text); \
    } while(0)
#define ATTRCUSTOM(_name, _val1, _val2) do { \
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">%2</td><td>%3</td></tr>") \
    .arg(_name).arg(_val1).arg(_val2); \
    } while(0)
#define ATTRSTRDESC(_name, _strDescInd, _device) do { \
    html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>") \
    .arg(_name) \
    .arg(_strDescInd, 2, 16, QChar('0')) \
    .arg(_device->getStringDescriptor(_strDescInd)); \
    } while(0)
#define APPEND(_sub) do { html += _sub->infomationToHtml(); } while(0)
#define END do { html += QString("</table>"); } while(0)
#define NEWLINE QString("<br />")
#define TAB QString("&nbsp;&nbsp;&nbsp;&nbsp;")

extern const char *usb_error_name(int code);
extern QString hexUuid(const QByteArray &uuid);

#endif // __USBMACRO_H
