#include "usbendpointreader.h"

namespace usb {
    const unsigned int UsbEndpointReader::_internalTimeout = 250;

    UsbEndpointReader::UsbEndpointReader(QObject *parent)
        : QObject{parent}, _device(nullptr), _endpointDescriptor(nullptr),
          _stopFlag(false)
    {

    }

    void UsbEndpointReader::init(UsbEndpointDescriptor *epDesc)
    {
        _endpointDescriptor = epDesc;
        if (_endpointDescriptor->direction() != EndpointDirection::EP_IN)
        {
            LOGE(tr("Can only accept IN endpoint!"));
            _endpointDescriptor = nullptr;
        }
        _device = _endpointDescriptor->interfaceDescriptor()->interface()->configDescriptor()->device();
    }

    const QByteArray &UsbEndpointReader::data() const
    {
        return _data;
    }

    void UsbEndpointReader::readOnce()
    {
        if (!_endpointDescriptor)
            return;
        _stopFlagMutex.lock();
        _stopFlag = false;
        _stopFlagMutex.unlock();

        _data.fill('\0', _endpointDescriptor->wMaxPacketSize());
        int realReadSize = 0;
        int ret;
        for(;;)
        {
            _stopFlagMutex.lock();
            if(_stopFlag)
            {
                _stopFlag = false;
                _stopFlagMutex.unlock();
                break;
            }
            _stopFlagMutex.unlock();

            ret = _endpointDescriptor->transfer(_data, realReadSize, _internalTimeout);
            if (ret >= LIBUSB_SUCCESS)
            {
                emit dataRead();
                break;
            }
            else if (ret == LIBUSB_ERROR_TIMEOUT)
            {
                /* Ignore timeout */
            }
            else
            {
                LOGE(tr("Data read failed (%1).").arg(libusb_error_name(ret)));
                emit readFailed(ret);
                break;
            }
        }

        emit safelyStopped();
    }

    void UsbEndpointReader::keepRead()
    {
        if (!_endpointDescriptor)
            return;
        _stopFlagMutex.lock();
        _stopFlag = false;
        _stopFlagMutex.unlock();

        _data.fill('\0', _endpointDescriptor->wMaxPacketSize());
        int realReadSize = 0;
        int ret;
        for(;;)
        {
            _stopFlagMutex.lock();
            if(_stopFlag)
            {
                _stopFlag = false;
                _stopFlagMutex.unlock();
                break;
            }
            _stopFlagMutex.unlock();

            ret = _endpointDescriptor->transfer(_data, realReadSize, _internalTimeout);
            if (ret >= LIBUSB_SUCCESS)
            {
                emit dataRead();
            }
            else if (ret == LIBUSB_ERROR_TIMEOUT)
            {
                /* Ignore timeout */
            }
            else
            {
                LOGE(tr("Data read failed (%1).").arg(libusb_error_name(ret)));
                emit readFailed(ret);
                break;
            }
        }

        emit safelyStopped();
    }

    void UsbEndpointReader::stopRead()
    {
        _stopFlagMutex.lock();
        _stopFlag = true;
        _stopFlagMutex.unlock();
    }
}
