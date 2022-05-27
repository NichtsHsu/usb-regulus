/*
 * vim:nomodifiable
 *
 * ************* DO NOT EDIT ***************
 * This file is autogenerated from page.c.m4
 * *****************************************
 */
/** @file
 * @brief HID report descriptor - usage pages
 *
 * Copyright (C) 2009-2010 Nikolai Kondrashov
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hidrd/cfg.h"
#include "hidrd/util/hex.h"
#include "hidrd/util/str.h"
#include "hidrd/util/tkn.h"
#include "hidrd/usage/page_desc_list.h"
#include "hidrd/usage/page.h"

bool
hidrd_usage_page_reserved(hidrd_usage_page page)
{
    assert(hidrd_usage_page_valid(page));
    if ((int)page == 0x000E)
        return true;
    if ((int)page >= 0x0011 && (int)page <= 0x0013)
        return true;
    if ((int)page >= 0x0015 && (int)page <= 0x003F)
        return true;
    if ((int)page >= 0x0041 && (int)page <= 0x007F)
        return true;
    if ((int)page == 0x0083)
        return true;
    if ((int)page >= 0x0086 && (int)page <= 0x008B)
        return true;
    if ((int)page == 0x008F)
        return true;
    if ((int)page >= 0x0092 && (int)page <= 0xFEFF)
        return true;
    return false;
}



bool
hidrd_usage_page_valid(hidrd_usage_page page)
{
    hidrd_usage_page    min = HIDRD_USAGE_PAGE_MIN;
    hidrd_usage_page    max = HIDRD_USAGE_PAGE_MAX;

    return (page >= min) && (page <= max);
}


bool
hidrd_usage_page_known(hidrd_usage_page page)
{
    return hidrd_usage_page_desc_list_lkp_by_value(page) != NULL;
}


/* Define usage page to numeric string conversion functions */
HIDRD_NUM_CONV_DEFS(usage_page, u16)

#ifdef HIDRD_WITH_TOKENS
char *
hidrd_usage_page_to_token(hidrd_usage_page page)
{
    const hidrd_usage_page_desc    *desc;

    assert(hidrd_usage_page_valid(page));
    desc = hidrd_usage_page_desc_list_lkp_by_value(page);

    return (desc != NULL) ? strdup(desc->token) : NULL;
}


bool
hidrd_usage_page_from_token(hidrd_usage_page *ppage, const char *token)
{
    const hidrd_usage_page_desc    *desc;

    assert(token != NULL);

    desc = hidrd_usage_page_desc_list_lkp_by_token(token);
    if (desc == NULL)
        return false;

    if (ppage != NULL)
        *ppage = desc->value;

    return true;
}

#endif /* HIDRD_WITH_TOKENS */

#ifdef HIDRD_WITH_NAMES
const char *
hidrd_usage_page_name(hidrd_usage_page page)
{
    const hidrd_usage_page_desc    *desc;

    assert(hidrd_usage_page_valid(page));

    desc = hidrd_usage_page_desc_list_lkp_by_value(page);

    return (desc != NULL) ? desc->name : NULL;
}

char *
hidrd_usage_page_set_membership_desc_str(hidrd_usage_page page)
{
    char       *result      = NULL;
    char       *str         = NULL;
    char       *new_str     = NULL;

    str = strdup("");
    if (str == NULL)
        goto cleanup;

#define MAP(_token, _name) \
    do {                                                    \
        if (!hidrd_usage_page_##_token(page))               \
            break;                                          \
                                                            \
        if (asprintf(&new_str,                              \
                     ((*str == '\0') ? "%s%s" : "%s, %s"),  \
                     str, _name) < 0)                       \
            goto cleanup;                                   \
                                                            \
        free(str);                                          \
        str = new_str;                                      \
        new_str = NULL;                                     \
    } while (0)

    MAP(reserved, "reserved");
    MAP(monitor, "monitor page");
    MAP(power, "power page");
    MAP(pos, "POS page");
    MAP(vendor, "vendor-defined");

    result = str;
    str = NULL;

cleanup:

    free(new_str);
    free(str);

    return result;
}


char *
hidrd_usage_page_desc_str(hidrd_usage_page page)
{
    char       *result          = NULL;
    const char *name;
    char       *set_membership  = NULL;
    char       *str             = NULL;
    char       *new_str;

    assert(hidrd_usage_page_valid(page));

    name = hidrd_usage_page_name(page);
    set_membership = hidrd_usage_page_set_membership_desc_str(page);
    if (set_membership == NULL)
        goto cleanup;


    if (name == NULL && *set_membership == '\0')
        str = strdup("");
    else
    {
        str = hidrd_usage_page_to_shex(page);
        if (str == NULL)
            goto cleanup;

        if (*set_membership != '\0')
        {
            if (asprintf(&new_str, "%s, %s", str, set_membership) < 0)
                goto cleanup;
            free(str);
            str = new_str;
        }

        if (name != NULL)
        {
            if (asprintf(&new_str, "%s (%s)", name, str) < 0)
                goto cleanup;
            free(str);
            str = new_str;
        }
    }


    result = str;
    str = NULL;

cleanup:

    free(str);
    free(set_membership);

    return result;
}
#endif /* HIDRD_WITH_NAMES */
