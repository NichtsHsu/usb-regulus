#include "usbendpointwriter.h"
#include "__usbmacro.h"

namespace usb{
    UsbEndpointWriter::UsbEndpointWriter(QObject *parent)
        : QObject{parent}, _device(nullptr), _endpointDescriptor(nullptr),
          _keepWrite(false), _wroteTimes(0)
    {

    }

    UsbEndpointWriter::~UsbEndpointWriter()
    {
        if (_endpointDescriptor)
            _endpointDescriptor->disconnect(this);
    }

    void UsbEndpointWriter::init(UsbEndpointDescriptor *epDesc)
    {
        _endpointDescriptor = epDesc;
        if (_endpointDescriptor->direction() != EndpointDirection::OUT)
        {
            LOGE(tr("Can only accept OUT endpoint!"));
            _endpointDescriptor = nullptr;
        }
        _device = _endpointDescriptor->interfaceDescriptor()->interface()->configurationDescriptor()->device();
        connect(epDesc, &UsbEndpointDescriptor::asyncTransferCompleted,
                this, &UsbEndpointWriter::transferCompleted);
        connect(epDesc, &UsbEndpointDescriptor::asyncTransferCancelled,
                this, &UsbEndpointWriter::transferCancelled);
        connect(epDesc, &UsbEndpointDescriptor::asyncTransferFailed,
                this, &UsbEndpointWriter::transferFailed);
    }

    void UsbEndpointWriter::writeOnce()
    {
        __startTransfer();
        _keepWriteMutex.lock();
        _keepWrite = false;
        _keepWriteMutex.unlock();
    }

    void UsbEndpointWriter::keepWrite()
    {
        __startTransfer();
        _keepWriteMutex.lock();
        _keepWrite = true;
        _keepWriteMutex.unlock();
    }

    void UsbEndpointWriter::stopWrite()
    {
        if (!_endpointDescriptor)
            return;

        _keepWriteMutex.lock();
        _keepWrite = false;
        _keepWriteMutex.unlock();
        _endpointDescriptor->cancelAsyncTransfer();
    }

    void UsbEndpointWriter::clearWroteTimes()
    {
        _wroteTimesMutex.lock();
        _wroteTimes = 0;
        _wroteTimesMutex.unlock();
    }

    void UsbEndpointWriter::transferCompleted()
    {
        _wroteTimesMutex.lock();
        _wroteTimes++;
        _wroteTimesMutex.unlock();
        emit writeSucceed(_wroteTimes);

        _keepWriteMutex.lock();
        if (_keepWrite)
            __startTransfer();
        else
            emit safelyStopped();
        _keepWriteMutex.unlock();
    }

    void UsbEndpointWriter::transferFailed(int code)
    {
        emit writeFailed(code);
        emit safelyStopped();
        _keepWriteMutex.lock();
        _keepWrite = false;
        _keepWriteMutex.unlock();
    }

    void UsbEndpointWriter::transferCancelled()
    {
        emit safelyStopped();
        _keepWriteMutex.lock();
        _keepWrite = false;
        _keepWriteMutex.unlock();
    }

    void UsbEndpointWriter::__startTransfer()
    {
        if (!_endpointDescriptor)
            return;

        int ret;
        ret = _endpointDescriptor->startAsyncTransfer(_data);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Commit transfer failed (%1).").arg(usb_error_name(ret)));
            emit writeFailed(ret);
            emit safelyStopped();
        }
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
