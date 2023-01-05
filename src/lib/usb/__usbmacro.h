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

extern const char *usb_error_name(int code);
extern QString hexUuid(const QByteArray &uuid);

#endif // __USBMACRO_H
