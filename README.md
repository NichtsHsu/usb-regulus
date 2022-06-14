# usb-regulus

![platform](https://img.shields.io/badge/platform-linux-lightgrey)
[![license](https://img.shields.io/badge/license-GPLv3-blue)](https://github.com/NichtsHsu/usb-regulus/blob/master/LICENSE)
[![996.icu](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg)](https://996.icu)

A GUI tool for USB viewing, reading and writing.

## Note

You are suggested to install an udev rule file for unprivileged users to be able to access USB device.

See: [libusb calls without sudo using udev-rules](https://stackoverflow.com/questions/40597515/libusb-calls-without-sudo-using-udev-rules).

Alternatively, simply run this application with root privileges.

## Download

The [release page](https://github.com/NichtsHsu/usb-regulus/releases) provides the lastest AppImage files.

## Build

## Clone with submodules

```bash
git clone --recurse-submodules https://github.com/NichtsHsu/usb-regulus.git
```

### Linux

1. Install [Qt6](https://www.qt.io/download) and select GCC x64 toolkit.
2. Install necessary C++ compiling enviroment, such as `build-essential`, `g++`, `make`, `cmake`, `ninja-build` and so on.
3. Install `libusb-1.0` and `libudev`. For an example, on Ubuntu:

    ```bash
    sudo apt install libusb-1.0-0-dev libudev-dev
    ```

4. Open Qt Creator, and select this project, then build. Or, if you would like to build with terminal:

    ```bash
    cd usb-regulus
    mkdir build
    cd build
    {Your Qt Install Path}/gcc_64/bin/qmake ..
    make
    ```

### MacOS

I have never used MacOS, but it should not be much different from Linux.

### Windows

**Not Support Yet**

Because `libusb` doesn't seem to work well on Windows, especially it does not support hotplug. I'm looking for the possibility to use WinUsb instead of libusb, but the work will not be started until the basic functions on Linux are done.

## Reference

1. [Qt](https://www.qt.io/).
2. [QtUsb](https://github.com/fpoussin/QtUsb).
3. [usbutils](https://github.com/gregkh/usbutils), especially lsusb.
4. [usbhid-dump](https://github.com/DIGImend/usbhid-dump).
5. [hidrd](https://github.com/DIGImend/hidrd).
6. [QHexEdit](https://github.com/Simsys/qhexedit2).
