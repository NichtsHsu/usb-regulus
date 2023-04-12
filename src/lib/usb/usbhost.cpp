#include "usbhost.h"
#include "usbhiddescriptor.h"
#include "__usbmacro.h"

namespace usb {
    using namespace __private;
    Q_GLOBAL_STATIC(QMutex, __mutW);

    UsbHost::UsbHost(QObject *parent):
        QObject{parent}, _context(nullptr), _protectMouse(true), _protectKeyboard(true), _rescanTimer(nullptr)
    {
        __init();
    }

    UsbHost::~UsbHost()
    {
        if (_initialized)
        {
            _libusbEventHandler->stop();
            _libusbEventThread->exit();

            if (_rescanTimer)
            {
                _rescanTimer->stop();
                _rescanTimer->deleteLater();
            }
            _rescaner->disconnect(this);
            _rescanThread->exit();

            _libusbEventHandler->deleteLater();
            _libusbEventThread->deleteLater();
            _rescaner->deleteLater();
            _rescanThread->deleteLater();

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
        QTimer::singleShot(0, _rescaner, &UsbDeviceRescanWorker::run);
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
        LOGD(tr("Libusb initialized."));

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

        _rescaner = new UsbDeviceRescanWorker;
        _rescanThread = new QThread;
        _rescaner->moveToThread(_rescanThread);
        connect(_rescaner, &UsbDeviceRescanWorker::finished,
                this, &UsbHost::deviceListRefreshed);
        connect(_rescanThread, &QThread::finished,
                _rescanThread, &QThread::deleteLater);
        _rescanThread->start();

        if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG))
        {
            LOGW(tr("Libusb has no hotplug capability on your OS."));
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
                LOGW(tr("Failed to register hotplug callback function."));
                _hasHotplug = false;
            }
            else
                _hasHotplug = true;
        }

        if (!_hasHotplug)
        {
            LOGW(tr("Since usb-regulus cannot register the hotplug callback function, "
                    "so usb-regulus will start a timer every 100ms to poll the devices list "
                    "and check if any device was plugged or unplugged"));
            _rescaner->setUpdateMode(true);
            _rescanTimer = new QTimer(this);
            connect(_rescanTimer, &QTimer::timeout, _rescaner, &UsbDeviceRescanWorker::run);
            _rescanTimer->start(100);
        }

#ifdef Q_OS_UNIX
        if (names_init() < 0)
            LOGW(tr("Unable to initialize USB name database, you may get UNKNOWN as the device name."));
#endif

        LOGD(tr("Initialization finished."));
        _initialized = true;
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
        log().d("usb::UsbHost", UsbHost::tr("Hotplug callback: event code is %1.").arg(event));
        UsbHost *usbHost = reinterpret_cast<UsbHost*>(user_data);
        if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
            /** Cannot create children for a parent that is in a different thread in Qt.
             * We use QTimer::singleShot to let the slot function run in the main thread.
             */
            QTimer::singleShot(0, usbHost, [usbHost, dev] () {
                usbHost->__addDevice(dev);
            });
        else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
            /** Cannot create children for a parent that is in a different thread in Qt.
             * We use QTimer::singleShot to let the slot function run in the main thread.
             */
            QTimer::singleShot(0, usbHost, [usbHost, dev] () {
                usbHost->__removeDevice(dev);
            });
        else
            log().w("usb::UsbHost", UsbHost::tr("Unhandled hotplug event %1.").arg(event));

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
                                 .arg(interfaceDesc->interface()->displayName(), device->displayName()));
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
                                 .arg(interfaceDesc->interface()->displayName(), device->displayName()));
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
        std::lock_guard<QMutex> lock{ *__mutW };
        LOGD(tr("Hotplug event: attached."));
        UsbDevice *usbDevice = new UsbDevice(device, this);

        if (!usbDevice->valid())
        {
            LOGW(tr("Ignored attached event with an invalid device."));
            delete usbDevice;
            return;
        }

        _usbDevices.append(usbDevice);
        __sortDeviceList();
        LOGI(tr("New USB device \"%1\" attached.").arg(usbDevice->displayName()));
        emit deviceAttached(usbDevice, __indexOfDevice(*usbDevice));
    }

    void UsbHost::__removeDevice(libusb_device *device)
    {
        std::lock_guard<QMutex> lock{ *__mutW };
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

    void UsbHost::__removeDevice(const UsbDevice &device)
    {
        std::lock_guard<QMutex> lock{ *__mutW };
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
            std::lock_guard<QMutex> lock{ *__mutW };
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
                log().e("usb::UsbHost", tr("Failed or timout to handle hotplug event."));

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
        std::lock_guard<QMutex> lock{_stopFlagMutex};
        _stopFlag = true;
    }


    UsbDeviceRescanWorker::UsbDeviceRescanWorker(QObject *parent):
        QObject(parent), _updateMode(false)
    {

    }

    void UsbDeviceRescanWorker::setUpdateMode(bool updateMode)
    {
        _updateMode = updateMode;
    }

    void UsbDeviceRescanWorker::run()
    {
        UsbHost *host = UsbHost::instance();
        int ret;
        if (!host->_initialized)
            host->__init();
        if (!host->_initialized)
            return;

        libusb_device **dev_list;
        if ((ret = libusb_get_device_list(host->_context, &dev_list)) < LIBUSB_SUCCESS)
        {
            log().e("usb::UsbHost", tr("Failed to get device list (%1).").arg(usb_error_name(ret)));
            return;
        }

        libusb_device *device = nullptr;
        int i = 0;
        if (_updateMode && !host->_usbDevices.empty())
        {
            QHash<const UsbDevice *, bool> deviceExistMap;
            foreach (const UsbDevice *device, host->usbDevices())
                deviceExistMap.insert(device, false);

            auto deviceExists = [host, &deviceExistMap] (libusb_device *usbDevice) -> bool {
                bool exists = false;
                foreach (const UsbDevice *device, host->usbDevices())
                    if (device->operator==(usbDevice))
                    {
                        deviceExistMap[device] = true;
                        exists = true;
                    }
                return exists;
            };

            while ((device = dev_list[i++]) != nullptr)
            {
                if (deviceExists(device))
                    continue;
                host->__addDevice(device);
            }

            QHash<const UsbDevice *, bool>::const_iterator iter = deviceExistMap.constBegin();
            while (iter != deviceExistMap.constEnd()) {
                if (!iter.value())
                    host->__removeDevice(*iter.key());
                ++iter;
            }
        }
        else
        {
            std::lock_guard<QMutex> lock{ *__mutW };
            foreach(UsbDevice * const oldDevice, host->_usbDevices)
                delete oldDevice;
            host->_usbDevices.clear();
            while ((device = dev_list[i++]) != nullptr)
            {
                UsbDevice *usbDevice = new UsbDevice(device, this);
                if (usbDevice->valid())
                {
                    host->_usbDevices.append(usbDevice);
                    log().i("usb::UsbHost", tr("New USB device \"%1\" attached.").arg(usbDevice->displayName()));
                }
                else
                    delete usbDevice;
            }

            host->__sortDeviceList();
            emit finished();
        }

        libusb_free_device_list(dev_list, false);
    }
}
