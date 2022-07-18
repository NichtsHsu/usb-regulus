#include "__usbmacro.h"

const char *usb_error_name(int code) {
    if (code > -100)
        return libusb_error_name(code);

    switch (code)
    {
        case ERROR_PROTECT_MOUSE:
        return "ERROR_PROTECT_MOUSE";
        case ERROR_PROTECT_KEYBOARD:
        return "ERROR_PROTECT_KEYBOARD";
        case ERROR_OUT_OF_RANGE:
        return "ERROR_OUT_OF_RANGE";
        case ERROR_DUPLICATE_REQUEST:
        return "ERROR_DUPLICATE_REQUEST";
        default:
        return "UNKNOWN";
    }
}

QString hexUuid(const QByteArray &uuid)
{
    if (uuid.length() < 16)
        return QString();

    const uint8_t *buffer = reinterpret_cast<const uint8_t *>(uuid.constData());
    return QString("{%1%2%3%4-%5%6-%7%8-%9%10-%11%12%13%14%15%16}")
            .arg(buffer[3], 2, 16, QChar('0'))
            .arg(buffer[2], 2, 16, QChar('0'))
            .arg(buffer[1], 2, 16, QChar('0'))
            .arg(buffer[0], 2, 16, QChar('0'))
            .arg(buffer[5], 2, 16, QChar('0'))
            .arg(buffer[4], 2, 16, QChar('0'))
            .arg(buffer[7], 2, 16, QChar('0'))
            .arg(buffer[6], 2, 16, QChar('0'))
            .arg(buffer[8], 2, 16, QChar('0'))
            .arg(buffer[9], 2, 16, QChar('0'))
            .arg(buffer[10], 2, 16, QChar('0'))
            .arg(buffer[11], 2, 16, QChar('0'))
            .arg(buffer[12], 2, 16, QChar('0'))
            .arg(buffer[13], 2, 16, QChar('0'))
            .arg(buffer[14], 2, 16, QChar('0'))
            .arg(buffer[15], 2, 16, QChar('0'));
}
