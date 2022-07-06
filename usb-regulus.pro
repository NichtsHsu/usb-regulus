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
    $$PWD/src/ \
    $$PWD/src/lib/ \
    $$PWD/reference/ \

FORMS += \
    ui/datatransferwindow/controltransferwidget.ui \
    ui/datatransferwindow/datatransferwindow.ui \
    ui/datatransferwindow/endpointinwidget.ui \
    ui/datatransferwindow/endpointoutwidget.ui \
    ui/mainwindow/mainwindow.ui

HEADERS += \
    reference/hidrd/src/hidrd_convert.h \
    reference/qhexedit2/src/chunks.h \
    reference/qhexedit2/src/commands.h \
    reference/qhexedit2/src/qhexedit.h \
    src/datatransferwindow/controltransferwidget.h \
    src/datatransferwindow/datatransferwindow.h \
    src/datatransferwindow/endpointinwidget.h \
    src/datatransferwindow/endpointoutwidget.h \
    src/global/settings.h \
    src/global/tools.h \
    src/lib/log/logger.h \
    src/lib/misc/option.hpp \
    src/lib/usb/__usbmacro.h \
    src/lib/usb/usbaudiocontrolinterfacedescriptor.h \
    src/lib/usb/usbaudiostreaminterfacedescriptor.h \
    src/lib/usb/usbbosdescriptor.h \
    src/lib/usb/usbconfigurationdescriptor.h \
    src/lib/usb/usbconfigurationextradescriptor.h \
    src/lib/usb/usbcontroltransferhandler.h \
    src/lib/usb/usbdevice.h \
    src/lib/usb/usbdevicecapabilitydescriptor.h \
    src/lib/usb/usbdevicedescriptor.h \
    src/lib/usb/usbdfudescriptor.h \
    src/lib/usb/usbendpointdescriptor.h \
    src/lib/usb/usbendpointreader.h \
    src/lib/usb/usbendpointwriter.h \
    src/lib/usb/usbhiddescriptor.h \
    src/lib/usb/usbhost.h \
    src/lib/usb/usbhubdescriptor.h \
    src/lib/usb/usbinterface.h \
    src/lib/usb/usbinterfaceassociationdescriptor.h \
    src/lib/usb/usbinterfacedescriptor.h \
    src/lib/usb/usbinterfaceextradescriptor.h \
    src/lib/usb/usbotgdescriptor.h \
    src/lib/usb/usbterminaltypes.h \
    src/mainwindow/mainwindow.h \
    src/mainwindow/usbdevicetreeview.h

SOURCES += \
    reference/qhexedit2/src/chunks.cpp \
    reference/qhexedit2/src/commands.cpp \
    reference/qhexedit2/src/qhexedit.cpp \
    src/datatransferwindow/controltransferwidget.cpp \
    src/datatransferwindow/datatransferwindow.cpp \
    src/datatransferwindow/endpointinwidget.cpp \
    src/datatransferwindow/endpointoutwidget.cpp \
    src/global/settings.cpp \
    src/global/tools.cpp \
    src/lib/log/logger.cpp \
    src/lib/usb/__usbmacro.cpp \
    src/lib/usb/usbaudiocontrolinterfacedescriptor.cpp \
    src/lib/usb/usbaudiostreaminterfacedescriptor.cpp \
    src/lib/usb/usbbosdescriptor.cpp \
    src/lib/usb/usbconfigurationdescriptor.cpp \
    src/lib/usb/usbconfigurationextradescriptor.cpp \
    src/lib/usb/usbcontroltransferhandler.cpp \
    src/lib/usb/usbdevice.cpp \
    src/lib/usb/usbdevicecapabilitydescriptor.cpp \
    src/lib/usb/usbdevicedescriptor.cpp \
    src/lib/usb/usbdfudescriptor.cpp \
    src/lib/usb/usbendpointdescriptor.cpp \
    src/lib/usb/usbendpointreader.cpp \
    src/lib/usb/usbendpointwriter.cpp \
    src/lib/usb/usbhiddescriptor.cpp \
    src/lib/usb/usbhost.cpp \
    src/lib/usb/usbhubdescriptor.cpp \
    src/lib/usb/usbinterface.cpp \
    src/lib/usb/usbinterfaceassociationdescriptor.cpp \
    src/lib/usb/usbinterfacedescriptor.cpp \
    src/lib/usb/usbinterfaceextradescriptor.cpp \
    src/lib/usb/usbotgdescriptor.cpp \
    src/lib/usb/usbterminaltypes.cpp \
    src/main.cpp \
    src/mainwindow/mainwindow.cpp \
    src/mainwindow/usbdevicetreeview.cpp

TRANSLATIONS += \
    assets/translations/usb-regulus.en_US.ts \
    assets/translations/usb-regulus.zh_CN.ts

unix {
    !packagesExist(libusb-1.0):error("Could not find libusb-1.0 using pkg-config, please install libusb-1.0")
    !packagesExist(libudev):error("Could not find libudev using pkg-config, please install libudev")
    PKGCONFIG += libusb-1.0
    PKGCONFIG += libudev
    CONFIG += \
        link_pkgconfig \
        object_parallel_to_source

    INCLUDEPATH += reference/hidrd/include

    HEADERS += \
        reference/usbutils/names.h \
        reference/usbutils/usb-spec.h \
        reference/usbutils/sysfs.h

    SOURCES += \
        reference/usbutils/names.c \
        reference/usbutils/sysfs.c \
        reference/hidrd/lib/fmt/natv.c \
        reference/hidrd/lib/fmt/hex.c \
        reference/hidrd/lib/fmt/hex/src.c \
        reference/hidrd/lib/fmt/hex/snk.c \
        reference/hidrd/lib/fmt/spec/snk/item.c \
        reference/hidrd/lib/fmt/spec/snk/ent.c \
        reference/hidrd/lib/fmt/spec/snk/ent_list.c \
        reference/hidrd/lib/fmt/spec/snk/item_ent.c \
        reference/hidrd/lib/fmt/spec/snk.c \
        reference/hidrd/lib/fmt/natv/src.c \
        reference/hidrd/lib/fmt/natv/snk.c \
        reference/hidrd/lib/fmt/spec.c \
        reference/hidrd/lib/fmt/list.c \
        reference/hidrd/lib/fmt/inst.c \
        reference/hidrd/lib/usage/id.c \
        reference/hidrd/lib/usage/all.c \
        reference/hidrd/lib/usage/page_desc.c \
        reference/hidrd/lib/usage/type.c \
        reference/hidrd/lib/usage/id_desc.c \
        reference/hidrd/lib/usage/page.c \
        reference/hidrd/lib/usage/page_desc_list.c \
        reference/hidrd/lib/usage/id_desc_list.c \
        reference/hidrd/lib/item/main.c \
        reference/hidrd/lib/item/global.c \
        reference/hidrd/lib/item/any.c \
        reference/hidrd/lib/item/local.c \
        reference/hidrd/lib/item/short.c \
        reference/hidrd/lib/item/basic.c \
        reference/hidrd/lib/item/long.c \
        reference/hidrd/lib/item/collection.c \
        reference/hidrd/lib/item/pfx.c \
        reference/hidrd/lib/util/tkn.c \
        reference/hidrd/lib/util/hex.c \
        reference/hidrd/lib/util/ttbl.c \
        reference/hidrd/lib/util/fmt.c \
        reference/hidrd/lib/util/buf.c \
        reference/hidrd/lib/util/bool.c \
        reference/hidrd/lib/util/str.c \
        reference/hidrd/lib/util/unit.c \
        reference/hidrd/lib/util/dec.c \
        reference/hidrd/lib/util/char.c \
        reference/hidrd/lib/util/fd.c \
        reference/hidrd/lib/util/num.c \
        reference/hidrd/lib/opt/tkn.c \
        reference/hidrd/lib/opt/tkns_list.c \
        reference/hidrd/lib/opt/value_tkn.c \
        reference/hidrd/lib/opt/tkns.c \
        reference/hidrd/lib/opt/type.c \
        reference/hidrd/lib/opt/spec.c \
        reference/hidrd/lib/opt/name_tkn.c \
        reference/hidrd/lib/opt/list.c \
        reference/hidrd/lib/opt/inst.c \
        reference/hidrd/lib/opt/spec_list.c \
        reference/hidrd/lib/strm/src/type.c \
        reference/hidrd/lib/strm/src/inst.c \
        reference/hidrd/lib/strm/snk/type.c \
        reference/hidrd/lib/strm/snk/inst.c \
        reference/hidrd/src/hidrd_convert.c

    DEFINES += _GNU_SOURCE
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    LICENSE \
    README.md

RESOURCES += \
    assets/icon/icon.qrc \
    assets/qss/qss.qrc \
    assets/translations/translations.qrc
