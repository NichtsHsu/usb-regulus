/*! Save/Load settings of program

 * Copyright (C) 2022-2023 Nichts Hsu

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

/**
 * @brief The WindowStartStates enum
 * The state when window start
 */
enum class WindowStartStates
{
    // Last position and size
    LAST_POS_SIZE,
    // Minimize
    MIN,
    // Maximize
    MAX,
    // Center with default size
    CENTER_DEFAULT_SIZE,
    // Center with last size
    CENTER_LAST_SIZE,
    // Center with custom size
    CENTER_CUSTOM_SIZE,
    // Custom size and position
    CUSTOM,
};

struct WindowProperties
{
    WindowStartStates startState;
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

    /**
     * @brief protectMouse
     * If Protect Mouse is selected, usb-regulas would avoid to claim mouse interfaces.
     */
    bool &protectMouse();
    const bool &protectMouse() const;

    /**
     * @brief protectKeyboard
     * If Protect Keyboard is selected, usb-regulas would avoid to claim keyboard interfaces.
     */
    bool &protectKeyboard();
    const bool &protectKeyboard() const;

    /**
     * @brief mainwindowProperties
     * Save the properties of main window at last time exits.
     */
    WindowProperties &mainwindowProperties();
    const WindowProperties &mainwindowProperties() const;

    /**
     * @brief logLevel
     * Save the log level (only when `startWithFixedLevel` is true).
     */
    Logger::Level &logLevel();
    const Logger::Level &logLevel() const;

    /**
     * @brief language
     * Save UI language.
     */
    QString &language();
    const QString &language() const;

    /**
     * @brief theme
     * Save UI theme.
     */
    QString &theme();
    const QString &theme() const;

    /**
     * @brief saveLogBeforeExit
     * Save the log when usb-regulus exit if selected.
     * @see autoSaveLogFile()
     */
    bool &saveLogBeforeExit();
    const bool &saveLogBeforeExit() const;

    /**
     * @brief autoSaveLogDir
     * The directory to save log if saveLogBeforeExit is selected.
     * @see saveLogBeforeExit()
     */
    QString &autoSaveLogDir();
    const QString &autoSaveLogDir() const;

    /**
     * @brief startWithRandomTheme
     * Determine if usb-regulus start with a random theme.
     */
    bool &startWithRandomTheme();
    const bool &startWithRandomTheme() const;

    /**
     * @brief uiFont
     * The font which UI uses.
     */
    QFont &uiFont();
    const QFont &uiFont() const;

    /**
     * @brief monospacedFont
     * The monospaced font which infomation broswer uses.
     */
    QFont &monospacedFont();
    const QFont &monospacedFont() const;

    /**
     * @brief saveToIni
     * Save settings to ini file.
     * @param filePath
     * Ini file path.
     */
    void saveToIni(const QString &filePath) const;

    /**
     * @brief loadFromIni
     * Load settings from ini file.
     * @param filePath
     * Ini file path.
     */
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
    bool _saveLogBeforeExit;
    QString _autoSaveLogDir;
    bool _startWithRandomTheme;
    QString _theme;
    QFont _uiFont;
    QFont _monospacedFont;
};

/**
 * @brief settings
 * @return the unique instance of Settings
 */
Settings &settings();

#endif // SETTINGS_H
