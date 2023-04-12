#include "usbendpointreader.h"
#include "__usbmacro.h"

namespace usb {
    UsbEndpointReader::UsbEndpointReader(QObject *parent)
        : QObject{parent}, _device(nullptr), _endpointDescriptor(nullptr), _readBufferSize(0),
          _keepRead(false)
    {

    }

    UsbEndpointReader::~UsbEndpointReader()
    {
        if (_endpointDescriptor)
            _endpointDescriptor->disconnect(this);
    }

    void UsbEndpointReader::init(UsbEndpointDescriptor *epDesc)
    {
        _endpointDescriptor = epDesc;
        if (_endpointDescriptor->transferType() == EndpointTransferType::ISOCHRONOUS)
            _readBufferSize = libusb_get_max_iso_packet_size(
                        _endpointDescriptor->interfaceDescriptor()->interface()->
                        configurationDescriptor()->device()->device(),
                        _endpointDescriptor->bEndpointAddress());
        else
            _readBufferSize = _endpointDescriptor->wMaxPacketSize();
        if (_endpointDescriptor->direction() != EndpointDirection::IN)
        {
            LOGE(tr("Can only accept IN endpoint!"));
            _endpointDescriptor = nullptr;
        }
        _device = _endpointDescriptor->interfaceDescriptor()->interface()->configurationDescriptor()->device();
        connect(epDesc, &UsbEndpointDescriptor::asyncTransferCompleted,
                this, &UsbEndpointReader::transferCompleted);
        connect(epDesc, &UsbEndpointDescriptor::asyncTransferCancelled,
                this, &UsbEndpointReader::transferCancelled);
        connect(epDesc, &UsbEndpointDescriptor::asyncTransferFailed,
                this, &UsbEndpointReader::transferFailed);
    }

    const QByteArray &UsbEndpointReader::data() const
    {
        return _data;
    }

    void UsbEndpointReader::readOnce()
    {
        __startTransfer();
        std::lock_guard<QMutex> lock{_keepReadMutex};
        _keepRead = false;
    }

    void UsbEndpointReader::keepRead()
    {
        __startTransfer();
        std::lock_guard<QMutex> lock{_keepReadMutex};
        _keepRead = true;
    }

    void UsbEndpointReader::stopRead()
    {
        if (!_endpointDescriptor)
            return;

        _keepReadMutex.lock();
        _keepRead = false;
        _keepReadMutex.unlock();
        _endpointDescriptor->cancelAsyncTransfer();
    }

    void UsbEndpointReader::transferCompleted(const QByteArray &data)
    {
        _data = data;
        emit dataRead();

        std::lock_guard<QMutex> lock{_keepReadMutex};
        if (_keepRead)
            __startTransfer();
        else
            emit safelyStopped();
    }

    void UsbEndpointReader::transferFailed(int code)
    {
        emit readFailed(code);
        emit safelyStopped();
        std::lock_guard<QMutex> lock{_keepReadMutex};
        _keepRead = false;
    }

    void UsbEndpointReader::transferCancelled()
    {
        emit safelyStopped();
        std::lock_guard<QMutex> lock{_keepReadMutex};
        _keepRead = false;
    }

    void UsbEndpointReader::__startTransfer()
    {
        if (!_endpointDescriptor)
            return;

        int ret;
        ret = _endpointDescriptor->startAsyncTransfer(QByteArray(), _readBufferSize);
        if (ret < LIBUSB_SUCCESS)
        {
            LOGE(tr("Commit transfer failed (%1).").arg(usb_error_name(ret)));
            emit readFailed(ret);
            emit safelyStopped();
        }
    }

    int UsbEndpointReader::readBufferSize() const
    {
        return _readBufferSize;
    }

    void UsbEndpointReader::setReadBufferSize(int readBufferSize)
    {
        _readBufferSize = readBufferSize;
    }
}
