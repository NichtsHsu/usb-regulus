/*! The USB endpoint writer who plays worker role in Qt's thread model.

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

#ifndef USBENDPOINTWRITER_H
#define USBENDPOINTWRITER_H

#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QMutex>
#include "usbendpointdescriptor.h"
#include "usbdevice.h"

namespace usb  {
    class UsbEndpointWriter : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbEndpointWriter(QObject *parent = nullptr);

        void init(UsbEndpointDescriptor *epDesc);

        void setData(const QByteArray &data);

    signals:
        /**
         * @brief writeSucceed
         * Emit when successfully wrote the data.
         * @param wroteCount
         * The total times of data wrote.
         */
        void writeSucceed(size_t wroteTimes);

        /**
         * @brief safelyStopped
         * Emit when the write is safely stopped the writing,
         * then you can start another writing.
         */
        void safelyStopped();

        /**
         * @brief writeFailed
         * Emit when the writer is failed to write data.
         * @param libusbCode
         * Libusb error code.
         */
        void writeFailed(int libusbCode);

    public slots:
        /**
         * @brief writeOnce
         * Write data once.
         */
        void writeOnce();

        /**
         * @brief keepWrite
         * Keep writing data in a infinite loop
         */
        void keepWrite();

        /**
         * @brief stopWrite
         * Stop write, no matter write once or keep write.
         */
        void stopWrite();

        /**
         * @brief clearWroteTimes
         * Set the wrote times to 0.
         */
        void clearWroteTimes();

    private:
        UsbDevice *_device;
        UsbEndpointDescriptor *_endpointDescriptor;

        QByteArray _data;

        bool _stopFlag;
        QMutex _stopFlagMutex;

        size_t _wroteTimes;

        /**
         * @brief _internalTimeout
         * It will be better to set a timeout for read/write, so we can break any
         * read/write by QThread::requestInterruption(). However, we don't regard this
         * timeout as a fault, just read again after timeout, as if nothing happened.
         */
        static const unsigned int _internalTimeout;
    };
}

#endif // USBENDPOINTWRITER_H
