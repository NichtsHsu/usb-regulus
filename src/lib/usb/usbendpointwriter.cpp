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
        if (_endpointDescriptor->direction() != EndpointDirection::EP_OUT)
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
            _stopFlagMutex.lock();
            if(_stopFlag)
            {
                _stopFlag = false;
                _stopFlagMutex.unlock();
                break;
            }
            _stopFlagMutex.unlock();

            ret = _endpointDescriptor->transfer(reinterpret_cast<unsigned char *>(_data.data()),
                                                realWriteSize, _internalTimeout);
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
                LOGE(tr("Data read failed (%1).").arg(libusb_error_name(ret)));
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
            _stopFlagMutex.lock();
            if(_stopFlag)
            {
                _stopFlag = false;
                _stopFlagMutex.unlock();
                break;
            }
            _stopFlagMutex.unlock();

            ret = _endpointDescriptor->transfer(reinterpret_cast<unsigned char *>(_data.data()),
                                                realWriteSize, _internalTimeout);
            if (ret >= LIBUSB_SUCCESS)
            {
                ++_wroteTimes;
                emit writeSucceed(_wroteTimes);
            }
            else if (ret == LIBUSB_ERROR_TIMEOUT)
            {
                /* Ignore timeout */
            }
            else
            {
                LOGE(tr("Data read failed (%1).").arg(libusb_error_name(ret)));
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
        _wroteTimes = 0;
    }

    void UsbEndpointWriter::setData(const QByteArray &data)
    {
        _data = data;
        if (_data.length() < _endpointDescriptor->wMaxPacketSize())
        {
            LOGW(tr("The length of data(%1) written to a OUT endpoint is less than its max packet size(%2)."
                    "Fill '\0' to the end of data.")
                 .arg(_data.length())
                 .arg(_endpointDescriptor->wMaxPacketSize()));
            _data.append(_endpointDescriptor->wMaxPacketSize() - _data.length(), '\0');
        }
        else if(_data.length() > _endpointDescriptor->wMaxPacketSize())
        {
            LOGW(tr("The length of data(%1) written to a OUT endpoint is greater than its max packet size(%2)."
                    "Truncatethe overflow part of data.")
                 .arg(_data.length())
                 .arg(_endpointDescriptor->wMaxPacketSize()));
            _data.resize(_endpointDescriptor->wMaxPacketSize());
        }
    }
}
