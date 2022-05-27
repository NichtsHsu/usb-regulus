/** @file
 * @brief HID report descriptor - usage ID description
 *
 * Copyright (C) 2010 Nikolai Kondrashov
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

#include "hidrd/cfg.h"
#include "hidrd/util/tkn.h"
#include "hidrd/usage/id_desc.h"


bool
hidrd_usage_id_desc_valid(const hidrd_usage_id_desc *desc)
{
    return desc != NULL &&
           hidrd_usage_id_valid(desc->value) &&
           hidrd_usage_type_set_valid(desc->type_set)
#ifdef HIDRD_WITH_TOKENS
           && hidrd_tkn_valid(desc->token)
#endif
#ifdef HIDRD_WITH_NAMES
           && desc->name != NULL
#endif
           ;
}


