/*! USB HTML builder to help USB descriptors generate informations

 * Copyright (C) 2022-2023 Nichts Hsu

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef USBHTMLBUILDER_H
#define USBHTMLBUILDER_H

#include <QtGlobal>
#include <QString>
#if __cplusplus >= 202002L
#include <concepts>
#endif

namespace usb {
    class UsbDevice;
}

/**
 * @brief The UsbHtmlBuilder class
 * Help USB descriptors generate informations.
 * Basic usage:
 *      QString information = UsbHtmlBuilder()
 *              .start("My Descriptor")
 *              .attr("attrname", value)
 *              .end()
 *              .build();
 */
class UsbHtmlBuilder
{
public:
    UsbHtmlBuilder();

    /**
     * @brief NEWLINE
     * HTML `<br />` tag.
     */
    static const QString NEWLINE;

    /**
     * @brief TAB
     * HTML four `&nbsp;` character.
     */
    static const QString TAB;

    /**
     * @brief title
     * Title of a page, HTML `<h1>` tag.
     */
    UsbHtmlBuilder &title(const QString &text);

    /**
     * @brief start
     * Start a attributes section in a HTML table.
     * @param title
     * Title of this section, HTML `<h2> tag.
     * @param enableQuickReference
     * Generate quick reference link for fields.
     * The link format is: "help://127.0.0.1/quickref?desc=${Descriptor Name}&field=${Field Name}".
     * These links will be handled by FieldQuickReference on usb-regulus.
     * @param version
     * Some descriptors (like UAC) behave differently in different version
     * @see class FieldQuickReference
     */
    UsbHtmlBuilder &start(const QString &title, bool enableQuickReference = false, const QString &specVersion = QString());

    /**
     * @brief attr
     * Add a attribute into table.
     * Attribute table have three columns, usually the first column is attribute name,
     * the second column is the hexadecimal value, the third column is decimal value.
     * Also, the section column and the third column can be customized as string value.
     */
    template<typename T>
#if __cplusplus >= 202002L
    requires std::integral<T>
#endif
    UsbHtmlBuilder &attr(const QString &name, T value, int index = -1);
    template<typename T>
#if __cplusplus >= 202002L
    requires std::integral<T>
#endif
    UsbHtmlBuilder &attr(const QString &name, T value, const QString &text, int index = -1);

    // Compatible with Qt5
    UsbHtmlBuilder &attr(const QString &name, const QString &text, const QString &text2, int index = -1);
    UsbHtmlBuilder &attr(const QString &name, const char *text, const QString &text2, int index = -1);
    UsbHtmlBuilder &attr(const QString &name, const QString &text, const char *text2, int index = -1);
    UsbHtmlBuilder &attr(const QString &name, const char *text, const char *text2, int index = -1);

    /**
     * @brief strdesc
     * Add a string descriptor attribute into table.
     * @param name
     * Attribute name.
     * @param strDescIndex
     * String descriptor index.
     * @param device
     * USB device that the string descriptor belongs to.
     */
    UsbHtmlBuilder &strdesc(const QString &name, size_t strDescIndex, usb::UsbDevice *device, int index = -1);

    /**
     * @brief end
     * End a section, HTML `</table>` tag.
     */
    UsbHtmlBuilder &end();

    /**
     * @brief append
     * Append a subcompenent's information.
     */
    template<typename T>
#if __cplusplus >= 202002L
    requires requires(T *sub) { sub->infomationToHtml(); }
#endif
    UsbHtmlBuilder &append(T *const subcomponent);

    /**
     * @brief note
     * Add a note message, HTML `<i>` tag.
     */
    UsbHtmlBuilder &note(const QString &message);

    /**
     * @brief build
     * Get the whole HTML string.
     */
    const QString &build() const;

private:
    QString __genUrlInHtml(const QString &field) const;
    QString _html, _descriptorName;
    bool _enableQuickReference;
};

template<typename T>
#if __cplusplus >= 202002L
requires std::integral<T>
#endif
UsbHtmlBuilder &UsbHtmlBuilder::attr(const QString &name, T value, int index)
{
    if (index < 0)
        _html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>")
                .arg(__genUrlInHtml(name)).arg(value, sizeof(value) * 2, 16, QChar('0')).arg(value);
    else
        _html += QString("<tr><td width=\"30%\">%1[%4]</td><td width=\"15%\">0x%2</td><td>%3</td></tr>")
                .arg(__genUrlInHtml(name)).arg(value, sizeof(value) * 2, 16, QChar('0')).arg(value).arg(index);
    return *this;
}

template<typename T>
#if __cplusplus >= 202002L
requires std::integral<T>
#endif
UsbHtmlBuilder &UsbHtmlBuilder::attr(const QString &name, T value, const QString &text, int index)
{
    if (index < 0)
        _html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>")
                .arg(__genUrlInHtml(name)).arg(value, sizeof(value) * 2, 16, QChar('0')).arg(text);
    else
        _html += QString("<tr><td width=\"30%\">%1[%4]</td><td width=\"15%\">0x%2</td><td>%3</td></tr>")
                .arg(__genUrlInHtml(name)).arg(value, sizeof(value) * 2, 16, QChar('0')).arg(text).arg(index);
    return *this;
}

template<typename T>
#if __cplusplus >= 202002L
requires requires(T *sub) { sub->infomationToHtml(); }
#endif
UsbHtmlBuilder &UsbHtmlBuilder::append(T *const subcomponent)
{
    if (subcomponent)
        _html += subcomponent->infomationToHtml();
    return *this;
}

#endif // USBHTMLBUILDER_H
