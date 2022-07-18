/*! Save/Load settings of program

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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QWidget>
#include <atomic>
#include <mutex>

#include "misc/option.hpp"
#include "log/logger.h"

struct WindowProperties
{
    Option<Qt::WindowStates> state;
    Option<QSize> size;
    Option<QPoint> position;
};

/**
 * @brief The Settings class
 * Saves the global properties setting in the singleton instance.
 */
class Settings : public QObject
{
    Q_OBJECT
public:

    static Settings *instance();

    bool &protectMouse();
    const bool &protectMouse() const;

    bool &protectKeyboard();
    const bool &protectKeyboard() const;

    WindowProperties &mainwindowProperties();
    const WindowProperties &mainwindowProperties() const;

    Logger::Level &logLevel();
    const Logger::Level &logLevel() const;

    QString &language();
    const QString &language() const;

    void saveToIni(const QString &filePath) const;

    void loadFromIni(const QString &filePath);

private:
    explicit Settings(QObject *parent = nullptr);
    explicit Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    static std::atomic<Settings *> _instance;

    bool _protectMouse;
    bool _protectKeyboard;
    WindowProperties _mainwindowProperties;
    Logger::Level _logLevel;
    QString _language;
};

/**
 * @brief settings
 * @return the unique instance of Settings
 */
Settings &settings();

#endif // SETTINGS_H
