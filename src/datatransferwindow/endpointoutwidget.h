/*! Sub-component of DataTransferWindow, responsible for writing data to OUT endpoint

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

#ifndef ENDPOINTOUTWIDGET_H
#define ENDPOINTOUTWIDGET_H

#include <functional>
#include <QWidget>
#include <QHideEvent>
#include <QMessageBox>
#include <usb/usbendpointdescriptor.h>
#include <usb/usbendpointwriter.h>
#include <qhexedit2/src/qhexedit.h>

namespace Ui {
    class EndpointOutWidget;
}

class EndpointOutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EndpointOutWidget(QWidget *parent = nullptr);
    ~EndpointOutWidget();

    void setEndpoint(usb::UsbEndpointDescriptor *epDesc);
    void hideEvent(QHideEvent *event) override;
    void changeEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *) override;

signals:
    void __writeOnceTriggered();
    void __keepWriteTriggered();

private slots:
    void __buttonWriteOnceReleased();
    void __buttonKeepWriteReleased();
    void __buttonClearCounterReleased();
    void __buttonResetDataReleased();
    void __resetButton();
    void __setOverwriteMode(bool overwrite);

private:
    Ui::EndpointOutWidget *ui;

    bool _writeOnce, _keepWrite;
    QThread *_workerThread;
    QHexEdit *_hexEdit;
    QBuffer _buffer;
    std::function<void()> _resetButtonAfterSafelyStop;
    usb::UsbEndpointDescriptor *_endpointDescriptor;
    usb::UsbEndpointWriter *_writer;
};

#endif // ENDPOINTOUTWIDGET_H
