/*! Sub-component of DataTransferWindow, responsible for transferring data from control endpoint

 * Copyright (C) 2022 Nichts Hsu

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CONTROLTRANSFERWIDGET_H
#define CONTROLTRANSFERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QStringList>
#include <QStringListModel>
#include <QInputDialog>
#include <QMessageBox>
#include <QThread>
#include <QShowEvent>
#include <qhexedit2/src/qhexedit.h>
#include <usb/usbcontroltransferhandler.h>

namespace Ui {
    class ControlTransferWidget;
}

class ControlTransferWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlTransferWidget(QWidget *parent = nullptr);
    ~ControlTransferWidget();

    void setDevice(usb::UsbDevice *device);

    void showEvent(QShowEvent *event);

private slots:
    void __bmRequestUseRawHex(bool useRawHex);
    void __bRequestUseRawHex(bool useRawHex);
    void __updateBRequestList();
    void __updateBmRequest();
    void __updateBRequest();
    void __setOverwriteMode(bool overwrite);
    void __clearAll();
    void __buttonSendReleased();
    void __buttonSetBufferSizeReleased();
    void __resetButtons();
    void __readData();

signals:
    void __sendTriggered();

private:
    inline uint8_t __calcBmRequest(bool &isValid) const;
    inline uint8_t __calcBRequest(bool &isValid) const;
    static const QMap<QString, uint8_t> bRequestCodeMap;
    static const QStringList standardBRequestTable[3][2];

    Ui::ControlTransferWidget *ui;

    usb::UsbDevice *_device;
    QHexEdit *_hexEditSend, *_hexEditReceive;
    QStringListModel *_comboxBRequestModel;
    QBuffer _sendBuffer, _receiveBuffer;
    usb::UsbControlTransferHandler *_handler;
    QThread _workerThread;
    bool _transfer;
};

#endif // CONTROLTRANSFERWIDGET_H
