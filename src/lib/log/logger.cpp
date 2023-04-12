#include "logger.h"
#include "savelogdialog.h"
#include <QFile>
#include <QTimer>
#include <QGuiApplication>
#include <QTextStream>
#include <QClipboard>

Logger::Logger(QWidget *parent):
    QTextBrowser(parent), _logLevel(Level::Info)
{
    _menu = new QMenu(this);
    _actionCopy = new QAction(tr("Copy"));
    _menu->addAction(_actionCopy);
    _actionCopyAll = new QAction(tr("Copy All"));
    _menu->addAction(_actionCopyAll);
    _actionSave = new QAction(tr("Save to File"));
    _menu->addAction(_actionSave);
    _menu->addSeparator();
    _actionClear = new QAction(tr("Clear"));
    _menu->addAction(_actionClear);

    document()->setMaximumBlockCount(500);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &Logger::customContextMenuRequested,
            this, &Logger::__customMenu);
    connect(_actionCopy, &QAction::triggered, this, [this] () {
        qGuiApp->clipboard()->setText(textCursor().selectedText());
    });
    connect(_actionCopyAll, &QAction::triggered, this, [this] () {
        qGuiApp->clipboard()->setText(toPlainText());
    });
    connect(_actionSave, &QAction::triggered,
            this, &Logger::__actionSaveFile);
    connect(_actionClear, &QAction::triggered,
            this, &Logger::clear);
}

QString Logger::__strLogLevel()
{
    switch (_logLevel)
    {
        case Logger::Level::Debug:
        return tr("Debug");
        break;
        case Logger::Level::Info:
        return tr("Info");
        break;
        case Logger::Level::Warning:
        return tr("Warning");
        break;
        case Logger::Level::Error:
        return tr("Error");
        break;
        default:
        return QString::number(int(_logLevel));
    }
}

void Logger::__record(Level type, const QString &module, const QString &message)
{
    std::lock_guard<QMutex> lock{ _writeMutex };
    QString time = QTime::currentTime().toString();

    if (_logLevel <= type) {
        QString messageHtml = QString("<span style=\"color: purple;\">") + time + QString("</span> ");
        switch(type)
        {
            case Level::Debug:
                messageHtml += QString("<b>[ <span style=\"color: green;\">") + tr("Debug") + QString("</span> ]</b> ");
            break;

            case Level::Info:
                messageHtml += QString("<b>[ <span style=\"color: blue;\">") + tr("Info") + QString("</span> ]</b> ");
            break;

            case Level::Warning:
                messageHtml += QString("<b>[ <span style=\"color: orange;\">") + tr("Warning") + QString("</span> ]</b> ");
            break;

            case Level::Error:
                messageHtml += QString("<b>[ <span style=\"color: red;\">") + tr("Error") + QString("</span> ]</b> ");
            break;
        }

        messageHtml += QString("<span style=\"color: purple;\"><u>") + module + QString("</u></span> :");
        messageHtml += message + QString("<br />");

        moveCursor(QTextCursor::End);
        textCursor().insertHtml(messageHtml);
    }

    _backup.append(Logger::BackupBlock{type, module, message, time});
}

std::atomic<Logger *> Logger::_instance{nullptr};
Q_GLOBAL_STATIC(QMutex, __mutW);

Logger *Logger::instance()
{
    if (Logger::_instance == nullptr)
    {
        std::lock_guard<QMutex> lock{ *__mutW };
        if (Logger::_instance == nullptr)
            Logger::_instance = new Logger;
    }
    return Logger::_instance;
}

void Logger::changeEvent(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::LanguageChange:
            _actionCopy->setText(tr("Copy"));
            _actionCopyAll->setText(tr("Copy All"));
            _actionSave->setText(tr("Save to File"));
            _actionClear->setText(tr("Clear"));
        break;
        default:
        break;
    }
    QTextBrowser::changeEvent(event);
}

Logger::Level Logger::logLevel() const
{
    return _logLevel;
}

void Logger::record(Logger::Level type, const QString &module, const QString &message)
{
    QTimer::singleShot(0, this, [this, type, module, message] () {
        __record(type, module, message);
    });
}

void Logger::d(const QString &module, const QString &message)
{
    record(Level::Debug, module, message);
}

void Logger::i(const QString &module, const QString &message)
{
    record(Level::Info, module, message);
}

void Logger::w(const QString &module, const QString &message)
{
    record(Level::Warning, module, message);
}

void Logger::e(const QString &module, const QString &message)
{
    record(Level::Error, module, message);
}

void Logger::setLogLevel(Logger::Level logLevel)
{
    _logLevel = logLevel;
    LOGI(tr("Set log level %1.").arg(__strLogLevel()));
}

void Logger::toFile(const QString &filePath, Logger::Level logLevel)
{
    std::lock_guard<QMutex> lock{_writeMutex};
    QFile file(filePath);
    if(!file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate))
        record(Level::Error, "Logger", tr("Failed to export log because cannot open file \"%1\" to write.").arg(filePath));

    QTextStream qts(&file);
    foreach(const auto &block, _backup)
        if (logLevel <= block.type) {
            QString message = block.time + " ";
            switch(block.type)
            {
                case Level::Debug:
                    message += QString("[ ") + tr("Debug") + QString(" ] ");
                break;

                case Level::Info:
                    message += QString("[ ") + tr("Info") + QString(" ] ");
                break;

                case Level::Warning:
                    message += QString("[ ") + tr("Warning") + QString(" ] ");
                break;

                case Level::Error:
                    message += QString("[ ") + tr("Error") + QString(" ] ");
                break;
            }

            message += block.module + " :" + block.message;
            qts << message << Qt::endl;
        }
    file.flush();
    file.close();

    record(Level::Info, "Logger", tr("Log exported to file \"%1\".").arg(filePath));
}

void Logger::clear()
{
    setHtml("");
    _backup.clear();
}

void Logger::__customMenu(const QPoint &point)
{
    _menu->exec(viewport()->mapToGlobal(point));
}

void Logger::__actionSaveFile()
{
    SaveLogDialog *dialog = new SaveLogDialog;
    if (dialog->exec() == SaveLogDialog::Accepted)
        toFile(dialog->logFilePath(), dialog->selectedLogLevel());
    dialog->deleteLater();
}

Logger &log()
{
    return *Logger::instance();
}
