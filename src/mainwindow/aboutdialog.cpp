#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "global/tools.h"
#include <QApplication>
#include <QDesktopServices>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->pushButtonIcon->setIcon(QIcon(qApp->applicationDirPath() + "/assets/icon/usb-regulus.png"));
    ui->pushButtonIcon->setIconSize({48, 48});
    ui->labelVersion->setText(tr("version: %1").arg(Tools::version()));

    connect(ui->pushButtonIcon, &QPushButton::clicked, [] () {
        QDesktopServices::openUrl(QUrl("https://github.com/NichtsHsu/usb-regulus"));
    });

    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setHtml(
                QString("<p>%1</p>").arg(tr("usb-regulus: USB device viewer")) +
                QString("<p>(C) 2022-2023 <a href=\"https://nihil.cc/\">Nichts Hsu</a></p>") +
                QString("<p>%1</p>").arg(tr("License: %1")
                                         .arg("<a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">"
                                              "GNU General Public License version 3</a>")) +
                QString("<p>%1:</p>").arg(tr("Credits")) +
                QString("<ol>") +
                QString("<li><a href=\"https://www.qt.io/\">Qt</a></li>") +
                QString("<li><a href=\"https://github.com/fpoussin/QtUsb\">QtUsb</a></li>") +
                QString("<li><a href=\"https://github.com/gregkh/usbutils\">usbutils</a></li>") +
                QString("<li><a href=\"https://github.com/DIGImend/usbhid-dump\">usbhid-dump</a></li>") +
                QString("<li><a href=\"https://github.com/DIGImend/hidrd\">hidrd</a></li>") +
                QString("<li><a href=\"https://github.com/Simsys/qhexedit2\">QHexEdit</a></li>") +
                QString("<li><a href=\"https://github.com/GTRONICK/QSS\">Qt Themes</a></li>") +
                QString("</ol>")
                );
}

void AboutDialog::changeEvent(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            ui->labelVersion->setText(tr("version: %1").arg(Tools::version()));
            ui->textBrowser->setHtml(
                        QString("<p>%1</p>").arg(tr("usb-regulus: USB device viewer")) +
                        QString("<p>(C) 2022-2023 <a href=\"https://nihil.cc/\">Nichts Hsu</a></p>") +
                        QString("<p>%1</p>").arg(tr("License: %1")
                                                 .arg("<a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">"
                                                      "GNU General Public License version 3</a>")) +
                        QString("<p>%1:</p>").arg(tr("Credits")) +
                        QString("<ol>") +
                        QString("<li><a href=\"https://www.qt.io/\">Qt</a></li>") +
                        QString("<li><a href=\"https://github.com/fpoussin/QtUsb\">QtUsb</a></li>") +
                        QString("<li><a href=\"https://github.com/gregkh/usbutils\">usbutils</a></li>") +
                        QString("<li><a href=\"https://github.com/DIGImend/usbhid-dump\">usbhid-dump</a></li>") +
                        QString("<li><a href=\"https://github.com/DIGImend/hidrd\">hidrd</a></li>") +
                        QString("<li><a href=\"https://github.com/Simsys/qhexedit2\">QHexEdit</a></li>") +
                        QString("<li><a href=\"https://github.com/GTRONICK/QSS\">Qt Themes</a></li>") +
                        QString("</ol>")
                        );
        break;
        default:
        break;
    }
    QDialog::changeEvent(event);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
