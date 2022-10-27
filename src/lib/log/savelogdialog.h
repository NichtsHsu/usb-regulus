/*! Select file and level to save log. Subcomponent of Logger

 * Copyright (C) 2022 Nichts Hsu

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

#ifndef SAVELOGDIALOG_H
#define SAVELOGDIALOG_H

#include <QDialog>
#include "logger.h"

namespace Ui {
    class SaveLogDialog;
}

/**
 * @brief The SaveLogDialog class
 * A dialog appears when you select 'Save to File' option on the menu of Logger,
 * and it will ask you for the log file path and log level.
 */
class SaveLogDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SaveLogDialog(QWidget *parent = nullptr);
    ~SaveLogDialog();

    /**
     * @brief selectedLogLevel
     * @return selected log level to save
     */
    Logger::Level selectedLogLevel();

    /**
     * @brief logFilePath
     * @return file path to save log
     */
    QString logFilePath();

private slots:
    void __updateFilePath();
private:
    Ui::SaveLogDialog *ui;
};

#endif // SAVELOGDIALOG_H
