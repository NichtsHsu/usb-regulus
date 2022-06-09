#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <log/logger.h>

class Tools
{
public:
    /**
     * @brief getQStyleSheet
     * Read common QSS and specified QSS file.
     * @param name
     * If not set, only read common QSS file.
     * @return Qt's Style Sheet
     */
    static QString getQStyleSheet(const QString &name = QString());

    /**
     * @brief getConfigFilePath
     * @note
     * Unix: $HOME/.config/usb-regulus/config.ini
     *      The directory path will be create if not exist
     * Others: $PWD/config.ini
     * @return the config.ini file path
     */
    static QString getConfigFilePath();

private:
    Tools() = delete;
    Tools(const Tools &) = delete;
    Tools &operator=(const Tools &) = delete;
};

#endif // TOOLS_H
