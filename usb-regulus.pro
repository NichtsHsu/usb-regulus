QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    src/lib/ \
    reference/ \

FORMS += \
    ui/mainwindow/mainwindow.ui

HEADERS += \
    reference/usbutils/names.h \
    reference/usbutils/usb-spec.h \
    reference/usbutils/sysfs.h \
    src/lib/log/logger.h \
    src/lib/usb/usbconfigurationdescriptor.h \
    src/lib/usb/usbdevice.h \
    src/lib/usb/usbdevicedescriptor.h \
    src/lib/usb/usbendpointdescriptor.h \
    src/lib/usb/usbhost.h \
    src/lib/usb/usbinterface.h \
    src/lib/usb/usbinterfacedescriptor.h \
    src/mainwindow/mainwindow.h \
    src/mainwindow/usbdevicetreeview.h

SOURCES += \
    reference/usbutils/names.c \
    reference/usbutils/sysfs.c \
    src/lib/log/logger.cpp \
    src/lib/usb/usbconfigurationdescriptor.cpp \
    src/lib/usb/usbdevice.cpp \
    src/lib/usb/usbdevicedescriptor.cpp \
    src/lib/usb/usbendpointdescriptor.cpp \
    src/lib/usb/usbhost.cpp \
    src/lib/usb/usbinterface.cpp \
    src/lib/usb/usbinterfacedescriptor.cpp \
    src/main.cpp \
    src/mainwindow/mainwindow.cpp \
    src/mainwindow/usbdevicetreeview.cpp

TRANSLATIONS += \
    usb-regulus_zh_CN.ts

unix {
    !packagesExist(libusb-1.0):error("Could not find libusb-1.0 using pkg-config, please install libusb-1.0")
    !packagesExist(libudev):error("Could not find libudev using pkg-config, please install libudev")
    PKGCONFIG += libusb-1.0
    PKGCONFIG += libudev
    CONFIG += link_pkgconfig
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    LICENSE \
    README.md
