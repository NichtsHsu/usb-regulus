#include "tools.h"
#ifdef Q_OS_UNIX
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <QDir>
#endif

QString Tools::getQStyleSheet(const QString &name)
{
    QString finalQss;
    QFile baseCssFile(":/qss/common.qss");
    if (baseCssFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream qts(&baseCssFile);
        finalQss = qts.readAll();
    }
    else
    {
        log().w("Tools", QObject::tr("Failed to open qss file \"%1\".").arg(":/qss/common.qss"));
        return QString();
    }
    baseCssFile.close();

    if (!name.isEmpty())
    {
        QFile cssFile(QString(":/qss/%1.qss").arg(name));
        if (cssFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream qts(&cssFile);
            finalQss += qts.readAll();
        }
        else
        {
            log().w("Tools", QObject::tr("Failed to open qss file \"%1\".").arg(QString(":/qss/%1.qss").arg(name)));
            return QString();
        }
        cssFile.close();
    }

    return finalQss;
}

QString Tools::getConfigFilePath()
{
#ifdef Q_OS_UNIX
    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    QDir configDir(QString(homedir) + "/.config/usb-regulus");
    if (!configDir.exists())
        if (!configDir.mkpath("."))
        {
            log().w("Tools", QObject::tr("Failed to create config directory \"%1\", fallback to \"%2\".")
                    .arg(QString(homedir) + "/.config/usb-regulus")
                    .arg(QString(qApp->applicationDirPath() + QString("/config.ini"))));
            return QString(qApp->applicationDirPath() + QString("/config.ini"));
        }

    return QString(homedir) + "/.config/usb-regulus/config.ini";
#else
    return QString(qApp->applicationDirPath() + QString("/config.ini"));
#endif
}
