#ifndef USBCONTROLTRANSFERHANDLER_H
#define USBCONTROLTRANSFERHANDLER_H

#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QMutex>
#include "usbdevice.h"

namespace usb {
    /**
     * @brief The UsbControlTransferHandler class
     * The USB control transfer handler plays the worker role in Qt's thread model.
     * For an example:
     *      UsbControlTransferHandler *handler = new UsbControlTransferHandler;
     *      handler->moveToThread(&theWorkThread);
     * @note
     * It will be better to set a timeout for transfer, so we can break any
     * transfer by set a stop flag. However, we don't regard this
     * timeout as a fault, just transfer again after timeout, as if nothing happened.
     */
    class UsbControlTransferHandler : public QObject
    {
        Q_OBJECT
    public:
        explicit UsbControlTransferHandler(QObject *parent = nullptr);

        void init(UsbDevice *device);

        /**
         * @brief data
         * Get the data if direction is device to host.
         */
        const QByteArray &data() const;

        /**
         * @brief readBufferSize
         * @return the read buffer size, defaultly UsbDeviceDescriptor::bMaxPacketSize0()
         */
        uint16_t readBufferSize() const;

        /**
         * @brief setReadBufferSize
         * Set the read buffer size if direction is device to host.
         */
        void setReadBufferSize(uint16_t readBufferSize);

        /**
         * @brief setData
         * @param data
         * Set the data if direction is host to device.

         */
        void setData(const QByteArray &data);

        /**
         * @brief setBmRequest
         * Set bmRequest.
         */
        void setBmRequest(uint8_t bmRequest);

        /**
         * @brief setBRequest
         * Set bRequest.
         */
        void setBRequest(uint8_t bRequest);

        /**
         * @brief setWIndex
         * Set wIndex.
         */
        void setWIndex(uint16_t wIndex);

        /**
         * @brief setWValue
         * Set wValue.
         */
        void setWValue(uint16_t wValue);

        /**
         * @brief bmRequest
         * @return current bmRequest
         */
        uint8_t bmRequest() const;

        /**
         * @brief bRequest
         * @return current bRequest
         */
        uint8_t bRequest() const;

        /**
         * @brief wIndex
         * @return current wIndex
         */
        uint16_t wIndex() const;

        /**
         * @brief wValue
         * @return current wValue
         */
        uint16_t wValue() const;

    signals:
        /**
         * @brief finishedTransfer
         * Emit when transfer is finished.
         */
        void finishedTransfer();

        /**
         * @brief safelyStopped
         * Emit when the reader is safely stopped the transfer,
         * then you can start another reading.
         */
        void safelyStopped();

        /**
         * @brief transferFailed
         * Emit when the reader is failed to read data.
         * @param libusbCode
         * Libusb error code.
         */
        void transferFailed(int libusbCode);

    public slots:
        /**
         * @brief startTransfer
         * Start the control transfer.
         */
        void startTransfer();

        /**
         * @brief stopTransfer
         * Stop transfer.
         */
        void stopTransfer();

    private:
        UsbDevice *_device;
        uint8_t _bmRequest, _bRequest;
        uint16_t _wIndex, _wValue;
        QByteArray _data;
        uint16_t _readBufferSize;
        bool _stopFlag;
        QMutex _stopFlagMutex;
    };
}

#endif // USBCONTROLTRANSFERHANDLER_H
