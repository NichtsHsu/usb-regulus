﻿/*! The USB endpoint reader who plays worker role in Qt's thread model.

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

#ifndef USBENDPOINTREADER_H
#define USBENDPOINTREADER_H

#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QMutex>
#include "usbendpointdescriptor.h"
#include "usbdevice.h"

namespace usb {
    /**
     * @brief The UsbEndpointReader class
     * The reader plays the worker role in Qt's thread model.
     * For an example:
     *      UsbEndpointReader *reader = new UsbEndpointReader;
     *      reader->moveToThread(&theWorkThread);
     */
    class UsbEndpointReader : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbEndpointReader(QObject *parent = nullptr);

        void init(UsbEndpointDescriptor *epDesc);

        const QByteArray &data() const;

    signals:
        /**
         * @brief dataRead
         * Emit when data was read.
         * @note
         * It is not good to directly send the read data from signal,
         * because the signal-slot cross thread is queued connection.
         * Instead, we read the data by calling data() in the slot function.
         * @see UsbEndpointReader::data()
         */
        void dataRead();

        /**
         * @brief safelyStopped
         * Emit when the reader is safely stopped the reading,
         * then you can start another reading.
         */
        void safelyStopped();

        /**
         * @brief transfetFailed
         * Emit when the reader is failed to read data.
         * @param libusbCode
         * Libusb error code.
         */
        void readFailed(int libusbCode);

    public slots:
        /**
         * @brief readOnce
         * Read data once.
         */
        void readOnce();

        /**
         * @brief keepRead
         * Keep reading data in a infinite loop.
         */
        void keepRead();

        /**
         * @brief stopRead
         * Stop read, no matter read once or keep read.
         */
        void stopRead();

    private:
        UsbDevice *_device;
        UsbEndpointDescriptor *_endpointDescriptor;

        QByteArray _data;
        bool _stopFlag;
        QMutex _stopFlagMutex;

        /**
         * @brief _internalTimeout
         * It will be better to set a timeout for read/write, so we can break any
         * read/write by QThread::requestInterruption(). However, we don't regard this
         * timeout as a fault, just read again after timeout, as if nothing happened.
         */
        static const unsigned int _internalTimeout;
    };
}
#endif // USBENDPOINTREADER_H