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
     */
    UsbHtmlBuilder &start(const QString &title);

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
    UsbHtmlBuilder &attr(const QString &name, T value);
    template<typename T>
#if __cplusplus >= 202002L
    requires std::integral<T>
#endif
    UsbHtmlBuilder &attr(const QString &name, T value, const QString &text);
    UsbHtmlBuilder &attr(const QString &name, const QString &text, const QString &text2);

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
    UsbHtmlBuilder &strdesc(const QString &name, size_t strDescIndex, usb::UsbDevice *device);

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
    QString _html;
};

template<typename T>
#if __cplusplus >= 202002L
requires std::integral<T>
#endif
UsbHtmlBuilder &UsbHtmlBuilder::attr(const QString &name, T value)
{
    _html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>")
            .arg(name).arg(value, sizeof(value) * 2, 16, QChar('0')).arg(value);
    return *this;
}

template<typename T>
#if __cplusplus >= 202002L
requires std::integral<T>
#endif
UsbHtmlBuilder &UsbHtmlBuilder::attr(const QString &name, T value, const QString &text)
{
    _html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>")
            .arg(name).arg(value, sizeof(value) * 2, 16, QChar('0')).arg(text);
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
