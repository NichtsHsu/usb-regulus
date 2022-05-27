/** @file
 * @brief HID report descriptor - collection end item
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

#ifndef __HIDRD_ITEM_END_COLLECTION_H__
#define __HIDRD_ITEM_END_COLLECTION_H__

#include "hidrd/item/main.h"

#ifdef __cplusplus
extern "C" {
#endif

HIDRD_ITEM_MAIN_GEN_FUNCS(end_collection, END_COLLECTION)

static inline hidrd_item *
hidrd_item_end_collection_init(hidrd_item *item)
{
    return hidrd_item_end_collection_validate(
        hidrd_item_main_init(item, HIDRD_ITEM_MAIN_TAG_END_COLLECTION));
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HIDRD_ITEM_END_COLLECTION_H__ */

