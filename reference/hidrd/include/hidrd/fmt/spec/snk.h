/** @file
 * @brief HID report descriptor - specification example sink
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

#ifndef __HIDRD_FMT_SPEC_SNK_H__
#define __HIDRD_FMT_SPEC_SNK_H__

#include "hidrd/strm/snk/inst.h"
#include "hidrd/fmt/spec/snk/ent_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Declarations of the specification example sink type methods and constants
 */
extern bool hidrd_spec_snk_init(hidrd_snk *snk, char **perr,
                                size_t tabstop, bool dumps, bool comments);
extern bool hidrd_spec_snk_initv(hidrd_snk *snk, char **perr, va_list ap);
extern const hidrd_opt_spec hidrd_spec_snk_opts_spec[];
extern bool hidrd_spec_snk_init_opts(hidrd_snk         *snk,
                                     char             **perr,
                                     const hidrd_opt   *list);
extern bool hidrd_spec_snk_valid(const hidrd_snk *snk);
extern char *hidrd_spec_snk_errmsg(const hidrd_snk *snk);
extern bool hidrd_spec_snk_put(hidrd_snk *snk, const hidrd_item *item);
extern bool hidrd_spec_snk_flush(hidrd_snk *snk);
extern void hidrd_spec_snk_clnp(hidrd_snk *snk);

/** Specification example sink type */
extern const hidrd_snk_type hidrd_spec_snk;

/** Specification example sink item state table */
typedef struct hidrd_spec_snk_state hidrd_spec_snk_state;
struct hidrd_spec_snk_state {
    hidrd_spec_snk_state   *prev;       /**< Previous state */
    hidrd_usage_page        usage_page; /**< Usage page in effect */
};

/** Specification example sink error code */
typedef enum hidrd_spec_snk_err {
    HIDRD_SPEC_SNK_ERR_NONE,    /**< No error */
    HIDRD_SPEC_SNK_ERR_ALLOC    /**< Memory allocation failure */
} hidrd_spec_snk_err;

/** Specification example sink instance */
typedef struct hidrd_spec_snk_inst {
    hidrd_snk                   snk;        /**< Parent structure */
    size_t                      tabstop;    /**< Number of spaces per tab */
    bool                        dumps;      /**< "Output item dumps" flag */
    bool                        comments;   /**< "Output comments" flag */

    int                         depth;      /**< Current nesting depth */
    hidrd_spec_snk_state       *state;      /**< Item state table stack */

    hidrd_spec_snk_ent_list     list;       /**< Entry list */
    hidrd_spec_snk_err          err;        /**< Last error code */
} hidrd_spec_snk_inst;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HIDRD_FMT_SPEC_SNK_H__ */
