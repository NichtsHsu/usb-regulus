#include "tools.h"

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
