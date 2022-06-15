/*! C++ class wrapper of libusb_interface

 * Copyright (C) 2022 Nichts Hsu

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <libusb.h>
#include <atomic>
#include <mutex>
#include <QObject>
#include <QVector>

#ifdef Q_OS_UNIX
#include <usbutils/names.h>
#endif

namespace usb {
    class UsbInterfaceDescriptor;
    class UsbConfigurationDescriptor;
}

#include "log/logger.h"

namespace usb {

    /**
     * @brief The UsbInterface class
     * C++ class wrapper of libusb_interface
     */
    class UsbInterface : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbInterface(const libusb_interface *interface, UsbConfigurationDescriptor *parent);

        /**
         * @brief num_altsetting
         * @return the number of alternate settings that belong to this interface
         */
        int numAltsetting() const;

        /**
         * @brief altsetting
         * @param index
         * Usually you are expected to use index 0,
         * unless you have manually set the alternate setting.
         * @return get the specified alternate setting
         */
        UsbInterfaceDescriptor *altsetting(int index) const;

        /**
         * @brief displayName
         * @return the name displayed in device tree view.
         * Prefer the iInterface; if iIterface is 0, use protocol;
         * if protocol value can not be translated to string,
         * use class+subclass as the display name.
         */
        const QString &displayName();

        /**
         * @brief configurationDescriptor
         * @return get the configuration descriptor which this interface is belong to
         */
        UsbConfigurationDescriptor *configurationDescriptor() const;

        /**
         * @brief currentInterfaceDescriptor
         * @return Ucurrent altsetting
         */
        UsbInterfaceDescriptor *currentInterfaceDescriptor() const;

        /**
         * @brief infomationToHtml
         * @return HTML form interface informations
         */
        QString infomationToHtml() const;

        /**
         * @brief currentAltsetting
         * @return current altsetting
         */
        int currentAltsetting() const;

        /**
         * @brief claim
         * Claim the interface.
         * Must claim the interface before transfer data via its endpoints.
         * @return libusb error code
         */
        int claim();

        /**
         * @brief release
         * Release the interface
         * Should release the interface if it is no longer used
         */
        void release();

        /**
         * @brief setAltsetting
         * @param altsetting
         * Index of altsetting
         * @return error code
         */
        int setAltsetting(int altsetting);

    signals:

    private:
        QVector<UsbInterfaceDescriptor *> _altsetting;
        int _numAltsetting;
        UsbConfigurationDescriptor *_configurationDescriptor;

        int _currentAltsetting;
        QString _displayName;
        std::atomic<size_t> _claimCount;
    };
}

#ifndef USBCONFIGURATIONDESCRIPTOR_H
#include "usbconfigurationdescriptor.h"
#endif
#ifndef USBINTERFACEDESCRIPTOR_H
#include "usbinterfacedescriptor.h"
#endif

#endif // USBINTERFACE_H
