#include "fieldquickreference.h"
#include "ui_fieldquickreference.h"
#include <QUrl>
#include <QUrlQuery>
#include "log/logger.h"
#include "usb/usbdevicedescriptor.h"
#include "usb/usbhiddescriptor.h"
#include "usb/usbdfudescriptor.h"
#include "usb/usbvideocontrolinterfacedescriptor.h"
#include "usb/usbvideostreaminterfacedescriptor.h"
#include <QAbstractItemView>

using namespace usb;

FieldQuickReference::FieldQuickReference(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FieldQuickReference)
{
    ui->setupUi(this);
    ui->comboBoxDescriptor->addItems(
                {tr("Authentication Capability Descriptor"),
                 tr("Battery Info Capability Descriptor"),
                 tr("Billboard AUM Capability Descriptor"),
                 tr("Billboard Capability Descriptor"),
                 tr("BOS Descriptor"),
                 tr("Camera Terminal Descriptor (UVC 1.5)"),
                 tr("Class-Specific VC Interface Header Descriptor (UVC 1.5)"),
                 tr("Color Matching Descriptor (UVC 1.5)"),
                 tr("Configuration Descriptor"),
                 tr("Configuration Summary Descriptor"),
                 tr("Container ID Descriptor"),
                 tr("Device Descriptor"),
                 tr("Device Firmware Upgrade Functional Descriptor"),
                 tr("DV Format Descriptor (UVC 1.5)"),
                 tr("Encoding Unit Descriptor (UVC 1.5)"),
                 tr("Encryption Type Descriptor"),
                 tr("Endpoint Descriptor"),
                 tr("Enhanced SuperSpeed Hub Descriptor"),
                 tr("Extended Wireless Device Capability Descriptor"),
                 tr("Extension Unit Descriptor (UVC 1.5)"),
                 tr("Frame Based Payload Video Format Descriptor (UVC 1.5)"),
                 tr("Frame Based Payload Video Frame Descriptor (UVC 1.5)"),
                 tr("HID Descriptor"),
                 tr("Hub Descriptor"),
                 tr("H.264 Payload Video Format Descriptor (UVC 1.5)"),
                 tr("H.264 Payload Video Frame Descriptor (UVC 1.5)"),
                 tr("Input Header Descriptor (UVC 1.5)"),
                 tr("Input Terminal Descriptor (UVC 1.5)"),
                 tr("Interface Association Descriptor"),
                 tr("Interface Descriptor"),
                 tr("Motion-JPEG Video Format Descriptor (UVC 1.5)"),
                 tr("Motion-JPEG Video Frame Descriptor (UVC 1.5)"),
                 tr("MPEG-2 TS Format Descriptor (UVC 1.5)"),
                 tr("OTG Descriptor"),
                 tr("Output Header Descriptor (UVC 1.5)"),
                 tr("Output Terminal Descriptor (UVC 1.5)"),
                 tr("PD Consumer Port Capability Descriptor"),
                 tr("PD Provider Port Capability Descriptor"),
                 tr("Platform Capability Descriptor"),
                 tr("Precision Time Measurement Capability Descriptor"),
                 tr("Processing Unit Descriptor (UVC 1.5)"),
                 tr("Security Descriptor"),
                 tr("Selector Unit Descriptor (UVC 1.5)"),
                 tr("Still Image Frame Descriptor (UVC 1.5)"),
                 tr("Stream Based Format Descriptor (UVC 1.5)"),
                 tr("SuperSpeed Device Capability Descriptor"),
                 tr("SuperSpeedPlus Device Capability Descriptor"),
                 tr("Uncompressed Video Format Descriptor (UVC 1.5)"),
                 tr("Uncompressed Video Frame Descriptor (UVC 1.5)"),
                 tr("USB Power Delivery Capability Descriptor"),
                 tr("USB 2.0 Extension Descriptor"),
                 tr("VP8 Payload Video Format Descriptor (UVC 1.5)"),
                 tr("VP8 Payload Video Frame Descriptor (UVC 1.5)"),
                 tr("Wireless USB Device Capability Descriptor"),});
    ui->comboBoxDescriptor->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBoxDescriptor->setMaxVisibleItems(10);
    ui->comboBoxField->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBoxField->setMaxVisibleItems(10);
    ui->textBrowser->document()->setDefaultStyleSheet("p { margin: 0px; margin-top: 4px; }"
                                                      "h1,h2,h3,h4 { margin: 0px; margin-top: 6px; margin-bottom: 6px;}"
                                                      "td, th { padding-left: 16px; }"
                                                      "td p { margin: 0px; margin-top: 2px; }");

    showInformation(tr("Device Descriptor"), UsbDeviceDescriptor::getFieldNames()[0]);

    connect(ui->comboBoxDescriptor, &QComboBox::currentTextChanged,
            this, [this](const QString &descriptor) {
        QStringList feilds = _db.getFieldNames(descriptor);
        ui->comboBoxField->clear();
        ui->comboBoxField->addItems(feilds);
        ui->comboBoxField->setCurrentIndex(0);
        ui->textBrowser->setHtml(_db.getFieldInformation(descriptor, feilds[0]));
    });
    connect(ui->comboBoxField, &QComboBox::currentTextChanged,
            this, [this](const QString &field){
        ui->textBrowser->setHtml(_db.getFieldInformation(ui->comboBoxDescriptor->currentText(), field));
    });
}

FieldQuickReference::~FieldQuickReference()
{
    delete ui;
}

void FieldQuickReference::changeEvent(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::LanguageChange:
        {
            ui->retranslateUi(this);
            ui->comboBoxDescriptor->blockSignals(true);
            int index = ui->comboBoxDescriptor->currentIndex();
            ui->comboBoxDescriptor->clear();
            ui->comboBoxDescriptor->addItems(
                        {tr("Authentication Capability Descriptor"),
                         tr("Battery Info Capability Descriptor"),
                         tr("Billboard AUM Capability Descriptor"),
                         tr("Billboard Capability Descriptor"),
                         tr("BOS Descriptor"),
                         tr("Camera Terminal Descriptor (UVC 1.5)"),
                         tr("Class-Specific VC Interface Header Descriptor (UVC 1.5)"),
                         tr("Color Matching Descriptor (UVC 1.5)"),
                         tr("Configuration Descriptor"),
                         tr("Configuration Summary Descriptor"),
                         tr("Container ID Descriptor"),
                         tr("Device Descriptor"),
                         tr("Device Firmware Upgrade Functional Descriptor"),
                         tr("DV Format Descriptor (UVC 1.5)"),
                         tr("Encoding Unit Descriptor (UVC 1.5)"),
                         tr("Encryption Type Descriptor"),
                         tr("Endpoint Descriptor"),
                         tr("Enhanced SuperSpeed Hub Descriptor"),
                         tr("Extended Wireless Device Capability Descriptor"),
                         tr("Extension Unit Descriptor (UVC 1.5)"),
                         tr("Frame Based Payload Video Format Descriptor (UVC 1.5)"),
                         tr("Frame Based Payload Video Frame Descriptor (UVC 1.5)"),
                         tr("HID Descriptor"),
                         tr("Hub Descriptor"),
                         tr("H.264 Payload Video Format Descriptor (UVC 1.5)"),
                         tr("H.264 Payload Video Frame Descriptor (UVC 1.5)"),
                         tr("Input Header Descriptor (UVC 1.5)"),
                         tr("Input Terminal Descriptor (UVC 1.5)"),
                         tr("Interface Association Descriptor"),
                         tr("Interface Descriptor"),
                         tr("Motion-JPEG Video Format Descriptor (UVC 1.5)"),
                         tr("Motion-JPEG Video Frame Descriptor (UVC 1.5)"),
                         tr("MPEG-2 TS Format Descriptor (UVC 1.5)"),
                         tr("OTG Descriptor"),
                         tr("Output Header Descriptor (UVC 1.5)"),
                         tr("Output Terminal Descriptor (UVC 1.5)"),
                         tr("PD Consumer Port Capability Descriptor"),
                         tr("PD Provider Port Capability Descriptor"),
                         tr("Platform Capability Descriptor"),
                         tr("Precision Time Measurement Capability Descriptor"),
                         tr("Processing Unit Descriptor (UVC 1.5)"),
                         tr("Security Descriptor"),
                         tr("Selector Unit Descriptor (UVC 1.5)"),
                         tr("Still Image Frame Descriptor (UVC 1.5)"),
                         tr("Stream Based Format Descriptor (UVC 1.5)"),
                         tr("SuperSpeed Device Capability Descriptor"),
                         tr("SuperSpeedPlus Device Capability Descriptor"),
                         tr("Uncompressed Video Format Descriptor (UVC 1.5)"),
                         tr("Uncompressed Video Frame Descriptor (UVC 1.5)"),
                         tr("USB Power Delivery Capability Descriptor"),
                         tr("USB 2.0 Extension Descriptor"),
                         tr("VP8 Payload Video Format Descriptor (UVC 1.5)"),
                         tr("VP8 Payload Video Frame Descriptor (UVC 1.5)"),
                         tr("Wireless USB Device Capability Descriptor"),});
            ui->comboBoxDescriptor->setCurrentIndex(index);
            _db.updateTranslations();
            ui->comboBoxDescriptor->blockSignals(false);
        }
        break;
        default:
        break;
    }
    QWidget::changeEvent(event);
}

bool FieldQuickReference::showInformation(const QString &descriptor, const QString &field)
{
    int index = ui->comboBoxDescriptor->findText(descriptor);
    if (index == -1)
        return false;
    ui->comboBoxDescriptor->setCurrentIndex(index);

    ui->comboBoxDescriptor->blockSignals(true);
    ui->comboBoxField->blockSignals(true);
    const QStringList &validFields = _db.getFieldNames(descriptor);
    ui->comboBoxField->clear();
    ui->comboBoxField->addItems(validFields);
    if (!validFields.contains(field))
    {
        ui->comboBoxField->setCurrentIndex(0);
        ui->textBrowser->setHtml(_db.getFieldInformation(descriptor, validFields[0]));
        ui->comboBoxDescriptor->blockSignals(false);
        ui->comboBoxField->blockSignals(false);
        return false;
    }
    ui->comboBoxField->setCurrentIndex(ui->comboBoxField->findText(field));
    ui->textBrowser->setHtml(_db.getFieldInformation(descriptor, field));
    ui->comboBoxDescriptor->blockSignals(false);
    ui->comboBoxField->blockSignals(false);
    return true;
}

void FieldQuickReference::showInformationByUrl(const QUrl &url)
{
    QUrlQuery q(url);
    if (!showInformation(q.queryItemValue("desc"), q.queryItemValue("field")))
        LOGE(tr("Failed to resolve quick reference URL \"%1\".").arg(url.toString()));

    LOGD(tr("Open quick reference URL \"%1\".").arg(url.toString()));
    show();
    raise();
}

FieldQuickReference::Database::Database()
{
    updateTranslations();
}

const QStringList &FieldQuickReference::Database::getFieldNames(const QString &descriptor) const
{
    return _getFieldNames[descriptor]();
}

QString FieldQuickReference::Database::getFieldInformation(const QString &descriptor, const QString &field)
{
    return _getFieldInformation[descriptor](field);
}

void FieldQuickReference::Database::updateTranslations()
{
    _getFieldNames.clear();
    _getFieldInformation.clear();

#define DB_INSERT(__desc__, __T__) \
    _getFieldNames.insert(FieldQuickReference::tr(__desc__), &__T__::getFieldNames); \
    _getFieldInformation.insert(FieldQuickReference::tr(__desc__), &__T__::getFieldInformation);

    DB_INSERT("Authentication Capability Descriptor", UsbAuthenticationCapabilityDescriptor);
    DB_INSERT("Battery Info Capability Descriptor", UsbBatteryInfoCapabilityDescriptor);
    DB_INSERT("Billboard AUM Capability Descriptor", UsbBillboardAumCapabilityDescriptor);
    DB_INSERT("Billboard Capability Descriptor", UsbBillboardCapabilityDescriptor);
    DB_INSERT("BOS Descriptor", UsbBosDescriptor);
    DB_INSERT("Camera Terminal Descriptor (UVC 1.5)", uvc::UsbCameraTerminalDescriptor);
    DB_INSERT("Class-Specific VC Interface Header Descriptor (UVC 1.5)", uvc::UsbClassSpecificVCInterfaceHeaderDescriptor);
    DB_INSERT("Color Matching Descriptor (UVC 1.5)", uvc::UsbColorMatchingDescriptor);
    DB_INSERT("Configuration Descriptor", UsbConfigurationDescriptor);
    DB_INSERT("Configuration Summary Descriptor", UsbConfigurationSummaryDescriptor);
    DB_INSERT("Container ID Descriptor", UsbContainerIdDescriptor);
    DB_INSERT("Device Descriptor", UsbDeviceDescriptor);
    DB_INSERT("Device Firmware Upgrade Functional Descriptor", UsbDfuDescriptor);
    DB_INSERT("DV Format Descriptor (UVC 1.5)", uvc::UsbDvFormatDescriptor);
    DB_INSERT("Encoding Unit Descriptor (UVC 1.5)", uvc::UsbEncodingUnitDescriptor);
    DB_INSERT("Encryption Type Descriptor", UsbWirelessEncryptionTypeDescriptor);
    DB_INSERT("Endpoint Descriptor", UsbEndpointDescriptor);
    DB_INSERT("Enhanced SuperSpeed Hub Descriptor", UsbEnhancedSuperSpeedHubDescriptor);
    DB_INSERT("Extended Wireless Device Capability Descriptor", UsbExtendedWirelessDeviceCapabilityDescriptor);
    DB_INSERT("Extension Unit Descriptor (UVC 1.5)", uvc::UsbExtensionUnitDescriptor);
    DB_INSERT("Frame Based Payload Video Format Descriptor (UVC 1.5)", uvc::UsbFrameBasedPayloadVideoFormatDescriptor);
    DB_INSERT("Frame Based Payload Video Frame Descriptor (UVC 1.5)", uvc::UsbFrameBasedPayloadVideoFrameDescriptor);
    DB_INSERT("HID Descriptor", UsbHidDescriptor);
    DB_INSERT("Hub Descriptor", Usb20HubDescriptor);
    DB_INSERT("H.264 Payload Video Format Descriptor (UVC 1.5)", uvc::UsbH264PayloadVideoFormatDescriptor);
    DB_INSERT("H.264 Payload Video Frame Descriptor (UVC 1.5)", uvc::UsbH264PayloadVideoFrameDescriptor);
    DB_INSERT("Input Header Descriptor (UVC 1.5)", uvc::UsbInputHeaderDescriptor);
    DB_INSERT("Input Terminal Descriptor (UVC 1.5)", uvc::UsbInputTerminalDescriptor);
    DB_INSERT("Interface Association Descriptor", UsbInterfaceAssociationDescriptor);
    DB_INSERT("Interface Descriptor", UsbInterfaceDescriptor);
    DB_INSERT("Motion-JPEG Video Format Descriptor (UVC 1.5)", uvc::UsbMotionJpegVideoFormatDescriptor);
    DB_INSERT("Motion-JPEG Video Frame Descriptor (UVC 1.5)", uvc::UsbMotionJpegVideoFrameDescriptor);
    DB_INSERT("MPEG-2 TS Format Descriptor (UVC 1.5)", uvc::UsbMpeg2TsFormatDescriptor);
    DB_INSERT("OTG Descriptor", UsbOtgDescriptor);
    DB_INSERT("Output Header Descriptor (UVC 1.5)", uvc::UsbOutputHeaderDescriptor);
    DB_INSERT("Output Terminal Descriptor (UVC 1.5)", uvc::UsbOutputTerminalDescriptor);
    DB_INSERT("PD Consumer Port Capability Descriptor", UsbPowerDeliveryConsumerPortCapabilityDescriptor);
    DB_INSERT("PD Provider Port Capability Descriptor", UsbPowerDeliveryProviderPortCapabilityDescriptor);
    DB_INSERT("Platform Capability Descriptor", UsbPlatformDescriptor);
    DB_INSERT("Precision Time Measurement Capability Descriptor", UsbPrecisionTimeMeasurementCapabilityDescriptor);
    DB_INSERT("Processing Unit Descriptor (UVC 1.5)", uvc::UsbProcessingUnitDescriptor);
    DB_INSERT("Security Descriptor", UsbWirelessSecurityDescriptor);
    DB_INSERT("Selector Unit Descriptor (UVC 1.5)", uvc::UsbSelectorUnitDescriptor);
    DB_INSERT("Still Image Frame Descriptor (UVC 1.5)", uvc::UsbStillImageFrameDescriptor);
    DB_INSERT("Stream Based Format Descriptor (UVC 1.5)", uvc::UsbStreamBasedFormatDescriptor);
    DB_INSERT("SuperSpeed Device Capability Descriptor", UsbSuperSpeedDeviceCapabilityDescriptor);
    DB_INSERT("SuperSpeedPlus Device Capability Descriptor", UsbSuperSpeedPlusDeviceCapabilityDescriptor);
    DB_INSERT("Uncompressed Video Format Descriptor (UVC 1.5)", uvc::UsbUncompressedVideoFormatDescriptor);
    DB_INSERT("Uncompressed Video Frame Descriptor (UVC 1.5)", uvc::UsbUncompressedVideoFrameDescriptor);
    DB_INSERT("USB Power Delivery Capability Descriptor", UsbPowerDeliveryCapabilityDescriptor);
    DB_INSERT("USB 2.0 Extension Descriptor", Usb20ExtensionDescriptor);
    DB_INSERT("VP8 Payload Video Format Descriptor (UVC 1.5)", uvc::UsbVp8PayloadVideoFormatDescriptor);
    DB_INSERT("VP8 Payload Video Frame Descriptor (UVC 1.5)", uvc::UsbVp8PayloadVideoFrameDescriptor);
    DB_INSERT("Wireless USB Device Capability Descriptor", UsbWirelessDeviceCapabilityDescriptor);
}
