#include "usbhtmlbuilder.h"
#include "usbdevice.h"

const QString UsbHtmlBuilder::NEWLINE = QString("<br />");
const QString UsbHtmlBuilder::TAB = QString("&nbsp;&nbsp;&nbsp;&nbsp;");

UsbHtmlBuilder::UsbHtmlBuilder() : _enableQuickReference(false)
{
}

UsbHtmlBuilder &UsbHtmlBuilder::title(const QString &text)
{
    _html += QString("<h1 align='center'>%1</h1>").arg(text);
    return *this;
}

UsbHtmlBuilder &UsbHtmlBuilder::start(const QString &title, bool enableQuickReference, const QString &specVersion)
{
    _html += QString("<h2 align='center'>%1</h2><table width=\"100%\">").arg(title);
    _enableQuickReference = enableQuickReference;
    _descriptorName = title;
    if (!specVersion.isEmpty())
        _descriptorName += QString(" (%1)").arg(specVersion);
    return *this;
}

UsbHtmlBuilder &UsbHtmlBuilder::attr(const QString &name, const QString &text, const QString &text2, int index)
{
    if (index < 0)
        _html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">%2</td><td>%3</td></tr>")
                .arg(__genUrlInHtml(name), text, text2);
    else
        _html += QString("<tr><td width=\"30%\">%1[%4]</td><td width=\"15%\">%2</td><td>%3</td></tr>")
                .arg(__genUrlInHtml(name), text, text2).arg(index);
    return *this;
}

UsbHtmlBuilder &UsbHtmlBuilder::strdesc(const QString &name, size_t strDescIndex, usb::UsbDevice *device, int index)
{
    if (index < 0)
        _html += QString("<tr><td width=\"30%\">%1</td><td width=\"15%\">0x%2</td><td>%3</td></tr>")
                .arg(__genUrlInHtml(name))
                .arg(strDescIndex, 2, 16, QChar('0'))
                .arg(device->getStringDescriptor(strDescIndex));
    else
        _html += QString("<tr><td width=\"30%\">%1[%4]</td><td width=\"15%\">0x%2</td><td>%3</td></tr>")
                .arg(__genUrlInHtml(name))
                .arg(strDescIndex, 2, 16, QChar('0'))
                .arg(device->getStringDescriptor(strDescIndex))
                .arg(index);
    return *this;
}

UsbHtmlBuilder &UsbHtmlBuilder::end()
{
    _html += QString("</table>");
    _enableQuickReference = false;
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

QString UsbHtmlBuilder::__genUrlInHtml(const QString &field) const
{
    return _enableQuickReference ?
                QString("<a href=\"help://127.0.0.1/quickref?desc=%1&field=%2\">%3</a>")
                .arg(_descriptorName, field, field) :
                field;
}

