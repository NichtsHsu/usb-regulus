/** @file
 * @brief HID report descriptor - XML sink - element handling
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

#include "element.h"

bool
xml_snk_element_nt_valid(xml_snk_element_nt nt)
{
    switch (nt)
    {
        case XML_SNK_ELEMENT_NT_NONE:
        case XML_SNK_ELEMENT_NT_CONTENT:
        case XML_SNK_ELEMENT_NT_COMMENT:
        case XML_SNK_ELEMENT_NT_ATTR:
            return true;
        default:
            return false;
    }
}

bool
xml_snk_element_new(hidrd_xml_snk_inst        *xml_snk,
                    const char                 *name)
{
    assert(xml_snk->cur == NULL);

    xml_snk->cur = xmlNewChild(xml_snk->prnt, NULL, BAD_CAST name, NULL);

    return (xml_snk->cur != NULL);
}


bool
xml_snk_element_set_attrpv(hidrd_xml_snk_inst  *xml_snk,
                           const char          *name,
                           hidrd_fmt_type       fmt,
                           va_list             *pap)
{
    char       *value;
    xmlAttrPtr  attr;

    assert(xml_snk->cur != NULL);

    if (!hidrd_fmtpva(&value, fmt, pap))
    {
        XML_ERR("failed to format \"%s\" element \"%s\" attribute value",
                (const char *)xml_snk->cur->name, name);
        return false;
    }

    attr = xmlSetProp(xml_snk->cur, BAD_CAST name, BAD_CAST value);

    free(value);

    return (attr != NULL);
}


bool
xml_snk_element_add_contentpv(hidrd_xml_snk_inst   *xml_snk,
                              hidrd_fmt_type        fmt,
                              va_list               *pap)
{
    char   *content;

    assert(xml_snk->cur != NULL);

    if (!hidrd_fmtpva(&content, fmt, pap))
    {
        XML_ERR("failed to format \"%s\" element content",
                (const char *)xml_snk->cur->name);
        return false;
    }

    xmlNodeAddContent(xml_snk->cur, BAD_CAST content);

    free(content);

    return true;
}


bool
xml_snk_element_add_commentpv(hidrd_xml_snk_inst   *xml_snk,
                              hidrd_fmt_type        fmt,
                              va_list              *pap)
{
    char       *content;
    xmlNodePtr  comment;

    assert(xml_snk->cur != NULL);

    if (!hidrd_fmtpva(&content, fmt, pap))
    {
        XML_ERR("failed to format \"%s\" element comment",
                (const char *)xml_snk->cur->name);
        return false;
    }

    comment = xmlNewDocComment(xml_snk->doc, BAD_CAST content);
    free(content);
    if (comment == NULL)
        return false;

    return (xmlAddChild(xml_snk->cur, comment) != NULL);
}


void
xml_snk_element_commit(hidrd_xml_snk_inst *xml_snk,
                       bool                 container)
{
    assert(xml_snk->cur != NULL);
    
    if (container)
        xml_snk->prnt = xml_snk->cur;

    xml_snk->cur = NULL;
}


bool
xml_snk_element_addpv(hidrd_xml_snk_inst   *xml_snk,
                      bool                  container,
                      const char           *name,
                      va_list              *pap)
{
    bool    success = true;
    bool    end     = false;

    assert(xml_snk->cur == NULL);

    if (!xml_snk_element_new(xml_snk, name))
        return false;

    while (success && !end)
    {
        xml_snk_element_nt  nt  = va_arg(*pap, xml_snk_element_nt);

        assert(xml_snk_element_nt_valid(nt));

        switch (nt)
        {
            case XML_SNK_ELEMENT_NT_ATTR:
                {
                    const char     *name        = va_arg(*pap,
                                                         const char *);
                    hidrd_fmt_type  value_fmt   = va_arg(*pap,
                                                         hidrd_fmt_type);

                    success = xml_snk_element_set_attrpv(
                                        xml_snk, name, value_fmt, pap);
                }
                break;

            case XML_SNK_ELEMENT_NT_COMMENT:
                {
                    hidrd_fmt_type  comment_fmt = va_arg(*pap,
                                                         hidrd_fmt_type);

                    success = xml_snk_element_add_commentpv(
                                        xml_snk, comment_fmt, pap);
                }
                break;

            case XML_SNK_ELEMENT_NT_CONTENT:
                {
                    hidrd_fmt_type  content_fmt = va_arg(*pap,
                                                         hidrd_fmt_type);

                    success = xml_snk_element_add_contentpv(
                                        xml_snk, content_fmt, pap);
                }
                break;

            case XML_SNK_ELEMENT_NT_NONE:
                end = true;
                break;

            default:
                assert(!"Unknown node type");
                XML_ERR("unknown element node type %u", nt);
                success = false;
                break;
        }
    }

    if (success)
    {
        xml_snk_element_commit(xml_snk, container);
        return true;
    }

    /*
     * Cleanup
     */
    while (!end)
    {
        xml_snk_element_nt  nt  = va_arg(*pap, xml_snk_element_nt);
        hidrd_fmt_type      fmt;

        assert(xml_snk_element_nt_valid(nt));

        switch (nt)
        {
            case XML_SNK_ELEMENT_NT_ATTR:
                /* Retrieve name */
                (void)va_arg(*pap, const char *);
                /* FALLTHROUGH */
            case XML_SNK_ELEMENT_NT_COMMENT:
            case XML_SNK_ELEMENT_NT_CONTENT:
                fmt =  va_arg(*pap, hidrd_fmt_type);
                hidrd_fmtfreepv(fmt, pap);
                break;

            case XML_SNK_ELEMENT_NT_NONE:
                end = true;
                break;

            default:
                assert(!"Unknown node type");
                XML_ERR("\nunknown element node type %u", nt);
                return false;
        }
    }

    return false;
}


bool
xml_snk_element_add(hidrd_xml_snk_inst    *xml_snk,
                    bool                    container,
                    const char             *name,
                    ...)
{
    va_list ap;
    bool    success;

    va_start(ap, name);
    success = xml_snk_element_addpv(xml_snk, container, name, &ap);
    va_end(ap);

    return success;
}


