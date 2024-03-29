/*! A simple logger system which is derived from QTextBrowser

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

#ifndef LOGGER_H
#define LOGGER_H

#include <QTextBrowser>
#include <QString>
#include <QTime>
#include <QList>
#include <QtGlobal>
#include <QMutex>
#include <QMenu>
#include <mutex>

/**
  * Simplified interfaces for any QObject
  */
#define LOGD(msg) log().d(metaObject()->className(), msg)
#define LOGI(msg) log().i(metaObject()->className(), msg)
#define LOGW(msg) log().w(metaObject()->className(), msg)
#define LOGE(msg) log().e(metaObject()->className(), msg)

/**
 * @brief The Logger class
 * It is derived from QTextBrowser,
 * you can add it into your window.
 */
class Logger : public QTextBrowser
{
    Q_OBJECT
public:
    /**
     * @brief The Level enum
     * Log level type.
     */
    enum class Level
    {
        Debug, Info, Warning, Error
    };

    /**
     * @brief logLevel
     * @return current log level
     */
    Level logLevel() const;

    /**
     * @brief instance
     * @return the global single instance
     * @see log()
     */
    static Logger *instance();

    void changeEvent(QEvent *event) override;

public slots:
    /**
     * @brief record
     * Put a message to logger.
     * @note
     * For thread safety, the function that actually works is __record().
     * @see Logger::__record()
     */
    void record(Logger::Level type, const QString &module, const QString &message);

    /**
     * @brief d
     * Short of record(Level::Debug, ..)
     */
    void d(const QString &module, const QString &message);

    /**
     * @brief i
     * Short of record(Level::Info, ..)
     */
    void i(const QString &module, const QString &message);

    /**
     * @brief w
     * Short of record(Level::Warning, ..)
     */
    void w(const QString &module, const QString &message);

    /**
     * @brief e
     * Short of record(Level::Error, ..)
     */
    void e(const QString &module, const QString &message);

    /**
     * @brief setLogLevel
     * Set log level to specified level, default is Info.
     * Any message whose level is lower will not be printed.
     */
    void setLogLevel(Logger::Level logLevel);

    /**
     * @brief logToFile
     * Save log to file.
     * @param filePath
     * Log file path to save.
     * @param logLevel
     * The lowest level for log to save.
     */
    void toFile(const QString &filePath, Logger::Level logLevel = Logger::Level::Debug);

    /**
     * @brief clear
     * Clear all log.
     */
    void clear();

private slots:
    void __customMenu(const QPoint &point);
    void __actionSaveFile();

private:
    explicit Logger(QWidget *parent = nullptr);
    explicit Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    QString __strLogLevel();

    /**
     * @brief record
     * Put a message to logger.
     */
    void __record(Logger::Level type, const QString &module, const QString &message);

    static std::atomic<Logger *> _instance;

    Level _logLevel;

    struct BackupBlock {
        Logger::Level type;
        QString module;
        QString message;
        QString time;
    };

    QList<BackupBlock> _backup;
    QMutex _writeMutex;

    QMenu *_menu;
    QAction *_actionCopy, *_actionCopyAll, *_actionSave, *_actionClear;
};

/**
 * @brief logger
 * @return the unique instance of Logger
 */
Logger &log();

#endif // LOGGER_H
