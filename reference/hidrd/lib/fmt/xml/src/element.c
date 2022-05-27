/** @file
 * @brief HID report descriptor - XML source - element handling
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

#include "hidrd/util/hex.h"
#include "hidrd/util/str.h"
#include "element.h"
#include "element_bitmap.h"
#include "element_unit.h"

static bool
xml_src_element_push_state(hidrd_xml_src_inst *xml_src)
{
    hidrd_xml_src_state   *new_state;

    new_state = malloc(sizeof(*new_state));
    if (new_state == NULL)
    {
        ELEMENT_ERR("failed to allocate a state table");
        return false;
    }

    memcpy(new_state, xml_src->state, sizeof(*new_state));
    new_state->prev = xml_src->state;
    xml_src->state = new_state;

    return true;
}


static void
xml_src_element_pop_state(hidrd_xml_src_inst *xml_src)
{
    hidrd_xml_src_state   *prev_state;

    prev_state = xml_src->state->prev;
    if (prev_state != NULL)
    {
        free(xml_src->state);
        xml_src->state = prev_state;
    }
}


static ELEMENT(basic)
{
    xml_src_element_rc  result_rc   = XML_SRC_ELEMENT_RC_ERROR;
    char       *data_str    = NULL;

    ELEMENT_PROP_DECL(item_basic_data_bytes,    size);
    ELEMENT_PROP_DECL(item_basic_type,          type);
    ELEMENT_PROP_DECL(item_basic_tag,           tag);

    ELEMENT_PROP_RETR(item_basic_data_bytes,    size,   dec);
    ELEMENT_PROP_RETR_ALT2(item_basic_type,     type,   token, dec);
    ELEMENT_PROP_RETR(item_basic_tag,           tag,    dec);

    hidrd_item_basic_init(item, type, tag,
                          hidrd_item_basic_data_size_from_bytes(size));

    data_str = (char *)xmlNodeGetContent(e);
    if (data_str == NULL)
        ELEMENT_CONTENT_RETR_ERR_CLNP("basic");
    memset(item + HIDRD_ITEM_BASIC_MIN_SIZE, 0,
           (HIDRD_ITEM_BASIC_MAX_SIZE - HIDRD_ITEM_BASIC_MIN_SIZE));
    if (!hidrd_hex_buf_from_str(item + HIDRD_ITEM_BASIC_MIN_SIZE,
                                (HIDRD_ITEM_BASIC_MAX_SIZE -
                                 HIDRD_ITEM_BASIC_MIN_SIZE),
                                NULL, data_str))
        ELEMENT_CONTENT_PRSE_ERR_CLNP("basic");

    if (!hidrd_item_valid(item))
        ELEMENT_ITEM_INVALID_ERR_CLNP("basic");

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    xmlFree(data_str);
    ELEMENT_PROP_CLNP(tag);
    ELEMENT_PROP_CLNP(type);
    ELEMENT_PROP_CLNP(size);

    return result_rc;
}

static ELEMENT(short)
{
    xml_src_element_rc  result_rc   = XML_SRC_ELEMENT_RC_ERROR;
    char               *data_str    = NULL;
    size_t              data_len;

    ELEMENT_PROP_DECL(item_short_type,  type);
    ELEMENT_PROP_DECL(item_short_tag,   tag);

    ELEMENT_PROP_RETR_ALT2(item_short_type, type, token, dec);
    ELEMENT_PROP_RETR(item_short_tag, tag, dec);

    hidrd_item_short_init(item, type, tag);

    data_str = (char *)xmlNodeGetContent(e);
    if (data_str == NULL)
        ELEMENT_CONTENT_RETR_ERR_CLNP("short");
    memset(hidrd_item_short_get_data(item), 0,
           HIDRD_ITEM_SHORT_DATA_BYTES_MAX);
    if (!hidrd_hex_buf_from_str(hidrd_item_short_get_data(item),
                                HIDRD_ITEM_SHORT_DATA_BYTES_MAX,
                                &data_len, data_str))
        ELEMENT_CONTENT_PRSE_ERR_CLNP("short");

    hidrd_item_short_set_data_size(
            item, hidrd_item_short_data_size_from_bytes(data_len));

    if (!hidrd_item_valid(item))
        ELEMENT_ITEM_INVALID_ERR_CLNP("short");

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    xmlFree(data_str);
    ELEMENT_PROP_CLNP(tag);
    ELEMENT_PROP_CLNP(type);

    return result_rc;
}

static ELEMENT(main)
{
    xml_src_element_rc  result_rc   = XML_SRC_ELEMENT_RC_ERROR;
    char               *data_str    = NULL;
    size_t              data_len;

    ELEMENT_PROP_DECL(item_main_tag, tag);

    ELEMENT_PROP_RETR_ALT2(item_main_tag, tag, token, dec);

    hidrd_item_main_init(item, tag);

    data_str = (char *)xmlNodeGetContent(e);
    if (data_str == NULL)
        ELEMENT_CONTENT_RETR_ERR_CLNP("main");
    memset(hidrd_item_short_get_data(item), 0,
           HIDRD_ITEM_SHORT_DATA_BYTES_MAX);
    if (!hidrd_hex_buf_from_str(hidrd_item_short_get_data(item),
                                HIDRD_ITEM_SHORT_DATA_BYTES_MAX,
                                &data_len, data_str))
        ELEMENT_CONTENT_PRSE_ERR_CLNP("main");

    hidrd_item_short_set_data_size(
            item, hidrd_item_short_data_size_from_bytes(data_len));

    if (!hidrd_item_valid(item))
        ELEMENT_ITEM_INVALID_ERR_CLNP("main");

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    xmlFree(data_str);
    ELEMENT_PROP_CLNP(tag);

    return result_rc;
}

static ELEMENT(global)
{
    xml_src_element_rc  result_rc   = XML_SRC_ELEMENT_RC_ERROR;
    char               *data_str    = NULL;
    size_t              data_len;

    ELEMENT_PROP_DECL(item_global_tag, tag);

    ELEMENT_PROP_RETR_ALT2(item_global_tag, tag, token, dec);

    hidrd_item_global_init(item, tag);

    data_str = (char *)xmlNodeGetContent(e);
    if (data_str == NULL)
        ELEMENT_CONTENT_RETR_ERR_CLNP("global");
    memset(hidrd_item_short_get_data(item), 0,
           HIDRD_ITEM_SHORT_DATA_BYTES_MAX);
    if (!hidrd_hex_buf_from_str(hidrd_item_short_get_data(item),
                                HIDRD_ITEM_SHORT_DATA_BYTES_MAX,
                                &data_len, data_str))
        ELEMENT_CONTENT_PRSE_ERR_CLNP("global");

    hidrd_item_short_set_data_size(
            item, hidrd_item_short_data_size_from_bytes(data_len));

    if (!hidrd_item_valid(item))
        ELEMENT_ITEM_INVALID_ERR_CLNP("global");

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    xmlFree(data_str);
    ELEMENT_PROP_CLNP(tag);

    return result_rc;
}

static ELEMENT(usage_page)
{
    xml_src_element_rc      result_rc   = XML_SRC_ELEMENT_RC_ERROR;
    char                   *value_str   = NULL;
    hidrd_usage_page        value;

    value_str = (char *)xmlNodeGetContent(e);
    if (value_str == NULL)
        ELEMENT_CONTENT_RETR_ERR_CLNP("usage_page");
    if (!HIDRD_NUM_FROM_ALT_STR2(usage_page, &value, value_str, token, hex))
        ELEMENT_CONTENT_PRSE_ERR_CLNP("usage_page");

    xml_src->state->usage_page = value;

    hidrd_item_usage_page_init(item, value);

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    xmlFree(value_str);

    return result_rc;
}

static ELEMENT(push)
{
    (void)e;

    if (!xml_src_element_push_state(xml_src))
        return XML_SRC_ELEMENT_RC_ERROR;

    hidrd_item_push_init(item);

    return XML_SRC_ELEMENT_RC_ITEM;
}

static ELEMENT(pop)
{
    (void)e;
    xml_src_element_pop_state(xml_src);
    hidrd_item_pop_init(item);
    return XML_SRC_ELEMENT_RC_ITEM;
}

static ELEMENT(PUSH)
{
    (void)e;

    if (!xml_src_element_push_state(xml_src))
        return XML_SRC_ELEMENT_RC_ERROR;

    hidrd_item_push_init(item);
    return XML_SRC_ELEMENT_RC_ITEM;
}

static ELEMENT_EXIT(PUSH)
{
    (void)e;
    xml_src_element_pop_state(xml_src);
    hidrd_item_pop_init(item);
    return XML_SRC_ELEMENT_RC_ITEM;
}

static ELEMENT(local)
{
    xml_src_element_rc  result_rc   = XML_SRC_ELEMENT_RC_ERROR;
    char               *data_str    = NULL;
    size_t              data_len;

    ELEMENT_PROP_DECL(item_local_tag,    tag);

    ELEMENT_PROP_RETR_ALT2(item_local_tag, tag, token, dec);

    hidrd_item_local_init(item, tag);

    data_str = (char *)xmlNodeGetContent(e);
    if (data_str == NULL)
        ELEMENT_CONTENT_RETR_ERR_CLNP("local");
    memset(hidrd_item_short_get_data(item), 0,
           HIDRD_ITEM_SHORT_DATA_BYTES_MAX);
    if (!hidrd_hex_buf_from_str(hidrd_item_short_get_data(item),
                                HIDRD_ITEM_SHORT_DATA_BYTES_MAX,
                                &data_len, data_str))
        ELEMENT_CONTENT_PRSE_ERR_CLNP("local");

    hidrd_item_short_set_data_size(
            item, hidrd_item_short_data_size_from_bytes(data_len));

    if (!hidrd_item_valid(item))
        ELEMENT_ITEM_INVALID_ERR_CLNP("local");

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    xmlFree(data_str);
    ELEMENT_PROP_CLNP(tag);

    return result_rc;
}

static ELEMENT(collection)
{
    xml_src_element_rc  result_rc   = XML_SRC_ELEMENT_RC_ERROR;

    ELEMENT_PROP_DECL(item_collection_type, type);

    ELEMENT_PROP_RETR_ALT2(item_collection_type, type, token, dec);

    hidrd_item_collection_init(item, type);

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    ELEMENT_PROP_CLNP(type);
    return result_rc;
}

static ELEMENT(end_collection)
{
    (void)e;
    hidrd_item_end_collection_init(item);
    return XML_SRC_ELEMENT_RC_ITEM;
}

static ELEMENT(COLLECTION)
{
    xml_src_element_rc  result_rc   = XML_SRC_ELEMENT_RC_ERROR;

    ELEMENT_PROP_DECL(item_collection_type, type);

    ELEMENT_PROP_RETR_ALT2(item_collection_type, type, token, dec);

    hidrd_item_collection_init(item, type);

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    ELEMENT_PROP_CLNP(type);
    return result_rc;
}

static ELEMENT_EXIT(COLLECTION)
{
    (void)e;
    hidrd_item_end_collection_init(item);
    return XML_SRC_ELEMENT_RC_ITEM;
}

#define NUM_ELEMENT(_name, _t) \
    ELEMENT(_name)                                                      \
    {                                                                   \
        xml_src_element_rc      result_rc   = XML_SRC_ELEMENT_RC_ERROR; \
        char                   *value_str   = NULL;                     \
        HIDRD_NUM_##_t##_TYPE   value;                                  \
                                                                        \
        value_str = (char *)xmlNodeGetContent(e);                       \
        if (value_str == NULL)                                          \
            ELEMENT_CONTENT_RETR_ERR_CLNP(#_name);                      \
        if (!HIDRD_DEC_FROM_STR(_t, &value, value_str))                 \
            ELEMENT_CONTENT_PRSE_ERR_CLNP(#_name);                      \
                                                                        \
        hidrd_item_##_name##_init(item, value);                         \
                                                                        \
        result_rc = XML_SRC_ELEMENT_RC_ITEM;                            \
                                                                        \
    cleanup:                                                            \
                                                                        \
        xmlFree(value_str);                                             \
                                                                        \
        return result_rc;                                               \
    }

static NUM_ELEMENT(logical_minimum,    s32)
static NUM_ELEMENT(logical_maximum,    s32)
static NUM_ELEMENT(physical_minimum,   s32)
static NUM_ELEMENT(physical_maximum,   s32)
static NUM_ELEMENT(unit_exponent,      s32)
static NUM_ELEMENT(report_size,        u32)
static NUM_ELEMENT(report_count,       u32)
static NUM_ELEMENT(report_id,          u8)

#define USAGE_ELEMENT(_name) \
    ELEMENT(_name)                                                      \
    {                                                                   \
        xml_src_element_rc  result_rc   = XML_SRC_ELEMENT_RC_ERROR;     \
        char               *value_str   = NULL;                         \
        hidrd_usage         value;                                      \
                                                                        \
        value_str = (char *)xmlNodeGetContent(e);                       \
        if (value_str == NULL)                                          \
            ELEMENT_CONTENT_RETR_ERR_CLNP(#_name);                      \
        if (!HIDRD_NUM_FROM_ALT_STR2(usage, &value, value_str,          \
                                     token, hex))                       \
            ELEMENT_CONTENT_PRSE_ERR_CLNP(#_name);                      \
                                                                        \
        if (xml_src->state->usage_page != HIDRD_USAGE_PAGE_UNDEFINED && \
            hidrd_usage_get_page(value) == xml_src->state->usage_page)  \
            value = hidrd_usage_set_page(value,                         \
                                         HIDRD_USAGE_PAGE_UNDEFINED);   \
                                                                        \
        hidrd_item_##_name##_init(item, value);                         \
                                                                        \
        result_rc = XML_SRC_ELEMENT_RC_ITEM;                            \
                                                                        \
    cleanup:                                                            \
                                                                        \
        xmlFree(value_str);                                             \
                                                                        \
        return result_rc;                                               \
    }

static USAGE_ELEMENT(usage);
static USAGE_ELEMENT(usage_minimum);
static USAGE_ELEMENT(usage_maximum);

static NUM_ELEMENT(designator_index,   u32)
static NUM_ELEMENT(designator_minimum, u32)
static NUM_ELEMENT(designator_maximum, u32)
static NUM_ELEMENT(string_index,       u32)
static NUM_ELEMENT(string_minimum,     u32)
static NUM_ELEMENT(string_maximum,     u32)

static ELEMENT(delimiter)
{
    xml_src_element_rc  result_rc   = XML_SRC_ELEMENT_RC_ERROR;

    ELEMENT_PROP_DECL(item_delimiter_set, open);

    ELEMENT_PROP_RETR(item_delimiter_set, open,   bool_str);

    hidrd_item_delimiter_init(item, open);

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    ELEMENT_PROP_CLNP(open);
    return result_rc;
}

static ELEMENT(SET)
{
    (void)e;
    hidrd_item_delimiter_init(item,
                              HIDRD_ITEM_DELIMITER_SET_OPEN);
    return XML_SRC_ELEMENT_RC_ITEM;
}

static ELEMENT_EXIT(SET)
{
    (void)e;
    hidrd_item_delimiter_init(item,
                              HIDRD_ITEM_DELIMITER_SET_CLOSE);
    return XML_SRC_ELEMENT_RC_ITEM;
}

static ELEMENT(long)
{
    xml_src_element_rc  result_rc       = XML_SRC_ELEMENT_RC_ERROR;
    char               *data_str        = NULL;
    size_t              data_len;

    ELEMENT_PROP_DECL(item_long_tag,    tag);

    ELEMENT_PROP_RETR(item_long_tag,    tag,    dec);

    hidrd_item_long_init(item, tag);

    data_str = (char *)xmlNodeGetContent(e);
    if (data_str == NULL)
        ELEMENT_CONTENT_RETR_ERR_CLNP("long");
    memset(hidrd_item_long_get_data(item), 0,
           HIDRD_ITEM_LONG_DATA_SIZE_MAX);
    if (!hidrd_hex_buf_from_str(hidrd_item_long_get_data(item),
                                HIDRD_ITEM_LONG_DATA_SIZE_MAX,
                                &data_len, data_str))
        ELEMENT_CONTENT_PRSE_ERR_CLNP("long");

    hidrd_item_long_set_data_size(item, data_len);

    if (!hidrd_item_valid(item))
        ELEMENT_ITEM_INVALID_ERR_CLNP("long");

    result_rc = XML_SRC_ELEMENT_RC_ITEM;

cleanup:

    xmlFree(data_str);
    ELEMENT_PROP_CLNP(tag);

    return result_rc;
}

static ELEMENT(descriptor)
{
    (void)item;
    (void)e;
    /* No item yet */
    return XML_SRC_ELEMENT_RC_NONE;
}

static ELEMENT_EXIT(descriptor)
{
    (void)item;
    (void)e;
    /* No more items */
    return XML_SRC_ELEMENT_RC_END;
}

/** Element handler */
typedef struct xml_src_element_handler {
    const char *name;                   /**< Element name */
    xml_src_element_fn *handle;         /**< Element handling function */
    xml_src_element_fn *handle_exit;    /**< Element exit handling
                                             function */
} xml_src_element_handler;

/** Element handler list */
static const xml_src_element_handler handler_list[] = {
#define IGNORE(_name)   {.name = #_name}
#define HANDLE(_name)   {.name      = #_name, \
                         .handle    = xml_src_element_##_name}
#define ENTER(_name)    {.name        = #_name, \
                         .handle      = xml_src_element_##_name,        \
                         .handle_exit = xml_src_element_##_name##_exit}
    HANDLE(basic),
    HANDLE(short),
    HANDLE(main),
    HANDLE(input),
    HANDLE(output),
    HANDLE(feature),
    HANDLE(collection),
    HANDLE(end_collection),
    ENTER(COLLECTION),
    HANDLE(global),
    HANDLE(usage_page),
    HANDLE(logical_minimum),
    HANDLE(logical_maximum),
    HANDLE(physical_minimum),
    HANDLE(physical_maximum),
    HANDLE(unit_exponent),
    HANDLE(unit),
    HANDLE(report_size),
    HANDLE(report_count),
    HANDLE(report_id),
    HANDLE(push),
    HANDLE(pop),
    ENTER(PUSH),
    HANDLE(local),
    HANDLE(usage),
    HANDLE(usage_minimum),
    HANDLE(usage_maximum),
    HANDLE(designator_index),
    HANDLE(designator_minimum),
    HANDLE(designator_maximum),
    HANDLE(string_index),
    HANDLE(string_minimum),
    HANDLE(string_maximum),
    HANDLE(delimiter),
    ENTER(SET),
    HANDLE(long),
    ENTER(descriptor),
#undef ENTER
#undef HANDLE
#undef IGNORE
};


xml_src_element_rc
xml_src_element(hidrd_xml_src_inst *xml_src, bool *penter)
{
    const char                     *name;
    size_t                          i;
    const xml_src_element_handler  *handler;

    assert(xml_src != NULL);
    assert(penter != NULL);
    /* We have to process something */
    assert(xml_src->cur != NULL);
    /* We process elements only */
    assert(xml_src->cur->type == XML_ELEMENT_NODE);
    /* Either no parent or an element parent */
    assert(xml_src->prnt == NULL ||
           xml_src->prnt->type == XML_ELEMENT_NODE);

    name = (const char *)xml_src->cur->name;

    for (i = 0; i < sizeof(handler_list) / sizeof(*handler_list); i++)
    {
        handler = handler_list + i;

        if (strcmp(handler->name, name) == 0)
        {
            if (handler->handle_exit != NULL)
                *penter = true;
            if (handler->handle == NULL)
                return XML_SRC_ELEMENT_RC_NONE;
            return (*handler->handle)(xml_src, xml_src->item, xml_src->cur);
        }
    }

    ELEMENT_UNKNOWN_ERR(name);

    return XML_SRC_ELEMENT_RC_ERROR;
}


xml_src_element_rc
xml_src_element_exit(hidrd_xml_src_inst *xml_src)
{
    const char                     *name;
    size_t                          i;
    const xml_src_element_handler  *handler;

    assert(xml_src != NULL);
    /* We have to process something */
    assert(xml_src->prnt != NULL);
    /* We process elements only */
    assert(xml_src->prnt->type == XML_ELEMENT_NODE);
    /* We should be at the end of the node list */
    assert(xml_src->cur == NULL);

    name = (const char *)xml_src->prnt->name;

    for (i = 0; i < sizeof(handler_list) / sizeof(*handler_list); i++)
    {
        handler = handler_list + i;
        if (strcmp(handler->name, name) == 0)
        {
            assert(handler->handle_exit != NULL);
            return (*handler->handle_exit)(xml_src,
                                           xml_src->item, xml_src->prnt);
        }
    }

    assert(!"Exiting an element without an exit handler");

    ELEMENT_UNKNOWN_ERR(name);

    return XML_SRC_ELEMENT_RC_ERROR;
}


