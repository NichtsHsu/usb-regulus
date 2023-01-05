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

class UsbHtmlBuilder
{
public:
    UsbHtmlBuilder();

    static const QString NEWLINE;
    static const QString TAB;

    UsbHtmlBuilder &title(const QString &text);
    UsbHtmlBuilder &start(const QString &title);
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
    UsbHtmlBuilder &strdesc(const QString &name, size_t strDescIndex, usb::UsbDevice *device);
    UsbHtmlBuilder &end();
    template<typename T>
#if __cplusplus >= 202002L
    requires requires(T *sub) { sub->infomationToHtml(); }
#endif
    UsbHtmlBuilder &append(T *const subcomponent);
    UsbHtmlBuilder &note(const QString &message);
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
