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
