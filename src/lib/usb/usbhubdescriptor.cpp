#include "usbhubdescriptor.h"
#include "__usbmacro.h"
#include "usbhtmlbuilder.h"

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

    const QStringList &UsbEnhancedSuperSpeedHubDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bNbrPorts",
            "wHubCharacteristics",
            "bPwrOn2PwrGood",
            "bHubContrCurrent",
            "bHubHdrDecLat",
            "wHubDelay",
            "DeviceRemovable",
        };

        return fields;
    }

    QString UsbEnhancedSuperSpeedHubDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Number of bytes in this descriptor, including this byte. (12 bytes)"},
            {"bDescriptorType", "Descriptor Type, value: 2AH for Enhanced SuperSpeed hub descriptor"},
            {"bNbrPorts", "Number of downstream facing ports that this hub supports. The "
             "maximum number of ports of ports a hub can support is 15."},
            {"wHubCharacteristics", "<p>D1...D0: Logical Power Switching Mode</p>"
             "<table><tr><td>00:</td><td>Ganged power switching (all ports’ power at once)</td></tr>"
             "<tr><td>01:</td><td>Individual port power switching</td></tr>"
             "<tr><td>1X:</td><td>Reserved</td></tr></table>"
             "<p>Identifies a Compound Device</p>"
             "<table><tr><td>0:</td><td>Hub is not part of a compound device.</td></tr>"
             "<tr><td>1:</td><td>Hub is part of a compound device.</td></tr></table>"
             "<p>D4...D3: Over-current Protection Mode</p>"
             "<table><tr><td>00:</td><td>Global Over-current Protection. The hub reports over-"
             "current as a summation of all ports’ current draw, without "
             "a breakdown of individual port over-current status.</td></tr>"
             "<tr><td>01:</td><td>Individual Port Over-current Protection. The hub reports "
             "over-current on a per-port basis. Each port has an over-"
             "current status.</td></tr>"
             "<tr><td>1X:</td><td>No Over-current Protection. This option is allowed only "
             "for bus-powered hubs that do not implement over-current "
             "protection.</td></tr></table>"
             "<p>D15...D5: Reserved</p>"},
            {"bPwrOn2PwrGood", "Time (in 2 ms intervals) from the time the power-on sequence "
             "begins on a port until power is good on that port. The USB system "
             "software uses this value to determine how long to wait before "
             "accessing a powered-on port. This value is set to zero if power-"
             "switching is not supported by the hub."},
            {"bHubContrCurrent", "Maximum current requirements of the Hub Controller electronics "
             "when the hub is operating on both USB 2.0 and Enhanced "
             "SuperSpeed expressed in units of aCurrentUnit (i.e., 50 = 50* "
             "aCurrentUnit mA). Note that the encoding of this field is different if "
             "the encoding used is the USB 2.0 specification for USB 2.0 hubs. A "
             "USB hub shall report the current requirements when it is only "
             "operating on USB 2.0 (not Enhanced SuperSpeed) in the USB 2.0 "
             "hub descriptor."},
            {"bHubHdrDecLat", "<p>Hub Packet Header Decode Latency.</p>"
            "<p>Worst case latency for hubs whose upstream link is in U0 to decode "
             "the header of a downstream flowing TP or DP packet and initiate a "
             "transition to U0 on the relevant downstream port. The time is "
             "measured from receipt of the last symbol of the header packet by "
             "the upstream port until the hubs starts LFPS on the intended "
             "downstream port.</p>"
             "<p>This field is used to calculate the total path exit latency through a "
             "hub.</p>"
             "<p>The following are permissible values:</p>"
             "<table><tr><th>Value</th><th>Meaning</th></tr>"
             "<tr><td>00H</td><td>Much less than 0.1 µs.</td></tr>"
             "<tr><td>01H</td><td>0.1 µs</td></tr>"
             "<tr><td>02H</td><td>0.2 µs</td></tr>"
             "<tr><td>03H</td><td>0.3 µs</td></tr>"
             "<tr><td>04H</td><td>0.4 µs</td></tr>"
             "<tr><td>05H</td><td>0.5 µs</td></tr>"
             "<tr><td>06H</td><td>0.6 µs</td></tr>"
             "<tr><td>07H</td><td>0.7 µs</td></tr>"
             "<tr><td>08H</td><td>0.8 µs</td></tr>"
             "<tr><td>09H</td><td>0.9 µs</td></tr>"
             "<tr><td>0AH</td><td>1.0 µs</td></tr>"
             "<tr><td>0BH-FFH</td><td>Reserved</td></tr></table>"
            },
            {"wHubDelay", "This field defines the maximum delay in nanoseconds a hub "
             "introduces while forwarding packets in either direction. Note that "
             "the maximum value a hub is allowed to report in this field is "
             "tHubDelay. See Section 10.19."},
            {"DeviceRemovable", "<p>Indicates if a port has a removable device attached. "
             "This field is reported on byte-granularity. Within a "
             "byte, if no port exists for a given location, the field "
             "representing the port characteristics returns 0.</p>"
             "<p>Bit value definition:</p>"
             "<table><tr><td>0B - Device is removable</td></tr>"
             "<tr><td>1B - Device is non-removable</td></tr></table>"
             "<p>This is a bitmap corresponding to the individual ports "
             "on the hub:</p>"
             "<table><tr><td>Bit 0:</td><td>Reserved for future use</td></tr>"
             "<tr><td>Bit 1:</td><td>Port 1</td></tr>"
             "<tr><td>Bit 2:</td><td>Port 2</td></tr>"
             "<tr><td>...</td><td></td></tr>"
             "<tr><td>Bit n:</td><td>Port n (implementation-dependent, up to a "
             "maximum of 15 ports)</td></tr></table>"},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString UsbEnhancedSuperSpeedHubDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Enhanced SuperSpeed Hub Descriptor"), true)
                .attr("bDescLength", _bDescLength)
                .attr("bDescriptorType", _bDescriptorType, "ENHANCED_SUPERSPEED_HUB")
                .attr("bNbrPorts", _bNbrPorts)
                .attr("wHubCharacteristics", _wHubCharacteristics, __parseWHubCharacteristics())
                .attr("bPwrOn2PwrGood", _bPwrOn2PwrGood, tr("%1 ms").arg(_bPwrOn2PwrGood * 2))
                .attr("bHubContrCurrent", _bHubContrCurrent, tr("%1 mA").arg(_bHubContrCurrent * 4))
                .attr("bHubHdrDecLat", _bHubHdrDecLat,
                      QString(_bHubHdrDecLat ?
                                  tr("%1 µs").arg(double(_bHubHdrDecLat) / 10):
                                  tr("Less than 0.1 µs")))
                .attr("wHubDelay", _wHubDelay, tr("%1 ns").arg(_wHubDelay))
                .attr("DeviceRemovable", _DeviceRemovable, "")
                .end()
                .build();
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

        return features.join(UsbHtmlBuilder::NEWLINE);
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

    const QStringList &Usb20HubDescriptor::getFieldNames()
    {
        static const QStringList fields = {
            "bLength",
            "bDescriptorType",
            "bNbrPorts",
            "wHubCharacteristics",
            "bPwrOn2PwrGood",
            "bHubContrCurrent",
            "DeviceRemovable",
            "PortPwrCtrlMask",
        };

        return fields;
    }

    QString Usb20HubDescriptor::getFieldInformation(const QString &field)
    {
        static const QMap<QString, QString> fieldDescription = {
            {"bLength", "Number of bytes in this descriptor, including this byte"},
            {"bDescriptorType", "Descriptor Type, value: 29H for hub descriptor"},
            {"bNbrPorts", "Number of downstream facing ports that this hub supports"},
            {"wHubCharacteristics", "<p>D1...D0: Logical Power Switching Mode</p>"
             "<table><tr><td>00:</td><td>Ganged power switching (all ports’ power at once)</td></tr>"
             "<tr><td>01:</td><td>Individual port power switching</td></tr>"
             "<tr><td>1X:</td><td>Reserved. Used only on 1.0 compliant hubs that "
             "implement no power switching</td></tr></table>"
             "<p>D2: Identifies a Compound Device</p>"
             "<table><tr><td>0:</td><td>Hub is not part of a compound device.</td></tr>"
             "<tr><td>1:</td><td>Hub is part of a compound device.</td></tr></table>"
             "<p>D4...D3: Over-current Protection Mode</p>"
             "<table><tr><td>00:</td><td>Global Over-current Protection. The hub "
             "reports over-current as a summation of all "
             "ports’ current draw, without a breakdown of "
             "individual port over-current status.</td></tr>"
             "<tr><td>01:</td><td>Individual Port Over-current Protection. The "
             "hub reports over-current on a per-port basis. "
             "Each port has an over-current status.</td></tr>"
             "<tr><td>1X:</td><td>No Over-current Protection. This option is "
             "allowed only for bus-powered hubs that do not "
             "implement over-current protection.</td></tr></table>"
             "<p>D6...D5: TT Think TIme</p>"
             "<table><tr><td>00:</td><td>TT requires at most 8 FS bit times of inter "
             "transaction gap on a full-/low-speed "
             "downstream bus.</td></tr>"
             "<tr><td>01:</td><td>TT requires at most 16 FS bit times.</td></tr>"
             "<tr><td>10:</td><td>TT requires at most 24 FS bit times.</td></tr>"
             "<tr><td>11:</td><td>TT requires at most 32 FS bit times.</td></tr></table>"
             "<p>D7: Port Indicators Supported</p>"
             "<table><tr><td>0:</td><td>Port Indicators are not supported on its "
             "downstream facing ports and the "
             "PORT_INDICATOR request has no effect.</td></tr>"
             "<tr><td>1:</td><td>Port Indicators are supported on its "
             "downstream facing ports and the "
             "PORT_INDICATOR request controls the "
             "indicators. See Section 11.5.3.</td></tr></table>"
             "<p>D15...D8: Reserved</p>"},
            {"bPwrOn2PwrGood", " Time (in 2 ms intervals) from the time the power-on "
             "sequence begins on a port until power is good on that "
             "port. The USB System Software uses this value to "
             "determine how long to wait before accessing a "
             "powered-on port."},
            {"bHubContrCurrent", "Maximum current requirements of the Hub Controller "
             "electronics in mA."},
            {"DeviceRemovable", "<p>Indicates if a port has a removable device attached. "
             "This field is reported on byte-granularity. Within a "
             "byte, if no port exists for a given location, the field "
             "representing the port characteristics returns 0.</p>"
             "<p>Bit value definition:</p>"
             "<table><tr><td>0B - Device is removable</td></tr>"
             "<tr><td>1B - Device is non-removable</td></tr></table>"
             "<p>This is a bitmap corresponding to the individual ports "
             "on the hub:</p>"
             "<table><tr><td>Bit 0:</td><td>Reserved for future use</td></tr>"
             "<tr><td>Bit 1:</td><td>Port 1</td></tr>"
             "<tr><td>Bit 2:</td><td>Port 2</td></tr>"
             "<tr><td>...</td><td></td></tr>"
             "<tr><td>Bit n:</td><td>Port n (implementation-dependent, up to a "
             "maximum of 255 ports)</td></tr></table>"},
            {"PortPwrCtrlMask", "This field exists for reasons of compatibility with "
             "software written for 1.0 compliant devices. All bits in "
             "this field should be set to 1B. This field has one bit for "
             "each port on the hub with additional pad bits, if "
             "necessary, to make the number of bits in the field an "
             "integer multiple of 8."},
        };

        if (fieldDescription.contains(field))
            return fieldDescription[field];
        else
            return QString();
    }

    QString Usb20HubDescriptor::infomationToHtml() const
    {
        return UsbHtmlBuilder()
                .start(tr("Hub Descriptor"), true)
                .attr("bDescLength", _bDescLength)
                .attr("bDescriptorType", _bDescriptorType, "HUB")
                .attr("bNbrPorts", _bNbrPorts)
                .attr("wHubCharacteristics", _wHubCharacteristics, __parseWHubCharacteristics())
                .attr("bPwrOn2PwrGood", _bPwrOn2PwrGood, tr("%1 ms").arg(_bPwrOn2PwrGood * 2))
                .attr("bHubContrCurrent", _bHubContrCurrent, tr("%1 mA").arg(_bHubContrCurrent))
                .attr("DeviceRemovable", QString("0x") + _DeviceRemovable.toHex(), "")
                .attr("PortPwrCtrlMask", QString("0x") + _PortPwrCtrlMask.toHex(), "")
                .end()
                .build();
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

        return features.join(UsbHtmlBuilder::NEWLINE);
    }

} // namespace usb
