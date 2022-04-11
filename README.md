# usb-regulus
A GUI tool for USB scan, view and test.

## Build

### Linux

1. Install [Qt6](https://www.qt.io/download).
2. Install necessary C++ compiling enviroment, such as `build-essential`, `g++`, `make`, `cmake`, `ninja-build` and so on.
3. Install `libusb-1.0` and `libudev`. For an example, on Ubuntu:

    ```bash
    sudo apt install libusb-1.0-0-dev libudev-dev
    ```

4. Open Qt Creator, and select this project, then build.

## Reference

1. Some implementation refers to [QtUsb](https://github.com/fpoussin/QtUsb).
2. Some source files from [usbutils](https://github.com/gregkh/usbutils).
