/*! The data transfer window is designed to read from or write to endpoints.

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

#ifndef DATATRANSFERWINDOW_H
#define DATATRANSFERWINDOW_H

#include <QWidget>
#include <QShowEvent>
#include <QCloseEvent>
#include <usb/usbdevice.h>
#include <usb/usbinterface.h>
#include <usb/usbendpointwriter.h>
#include <global/tools.h>

#include "endpointinwidget.h"
#include "endpointoutwidget.h"
#include "controltransferwidget.h"

namespace Ui {
    class DataTransferWindow;
}

class DataTransferWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DataTransferWindow(QWidget *parent = nullptr);
    ~DataTransferWindow();

    void setInterface(usb::UsbInterface *interface);
    void setDevice(usb::UsbDevice *device);

    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    Ui::DataTransferWindow *ui;

    usb::UsbDevice *_device;
    usb::UsbInterface *_interface;
    bool _isControlTransfer;
    uint8_t _currentAltsetting;
};

#endif // DATATRANSFERWINDOW_H
