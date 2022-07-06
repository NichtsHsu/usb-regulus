#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString MainWindow::_textBroswerCss;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _translator(nullptr)
{
    settings().loadFromIni(Tools::getConfigFilePath());

    ui->setupUi(this);

    ui_mainBrowser = new QTextBrowser;
    ui_widgetRightSplitter = new QSplitter(Qt::Vertical);
    ui->widgetRight->layout()->addWidget(ui_widgetRightSplitter);
    ui_widgetRightSplitter->addWidget(ui_mainBrowser);
    ui_widgetRightSplitter->addWidget(Logger::instance());
    ui_widgetRightSplitter->setStretchFactor(0, 4);
    ui_widgetRightSplitter->setStretchFactor(1, 1);
    ui->splitterMain->setStretchFactor(0, 1);
    ui->splitterMain->setStretchFactor(1, 3);

    ui_mainBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    ui_mainBrowser->document()->setDefaultStyleSheet(_textBroswerCss);
    ui_mainBrowser->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    _mainBrowserMenu = new QMenu(ui_mainBrowser);
    _actionCopy = new QAction(tr("Copy"));
    _mainBrowserMenu->addAction(_actionCopy);
    _actionCopyAll = new QAction(tr("Copy All"));
    _mainBrowserMenu->addAction(_actionCopyAll);
    _actionCopyAllHtml = new QAction(tr("Copy All (HTML)"));
    _mainBrowserMenu->addAction(_actionCopyAllHtml);
    _actionCopyAllMarkdown = new QAction(tr("Copy All (Markdown)"));
    _mainBrowserMenu->addAction(_actionCopyAllMarkdown);

    _logLevelActionMap = {
        { Logger::Level::Debug, ui->actionDebug },
        { Logger::Level::Info, ui->actionInfo },
        { Logger::Level::Warning, ui->actionWarning },
        { Logger::Level::Error, ui->actionError },
    };

    __initWithSettings();

    connect(qGuiApp, &QGuiApplication::lastWindowClosed,
            qApp, &QApplication::quit, Qt::QueuedConnection);
    connect(ui->actionExit, &QAction::triggered,
            qApp, &QApplication::quit, Qt::QueuedConnection);
    connect(ui->actionRefresh, &QAction::triggered, this, [this]() {
        LOGI(tr("Rescaning devices."));
    });
    connect(ui->actionRefresh, &QAction::triggered,
            usb::UsbHost::instance(), &usb::UsbHost::rescan);
    connect(usb::UsbHost::instance(), &usb::UsbHost::deviceListRefreshed,
            this, &MainWindow::__refreshDeviceList);
    connect(usb::UsbHost::instance(), &usb::UsbHost::deviceAttached,
            this, &MainWindow::__insertDevice);
    connect(usb::UsbHost::instance(), &usb::UsbHost::deviceDetached,
            this, &MainWindow::__removeDevice);
    connect(ui->usbDeviceTreeView, &UsbDeviceTreeView::currentIndexChanged,
            this, &MainWindow::__updateTextBrowser);
    connect(ui->actionProtectMouse, &QAction::triggered,
            usb::UsbHost::instance(), &usb::UsbHost::setProtectMouse);
    connect(ui->actionProtectKeyboard, &QAction::triggered,
            usb::UsbHost::instance(), &usb::UsbHost::setProtectKeyboard);
    connect(ui->actionDebug, &QAction::triggered, this, [this] () {
        __setLoggerLevel(Logger::Level::Debug);
    });
    connect(ui->actionInfo, &QAction::triggered, this, [this] () {
        __setLoggerLevel(Logger::Level::Info);
    });
    connect(ui->actionWarning, &QAction::triggered, this, [this] () {
        __setLoggerLevel(Logger::Level::Warning);
    });
    connect(ui->actionError, &QAction::triggered, this, [this] () {
        __setLoggerLevel(Logger::Level::Error);
    });
    connect(ui->actionLangEnUS, &QAction::triggered, this, [this] () {
        __loadTranslation("en_US");
    });
    connect(ui->actionLangZhCN, &QAction::triggered, this, [this] () {
        __loadTranslation("zh_CN");
    });
    connect(ui->actionCenterTheWindow, &QAction::triggered, this, [this] () {
        move(frameGeometry().topLeft() + screen()->geometry().center() - frameGeometry().center());
    });
    connect(_actionCopy, &QAction::triggered, this, [this] () {
        qGuiApp->clipboard()->setText(ui_mainBrowser->textCursor().selectedText());
    });
    connect(_actionCopyAll, &QAction::triggered, this, [this] () {
        qGuiApp->clipboard()->setText(ui_mainBrowser->toPlainText());
    });
    connect(_actionCopyAllHtml, &QAction::triggered, this, [this] () {
        qGuiApp->clipboard()->setText(ui_mainBrowser->toHtml());
    });
    connect(_actionCopyAllMarkdown, &QAction::triggered, this, [this] () {
        qGuiApp->clipboard()->setText(ui_mainBrowser->toMarkdown());
    });
    connect(ui_mainBrowser, &QTextBrowser::customContextMenuRequested,
            this, &MainWindow::__mainBrowserMenuShow);

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

void MainWindow::__insertDevice(usb::UsbDevice *device, int index)
{
    /* Delay 1 second for loading string descriptor */
    QTimer::singleShot(1000, this, [this, index, device]() {
        this->ui->usbDeviceTreeView->insert(index, device);
    });
}

void MainWindow::__removeDevice(usb::UsbDevice *device, int index)
{
    ui->usbDeviceTreeView->remove(index);
    if (_currentDisplayedDeviceItem)
    {
        if (_currentDisplayedDeviceItem.get()->device() == device)
        {
            ui_mainBrowser->clear();
            _currentDisplayedDeviceItem.setNone();
        }
    }
    else if (_currentDisplayedInterfaceItem)
        for (uint8_t i = 0; i < device->configurationDescriptor()->bNumInterfaces(); ++i)
            if (_currentDisplayedInterfaceItem.get()->interface() ==
                    device->configurationDescriptor()->interface(i))
            {
                ui_mainBrowser->clear();
                _currentDisplayedInterfaceItem.setNone();
                break;
            }
}

void MainWindow::__updateTextBrowser(const QModelIndex &index, bool ignoreSame)
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
        if (ignoreSame && _currentDisplayedDeviceItem)
            if (_currentDisplayedDeviceItem.get() == item)
                return;

        _currentDisplayedInterfaceItem.setNone();
        ui_mainBrowser->setHtml(item->device()->infomationToHtml());
        _currentDisplayedDeviceItem.set(item);
    }
    else
    {
        /* QStandardItem was not declared with Q_OBJECT,
         * we can not use qobject_cast here.
         */
        UsbInterfaceItem *item = dynamic_cast<UsbInterfaceItem *>(model->itemFromIndex(index));
        if (ignoreSame && _currentDisplayedInterfaceItem)
            if (_currentDisplayedInterfaceItem.get() == item)
                return;

        _currentDisplayedDeviceItem.setNone();
        ui_mainBrowser->setHtml(item->interface()->infomationToHtml());
        _currentDisplayedInterfaceItem.set(item);
    }
}

void MainWindow::__mainBrowserMenuShow(const QPoint &point)
{
    _mainBrowserMenu->exec(ui_mainBrowser->viewport()->mapToGlobal(point));
}

void MainWindow::changeEvent(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            _actionCopyAll->setText(tr("Copy All"));
            _actionCopyAllHtml->setText(tr("Copy All (HTML)"));
            _actionCopyAllMarkdown->setText(tr("Copy All (Markdown)"));
            if (_currentDisplayedDeviceItem)
                ui_mainBrowser->setHtml(_currentDisplayedDeviceItem.get()->device()->infomationToHtml());
            else if (_currentDisplayedInterfaceItem)
                ui_mainBrowser->setHtml(_currentDisplayedInterfaceItem.get()->interface()->infomationToHtml());
        break;
        case QEvent::WindowStateChange:
            settings().mainwindowProperties().state = windowState();
        break;

        default:
        break;
    }
    event->accept();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    ui->usbDeviceTreeView->closeAllDataTransferWindow();
    settings().protectMouse() = ui->actionProtectMouse->isChecked();
    settings().protectKeyboard() = ui->actionProtectKeyboard->isChecked();
    settings().saveToIni(Tools::getConfigFilePath());
    usb::UsbHost::instance()->deleteLater();
    event->accept();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    settings().mainwindowProperties().size = event->size();
    event->accept();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    settings().mainwindowProperties().position = event->pos();
    event->accept();
}

void MainWindow::__initWithSettings()
{
    if (settings().mainwindowProperties().state)
        setWindowState(settings().mainwindowProperties().state.get());
    if (settings().mainwindowProperties().size)
        resize(settings().mainwindowProperties().size.get());
    if (settings().mainwindowProperties().position)
        move(settings().mainwindowProperties().position.get());
    ui->actionProtectMouse->setChecked(settings().protectMouse());
    usb::UsbHost::instance()->setProtectMouse(settings().protectMouse());
    ui->actionProtectKeyboard->setChecked(settings().protectKeyboard());
    usb::UsbHost::instance()->setProtectKeyboard(settings().protectKeyboard());

    __loadTranslation(settings().language());
    __setLoggerLevel(settings().logLevel());
}

void MainWindow::__setLoggerLevel(Logger::Level level)
{
    log().setLogLevel(level);
    ui->actionDebug->setChecked(false);
    ui->actionInfo->setChecked(false);
    ui->actionWarning->setChecked(false);
    ui->actionError->setChecked(false);
    _logLevelActionMap[level]->setChecked(true);
    settings().logLevel() = level;
}

void MainWindow::__loadTranslation(const QString &lang)
{
    /* If failed to load translation file, fallback to old translation */
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
        settings().language() = lang;
    }
    else
    {
        delete _translator;
        _translator = oldTranslator;
        LOGE(tr("Failed to load translation of language %1.").arg(lang));
    }
}
