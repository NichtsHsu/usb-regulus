#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _translator(nullptr)
{
    settings().loadFromIni(Tools::getConfigFilePath());

    ui->setupUi(this);

    ui_mainBrowser = new QTextBrowser;
    ui_mainBrowser->setObjectName("MainBrowser");
    ui_widgetRightSplitter = new QSplitter(Qt::Vertical);
    ui->widgetRight->layout()->addWidget(ui_widgetRightSplitter);
    ui_widgetRightSplitter->addWidget(ui_mainBrowser);
    ui_widgetRightSplitter->addWidget(Logger::instance());
    ui_widgetRightSplitter->setStretchFactor(0, 4);
    ui_widgetRightSplitter->setStretchFactor(1, 1);
    ui->splitterMain->setStretchFactor(0, 1);
    ui->splitterMain->setStretchFactor(1, 3);

    ui_mainBrowser->setContextMenuPolicy(Qt::CustomContextMenu);

    _mainBrowserMenu = new QMenu(ui_mainBrowser);
    _actionCopy = new QAction(tr("Copy"));
    _mainBrowserMenu->addAction(_actionCopy);
    _actionCopyAll = new QAction(tr("Copy All"));
    _mainBrowserMenu->addAction(_actionCopyAll);
    _actionCopyAllHtml = new QAction(tr("Copy All (HTML)"));
    _mainBrowserMenu->addAction(_actionCopyAllHtml);
    _actionCopyAllMarkdown = new QAction(tr("Copy All (Markdown)"));
    _mainBrowserMenu->addAction(_actionCopyAllMarkdown);

    _preferenceDialog = new PreferenceDialog;

    QStringList themes = Tools::getValidThemes();
    foreach (const QString &theme, themes)
    {
        QAction *action = new QAction(theme);
        connect(action, &QAction::triggered, this, [this, theme] () {
            this->__setTheme(theme);
        });
        ui->menuTheme->addAction(action);
    }

    _logLevelActionMap = {
        { Logger::Level::Debug, ui->actionDebug },
        { Logger::Level::Info, ui->actionInfo },
        { Logger::Level::Warning, ui->actionWarning },
        { Logger::Level::Error, ui->actionError },
    };

    __initWithSettings();

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
    connect(ui->actionPreferences, &QAction::triggered,
            this, &MainWindow::__openPreferenceDialog);
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
    connect(ui->actionThemeNone, &QAction::triggered, this, [this] () {
        this->__setTheme();
    });
    connect(ui_mainBrowser, &QTextBrowser::customContextMenuRequested,
            this, &MainWindow::__mainBrowserMenuShow);
    connect(_preferenceDialog, &PreferenceDialog::OkOrApplyClicked,
            this, &MainWindow::__preferenceDialogApply);

    usb::UsbHost::instance()->rescan();
}

MainWindow::~MainWindow()
{
    delete ui;
    _preferenceDialog->deleteLater();
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

void MainWindow::__openPreferenceDialog()
{
    _preferenceDialog->show();
}

void MainWindow::__preferenceDialogApply()
{
    _preferenceDialog->updateToSettings();
    __updateWithSettings();
}

void MainWindow::changeEvent(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            _actionCopy->setText(tr("Copy"));
            _actionCopyAll->setText(tr("Copy All"));
            _actionCopyAllHtml->setText(tr("Copy All (HTML)"));
            _actionCopyAllMarkdown->setText(tr("Copy All (Markdown)"));
            if (_currentDisplayedDeviceItem)
                ui_mainBrowser->setHtml(_currentDisplayedDeviceItem.get()->device()->infomationToHtml());
            else if (_currentDisplayedInterfaceItem)
                ui_mainBrowser->setHtml(_currentDisplayedInterfaceItem.get()->interface()->infomationToHtml());
        break;
        default:
        break;
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    ui->usbDeviceTreeView->closeAllDataTransferWindow();
    settings().protectMouse() = ui->actionProtectMouse->isChecked();
    settings().protectKeyboard() = ui->actionProtectKeyboard->isChecked();
    settings().saveToIni(Tools::getConfigFilePath());
    if (settings().saveLogBeforeExit())
    {
        QDateTime dateTime = QDateTime::currentDateTime();
        log().toFile(settings().autoSaveLogDir() +
                     QString("/usb-regulus_") + dateTime.toString("yyyyMMdd_hhmmss") + QString(".txt"));
    }
    usb::UsbHost::instance()->deleteLater();
    qApp->quit();
    QMainWindow::closeEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (settings().mainwindowProperties().startState == WindowStartStates::LAST_POS_SIZE ||
        settings().mainwindowProperties().startState ==
        WindowStartStates::CENTER_LAST_SIZE)
        settings().mainwindowProperties().size = event->size();
    QMainWindow::resizeEvent(event);
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    if (settings().mainwindowProperties().startState == WindowStartStates::LAST_POS_SIZE)
        settings().mainwindowProperties().position = event->pos();
    QMainWindow::moveEvent(event);
}

void MainWindow::__initWithSettings()
{
    /* General */
    __loadTranslation(settings().language());
    __setLoggerLevel(settings().logLevel());
    /* Theme will be handled later */

    /* Start & Exit */
    QPoint defaultPos(0, 0);
    QSize defaultSize(1280, 720);
    switch (settings().mainwindowProperties().startState)
    {
        case WindowStartStates::LAST_POS_SIZE:
        case WindowStartStates::CUSTOM:
            resize(settings().mainwindowProperties().size.safeGet(defaultSize));
            move(settings().mainwindowProperties().position.safeGet(defaultPos));
        break;
        case WindowStartStates::MIN:
            setWindowState(Qt::WindowState::WindowMinimized);
        break;
        case WindowStartStates::MAX:
            setWindowState(Qt::WindowState::WindowMaximized);
        break;
        case WindowStartStates::CENTER_DEFAULT_SIZE:
            resize(defaultSize);
            move(frameGeometry().topLeft() + screen()->geometry().center() - frameGeometry().center());
        break;
        case WindowStartStates::CENTER_LAST_SIZE:
        case WindowStartStates::CENTER_CUSTOM_SIZE:
            resize(settings().mainwindowProperties().size.safeGet(defaultSize));
            move(frameGeometry().topLeft() + screen()->geometry().center() - frameGeometry().center());
        break;
    }
    if (settings().startWithRandomTheme())
    {
        QStringList themeList = Tools::getValidThemes();
        srand(time(nullptr));
        int randomThemeIndex = rand() % themeList.length();
        settings().theme() = themeList[randomThemeIndex];
    }
    __setTheme(settings().theme());

    /* Others */
    ui->actionProtectMouse->setChecked(settings().protectMouse());
    usb::UsbHost::instance()->setProtectMouse(settings().protectMouse());
    ui->actionProtectKeyboard->setChecked(settings().protectKeyboard());
    usb::UsbHost::instance()->setProtectKeyboard(settings().protectKeyboard());

    /* Font */
    /* Qt prohibits mixing `setStyleSheet` and `setFont`, */
    /* therefore fonts will be handled in `__FontToCss` */
}

void MainWindow::__updateWithSettings()
{
    /* General */
    __loadTranslation(settings().language());
    __setLoggerLevel(settings().logLevel());
    __setTheme(settings().theme());

    /* Start & Exit */
    /* Ignore this section */

    /* Font */
    /* Qt prohibits mixing `setStyleSheet` and `setFont`, */
    /* therefore fonts will be handled in `__FontToCss` */
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

void MainWindow::__setTheme(const QString &theme)
{
    QString qss = Tools::getThemeStyleSheet(theme) + __FontToCss();
    settings().theme() = theme;
    qApp->setStyleSheet(qss);
    LOGI(tr("Set theme to %1.").arg(theme));
}

void MainWindow::__loadTranslation(const QString &lang)
{
    /* If failed to load translation file, fallback to old translation */
    QTranslator *oldTranslator = nullptr;
    if (_translator)
        oldTranslator = _translator;

    _translator = new QTranslator();
    QString filename = QString(qApp->applicationDirPath() + "/assets/translations/usb-regulus.%1.qm").arg(lang);
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

QString MainWindow::__FontToCss()
{
    QString fontCss =
            "* {"
            "   font-family: \"%1\";"
            "   font-size: %2pt;"
            "   font-style: %3;"
            "   font-weight: %4;"
            "}"
            "QTextBrowser#MainBrowser {"
            "   font-family: \"%5\";"
            "   font-size: %6pt;"
            "   font-style: %7;"
            "   font-weight: %8;"
            "}";
    return fontCss
            .arg(settings().uiFont().family())
            .arg(settings().uiFont().pointSize())
            .arg(settings().uiFont().italic() ? "italic" : "normal")
            .arg(settings().uiFont().weight())
            .arg(settings().monospacedFont().family())
            .arg(settings().monospacedFont().pointSize())
            .arg(settings().monospacedFont().italic() ? "italic" : "normal")
            .arg(settings().monospacedFont().weight());
}
