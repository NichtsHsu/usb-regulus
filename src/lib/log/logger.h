#ifndef LOGGER_H
#define LOGGER_H

#include <QTextBrowser>
#include <QString>
#include <QTime>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <QtGlobal>

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

signals:

public slots:
    /**
     * @brief record
     * Put a message to logger.
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
    void toFile(const QString &filePath, Logger::Level logLevel);

    /**
     * @brief clear
     * Clear all log.
     */
    void clear();

private:
    friend Logger &log();
    explicit Logger(QWidget *parent = nullptr);

    Level _logLevel;

    struct BackupBlock {
        Logger::Level type;
        QString module;
        QString message;
        QString time;
    };

    QList<BackupBlock> _backup;
};

/**
 * @brief logger
 * Get the unique Logger instance.
 */
Logger &log();

#endif // LOGGER_H
