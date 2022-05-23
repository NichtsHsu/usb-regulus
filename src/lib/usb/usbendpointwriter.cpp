#include "usbendpointwriter.h"

namespace usb{
    const unsigned int UsbEndpointWriter::_internalTimeout = 250;

    UsbEndpointWriter::UsbEndpointWriter(QObject *parent)
        : QObject{parent}, _device(nullptr), _endpointDescriptor(nullptr),
          _stopFlag(false), _wroteTimes(0)
    {

    }

    void UsbEndpointWriter::init(UsbEndpointDescriptor *epDesc)
    {
        _endpointDescriptor = epDesc;
        if (_endpointDescriptor->direction() != EndpointDirection::OUT)
        {
            LOGE(tr("Can only accept OUT endpoint!"));
            _endpointDescriptor = nullptr;
        }
        _device = _endpointDescriptor->interfaceDescriptor()->interface()->configDescriptor()->device();
        _data.fill('\0', _endpointDescriptor->wMaxPacketSize());
    }

    void UsbEndpointWriter::writeOnce()
    {
        if (!_endpointDescriptor)
            return;
        _stopFlagMutex.lock();
        _stopFlag = false;
        _stopFlagMutex.unlock();

        int realWriteSize = 0;
        int ret;
        for(;;)
        {
            ret = _endpointDescriptor->transfer(_data, realWriteSize, _internalTimeout);

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
                ++_wroteTimes;
                emit writeSucceed(_wroteTimes);
                break;
            }
            else if (ret == LIBUSB_ERROR_TIMEOUT)
            {
                /* Ignore timeout */
            }
            else
            {
                LOGE(tr("Data write failed (%1).").arg(libusb_error_name(ret)));
                emit writeFailed(ret);
                break;
            }
        }

        emit safelyStopped();
    }

    void UsbEndpointWriter::keepWrite()
    {
        if (!_endpointDescriptor)
            return;
        _stopFlagMutex.lock();
        _stopFlag = false;
        _stopFlagMutex.unlock();

        int realWriteSize = 0;
        int ret;
        for(;;)
        {
            ret = _endpointDescriptor->transfer(_data, realWriteSize, _internalTimeout);

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
                _wroteTimesMutex.lock();
                ++_wroteTimes;
                emit writeSucceed(_wroteTimes);
                _wroteTimesMutex.unlock();
            }
            else if (ret == LIBUSB_ERROR_TIMEOUT)
            {
                /* Ignore timeout */
            }
            else
            {
                LOGE(tr("Data write failed (%1).").arg(libusb_error_name(ret)));
                emit writeFailed(ret);
                break;
            }
        }

        emit safelyStopped();
    }

    void UsbEndpointWriter::stopWrite()
    {
        _stopFlagMutex.lock();
        _stopFlag = true;
        _stopFlagMutex.unlock();
    }

    void UsbEndpointWriter::clearWroteTimes()
    {
        _wroteTimesMutex.lock();
        _wroteTimes = 0;
        _wroteTimesMutex.unlock();
    }

    size_t UsbEndpointWriter::wroteTimes() const
    {
        return _wroteTimes;
    }

    void UsbEndpointWriter::setData(const QByteArray &data)
    {
        _data = data;
    }
}
