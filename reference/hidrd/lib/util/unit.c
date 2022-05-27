/** @file
 * @brief HID report descriptor - utilities - unit value
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
#include "hidrd/util/dec.h"
#include "hidrd/util/str.h"
#include "hidrd/util/tkn.h"
#include "hidrd/util/buf.h"
#include "hidrd/util/unit.h"


/* Define unit system to numeric string conversion functions */
HIDRD_NUM_CONV_DEFS(unit_system, u32);


char *
hidrd_unit_exp_to_dec(hidrd_unit_exp exp)
{
    assert(hidrd_unit_exp_valid(exp));
    return HIDRD_NUM_TO_STR(s32, hidrd_unit_exp_to_int(exp), NONE, DEC);
}


bool
hidrd_unit_exp_from_dec(hidrd_unit_exp *pexp, const char *str)
{
    int32_t exp_int;

    assert(str != NULL);

    if (!HIDRD_NUM_FROM_STR(s32, &exp_int, str, NONE, DEC))
        return false;

    if (!hidrd_unit_exp_valid_int(exp_int))
        return false;

    if (pexp != NULL)
        *pexp = hidrd_unit_exp_from_int(exp_int);

    return true;
}


#ifdef HIDRD_WITH_TOKENS
/* Define unit system <-> token conversion functions */
#define MAP(_N, _n) HIDRD_TKN_LINK(HIDRD_UNIT_SYSTEM_##_N, _n)
HIDRD_TKN_CONV_DEFS(unit_system,
                    MAP(NONE, none),
                    MAP(SI_LINEAR, SI_linear),
                    MAP(SI_ROTATION, SI_rotation),
                    MAP(ENGLISH_LINEAR, English_linear),
                    MAP(ENGLISH_ROTATION, English_rotation),
                    MAP(VENDOR, vendor))
#undef MAP


static const char *exp_tkn[HIDRD_UNIT_NIBBLE_INDEX_EXP_NUM]
                          [HIDRD_UNIT_SYSTEM_KNOWN_NUM + 1]    = {
#define EXP(_type, \
               _si_linear, _si_rotation,                            \
               _english_linear, _english_rotation)                  \
    {[HIDRD_UNIT_SYSTEM_NONE]   = #_type,                           \
     [HIDRD_UNIT_SYSTEM_SI_LINEAR]          = #_si_linear,          \
     [HIDRD_UNIT_SYSTEM_SI_ROTATION]        = #_si_rotation,        \
     [HIDRD_UNIT_SYSTEM_ENGLISH_LINEAR]     = #_english_linear,     \
     [HIDRD_UNIT_SYSTEM_ENGLISH_ROTATION]   = #_english_rotation}

    EXP(length,             centimeter, radians,    inch,       degrees),
    EXP(mass,               gram,       gram,       slug,       slug),
    EXP(time,               seconds,    seconds,    seconds,    seconds),
    EXP(temperature,        kelvin,     kelvin,     fahrenheit, fahrenheit),
    EXP(current,            ampere,     ampere,     ampere,     ampere),
    EXP(luminous_intensity, candela,    candela,    candela,    candela)

#undef EXP
};

/*
 * TODO add conversion of exponent nibble indexes to/from tokens.
 */


char *
hidrd_unit_to_expr(hidrd_unit unit, hidrd_tkn_hmnz_cap cap)
{
    void               *result      = NULL;
    hidrd_unit_system   system;
    hidrd_buf           buf         = HIDRD_BUF_EMPTY;
    bool                first       = true;
    size_t              i;
    hidrd_unit_exp      exp;
    char                tkn_buf[32];

    assert(hidrd_unit_valid(unit));
    assert(hidrd_unit_known(unit));

    system = hidrd_unit_get_system(unit);

    assert(hidrd_unit_system_known(system));

    for (i = 0; i < HIDRD_UNIT_NIBBLE_INDEX_EXP_NUM; i++)
    {
        exp = hidrd_unit_get_nibble(unit,
                                    i + HIDRD_UNIT_NIBBLE_INDEX_EXP_MIN);
        if (exp == HIDRD_UNIT_EXP_0)
            continue;

        if (snprintf(tkn_buf, sizeof(tkn_buf),
                     "%s", exp_tkn[i][system]) >= (int)sizeof(tkn_buf))
            goto cleanup;

        assert(hidrd_tkn_hmnzbl(tkn_buf));
        hidrd_tkn_hmnz(tkn_buf, cap);

        if (!first && !hidrd_buf_add_str(&buf, " * "))
            goto cleanup;

        if (exp == HIDRD_UNIT_EXP_1)
        {
            if (!hidrd_buf_add_str(&buf, tkn_buf))
                goto cleanup;
        }
        else
        {
            if (!hidrd_buf_add_printf(&buf, "%s^%d",
                                      tkn_buf, hidrd_unit_exp_to_int(exp)))
                goto cleanup;
        }

        first = false;
    }

    if (!hidrd_buf_add_char(&buf, '\0'))
        goto cleanup;
    hidrd_buf_retention(&buf);
    hidrd_buf_disown(&buf, &result, NULL, NULL);

cleanup:

    hidrd_buf_clnp(&buf);

    return result;
}


#endif /* HIDRD_WITH_TOKENS */


