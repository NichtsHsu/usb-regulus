/*! Sub-component of DataTransferWindow, responsible for reading data from IN endpoint

 * Copyright (C) 2022-2023 Nichts Hsu

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

#ifndef ENDPOINTINWIDGET_H
#define ENDPOINTINWIDGET_H

#include <functional>
#include <QWidget>
#include <QThread>
#include <QBuffer>
#include <QHideEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <usb/usbendpointdescriptor.h>
#include <usb/usbendpointreader.h>
#include <qhexedit2/src/qhexedit.h>

namespace Ui {
    class EndpointInWidget;
}

class EndpointInWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EndpointInWidget(QWidget *parent = nullptr);
    ~EndpointInWidget();

    void setEndpoint(usb::UsbEndpointDescriptor *epDesc);
    void changeEvent(QEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void paintEvent(QPaintEvent *) override;

signals:
    void __readOnceTriggered();
    void __keepReadTriggered();

private slots:
    void __buttonReadOnceReleased();
    void __buttonKeepReadReleased();
    void __buttonSetBufferSizeReleased();
    void __setDataReady();
    void __resetButtons();
    void __stopRead();

private:
    void __updateData();

    Ui::EndpointInWidget *ui;

    bool _dataReady, _readOnce, _keepRead;
    QThread *_workerThread;
    QTimer *_eventLoopTimer;
    QHexEdit *_hexEdit;
    QBuffer _buffer;
    std::function<void()> _resetButtonAfterSafelyStop;
    usb::UsbEndpointDescriptor *_endpointDescriptor;
    usb::UsbEndpointReader *_reader;
};

#endif // ENDPOINTINWIDGET_H
