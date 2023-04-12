#include "usbcontroltransferhandler.h"
#include "__usbmacro.h"

namespace usb {
    UsbControlTransferHandler::UsbControlTransferHandler(QObject *parent)
        : QObject{parent}, _stopFlag(false)
    {

    }

    void UsbControlTransferHandler::init(UsbDevice *device)
    {
        _device = device;
        _readBufferSize = _device->deviceDescriptor()->bMaxPacketSize0();
    }

    const QByteArray &UsbControlTransferHandler::data() const
    {
        return _data;
    }

    void UsbControlTransferHandler::setData(const QByteArray &data)
    {
        _data = data;
    }

    void UsbControlTransferHandler::setBmRequest(uint8_t bmRequest)
    {
        _bmRequest = bmRequest;
    }

    void UsbControlTransferHandler::setBRequest(uint8_t bRequest)
    {
        _bRequest = bRequest;
    }

    void UsbControlTransferHandler::setWIndex(uint16_t wIndex)
    {
        _wIndex = wIndex;
    }

    void UsbControlTransferHandler::setWValue(uint16_t wValue)
    {
        _wValue = wValue;
    }

    void UsbControlTransferHandler::startTransfer()
    {
        if (!_device)
            return;
        _stopFlagMutex.lock();
        _stopFlag = false;
        _stopFlagMutex.unlock();

        if (_bmRequest & 0x80)
            _data.fill('\0', _readBufferSize);
        if (CUT(_bmRequest, 0, 4) == 1)
            _device->claimInterface(uint8_t(_wIndex));

        int ret;
        for (;;)
        {
            ret = _device->controlTransfer(_bmRequest,
                                           _bRequest,
                                           _wValue,
                                           _wIndex,
                                           _data,
                                           TRANSFER_TIMEOUT);

            _stopFlagMutex.lock();
            if(_stopFlag)
            {
                _stopFlag = false;
                _stopFlagMutex.unlock();
                break;
            }
            _stopFlagMutex.unlock();

            if (ret >= LIBUSB_SUCCESS)
            {
                emit finishedTransfer();
                break;
            }
            else if (ret == LIBUSB_ERROR_TIMEOUT)
            {
                /* Ignore timeout */
            }
            else
            {
                LOGE(tr("Data read failed (%1).").arg(usb_error_name(ret)));
                emit transferFailed(ret);
                break;
            }
        }

        if (CUT(_bmRequest, 0, 4) == 1)
            _device->releaseInterface(uint8_t(_wIndex));
        emit safelyStopped();
    }

    void UsbControlTransferHandler::stopTransfer()
    {
        std::lock_guard<QMutex> lock{_stopFlagMutex};
        _stopFlag = true;
    }

    uint16_t UsbControlTransferHandler::wValue() const
    {
        return _wValue;
    }

    uint16_t UsbControlTransferHandler::wIndex() const
    {
        return _wIndex;
    }

    uint8_t UsbControlTransferHandler::bRequest() const
    {
        return _bRequest;
    }

    uint8_t UsbControlTransferHandler::bmRequest() const
    {
        return _bmRequest;
    }

    uint16_t UsbControlTransferHandler::readBufferSize() const
    {
        return _readBufferSize;
    }

    void UsbControlTransferHandler::setReadBufferSize(uint16_t readBufferSize)
    {
        _readBufferSize = readBufferSize;
    }
}
