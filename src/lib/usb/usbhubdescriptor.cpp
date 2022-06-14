#include "usbhubdescriptor.h"
#include "__usbmacro.h"

namespace usb {

    UsbHubDescriptor::UsbHubDescriptor(UsbDeviceDescriptor *deviceDesc)
        : QObject{deviceDesc}, _deviceDescriptor(deviceDesc)
    {

    }

    UsbHubDescriptor *UsbHubDescriptor::tryGet(UsbDeviceDescriptor *deviceDesc)
    {
        UsbDevice *device = deviceDesc->device();

        /* Hub Device */
        if (deviceDesc->bDeviceClass() != 0x09)
            return nullptr;

        QByteArray buffer(40, '\0');
        uint16_t descriptorType;
        if (deviceDesc->bcdUSB() >= 0x0300)
            descriptorType = LIBUSB_DT_SUPERSPEED_HUB;
        else
            descriptorType = LIBUSB_DT_HUB;
        int ret = device->controlTransfer(0xA0,
                                          LIBUSB_REQUEST_GET_DESCRIPTOR,
                                          descriptorType << 8,
                                          0,
                                          buffer,
                                          TRANSFER_TIMEOUT);
        if (ret < LIBUSB_SUCCESS)
        {
            log().e("UsbHubDescriptor", tr("Failed to request Hub Descriptor, error: %1.")
                    .arg(usb_error_name(ret)));
            return nullptr;
        }

        if (deviceDesc->bcdUSB() >= 0x0300)
        {
            UsbEnhancedSuperSpeedHubDescriptor *desc = new UsbEnhancedSuperSpeedHubDescriptor(deviceDesc);
            const uint8_t *data = reinterpret_cast<const uint8_t *>(buffer.constData());
            desc->_bDescLength = data[0];
            desc->_bDescriptorType = data[1];
            desc->_bNbrPorts = data[2];
            desc->_wHubCharacteristics = *reinterpret_cast<const uint16_t *>(data + 3);
            desc->_bPwrOn2PwrGood = data[5];
            desc->_bHubContrCurrent = data[6];
            desc->_bHubHdrDecLat = data[7];
            desc->_wHubDelay = *reinterpret_cast<const uint16_t *>(data + 8);
            desc->_DeviceRemovable = *reinterpret_cast<const uint16_t *>(data + 10);

            return desc;
        }
        else
        {
            Usb20HubDescriptor *desc = new Usb20HubDescriptor(deviceDesc);
            const uint8_t *data = reinterpret_cast<const uint8_t *>(buffer.constData());
            desc->_bDescLength = data[0];
            desc->_bDescriptorType = data[1];
            desc->_bNbrPorts = data[2];
            desc->_wHubCharacteristics = *reinterpret_cast<const uint16_t *>(data + 3);
            desc->_bPwrOn2PwrGood = data[5];
            desc->_bHubContrCurrent = data[6];
            int portBitmapSize = (desc->_bNbrPorts >> 3) + 1;
            desc->_DeviceRemovable = buffer.mid(7, portBitmapSize);
            desc->_PortPwrCtrlMask = buffer.mid(7 + portBitmapSize, portBitmapSize);

            return desc;
        }
    }

    UsbDeviceDescriptor *UsbHubDescriptor::deviceDescriptor() const
    {
        return _deviceDescriptor;
    }

    uint8_t UsbEnhancedSuperSpeedHubDescriptor::bDescLength() const
    {
        return _bDescLength;
    }

    uint8_t UsbEnhancedSuperSpeedHubDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint8_t UsbEnhancedSuperSpeedHubDescriptor::bNbrPorts() const
    {
        return _bNbrPorts;
    }

    uint16_t UsbEnhancedSuperSpeedHubDescriptor::wHubCharacteristics() const
    {
        return _wHubCharacteristics;
    }

    uint8_t UsbEnhancedSuperSpeedHubDescriptor::bPwrOn2PwrGood() const
    {
        return _bPwrOn2PwrGood;
    }

    uint8_t UsbEnhancedSuperSpeedHubDescriptor::bHubContrCurrent() const
    {
        return _bHubContrCurrent;
    }

    uint8_t UsbEnhancedSuperSpeedHubDescriptor::bHubHdrDecLat() const
    {
        return _bHubHdrDecLat;
    }

    uint16_t UsbEnhancedSuperSpeedHubDescriptor::wHubDelay() const
    {
        return _wHubDelay;
    }

    uint16_t UsbEnhancedSuperSpeedHubDescriptor::DeviceRemovable() const
    {
        return _DeviceRemovable;
    }

    QString UsbEnhancedSuperSpeedHubDescriptor::infomationToHtml() const
    {
        QString html;
        START("Enhanced SuperSpeed Hub Descriptor");
        ATTR("bDescLength", _bDescLength, _bDescLength);
        ATTR("bNbrPorts", _bNbrPorts, _bNbrPorts);
        ATTR("wHubCharacteristics", _wHubCharacteristics, __parseWHubCharacteristics());
        ATTR("bPwrOn2PwrGood", _bPwrOn2PwrGood, tr("%1 ms").arg(_bPwrOn2PwrGood * 2));
        ATTR("bHubContrCurrent", _bHubContrCurrent, tr("%1 mA").arg(_bHubContrCurrent * 4));
        ATTR("bHubHdrDecLat", _bHubHdrDecLat, QString(_bHubHdrDecLat ?
                                                          tr("%1 µs").arg(double(_bHubHdrDecLat) / 10):
                                                          tr("Less than 0.1 µs")));
        ATTR("wHubDelay", _wHubDelay, tr("%1 ns").arg(_wHubDelay));
        ATTRTEXT("DeviceRemovable", QString("0x%1").arg(_DeviceRemovable, sizeof(_DeviceRemovable) * 2, 16, QChar('0')));
        END;

        return html;
    }

    UsbEnhancedSuperSpeedHubDescriptor::UsbEnhancedSuperSpeedHubDescriptor(
            UsbDeviceDescriptor *deviceDesc):
        UsbHubDescriptor(deviceDesc)
    {

    }

    QString UsbEnhancedSuperSpeedHubDescriptor::__parseWHubCharacteristics() const
    {
        QStringList features;
        if (BIT(_wHubCharacteristics, 1))
            features.append(tr("No power switching"));
        else if (BIT(_wHubCharacteristics, 0))
            features.append(tr("Ganged power switching"));
        else
            features.append(tr("Individual port power switching"));

        if (BIT(_wHubCharacteristics, 2))
            features.append(tr("Hub is part of a compound device"));
        else
            features.append(tr("Hub is NOT part of a compound device"));

        if (BIT(_wHubCharacteristics, 4))
            features.append(tr("No Over-current Protection"));
        else if (BIT(_wHubCharacteristics, 3))
            features.append(tr("Individual Port Over-current Protection"));
        else
            features.append(tr("Global Over-current Protection"));

        return features.join(NEWLINE);
    }

    uint8_t Usb20HubDescriptor::bDescLength() const
    {
        return _bDescLength;
    }

    uint8_t Usb20HubDescriptor::bDescriptorType() const
    {
        return _bDescriptorType;
    }

    uint8_t Usb20HubDescriptor::bNbrPorts() const
    {
        return _bNbrPorts;
    }

    uint16_t Usb20HubDescriptor::wHubCharacteristics() const
    {
        return _wHubCharacteristics;
    }

    uint8_t Usb20HubDescriptor::bPwrOn2PwrGood() const
    {
        return _bPwrOn2PwrGood;
    }

    uint8_t Usb20HubDescriptor::bHubContrCurrent() const
    {
        return _bHubContrCurrent;
    }

    const QByteArray &Usb20HubDescriptor::DeviceRemovable() const
    {
        return _DeviceRemovable;
    }

    const QByteArray &Usb20HubDescriptor::PortPwrCtrlMask() const
    {
        return _PortPwrCtrlMask;
    }

    QString Usb20HubDescriptor::infomationToHtml() const
    {
        QString html;
        START("Hub Descriptor");
        ATTR("bDescLength", _bDescLength, _bDescLength);
        ATTR("bNbrPorts", _bNbrPorts, _bNbrPorts);
        ATTR("wHubCharacteristics", _wHubCharacteristics, __parseWHubCharacteristics());
        ATTR("bPwrOn2PwrGood", _bPwrOn2PwrGood, tr("%1 ms").arg(_bPwrOn2PwrGood * 2));
        ATTR("bHubContrCurrent", _bHubContrCurrent, tr("%1 mA").arg(_bHubContrCurrent));
        ATTRTEXT("DeviceRemovable", QString("0x") + _DeviceRemovable.toHex());
        ATTRTEXT("PortPwrCtrlMask", QString("0x") + _PortPwrCtrlMask.toHex());
        END;

        return html;
    }

    Usb20HubDescriptor::Usb20HubDescriptor(UsbDeviceDescriptor *deviceDesc):
        UsbHubDescriptor(deviceDesc)
    {

    }

    QString Usb20HubDescriptor::__parseWHubCharacteristics() const
    {
        QStringList features;
        if (BIT(_wHubCharacteristics, 1))
            features.append(tr("No power switching"));
        else if (BIT(_wHubCharacteristics, 0))
            features.append(tr("Ganged power switching"));
        else
            features.append(tr("Individual port power switching"));

        if (BIT(_wHubCharacteristics, 2))
            features.append(tr("Hub is part of a compound device"));
        else
            features.append(tr("Hub is NOT part of a compound device"));

        if (BIT(_wHubCharacteristics, 4))
            features.append(tr("No Over-current Protection"));
        else if (BIT(_wHubCharacteristics, 3))
            features.append(tr("Individual Port Over-current Protection"));
        else
            features.append(tr("Global Over-current Protection"));

        switch (CUT(_wHubCharacteristics, 5, 6))
        {
            case 0:
                features.append(tr("TT requires at most 8 FS bit times"));
            break;
            case 1:
                features.append(tr("TT requires at most 16 FS bit times"));
            break;
            case 2:
                features.append(tr("TT requires at most 24 FS bit times"));
            break;
            case 3:
                features.append(tr("TT requires at most 32 FS bit times"));
            break;
        }

        if (BIT(_wHubCharacteristics, 7))
            features.append(tr("Port Indicators are supported"));
        else
            features.append(tr("Port Indicators are NOT supported"));

        return features.join(NEWLINE);
    }

} // namespace usb
