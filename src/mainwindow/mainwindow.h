/*! The main window of usb-regulus

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QGuiApplication>
#include <QLayout>
#include <QTextBrowser>
#include <QSplitter>
#include <QTranslator>
#include <QFontDatabase>
#include <QCloseEvent>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QMap>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QMutex>

#include <log/logger.h>
#include <usb/usbhost.h>
#include <global/settings.h>
#include <global/tools.h>
#include <misc/option.hpp>

#include "usbdevicetreeview.h"
#include "preferencedialog.h"
#include "aboutdialog.h"
#include "fieldquickreference.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private slots:
    /**
     * @brief __refreshDeviceList
     * This slot is a wrapper of UsbDeviceTreeView::refresh()
     * @see UsbDeviceTreeView::refresh()
     */
    void __refreshDeviceList();

    /**
     * @brief __insertDevice
     * This slot is a wrapper of UsbDeviceTreeView::insert()
     * @see UsbDeviceTreeView::insert()
     */
    void __insertDevice(usb::UsbDevice *device, int index);

    /**
     * @brief __removeDevice
     * This slot is a wrapper of UsbDeviceTreeView::remove()
     * @see UsbDeviceTreeView::remove()
     */
    void __removeDevice(usb::UsbDevice *device, int index);

    /**
     * @brief __updateTextBrowser
     * This slot is connected with UsbDeviceTreeView::clicked(),
     * to display the information of USB devices or interfaces.
     * @param index
     * Index of the clicked item
     * @param ignoreSame
     * If new item is the same as current item, ignore update.
     */
    void __updateTextBrowser(const QModelIndex &index, bool ignoreSame = false);

    /**
     * @brief __mainBrowserMenuShow
     * Show menu of main browser.
     */
    void __mainBrowserMenuShow(const QPoint &point);

    /**
     * @brief __openPreferenceDialog
     * Open prefrence dialog.
     */
    void __openPreferenceDialog();

    /**
     * @brief __preferenceDialogApply
     * When click `Ok` or `Apply` in the preference dialog.
     */
    void __preferenceDialogApply();

    /**
     * @brief __openFieldQuickReference
     * Open Field Quick Reference Dialog.
     */
    void __openFieldQuickReference();

    /**
     * @brief __openAboutDialog
     * Open about dialog.
     */
    void __openAboutDialog();

private:
    void __initWithSettings();
    void __updateWithSettings();
    void __setLoggerLevel(Logger::Level level);
    void __setTheme(const QString &theme = "(none)");
    void __loadTranslation(const QString &lang);
    static QString __extraQss();

    Ui::MainWindow *ui;

    QTextBrowser *ui_mainBrowser;
    QSplitter *ui_widgetRightSplitter;
    QTranslator *_translator;
    QMap<Logger::Level, QAction *> _logLevelActionMap;
    QMenu *_mainBrowserMenu;
    QAction *_actionCopy, *_actionCopyAll, *_actionCopyAllHtml, *_actionCopyAllMarkdown;
    Option<UsbDeviceItem *> _currentDisplayedDeviceItem;
    Option<UsbInterfaceItem *> _currentDisplayedInterfaceItem;
    QSet<usb::UsbDevice *> _insertDeviceNotReady;
    QMutex _deviceTreeViewMutex;
    PreferenceDialog *_preferenceDialog;
    AboutDialog *_aboutDialog;
    FieldQuickReference *_quickReference;
};
#endif // MAINWINDOW_H
