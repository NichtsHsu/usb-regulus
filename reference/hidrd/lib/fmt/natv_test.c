/** @file
 * @brief HID report descriptor - native format support test
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

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "hidrd/fmt/natv.h"

typedef struct item_desc {
    uint8_t     buf[HIDRD_ITEM_MAX_SIZE];
    size_t      len;
} item_desc;

/** WP8060U report descriptor */
const item_desc item_list[] = {
    {.buf = {0x05, 0x0d}, .len = 2},       // USAGE_PAGE (Digitizers)
    {.buf = {0x09, 0x02}, .len = 2},       // USAGE (Pen)
    {.buf = {0xa1, 0x01}, .len = 2},       // COLLECTION (Application)
    {.buf = {0x85, 0x07}, .len = 2},       //   REPORT_ID (7)
    {.buf = {0x09, 0x20}, .len = 2},       //   USAGE (Stylus)
    {.buf = {0xa1, 0x00}, .len = 2},       //   COLLECTION (Physical)
    {.buf = {0x09, 0x42}, .len = 2},       //     USAGE (Tip Switch)
    {.buf = {0x09, 0x44}, .len = 2},       //     USAGE (Barrel Switch)
    {.buf = {0x09, 0x45}, .len = 2},       //     USAGE (Eraser)
    {.buf = {0x15, 0x00}, .len = 2},       //     LOGICAL_MINIMUM (0)
    {.buf = {0x25, 0x01}, .len = 2},       //     LOGICAL_MAXIMUM (1)
    {.buf = {0x75, 0x01}, .len = 2},       //     REPORT_SIZE (1)
    {.buf = {0x95, 0x03}, .len = 2},       //     REPORT_COUNT (3)
    {.buf = {0x81, 0x02}, .len = 2},       //     INPUT (Data,Var,Abs)
    {.buf = {0x95, 0x03}, .len = 2},       //     REPORT_COUNT (3)
    {.buf = {0x81, 0x03}, .len = 2},       //     INPUT (Cnst,Var,Abs)
    {.buf = {0x09, 0x32}, .len = 2},       //     USAGE (In Range)
    {.buf = {0x95, 0x01}, .len = 2},       //     REPORT_COUNT (1)
    {.buf = {0x81, 0x02}, .len = 2},       //     INPUT (Data,Var,Abs)
    {.buf = {0x95, 0x01}, .len = 2},       //     REPORT_COUNT (1)
    {.buf = {0x81, 0x03}, .len = 2},       //     INPUT (Cnst,Var,Abs)
    {.buf = {0x05, 0x01}, .len = 2},       //     USAGE_PAGE (Generic Desktop)
    {.buf = {0x09, 0x30}, .len = 2},       //     USAGE (X)
    {.buf = {0x75, 0x10}, .len = 2},       //     REPORT_SIZE (16)
    {.buf = {0x95, 0x01}, .len = 2},       //     REPORT_COUNT (1)
    {.buf = {0xa4}, .len = 1},             //     PUSH
    {.buf = {0x55, 0x0d}, .len = 2},       //     UNIT_EXPONENT (-3)
    {.buf = {0x65, 0x33}, .len = 2},       //     UNIT (Eng Lin:0x33)
    {.buf = {0x35, 0x00}, .len = 2},       //     PHYSICAL_MINIMUM (0)
    {.buf = {0x46, 0x40, 0x1f}, .len = 3}, //     PHYSICAL_MAXIMUM (8000)
    {.buf = {0x26, 0x80, 0x3e}, .len = 3}, //     LOGICAL_MAXIMUM (16000)
    {.buf = {0x81, 0x02}, .len = 2},       //     INPUT (Data,Var,Abs)
    {.buf = {0x09, 0x31}, .len = 2},       //     USAGE (Y)
    {.buf = {0x46, 0x70, 0x17}, .len = 3}, //     PHYSICAL_MAXIMUM (6000)
    {.buf = {0x26, 0xe0, 0x2e}, .len = 3}, //     LOGICAL_MAXIMUM (12000)
    {.buf = {0x81, 0x02}, .len = 2},       //     INPUT (Data,Var,Abs)
    {.buf = {0xb4}, .len = 1},             //     POP
    {.buf = {0x05, 0x0d}, .len = 2},       //     USAGE_PAGE (Digitizers)
    {.buf = {0x09, 0x30}, .len = 2},       //     USAGE (Tip Pressure)
    {.buf = {0x26, 0xff, 0x03}, .len = 3}, //     LOGICAL_MAXIMUM (1023)
    {.buf = {0x81, 0x02}, .len = 2},       //     INPUT (Data,Var,Abs)
    {.buf = {0x75, 0x10}, .len = 2},       //     REPORT_SIZE (16)
    {.buf = {0xc0}, .len = 1},             //   END_COLLECTION
    {.buf = {0xc0}, .len = 1},             // END_COLLECTION
    {.buf = {0x05, 0x01}, .len = 2},       // USAGE_PAGE (Generic Desktop)
    {.buf = {0x09, 0x02}, .len = 2},       // USAGE (Mouse)
    {.buf = {0xa1, 0x01}, .len = 2},       // COLLECTION (Application)
    {.buf = {0x85, 0x08}, .len = 2},       //   REPORT_ID (8)
    {.buf = {0x09, 0x01}, .len = 2},       //   USAGE (Pointer)
    {.buf = {0xa1, 0x00}, .len = 2},       //   COLLECTION (Physical)
    {.buf = {0x05, 0x09}, .len = 2},       //     USAGE_PAGE (Button)
    {.buf = {0x19, 0x01}, .len = 2},       //     USAGE_MINIMUM (Undefined)
    {.buf = {0x29, 0x03}, .len = 2},       //     USAGE_MAXIMUM (Undefined)
    {.buf = {0x15, 0x00}, .len = 2},       //     LOGICAL_MINIMUM (0)
    {.buf = {0x25, 0x01}, .len = 2},       //     LOGICAL_MAXIMUM (1)
    {.buf = {0x95, 0x03}, .len = 2},       //     REPORT_COUNT (3)
    {.buf = {0x75, 0x01}, .len = 2},       //     REPORT_SIZE (1)
    {.buf = {0x81, 0x02}, .len = 2},       //     INPUT (Data,Var,Abs)
    {.buf = {0x95, 0x05}, .len = 2},       //     REPORT_COUNT (5)
    {.buf = {0x81, 0x01}, .len = 2},       //     INPUT (Cnst,Ary,Abs)
    {.buf = {0x05, 0x01}, .len = 2},       //     USAGE_PAGE (Generic Desktop)
    {.buf = {0x09, 0x30}, .len = 2},       //     USAGE (X)
    {.buf = {0x09, 0x31}, .len = 2},       //     USAGE (Y)
    {.buf = {0x09, 0x38}, .len = 2},       //     USAGE (Wheel)
    {.buf = {0x09, 0x00}, .len = 2},       //     USAGE (Undefined)
    {.buf = {0x15, 0x81}, .len = 2},       //     LOGICAL_MINIMUM (-127)
    {.buf = {0x25, 0x7f}, .len = 2},       //     LOGICAL_MAXIMUM (127)
    {.buf = {0x75, 0x08}, .len = 2},       //     REPORT_SIZE (8)
    {.buf = {0x95, 0x04}, .len = 2},       //     REPORT_COUNT (4)
    {.buf = {0x81, 0x06}, .len = 2},       //     INPUT (Data,Var,Rel)
    {.buf = {0xc0}, .len = 1},             //   END_COLLECTION
    {.buf = {0xc0}, .len = 1},             // END_COLLECTION
    {.buf = {0x05, 0x01}, .len = 2},       // USAGE_PAGE (Generic Desktop)
    {.buf = {0x09, 0x02}, .len = 2},       // USAGE (Mouse)
    {.buf = {0xa1, 0x01}, .len = 2},       // COLLECTION (Application)
    {.buf = {0x85, 0x09}, .len = 2},       //   REPORT_ID (9)
    {.buf = {0x09, 0x01}, .len = 2},       //   USAGE (Pointer)
    {.buf = {0xa1, 0x00}, .len = 2},       //   COLLECTION (Physical)
    {.buf = {0x05, 0x09}, .len = 2},       //   USAGE_PAGE (Button)
    {.buf = {0x19, 0x01}, .len = 2},       //   USAGE_MINIMUM (Button 1)
    {.buf = {0x29, 0x03}, .len = 2},       //   USAGE_MAXIMUM (Button 3)
    {.buf = {0x15, 0x00}, .len = 2},       //   LOGICAL_MINIMUM (0)
    {.buf = {0x25, 0x01}, .len = 2},       //   LOGICAL_MAXIMUM (1)
    {.buf = {0x95, 0x03}, .len = 2},       //   REPORT_COUNT (3)
    {.buf = {0x75, 0x01}, .len = 2},       //     REPORT_SIZE (1)
    {.buf = {0x81, 0x02}, .len = 2},       //     INPUT (Data,Var,Abs)
    {.buf = {0x95, 0x05}, .len = 2},       //   REPORT_COUNT (5)
    {.buf = {0x81, 0x01}, .len = 2},       //     INPUT (Cnst,Ary,Abs)
    {.buf = {0x05, 0x01}, .len = 2},       //   USAGE_PAGE (Generic Desktop)
    {.buf = {0x09, 0x30}, .len = 2},       //   USAGE (X)
    {.buf = {0x09, 0x31}, .len = 2},       //   USAGE (Y)
    {.buf = {0x15, 0x00}, .len = 2},       //   LOGICAL_MINIMUM (0)
    {.buf = {0x26, 0xff, 0x7f}, .len = 3}, //   LOGICAL_MAXIMUM (32767)
    {.buf = {0x35, 0x00}, .len = 2},       //   PHYSICAL_MINIMUM (0)
    {.buf = {0x46, 0xff, 0x7f}, .len = 3}, //   PHYSICAL_MAXIMUM (32767)
    {.buf = {0x95, 0x02}, .len = 2},       //   REPORT_COUNT (2)
    {.buf = {0x75, 0x10}, .len = 2},       //   REPORT_SIZE (16)
    {.buf = {0x81, 0x02}, .len = 2},       //     INPUT (Data,Var,Abs)
    {.buf = {0x05, 0x0d}, .len = 2},       //     USAGE_PAGE (Digitizers)
    {.buf = {0x09, 0x30}, .len = 2},       //     USAGE (Tip Pressure)
    {.buf = {0x26, 0xff, 0x03}, .len = 3}, //   LOGICAL_MAXIMUM (1023)
    {.buf = {0x95, 0x01}, .len = 2},       //   REPORT_COUNT (1)
    {.buf = {0x75, 0x10}, .len = 2},       //   REPORT_SIZE (16)
    {.buf = {0x81, 0x02}, .len = 2},       //   INPUT (Data,Var,Abs)
    {.buf = {0xc0}, .len = 1},             //   END_COLLECTION
    {.buf = {0xc0}, .len = 1},             // END_COLLECTION
    {.len = 0}
};


static void
hexdump_cmp(FILE *f, bool ctrl,
            void *left_buf, size_t left_size,
            void *right_buf, size_t right_size)
{
    size_t  left_i;
    size_t  right_i;
    size_t  pos;

#define left_bbuf   ((uint8_t *)left_buf)
#define right_bbuf  ((uint8_t *)right_buf)

    for (left_i = 0, right_i = 0;
         (left_i < left_size) || (right_i < right_size);)
    {
#define LINE(_t, _o) \
    do {                                                                \
        for (pos = 0; pos < 8; pos++)                                   \
            if (_t##_i < _t##_size)                                     \
            {                                                           \
                if ((_t##_i >= _o##_size) ||                            \
                    (_t##_bbuf[_t##_i] != _o##_bbuf[_t##_i]))           \
                    fprintf(f,                                          \
                            (ctrl ? " \x1B[7m%.2X\x1B[0m" : "!%.2X"),   \
                            _t##_bbuf[_t##_i]);                         \
                else                                                    \
                    fprintf(f, " %.2X", _t##_bbuf[_t##_i]);             \
                _t##_i++;                                               \
            }                                                           \
            else                                                        \
                fprintf(f, "   ");                                      \
    } while (0)

        LINE(left, right);
        fprintf(stderr, "    ");
        LINE(right, left);
        fprintf(stderr, "\n");

#undef LINE
    }

#undef right_bbuf
#undef left_bbuf
}

#define ERR(_fmt, _args...) fprintf(stderr, _fmt "\n", ##_args)

#define ERR_CLNP(_fmt, _args...) \
    do {                            \
        ERR(_fmt, ##_args);         \
        goto cleanup;               \
    } while (0)

int
main(int argc, char **argv)
{
    int                 result          = 1;
    const item_desc    *orig_item;
    void               *orig_rd_buf     = NULL;
    size_t              orig_rd_len;
    void               *p;

    hidrd_src          *src             = NULL;
    hidrd_snk          *snk             = NULL;
    void               *test_rd_buf     = NULL;
    size_t              test_rd_len     = 0;

    const hidrd_item   *test_item;

    char               *err             = NULL;

    (void)argc;
    (void)argv;

    if (!hidrd_fmt_init(&hidrd_natv))
        ERR_CLNP("Failed to initialize native format support");

    /*
     * Generate original report descriptor
     */
    for (orig_rd_len = 0, orig_item = item_list;
         orig_item->len != 0; orig_item++)
        orig_rd_len += orig_item->len;

    orig_rd_buf = malloc(orig_rd_len);

    for (p = orig_rd_buf, orig_item = item_list;
         orig_item->len != 0;
         p += orig_item->len, orig_item++)
        memcpy(p, orig_item->buf, orig_item->len);

    /*
     * Write report descriptor to a native sink
     */
    snk = hidrd_snk_new(hidrd_natv.snk, &err, &test_rd_buf, &test_rd_len);
    if (snk == NULL)
        ERR_CLNP("Failed to create native sink:\n%s", err);
    free(err);
    err = NULL;

    for (orig_item = item_list; orig_item->len != 0; orig_item++)
        if (!hidrd_snk_put(snk, orig_item->buf))
            ERR_CLNP("Failed to put item #%zu:\n%s",
                     (orig_item - item_list),
                     (err = hidrd_snk_errmsg(snk)));

    if (!hidrd_snk_close(snk))
        ERR_CLNP("Failed to close native sink:\n%s",
                 (err = hidrd_snk_errmsg(snk)));
    snk = NULL;

    /*
     * Compare resulting and original descriptor.
     */
    if (test_rd_len != orig_rd_len)
    {
        ERR("Invalid test resource descriptor length (%zu != %zu)\n\n",
            test_rd_len, orig_rd_len);
        hexdump_cmp(stderr, true,
                    orig_rd_buf, orig_rd_len, test_rd_buf, test_rd_len);
        goto cleanup;
    }

    if (test_rd_buf == NULL)
        ERR_CLNP("Test resource descriptor buffer is NULL");

    if (memcmp(test_rd_buf, orig_rd_buf, orig_rd_len) != 0)
    {
        ERR("Test resource descriptor doesn't match\n\n");
        hexdump_cmp(stderr, true,
                    orig_rd_buf, orig_rd_len, test_rd_buf, test_rd_len);
        goto cleanup;
    }

    /*
     * Read test descriptor source and compare it to the items.
     */
    src = hidrd_src_new(hidrd_natv.src, &err, test_rd_buf, test_rd_len);
    if (src == NULL)
        ERR_CLNP("Failed to create the test source:\n%s", err);
    free(err);
    err = NULL;

    for (orig_item = item_list; orig_item->len != 0; orig_item++)
    {
        if ((test_item = hidrd_src_get(src)) == NULL)
            ERR_CLNP("Failed to retrieve item #%zu "
                     "from the test source:\n%s", (orig_item - item_list + 1),
                     (err = hidrd_src_errmsg(src)));
        if (memcmp(test_item, orig_item->buf, orig_item->len) != 0)
            ERR_CLNP("Item #%zu retrieved from the test source "
                     "doesn't match the original",
                     (orig_item - item_list + 1));
    }
    if (hidrd_src_get(src) != NULL)
        ERR_CLNP("The test source still has items to retrieve");

    if (hidrd_src_error(src))
        ERR_CLNP("The test source has unexpected error indicator");

    hidrd_src_delete(src);
    src = NULL;

    result = 0;

cleanup:

    hidrd_src_delete(src);
    hidrd_snk_delete(snk);
    free(test_rd_buf);
    free(orig_rd_buf);
    free(err);
    hidrd_fmt_clnp(&hidrd_natv);

    return result;
}


