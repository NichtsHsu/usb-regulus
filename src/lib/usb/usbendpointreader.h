/*! The USB endpoint reader who plays worker role in Qt's thread model.

 * Copyright (C) 2022-2023 Nichts Hsu

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
     * @note
     * It will be better to set a timeout for read, so we can break any
     * read by set a stop flag. However, we don't regard this
     * timeout as a fault, just read again after timeout, as if nothing happened.
     */
    class UsbEndpointReader : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbEndpointReader(QObject *parent = nullptr);
        ~UsbEndpointReader();

        void init(UsbEndpointDescriptor *epDesc);

        /**
         * @brief data
         * @return the read data
         */
        const QByteArray &data() const;

        /**
         * @brief readBufferSize
         * @return the read buffer size, defaultly max packet size
         */
        int readBufferSize() const;

        /**
         * @brief setReadBufferSize
         * Set the read buffer size
         */
        void setReadBufferSize(int readBufferSize);

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

    private slots:
        void transferCompleted(const QByteArray &data);
        void transferFailed(int code);
        void transferCancelled();

    private:
        void inline __startTransfer();

        UsbDevice *_device;
        UsbEndpointDescriptor *_endpointDescriptor;
        QByteArray _data;
        int _readBufferSize;
        bool _keepRead;
        QMutex _keepReadMutex;
    };
}
#endif // USBENDPOINTREADER_H
