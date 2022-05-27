/** @file
 * @brief HID report descriptor - specification example sink - entry list
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

#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include "hidrd/util/buf.h"
#include "hidrd/util/str.h"
#include "hidrd/fmt/spec/snk/ent_list.h"

void
hidrd_spec_snk_ent_list_init(hidrd_spec_snk_ent_list *list)
{
    assert(list != NULL);

    *list = (hidrd_spec_snk_ent_list)HIDRD_SPEC_SNK_ENT_LIST_EMPTY;
}


bool
hidrd_spec_snk_ent_list_valid(const hidrd_spec_snk_ent_list *list)
{
    hidrd_spec_snk_ent *p;
    size_t              l;

    if (list == NULL ||
        list->len > list->size ||
        (list->ptr == NULL && list->size > 0))
        return false;

    for (p = list->ptr, l = list->len; l > 0; p++, l--)
        if (!hidrd_spec_snk_ent_valid(p))
            return false;

    return true;
}


bool hidrd_spec_snk_ent_list_grow(hidrd_spec_snk_ent_list  *list)
{
    hidrd_spec_snk_ent *new_ptr;
    size_t              new_size;

    assert(hidrd_spec_snk_ent_list_valid(list));

    if (list->len < list->size)
        return true;

    new_size = (list->size < HIDRD_SPEC_SNK_ENT_LIST_MIN_SIZE)
                    ? HIDRD_SPEC_SNK_ENT_LIST_MIN_SIZE
                    : (list->size + list->size / 2);

    assert(new_size > list->len);

    new_ptr = realloc(list->ptr, new_size * sizeof(*new_ptr));
    if (new_ptr == NULL)
        return false;

    list->ptr = new_ptr;
    list->size = new_size;

    return true;
}


bool
hidrd_spec_snk_ent_list_adda(hidrd_spec_snk_ent_list   *list,
                             hidrd_spec_snk_ent        *ent)
{
    assert(hidrd_spec_snk_ent_list_valid(list));
    assert(hidrd_spec_snk_ent_valid(ent));

    if (!hidrd_spec_snk_ent_list_grow(list))
        return false;

    hidrd_spec_snk_ent_inita(&(list->ptr[list->len]),
                             ent->depth,
                             ent->item,
                             ent->name,
                             ent->value,
                             ent->comment);
    hidrd_spec_snk_ent_init(ent, 0, NULL, NULL, NULL, NULL);
    hidrd_spec_snk_ent_delete(ent);

    list->len++;

    return true;
}


bool
hidrd_spec_snk_ent_list_add(hidrd_spec_snk_ent_list    *list,
                            const hidrd_spec_snk_ent   *ent)
{
    assert(hidrd_spec_snk_ent_list_valid(list));
    assert(hidrd_spec_snk_ent_valid(ent));

    if (!hidrd_spec_snk_ent_list_grow(list))
        return false;

    hidrd_spec_snk_ent_init(&(list->ptr[list->len]),
                            ent->depth,
                            ent->item,
                            ent->name,
                            ent->value,
                            ent->comment);

    list->len++;

    return true;
}


int
hidrd_spec_snk_ent_list_min_depth(const hidrd_spec_snk_ent_list    *list)
{
    int                 min_depth   = INT_MAX;
    hidrd_spec_snk_ent *p;
    size_t              l;

    assert(hidrd_spec_snk_ent_list_valid(list));

    for (p = list->ptr, l = list->len; l > 0; p++, l--)
        if (p->depth < min_depth)
            min_depth = p->depth;

    return min_depth;
}


bool
hidrd_spec_snk_ent_list_to_tbl(hidrd_ttbl                     **ptbl,
                               const hidrd_spec_snk_ent_list   *list,
                               size_t                           tabstop,
                               bool                             dumps,
                               bool                             comments)
{
    bool                result      = false;
    hidrd_ttbl         *tbl         = NULL;
    int                 min_depth;
    hidrd_spec_snk_ent *p;
    size_t              last_l      = 0;
    size_t              l;
    hidrd_buf           buf         = HIDRD_BUF_EMPTY;
    uint8_t            *item_p;
    size_t              item_size;

    assert(hidrd_spec_snk_ent_list_valid(list));

    /* Create the table */
    tbl = hidrd_ttbl_new();
    if (tbl == NULL)
        goto cleanup;

    /* Find minimum depth */
    min_depth = hidrd_spec_snk_ent_list_min_depth(list);

    /* Find the last item */
    for (p = list->ptr, l = 0; l < list->len; p++, l++)
        if (p->name != NULL)
            last_l = l;

    /* Output the entries */
    for (p = list->ptr, l = 0; l < list->len; p++, l++)
    {
        /*
         * Output code cell
         */
        if (!hidrd_buf_add_span(&buf, ' ',
                                (p->depth - min_depth) * tabstop))
            goto cleanup;

        if (p->name != NULL && !hidrd_buf_add_str(&buf, p->name))
            goto cleanup;

        if (p->value != NULL && !hidrd_str_isblank(p->value))
        {
            if (!hidrd_buf_add_printf(&buf,
                                      (p->name == NULL)
                                        ? "(%s)" : " (%s)",
                                      p->value))
                goto cleanup;
        }

        /* Add comma, if it is an item and it is not the last one */
        if (p->name != NULL && l != last_l && !hidrd_buf_add_str(&buf, ","))
            goto cleanup;

        /* Terminate the buffer */
        if (!hidrd_buf_add_char(&buf, '\0'))
            goto cleanup;

        /* Set the code cell contents */
        hidrd_ttbl_set(tbl, HIDRD_SPEC_SNK_ENT_LIST_CODE_COL, l,
                       (const char *)buf.ptr);

        /* Reset the buffer contents */
        hidrd_buf_reset(&buf);

        /*
         * Output dump cell
         */
        if (dumps && p->item != NULL)
        {
            hidrd_buf_add_str(&buf, "; ");
            for (item_size = hidrd_item_get_size(p->item), item_p = p->item;
                 item_size > 0; item_size--, item_p++)
                if (!hidrd_buf_add_printf(&buf,
                                          (item_size == 1)
                                            ? "%.2hhX"
                                            : "%.2hhX ",
                                          *item_p))
            if (!hidrd_buf_add_char(&buf, '\0'))
                goto cleanup;
            hidrd_ttbl_set(tbl, HIDRD_SPEC_SNK_ENT_LIST_DUMP_COL, l,
                           (const char *)buf.ptr);
            hidrd_buf_reset(&buf);
        }

        /*
         * Output comment cell
         */
        /* Set the comment cell contents */
        if (comments && p->comment != NULL && *p->comment != '\0' &&
            !hidrd_ttbl_setf(tbl, HIDRD_SPEC_SNK_ENT_LIST_CMNT_COL, l,
                             ((dumps && p->item != NULL) ? "- %s" : "; %s"),
                             p->comment))
                goto cleanup;
    }

    if (ptbl != NULL)
    {
        *ptbl = tbl;
        tbl = NULL;
    }

    result = true;

cleanup:

    hidrd_buf_clnp(&buf);
    hidrd_ttbl_delete(tbl);

    return result;
}


bool
hidrd_spec_snk_ent_list_render(void                           **pbuf,
                               size_t                          *psize,
                               const hidrd_spec_snk_ent_list   *list,
                               size_t                           tabstop,
                               bool                             dumps,
                               bool                             comments)
{
    bool        result;
    hidrd_ttbl *tbl;

    if (!hidrd_spec_snk_ent_list_to_tbl(&tbl, list,
                                        tabstop, dumps, comments))
        return false;

    result = hidrd_ttbl_render((char **)pbuf, psize, tbl, tabstop);

    hidrd_ttbl_delete(tbl);

    return result;
}


void
hidrd_spec_snk_ent_list_clnp(hidrd_spec_snk_ent_list   *list)
{
    hidrd_spec_snk_ent *p;
    size_t              l;

    assert(hidrd_spec_snk_ent_list_valid(list));

    for (p = list->ptr, l = list->len; l > 0; p++, l--)
        hidrd_spec_snk_ent_clnp(p);

    free(list->ptr);
    hidrd_spec_snk_ent_list_init(list);
}


