#include "usbterminaltypes.h"
#include <QtGlobal>

#define COMMA ,

namespace usb {
    Q_GLOBAL_STATIC_WITH_ARGS(QMap<uint16_t COMMA const char *>, USB_TERMINAL_TYPES, (
                                  {{ 0x0100, "USB Undefined" },
                                   { 0x0101, "USB streaming" },
                                   { 0x01FF, "USB vendor specific" },
                                   { 0x0200, "Input Undefined" },
                                   { 0x0201, "Microphone" },
                                   { 0x0202, "Desktop microphone" },
                                   { 0x0203, "Personal microphone" },
                                   { 0x0204, "Omni-directional microphone" },
                                   { 0x0205, "Microphone array" },
                                   { 0x0206, "Processing microphone array" },
                                   { 0x0300, "Output Undefined" },
                                   { 0x0301, "Speaker" },
                                   { 0x0302, "Headphones" },
                                   { 0x0303, "Head Mounted Display Audio" },
                                   { 0x0304, "Desktop speaker" },
                                   { 0x0305, "Room speaker" },
                                   { 0x0306, "Communication speaker" },
                                   { 0x0307, "Low frequency effects speaker" },
                                   { 0x0400, "Bi-directional Undefined" },
                                   { 0x0401, "Handset" },
                                   { 0x0402, "Headset" },
                                   { 0x0403, "Speakerphone, no echo reduction" },
                                   { 0x0404, "Echo-suppressing speakerphone" },
                                   { 0x0405, "Echo-canceling speakerphone" },
                                   { 0x0500, "Telephony Undefined" },
                                   { 0x0501, "Phone line" },
                                   { 0x0502, "Telephone" },
                                   { 0x0503, "Down Line Phone" },
                                   { 0x0600, "External Undefined" },
                                   { 0x0601, "Analog connector" },
                                   { 0x0602, "Digital audio interface" },
                                   { 0x0603, "Line connector" },
                                   { 0x0604, "Legacy audio connector" },
                                   { 0x0605, "S/PDIF interface" },
                                   { 0x0606, "1394 DA stream" },
                                   { 0x0607, "1394 DV stream soundtrack" },
                                   { 0x0608, "ADAT Lightpipe" },
                                   { 0x0609, "TDIF" },
                                   { 0x060A, "MADI" },
                                   { 0x0700, "Embedded Undefined" },
                                   { 0x0701, "Level Calibration Noise Source" },
                                   { 0x0702, "Equalization Noise" },
                                   { 0x0703, "CD player" },
                                   { 0x0704, "DAT" },
                                   { 0x0705, "DCC" },
                                   { 0x0706, "MiniDisk" },
                                   { 0x0707, "Analog Tape" },
                                   { 0x0708, "Phonograph" },
                                   { 0x0709, "VCR Audio" },
                                   { 0x070A, "Video Disc Audio" },
                                   { 0x070B, "DVD Audio" },
                                   { 0x070C, "TV Tuner Audio" },
                                   { 0x070D, "Satellite Receiver Audio" },
                                   { 0x070E, "Cable Tuner Audio" },
                                   { 0x070F, "DSS Audio" },
                                   { 0x0710, "Radio Receiver" },
                                   { 0x0711, "Radio Transmitter" },
                                   { 0x0712, "Multi-track Recorder" },
                                   { 0x0713, "Synthesizer" },
                                   { 0x0714, "Piano" },
                                   { 0x0715, "Guitar" },
                                   { 0x0716, "Drums/Rhythm" },
                                   { 0x0717, "Other Musical Instrument" },
                                  }))

    QString strTerminalType(uint16_t wTerminalType)
    {
        return QObject::tr(USB_TERMINAL_TYPES->value(wTerminalType, "USB Undefined"));

        // Dummy code, just for triggering Linguist
#if 0
        QObject::tr("USB Undefined");
        QObject::tr("USB streaming");
        QObject::tr("USB vendor specific");
        QObject::tr("Input Undefined");
        QObject::tr("Microphone");
        QObject::tr("Desktop microphone");
        QObject::tr("Personal microphone");
        QObject::tr("Omni-directional microphone");
        QObject::tr("Microphone array");
        QObject::tr("Processing microphone array");
        QObject::tr("Output Undefined");
        QObject::tr("Speaker");
        QObject::tr("Headphones");
        QObject::tr("Head Mounted Display Audio");
        QObject::tr("Desktop speaker");
        QObject::tr("Room speaker");
        QObject::tr("Communication speaker");
        QObject::tr("Low frequency effects speaker");
        QObject::tr("Bi-directional Undefined");
        QObject::tr("Handset");
        QObject::tr("Headset");
        QObject::tr("Speakerphone, no echo reduction");
        QObject::tr("Echo-suppressing speakerphone");
        QObject::tr("Echo-canceling speakerphone");
        QObject::tr("Telephony Undefined");
        QObject::tr("Phone line");
        QObject::tr("Telephone");
        QObject::tr("Down Line Phone");
        QObject::tr("External Undefined");
        QObject::tr("Analog connector");
        QObject::tr("Digital audio interface");
        QObject::tr("Line connector");
        QObject::tr("Legacy audio connector");
        QObject::tr("S/PDIF interface");
        QObject::tr("1394 DA stream");
        QObject::tr("1394 DV stream soundtrack");
        QObject::tr("ADAT Lightpipe");
        QObject::tr("TDIF");
        QObject::tr("MADI");
        QObject::tr("Embedded Undefined");
        QObject::tr("Level Calibration Noise Source");
        QObject::tr("Equalization Noise");
        QObject::tr("CD player");
        QObject::tr("DAT");
        QObject::tr("DCC");
        QObject::tr("MiniDisk");
        QObject::tr("Analog Tape");
        QObject::tr("Phonograph");
        QObject::tr("VCR Audio");
        QObject::tr("Video Disc Audio");
        QObject::tr("DVD Audio");
        QObject::tr("TV Tuner Audio");
        QObject::tr("Satellite Receiver Audio");
        QObject::tr("Cable Tuner Audio");
        QObject::tr("DSS Audio");
        QObject::tr("Radio Receiver");
        QObject::tr("Radio Transmitter");
        QObject::tr("Multi-track Recorder");
        QObject::tr("Synthesizer");
        QObject::tr("Piano");
        QObject::tr("Guitar");
        QObject::tr("Drums/Rhythm");
        QObject::tr("Other Musical Instrument");
#endif
    }
}
