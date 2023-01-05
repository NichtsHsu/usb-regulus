#include "usbhtmlbuilder.h"
#include "usbdevice.h"

const QString UsbHtmlBuilder::NEWLINE = QString("<br />");
const QString UsbHtmlBuilder::TAB = QString("&nbsp;&nbsp;&nbsp;&nbsp;");

UsbHtmlBuilder::UsbHtmlBuilder()
{
}

UsbHtmlBuilder &UsbHtmlBuilder::title(const QString &text)
{
    _html += QString("<h1 align='center'>%1</h1>").arg(text);
    return *this;
}

UsbHtmlBuilder &UsbHtmlBuilder::start(const QString &title)
{
    _html += QString("<h2 align='center'>%1</h2><table width=\"100%\">").arg(title);
    return *this;
}

UsbHtmlBuilder &UsbHtmlBuilder::attr(const QString &name, const QString &text, const QString &text2)
{
    _html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">%2</td><td>%3</td></tr>")
            .arg(name, text, text2);
    return *this;
}

UsbHtmlBuilder &UsbHtmlBuilder::strdesc(const QString &name, size_t strDescIndex, usb::UsbDevice *device)
{
    _html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>") \
            .arg(name) \
            .arg(strDescIndex, 2, 16, QChar('0')) \
            .arg(device->getStringDescriptor(strDescIndex)); \
    return *this;
}

UsbHtmlBuilder &UsbHtmlBuilder::end()
{
    _html += QString("</table>");
    return *this;
}

UsbHtmlBuilder &UsbHtmlBuilder::note(const QString &message)
{
    _html += QString("<p><i>%1</i></p>")
            .arg(message);
    return *this;
}

const QString &UsbHtmlBuilder::build() const
{
    return _html;
}

