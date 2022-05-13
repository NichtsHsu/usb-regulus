# usb-regulus
A GUI tool for USB viewing, reading and writing.

## Clone with submodules

```bash
git clone --recurse-submodules https://github.com/NichtsHsu/usb-regulus.git
```

## Build

### Linux

1. Install [Qt6](https://www.qt.io/download).
2. Install necessary C++ compiling enviroment, such as `build-essential`, `g++`, `make`, `cmake`, `ninja-build` and so on.
3. Install `libusb-1.0` and `libudev`. For an example, on Ubuntu:

    ```bash
    sudo apt install libusb-1.0-0-dev libudev-dev
    ```

4. Open Qt Creator, and select this project, then build.

### MacOS

I have never used MacOS, but it should not be much different from Linux.

### Windows

**Not Support Yet**

Because `libusb` doesn't seem to work well on Windows, especially it does not support hotplug. I'm looking for the possibility to use WinUsb instead of libusb, but the work will not be started until the basic functions on Linux are done.

## Reference

1. [Qt](https://www.qt.io/).
2. [QtUsb](https://github.com/fpoussin/QtUsb).
3. [usbutils](https://github.com/gregkh/usbutils).
4. [QHexEdit](https://github.com/Simsys/qhexedit2).
