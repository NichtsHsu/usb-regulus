#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <log/logger.h>

class Tools: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief getValidThemes
     * @return string list includes all themes
     */
    static QStringList getValidThemes();

    /**
     * @brief getThemeStyleSheet
     * @param theme
     * Theme name included in getValidThemes()
     * @see Tools::getValidThemes()
     * @return style sheet of specified theme
     */
    static QString getThemeStyleSheet(const QString &theme = QString());

    /**
     * @brief getConfigFolder
     * Unix: $HOME/.config/usb-regulus
     *      The directory path will be create if not exist
     * Others: $PWD
     * @return the folder to save config
     */
    static QString getConfigFolder();

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
