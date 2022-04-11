#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("usb-regulus");

    ui_mainBrowser = new QTextBrowser;
    ui_widgetRightSplitter = new QSplitter(Qt::Vertical);
    ui->widgetRight->layout()->addWidget(ui_widgetRightSplitter);
    ui_widgetRightSplitter->addWidget(ui_mainBrowser);
    ui_widgetRightSplitter->addWidget(&log());
    ui_widgetRightSplitter->setStretchFactor(0, 4);
    ui_widgetRightSplitter->setStretchFactor(1, 1);
    ui->splitterMain->setStretchFactor(0, 1);
    ui->splitterMain->setStretchFactor(1, 3);

    connect(qGuiApp, &QGuiApplication::lastWindowClosed, qApp, &QApplication::quit, Qt::QueuedConnection);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit, Qt::QueuedConnection);
    connect(ui->actionRefresh, &QAction::triggered, this, [this]() {
        LOGI(tr("Rescaning devices."));
    });
    connect(ui->actionRefresh, &QAction::triggered, usb::UsbHost::instance(), &usb::UsbHost::rescan);
    connect(usb::UsbHost::instance(), &usb::UsbHost::deviceListRefreshed, this, &MainWindow::__refreshDeviceList);
    connect(usb::UsbHost::instance(), &usb::UsbHost::deviceAttached, this, &MainWindow::__insertDevice);
    connect(usb::UsbHost::instance(), &usb::UsbHost::deviceDetached, this, &MainWindow::__removeDevice);

    usb::UsbHost::instance()->rescan();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::__refreshDeviceList()
{
    ui->treeView->refresh(usb::UsbHost::instance()->usbDevices());
}

void MainWindow::__insertDevice(int index)
{
    ui->treeView->insert(index, usb::UsbHost::instance()->usbDevices()[index]);
}

void MainWindow::__removeDevice(int index)
{
    ui->treeView->remove(index);
}

