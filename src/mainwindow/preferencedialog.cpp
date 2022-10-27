#include "preferencedialog.h"
#include "ui_preferencedialog.h"
#include "global/settings.h"
#include "global/tools.h"
#include "log/logger.h"
#include <QMap>
#include <QFileDialog>
#include <QDir>

Q_GLOBAL_STATIC_WITH_ARGS(QStringList, AVALID_LANGUAGES, (
                              { "en_US", "zh_CN" }));

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, [this] (QAbstractButton *button) {
        switch (ui->buttonBox->buttonRole(button))
        {
            case QDialogButtonBox::AcceptRole:
            case QDialogButtonBox::ApplyRole:
                emit OkOrApplyClicked();
            break;
            default:
            break;
        };
    });
    connect(ui->comboBoxMainWindowStartState, &QComboBox::currentIndexChanged,
            this, &PreferenceDialog::__updateWindowPropTextEdit);
    connect(ui->checkBoxSaveExit, &QCheckBox::clicked,
            ui->lineEditLogDir, [this] (bool checked) {
        ui->lineEditLogDir->setReadOnly(!checked);
    });
    connect(ui->checkBoxSaveExit, &QCheckBox::clicked,
            ui->toolButtonLogDir, &QToolButton::setEnabled);
    connect(ui->toolButtonLogDir, &QToolButton::released,
            this, &PreferenceDialog::__openSelectLogDirDiag);
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::showEvent(QShowEvent *event)
{
    updateFromSettings();
    event->accept();
}

void PreferenceDialog::updateFromSettings()
{
    QStringList themes = Tools::getValidThemes();
    const static QPoint origin(0, 0);
    const static QSize defaultSize(1280, 720);

    /* General */
    ui->comboBoxLanguage->setCurrentIndex(AVALID_LANGUAGES->indexOf(settings().language()));
    ui->comboBoxLogLevel->setCurrentIndex(int(log().logLevel()));
    ui->comboBoxTheme->addItems(Tools::getValidThemes());
    ui->comboBoxTheme->setCurrentIndex(themes.indexOf(settings().theme()));

    /* Start & Exit */
    ui->comboBoxMainWindowStartState->setCurrentIndex(int(settings().mainwindowProperties().startState));
    __updateWindowPropTextEdit(int(settings().mainwindowProperties().startState));
    ui->lineEditLeft->setText(QString::number(settings().mainwindowProperties().position.safeGet(origin).x()));
    ui->lineEditTop->setText(QString::number(settings().mainwindowProperties().position.safeGet(origin).y()));
    ui->lineEditWidth->setText(QString::number(settings().mainwindowProperties().size.safeGet(defaultSize).width()));
    ui->lineEditHieght->setText(QString::number(settings().mainwindowProperties().size.safeGet(defaultSize).height()));

    ui->checkBoxSaveExit->setChecked(settings().saveLogBeforeExit());
    ui->lineEditLogDir->setText(settings().autoSaveLogDir());
    ui->lineEditLogDir->setReadOnly(!settings().saveLogBeforeExit());
    ui->toolButtonLogDir->setEnabled(settings().saveLogBeforeExit());

    ui->checkBoxRandomTheme->setChecked(settings().startWithRandomTheme());

    /* Font */
    int index;
    ui->fontComboBoxUI->setCurrentFont(settings().uiFont());
    ui->comboBoxFontStyleUI->setCurrentIndex(int(settings().uiFont().bold()) |
                                             (int(settings().uiFont().italic()) << 1));
    if ((index = ui->comboBoxFontSizeUI->findText(QString::number(settings().uiFont().pointSize()))) >= 0)
        ui->comboBoxFontSizeUI->setCurrentIndex(index);
    else
        ui->comboBoxFontSizeUI->setCurrentIndex(5);
    ui->fontComboBoxMono->setCurrentFont(settings().monospacedFont());
    ui->comboBoxFontStyleMono->setCurrentIndex(int(settings().monospacedFont().bold()) |
                                               (int(settings().monospacedFont().italic()) << 1));
    if ((index = ui->comboBoxFontSizeMono->findText(QString::number(settings().monospacedFont().pointSize()))) >= 0)
        ui->comboBoxFontSizeMono->setCurrentIndex(index);
    else
        ui->comboBoxFontSizeMono->setCurrentIndex(5);
}

void PreferenceDialog::updateToSettings()
{
    /* General */
    settings().language() = AVALID_LANGUAGES->at(ui->comboBoxLanguage->currentIndex());
    settings().logLevel() = Logger::Level(ui->comboBoxLogLevel->currentIndex());
    settings().theme() = ui->comboBoxTheme->currentText();

    /* Start & Exit */
    QPoint position;
    QSize size;
    settings().mainwindowProperties().startState = WindowStartStates(ui->comboBoxMainWindowStartState->currentIndex());
    switch (settings().mainwindowProperties().startState)
    {
        case WindowStartStates::MIN:
        case WindowStartStates::MAX:
        case WindowStartStates::CENTER_DEFAULT_SIZE:
            settings().mainwindowProperties().position.setNone();
            settings().mainwindowProperties().size.setNone();
        break;
        case WindowStartStates::CENTER_LAST_SIZE:
            settings().mainwindowProperties().position.setNone();
        break;
        case WindowStartStates::CENTER_CUSTOM_SIZE:
            settings().mainwindowProperties().position.setNone();
            size.setWidth(ui->lineEditWidth->text().toInt());
            size.setHeight(ui->lineEditHieght->text().toInt());
            settings().mainwindowProperties().size.set(size);
        break;
        case WindowStartStates::CUSTOM:
            position.setX(ui->lineEditLeft->text().toInt());
            position.setY(ui->lineEditTop->text().toInt());
            settings().mainwindowProperties().position.set(position);
            size.setWidth(ui->lineEditWidth->text().toInt());
            size.setHeight(ui->lineEditHieght->text().toInt());
            settings().mainwindowProperties().size.set(size);
        break;
        case WindowStartStates::LAST_POS_SIZE:
        break;
    }

    settings().saveLogBeforeExit() = ui->checkBoxSaveExit->isChecked();
    if (settings().saveLogBeforeExit())
    {
        settings().autoSaveLogDir() = ui->lineEditLogDir->text();
        QDir logDir(settings().autoSaveLogDir());
        if (!logDir.exists())
        {
            if (!logDir.mkpath("."))
            {
                LOGW(tr("Failed to create config directory \"%1\", fallback to \"%2\".")
                     .arg(settings().autoSaveLogDir())
                     .arg(Tools::getConfigFilePath()));
                settings().autoSaveLogDir() = Tools::getConfigFilePath();
            }
        }
    }

    settings().startWithRandomTheme() = ui->checkBoxRandomTheme->isChecked();

    /* Font */
    settings().uiFont() = ui->fontComboBoxUI->currentFont();
    settings().uiFont().setBold(ui->comboBoxFontStyleUI->currentIndex() & 0b01);
    settings().uiFont().setItalic(ui->comboBoxFontStyleUI->currentIndex() & 0b10);
    settings().uiFont().setPointSize(ui->comboBoxFontSizeUI->currentText().toInt());
    settings().monospacedFont() = ui->fontComboBoxMono->currentFont();
    settings().monospacedFont().setBold(ui->comboBoxFontStyleMono->currentIndex() & 0b01);
    settings().monospacedFont().setItalic(ui->comboBoxFontStyleMono->currentIndex() & 0b10);
    settings().monospacedFont().setPointSize(ui->comboBoxFontSizeMono->currentText().toInt());
}

void PreferenceDialog::__updateWindowPropTextEdit(int index)
{
    switch (WindowStartStates(index))
    {
        case WindowStartStates::LAST_POS_SIZE:
        case WindowStartStates::MIN:
        case WindowStartStates::MAX:
        case WindowStartStates::CENTER_DEFAULT_SIZE:
        case WindowStartStates::CENTER_LAST_SIZE:
            ui->lineEditLeft->setReadOnly(true);
            ui->lineEditTop->setReadOnly(true);
            ui->lineEditWidth->setReadOnly(true);
            ui->lineEditHieght->setReadOnly(true);
        break;
        case WindowStartStates::CENTER_CUSTOM_SIZE:
            ui->lineEditLeft->setReadOnly(true);
            ui->lineEditTop->setReadOnly(true);
            ui->lineEditWidth->setReadOnly(false);
            ui->lineEditHieght->setReadOnly(false);
        break;
        case WindowStartStates::CUSTOM:
            ui->lineEditLeft->setReadOnly(false);
            ui->lineEditTop->setReadOnly(false);
            ui->lineEditWidth->setReadOnly(false);
            ui->lineEditHieght->setReadOnly(false);
        break;
    }
}

void PreferenceDialog::__openSelectLogDirDiag()
{
    QString dir = QFileDialog::getExistingDirectory(
                nullptr, "Select Directory to Save Log Files", Tools::getConfigFolder());
    if (!dir.isEmpty())
        ui->lineEditLogDir->setText(dir);
}

void PreferenceDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    event->accept();
}
