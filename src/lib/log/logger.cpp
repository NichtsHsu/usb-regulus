﻿#include "logger.h"

Logger::Logger(QWidget *parent):
    QTextBrowser(parent), _logLevel(Level::Info)
{
    document()->setMaximumBlockCount(500);
}

Logger::Level Logger::logLevel() const
{
    return _logLevel;
}

void Logger::record(Logger::Level type, const QString &module, const QString &message)
{
    QString time = QTime::currentTime().toString();

    if (_logLevel <= type) {
        QString messageHtml = QString("<font color=purple>") + time + QString("</font> ");
        switch(type)
        {
            case Level::Debug:
                messageHtml += QString("<font color=black><b>[ <font color=green>") + tr("Debug") + QString("</font> ]</b></font> ");
            break;

            case Level::Info:
                messageHtml += QString("<font color=black><b>[ <font color=blue>") + tr("Info") + QString("</font> ]</b></font> ");
            break;

            case Level::Warning:
                messageHtml += QString("<font color=black><b>[ <font color=orange>") + tr("Warning") + QString("</font> ]</b></font> ");
            break;

            case Level::Error:
                messageHtml += QString("<font color=black><b>[ <font color=red>") + tr("Error") + QString("</font> ]</b></font> ");
            break;
        }

        messageHtml += QString("<font color=purple><u>") + module + QString("</u></font> :");
        messageHtml += QString("<font color=black>") + message + QString("</font><br />");

        moveCursor(QTextCursor::End);
        textCursor().insertHtml(messageHtml);
    }

    _backup.append(Logger::BackupBlock{type, module, message, time});
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
}

void Logger::toFile(const QString &filePath, Logger::Level logLevel)
{
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
            qts << message << endl;
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

Logger &log()
{
    static Logger *logger = new Logger;
    return *logger;
}