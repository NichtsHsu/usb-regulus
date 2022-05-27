#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString MainWindow::_textBroswerCss;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _translator(nullptr)
{
    ui->setupUi(this);

    ui_mainBrowser = new QTextBrowser;
    ui_widgetRightSplitter = new QSplitter(Qt::Vertical);
    ui->widgetRight->layout()->addWidget(ui_widgetRightSplitter);
    ui_widgetRightSplitter->addWidget(ui_mainBrowser);
    ui_widgetRightSplitter->addWidget(&log());
    ui_widgetRightSplitter->setStretchFactor(0, 4);
    ui_widgetRightSplitter->setStretchFactor(1, 1);
    ui->splitterMain->setStretchFactor(0, 1);
    ui->splitterMain->setStretchFactor(1, 3);

    ui_mainBrowser->document()->setDefaultStyleSheet(_textBroswerCss);
    ui_mainBrowser->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    connect(qGuiApp, &QGuiApplication::lastWindowClosed, qApp, &QApplication::quit, Qt::QueuedConnection);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit, Qt::QueuedConnection);
    connect(ui->actionRefresh, &QAction::triggered, this, [this]() {
        LOGI(tr("Rescaning devices."));
    });
    connect(ui->actionRefresh, &QAction::triggered, usb::UsbHost::instance(), &usb::UsbHost::rescan);
    connect(usb::UsbHost::instance(), &usb::UsbHost::deviceListRefreshed, this, &MainWindow::__refreshDeviceList);
    connect(usb::UsbHost::instance(), &usb::UsbHost::deviceAttached, this, &MainWindow::__insertDevice);
    connect(usb::UsbHost::instance(), &usb::UsbHost::deviceDetached, this, &MainWindow::__removeDevice);
    connect(ui->usbDeviceTreeView, &UsbDeviceTreeView::clicked, this, &MainWindow::__updateTextBrowser);
    connect(ui->actionDebug, &QAction::triggered, this, [this] () {
        this->__setLoggerLevel(Logger::Level::Debug);
    });
    connect(ui->actionInfo, &QAction::triggered, this, [this] () {
        this->__setLoggerLevel(Logger::Level::Info);
    });
    connect(ui->actionWarning, &QAction::triggered, this, [this] () {
        this->__setLoggerLevel(Logger::Level::Warning);
    });
    connect(ui->actionError, &QAction::triggered, this, [this] () {
        this->__setLoggerLevel(Logger::Level::Error);
    });
    connect(ui->actionLangEnUS, &QAction::triggered, this, [this] () {
        this->__loadTranslation("en_US");
    });
    connect(ui->actionLangZhCN, &QAction::triggered, this, [this] () {
        this->__loadTranslation("zh_CN");
    });

    __setLoggerLevel(Logger::Level::Info);
    usb::UsbHost::instance()->rescan();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::__refreshDeviceList()
{
    ui->usbDeviceTreeView->refresh(usb::UsbHost::instance()->usbDevices());
}

void MainWindow::__insertDevice(int index)
{
    /* Delay 1 second for loading string descriptor */
    QTimer::singleShot(1000, this, [this, index]() {
        this->ui->usbDeviceTreeView->insert(index, usb::UsbHost::instance()->usbDevices()[index]);
    });
}

void MainWindow::__removeDevice(int index)
{
    ui->usbDeviceTreeView->remove(index);
}

void MainWindow::__updateTextBrowser(const QModelIndex &index)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->usbDeviceTreeView->model());
    /* No parent, it is a UsbDeviceItem.
     * Otherwise, it is a UsbInterfaceItem.
     */
    if (index.parent() == QModelIndex())
    {
        /* QStandardItem was not declared with Q_OBJECT,
         * we can not use qobject_cast here.
         */
        UsbDeviceItem *item = dynamic_cast<UsbDeviceItem *>(model->itemFromIndex(index));
        ui_mainBrowser->setHtml(item->device()->infomationToHtml());
    }
    else
    {
        /* QStandardItem was not declared with Q_OBJECT,
         * we can not use qobject_cast here.
         */
        UsbInterfaceItem *item = dynamic_cast<UsbInterfaceItem *>(model->itemFromIndex(index));
        ui_mainBrowser->setHtml(item->interface()->infomationToHtml());
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    event->accept();
}

void MainWindow::__setLoggerLevel(Logger::Level level)
{
    log().setLogLevel(level);
    ui->actionDebug->setChecked(false);
    ui->actionInfo->setChecked(false);
    ui->actionWarning->setChecked(false);
    ui->actionError->setChecked(false);
    switch (level)
    {
        case Logger::Level::Debug:
            ui->actionDebug->setChecked(true);
        break;
        case Logger::Level::Info:
            ui->actionInfo->setChecked(true);
        break;
        case Logger::Level::Warning:
            ui->actionWarning->setChecked(true);
        break;
        case Logger::Level::Error:
            ui->actionError->setChecked(true);
        break;
    }
}

void MainWindow::__loadTranslation(const QString &lang)
{
    QTranslator *oldTranslator = nullptr;
    if (_translator)
        oldTranslator = _translator;

    _translator = new QTranslator();
    QString filename = QString(":/lang/usb-regulus.%1.qm").arg(lang);
    if (_translator->load(filename))
    {
        qApp->removeTranslator(oldTranslator);
        qApp->installTranslator(_translator);
        delete oldTranslator;
        LOGI(tr("Set language to %1.").arg(lang));
    }
    else
    {
        delete _translator;
        _translator = oldTranslator;
        LOGE(tr("Failed to load translation of language %1.").arg(lang));
    }
}
