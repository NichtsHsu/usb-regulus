/** @file
 * @brief HID report descriptor - option instance
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

#ifndef __HIDRD_OPT_INST_H__
#define __HIDRD_OPT_INST_H__

#include <stdbool.h>
#include "hidrd/opt/spec.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Option */
typedef struct hidrd_opt {
    const char         *name;
    hidrd_opt_type      type;
    hidrd_opt_value     value;
} hidrd_opt;


/**
 * Check if an option is valid.
 *
 * @param opt   Option to check.
 *
 * @return True if the option is valid, false otherwise.
 */
extern bool hidrd_opt_valid(const hidrd_opt *opt);

/**
 * Get a string value.
 *
 * @param opt   Option pointer.
 *
 * @return Option value.
 */
extern const char *hidrd_opt_get_string(const hidrd_opt *opt);

/**
 * Get a boolean value.
 *
 * @param opt   Option pointer.
 *
 * @return Option value.
 */
extern bool hidrd_opt_get_boolean(const hidrd_opt *opt);

/**
 * Get a signed 32-bit integer value.
 *
 * @param opt   Option pointer.
 *
 * @return Option value.
 */
extern int32_t hidrd_opt_get_s32(const hidrd_opt *opt);

/**
 * Get an unsigned 32-bit integer value.
 *
 * @param opt   Option pointer.
 *
 * @return Option value.
 */
extern uint32_t hidrd_opt_get_u32(const hidrd_opt *opt);

/**
 * Parse a token pair representation of an option.
 *
 * @param opt   Output option; will contain name and possibly value
 *              referenced from the token pair.
 * @param spec  Option specification to apply to conversion.
 * @param tkns  Token pair to parse; the name and possibly value tokens will
 *              be referenced in the resulting option.
 *
 * @return True if parsed successfully, false otherwise.
 */
extern bool hidrd_opt_parse_tkns(hidrd_opt             *opt,
                                 const hidrd_opt_spec  *spec,
                                 const hidrd_opt_tkns  *tkns);

/**
 * Parse a string representation of an option, modifying and referencing in
 * the resulting option.
 *
 * @param opt   Output option; will contain name and possibly value
 *              referenced from the original string.
 * @param spec  Option specification to apply to conversion.
 * @param buf   String to parse; will be modified and referenced in the
 *              resulting option.
 *
 * @return True if parsed successfully, false otherwise.
 */
extern bool hidrd_opt_parse(hidrd_opt              *opt,
                            const hidrd_opt_spec   *spec,
                            char                   *buf);
/**
 * Format a token pair representation of an option.
 *
 * @param tkns  Output token pair, will contain dynamically allocated value.
 * @param opt   The option to convert to the token pair representation.
 *
 * @return True if converted successfully, false if failed to allocate
 *         memory.
 */
extern bool hidrd_opt_format_tkns(hidrd_opt_tkns   *tkns,
                                  const hidrd_opt  *opt);

/**
 * Format a string representation of an option.
 *
 * @param spec  Option to format.
 *
 * @return Dynamically allocated string representation of the option, or
 *         NULL, if failed to allocate memory.
 */
extern char *hidrd_opt_format(const hidrd_opt *opt);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HIDRD_OPT_INST_H__ */
