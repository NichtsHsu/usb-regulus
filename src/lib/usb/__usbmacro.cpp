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
        default:
        return "UNKNOWN";
    }
}
