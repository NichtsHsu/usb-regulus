#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QGuiApplication>
#include <QLayout>
#include <QTextBrowser>
#include <QSplitter>

#include <log/logger.h>
#include <usb/usbhost.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    void __insertDevice(int index);

    /**
     * @brief __removeDevice
     * This slot is a wrapper of UsbDeviceTreeView::remove()
     * @see UsbDeviceTreeView::remove()
     */
    void __removeDevice(int index);

private:
    Ui::MainWindow *ui;

    QTextBrowser *ui_mainBrowser;
    QSplitter *ui_widgetRightSplitter;
};
#endif // MAINWINDOW_H
