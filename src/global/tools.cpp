#include "tools.h"
#ifdef Q_OS_UNIX
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif
#include <QApplication>
#include <QDir>

QStringList Tools::getValidThemes()
{
    QStringList themes;
    QDir themePath(qApp->applicationDirPath() + "/assets/themes/");
    foreach (const QFileInfo& entry, themePath.entryInfoList(
                 QStringList() << "*.qss", QDir::Files | QDir::Hidden | QDir::NoSymLinks ) ) {
        themes.append(entry.baseName());
    }

    return themes;
}

QString Tools::getThemeStyleSheet(const QString &theme)
{
    if (theme == "(none)")
        return QString();

    QString qss;
    QString fileName = qApp->applicationDirPath() + "/assets/themes/" + theme + ".qss";
    QFile qssFile(qApp->applicationDirPath() + "/assets/themes/" + theme + ".qss");
    if (!qssFile.exists())
    {
        log().w("Tools", tr("Qss file \"%1\" doesn't exist.").arg(fileName));
        return QString();
    }

    if (!qssFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        log().w("Tools", tr("Failed to open qss file \"%1\".").arg(fileName));
        return QString();
    }

    QTextStream qts(&qssFile);
    qss = qts.readAll();
    qssFile.close();

    return qss;
}

QString Tools::getConfigFolder()
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
                    .arg(qApp->applicationDirPath()));
            return qApp->applicationDirPath();
        }

    return QString(homedir) + "/.config/usb-regulus";
#else
    return qApp->applicationDirPath();
#endif
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
