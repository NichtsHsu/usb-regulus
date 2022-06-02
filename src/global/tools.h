#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QFile>
#include <QTextStream>
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

private:
    Tools() = delete;
    Tools(const Tools &) = delete;
    Tools &operator=(const Tools &) = delete;
};

#endif // TOOLS_H
