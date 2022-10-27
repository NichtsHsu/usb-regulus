#include "savelogdialog.h"
#include "ui_savelogdialog.h"
#include <QFileDialog>
#include <QStandardPaths>

SaveLogDialog::SaveLogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveLogDialog)
{
    ui->setupUi(this);
    connect(ui->toolButtonFilePath, &QToolButton::released,
            this, &SaveLogDialog::__updateFilePath);
}

SaveLogDialog::~SaveLogDialog()
{
    delete ui;
}

Logger::Level SaveLogDialog::selectedLogLevel()
{
    return Logger::Level(ui->comboBoxLogLevel->currentIndex());
}

QString SaveLogDialog::logFilePath()
{
    return ui->lineEditFilePath->text();
}

void SaveLogDialog::__updateFilePath()
{
    QString fileName = QFileDialog::getSaveFileName(
                nullptr,
                tr("Select File to Save Log"),
                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    ui->lineEditFilePath->setText(fileName);
}
