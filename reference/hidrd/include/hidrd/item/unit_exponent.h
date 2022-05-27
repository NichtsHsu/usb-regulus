/** @file
 * @brief HID report descriptor - unit exponent item
 *
 * Copyright (C) 2009 Nikolai Kondrashov
 *
 * This file is part of hidrd.
 *
 * Hidrd is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Hidrd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hidrd; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @author Nikolai Kondrashov <spbnick@gmail.com>
 */

#ifndef __HIDRD_ITEM_UNIT_EXPONENT_H__
#define __HIDRD_ITEM_UNIT_EXPONENT_H__

#include "hidrd/item/global.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline bool
hidrd_item_unit_exponent_value_valid(int32_t value)
{
    (void)value;
    return true;
}

HIDRD_ITEM_GLOBAL_FUNCS(unit_exponent, UNIT_EXPONENT,
                        signed, int32_t, int32_t)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HIDRD_ITEM_UNIT_EXPONENT_H__ */

