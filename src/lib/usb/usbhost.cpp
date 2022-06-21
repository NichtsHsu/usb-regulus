#include "usbhost.h"
#include "usbhiddescriptor.h"
#include "__usbmacro.h"

namespace usb {
    using namespace __private;
    static std::mutex __mutW;

    UsbHost::UsbHost(QObject *parent):
        QObject{parent}, _context(nullptr), _protectMouse(true), _protectKeyboard(true)
    {
        __init();
    }

    UsbHost::~UsbHost()
    {
        if (_initialized)
        {
            _libusbEventHandler->stop();

            if (_hasHotplug)
                libusb_hotplug_deregister_callback(_context, _hotplugCbHandle);
#ifdef Q_OS_UNIX
            names_exit();
#endif
            libusb_exit(_context);
        }
    }

    void UsbHost::rescan()
    {
        int ret;
        if (!_initialized)
            __init();
        if (!_initialized)
            return;

        libusb_device **dev_list;
        if ((ret = libusb_get_device_list(_context, &dev_list)) < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to get device list (%1).").arg(usb_error_name(ret)));
            return;
        }

        foreach(UsbDevice * const device, _usbDevices)
            delete device;
        _usbDevices.clear();

        libusb_device *device = nullptr;
        int i = 0;
        while ((device = dev_list[i++]) != nullptr)
        {
            UsbDevice *usbDevice = new UsbDevice(device, this);
            if (usbDevice->valid())
            {
                _usbDevices.append(usbDevice);
                LOGI(tr("New USB device \"%1\" attached.").arg(usbDevice->displayName()));
            }
            else
                delete usbDevice;
        }

        __sortDeviceList();
        emit deviceListRefreshed();

        libusb_free_device_list(dev_list, false);
    }

    void UsbHost::__init()
    {
        int ret;
        if ((ret = libusb_init(&_context)) < LIBUSB_SUCCESS)
        {
            LOGE(tr("Failed to initialize libusb (%1).").arg(usb_error_name(ret)));
            _initialized = false;
            _hasHotplug = false;
            return;
        }

        _libusbEventHandler = new UsbEventHandler;
        _libusbEventThread = new QThread;
        _libusbEventHandler->moveToThread(_libusbEventThread);
        connect(_libusbEventThread, &QThread::started,
                _libusbEventHandler, &UsbEventHandler::run);
        connect(_libusbEventHandler, &UsbEventHandler::finished,
                _libusbEventHandler, &UsbEventHandler::deleteLater);
        connect(_libusbEventHandler, &UsbEventHandler::finished,
                _libusbEventThread, &QThread::quit);
        connect(_libusbEventThread, &QThread::finished,
                _libusbEventThread, &QThread::deleteLater);
        _libusbEventThread->start();

        if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG))
        {
            LOGW(tr("Libusb has no hotplug capability in your system, usb-regulus can not automatically refresh the device list."));
            _hasHotplug = false;
        }
        else
        {
            /* register the hotplug callback function */
            ret = libusb_hotplug_register_callback(_context,
                                                   libusb_hotplug_event(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                                                        LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
                                                   LIBUSB_HOTPLUG_NO_FLAGS,
                                                   LIBUSB_HOTPLUG_MATCH_ANY,
                                                   LIBUSB_HOTPLUG_MATCH_ANY,
                                                   LIBUSB_HOTPLUG_MATCH_ANY,
                                                   reinterpret_cast<libusb_hotplug_callback_fn>(UsbHost::__hotplug_callback),
                                                   reinterpret_cast<void *>(this),
                                                   &_hotplugCbHandle);
            if (ret < LIBUSB_SUCCESS)
            {
                LOGW(tr("Failed to register hotplug callback function, "
                        "usb-regulus can not automatically refresh the device list."));
                _hasHotplug = false;
            }
        }

#ifdef Q_OS_UNIX
        if (names_init() < 0)
            LOGW(tr("Unable to initialize USB name database, you may get UNKNOWN as the device name."));
#endif

        _initialized = true;
        _hasHotplug = true;
    }

    void UsbHost::__sortDeviceList()
    {
        std::sort(_usbDevices.begin(), _usbDevices.end(), [](UsbDevice * const &l, UsbDevice * const &r){
            if (l->bus() == r->bus())
                return l->port() < r->port();
            else return l->bus() < r->bus();
        });
    }

    int UsbHost::__indexOfDevice(const UsbDevice &device) const
    {
        for(int i = 0; i < _usbDevices.length(); ++i)
            if (_usbDevices[i]->operator==(device))
                return i;
        return -1;
    }

    int UsbHost::__indexOfDevice(libusb_device *device)
    {
        for(int i = 0; i < _usbDevices.length(); ++i)
            if (_usbDevices[i]->operator==(device))
                return i;
        return -1;
    }

    int UsbHost::__hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
    {
        Q_UNUSED(ctx);
        UsbHost *usbHost = reinterpret_cast<UsbHost*>(user_data);
        if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
            /** Cannot create children for a parent that is in a different thread in Qt.
             * We use QTimer::singleShot to let the slot function run in main thread.
             */
            QTimer::singleShot(0, usbHost, [usbHost, dev] () {
                usbHost->__addDevice(dev);
            });
        else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
            /** Cannot create children for a parent that is in a different thread in Qt.
             * We use QTimer::singleShot to let the slot function run in main thread.
             */
            QTimer::singleShot(0, usbHost, [usbHost, dev] () {
                usbHost->__removeDevice(dev);
            });
        else
            log().w("UsbHost", UsbHost::tr("Unhandled hotplug event %1.").arg(event));

        return 0;
    }

    bool UsbHost::protectKeyboard() const
    {
        return _protectKeyboard;
    }

    void UsbHost::setProtectKeyboard(bool protect)
    {
        _protectKeyboard = protect;

        if (!_protectKeyboard)
            for (qsizetype i = 0; i < _usbDevices.length(); ++i)
            {
                UsbDevice *device = _usbDevices[i];
                for (uint8_t j = 0; j < device->configurationDescriptor()->bNumInterfaces(); ++j)
                {
                    UsbInterfaceDescriptor *interfaceDesc = device->configurationDescriptor()->interface(j)->currentInterfaceDescriptor();
                    UsbHidDescriptor *hidDescriptor;
                    if (interfaceDesc->isKeyboard() && (hidDescriptor = interfaceDesc->hidDescriptor()) != nullptr)
                    {
                        int ret = hidDescriptor->tryGetHidReportDescriptor();
                        if (ret < LIBUSB_SUCCESS)
                            LOGE(tr("Failed to update HID Report Descriptor for interface \"%1\" of device \"%2\".")
                                 .arg(interfaceDesc->interface()->displayName())
                                 .arg(device->displayName()));
                    }
                }
            }

        LOGD(tr("Set 'Protect Keyboard' to %1.").arg(_protectKeyboard));
    }

    bool UsbHost::protectMouse() const
    {
        return _protectMouse;
    }

    void UsbHost::setProtectMouse(bool protect)
    {
        _protectMouse = protect;

        if (!_protectMouse)
            for (qsizetype i = 0; i < _usbDevices.length(); ++i)
            {
                UsbDevice *device = _usbDevices[i];
                for (uint8_t j = 0; j < device->configurationDescriptor()->bNumInterfaces(); ++j)
                {
                    UsbInterfaceDescriptor *interfaceDesc = device->configurationDescriptor()->interface(j)->currentInterfaceDescriptor();
                    UsbHidDescriptor *hidDescriptor;
                    if (interfaceDesc->isMouse() && (hidDescriptor = interfaceDesc->hidDescriptor()) != nullptr)
                    {
                        int ret = hidDescriptor->tryGetHidReportDescriptor();
                        if (ret < LIBUSB_SUCCESS)
                            LOGE(tr("Failed to update HID Report Descriptor for interface \"%1\" of device \"%2\".")
                                 .arg(interfaceDesc->interface()->displayName())
                                 .arg(device->displayName()));
                    }
                }
            }

        LOGD(tr("Set 'Protect Mouse' to %1.").arg(_protectKeyboard));
    }

    libusb_context *UsbHost::context() const
    {
        return _context;
    }

    bool UsbHost::hasHotplug() const
    {
        return _hasHotplug;
    }

    void UsbHost::__addDevice(libusb_device *device)
    {
        if (!_hasHotplug)
            return;

        std::lock_guard<std::mutex> lock{ __mutW };
        LOGD(tr("Hotplug event: attached."));
        UsbDevice *usbDevice = new UsbDevice(device, this);

        if (!usbDevice->valid())
        {
            LOGW(tr("Ignored attached event with an invalid device."));
            return;
        }

        _usbDevices.append(usbDevice);
        __sortDeviceList();
        LOGI(tr("New USB device \"%1\" attached.").arg(usbDevice->displayName()));
        emit deviceAttached(usbDevice, __indexOfDevice(*usbDevice));
    }

    void UsbHost::__removeDevice(libusb_device *device)
    {
        if (!_hasHotplug)
            return;

        std::lock_guard<std::mutex> lock{ __mutW };
        LOGD(tr("Hotplug event: detached."));
        int index = __indexOfDevice(device);
        if (index == -1)
        {
            LOGW(tr("Ignore the detached event because we didn't find any device match it."));
            return;
        }

        UsbDevice *usbDevice = _usbDevices[index];
        emit deviceDetached(_usbDevices[index], index);
        LOGI(tr("USB device \"%1\" detached.").arg(_usbDevices[index]->displayName()));
        _usbDevices.remove(index);

        // Delay to delete
        QTimer::singleShot(10, usbDevice, [usbDevice] () {
            usbDevice->deleteLater();
        });
    }

    bool UsbHost::initialized() const
    {
        return _initialized;
    }

    std::atomic<UsbHost *> UsbHost::_instance{nullptr};

    UsbHost *UsbHost::instance()
    {
        if (UsbHost::_instance == nullptr)
        {
            std::lock_guard<std::mutex> lock{ __mutW };
            if (UsbHost::_instance == nullptr)
                UsbHost::_instance = new UsbHost;
        }
        return UsbHost::_instance;
    }

    const QVector<UsbDevice *> &UsbHost::usbDevices() const
    {
        return _usbDevices;
    }

    __private::UsbEventHandler::UsbEventHandler(QObject *parent):
        QObject(parent), _stopFlag(false)
    {

    }

    void __private::UsbEventHandler::run()
    {
        _stopFlagMutex.lock();
        _stopFlag = false;
        _stopFlagMutex.unlock();

        for(;;)
        {
            timeval tv = {0, 200};
            int ret = libusb_handle_events_timeout(UsbHost::instance()->_context, &tv);
            if (ret < 0)
                LOGE(tr("Failed or timout to handle hotplug event."));

            _stopFlagMutex.lock();
            if(_stopFlag)
            {
                _stopFlag = false;
                _stopFlagMutex.unlock();
                break;
            }
            _stopFlagMutex.unlock();
        }

        emit finished();
    }

    void __private::UsbEventHandler::stop()
    {
        _stopFlagMutex.lock();
        _stopFlag = true;
        _stopFlagMutex.unlock();
    }

}
