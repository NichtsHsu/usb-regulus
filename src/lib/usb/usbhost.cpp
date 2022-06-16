#include "usbhost.h"
#include "usbhiddescriptor.h"
#include "__usbmacro.h"

namespace usb {
    namespace __private {
        /**
         * @brief __hotplug_callback
         * The real hotplug callback function to register for libusb.
         * Internally calling the UsbHost::__addDevice() and UsbHost::__removeDevice().
         */
        static int LIBUSB_CALL __hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
        {
            Q_UNUSED(ctx);
            UsbHost *usbHost = reinterpret_cast<UsbHost*>(user_data);
            if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
                usbHost->__addDevice(dev);
            else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
                usbHost->__removeDevice(dev);
            else
                log().w("UsbHost", UsbHost::tr("Unhandled hotplug event %1.").arg(event));

            return 0;
        }
    }

    UsbHost::UsbHost(QObject *parent):
        QObject{parent}, _context(nullptr), _protectMouse(true), _protectKeyboard(true)
    {
        __init();
    }

    UsbHost::~UsbHost()
    {
        if (_initialized) {
            if (_hasHotplug)
            {
                _hotplugCbThread->exit();
                _hotplugCbThread->wait();
                _hotplugCbThread->deleteLater();

                _hotplugCbTimer->disconnect();
                _hotplugCbTimer->deleteLater();

                libusb_hotplug_deregister_callback(_context, _hotplugCbHandle);
            }
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

    void UsbHost::__refreshHotplutEvent()
    {
        timeval tv = {0, 200};
        int ret = libusb_handle_events_timeout(_context, &tv);
        if (ret < 0)
            LOGE(tr("Failed or timout to handle hotplug event."));
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
                                                   reinterpret_cast<libusb_hotplug_callback_fn>(__private::__hotplug_callback),
                                                   reinterpret_cast<void *>(this),
                                                   &_hotplugCbHandle);
            if (ret == LIBUSB_SUCCESS)
            {
                /* Create a thread to periodically trigger the hotplug event handle of libusb */
                _hotplugCbTimer = new QTimer;
                _hotplugCbThread = new QThread;
                _hotplugCbTimer->moveToThread(_hotplugCbThread);
                _hotplugCbTimer->setSingleShot(false);
                _hotplugCbTimer->setInterval(250);
                connect(_hotplugCbThread, &QThread::started, _hotplugCbTimer, static_cast<void(QTimer::*)()>(&QTimer::start));
                connect(_hotplugCbThread, &QThread::finished, _hotplugCbTimer, &QTimer::stop);
                connect(_hotplugCbTimer, &QTimer::timeout, this, &UsbHost::__refreshHotplutEvent);
                _hotplugCbThread->start();
            }
            else
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
        emit deviceAttached(__indexOfDevice(*usbDevice));
    }

    void UsbHost::__removeDevice(libusb_device *device)
    {
        if (!_hasHotplug)
            return;

        int index = __indexOfDevice(device);
        if (index == -1)
        {
            LOGW(tr("Ignore the detached event because we didn't find any device match it."));
            return;
        }

        /* Emit the signal before delete, to avoid wild pointer used in other module */
        emit deviceDetached(index);
        LOGI(tr("USB device \"%1\" detached.").arg(_usbDevices[index]->displayName()));
        delete _usbDevices[index];
        _usbDevices.remove(index);
    }

    bool UsbHost::initialized() const
    {
        return _initialized;
    }

    std::atomic<UsbHost *> UsbHost::_instance{nullptr};
    static std::mutex __mutW;

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
}
