#include "settings.h"
#include <QFontDatabase>
#include "tools.h"

Settings::Settings(QObject *parent)
    : QObject{parent},
      _protectMouse(true)
{

}

const QFont &Settings::monospacedFont() const
{
    return _monospacedFont;
}

const QFont &Settings::uiFont() const
{
    return _uiFont;
}

QFont &Settings::monospacedFont()
{
    return _monospacedFont;
}

const bool &Settings::startWithRandomTheme() const
{
    return _startWithRandomTheme;
}

QFont &Settings::uiFont()
{
    return _uiFont;
}

const QString &Settings::autoSaveLogDir() const
{
    return _autoSaveLogDir;
}

bool &Settings::startWithRandomTheme()
{
    return _startWithRandomTheme;
}

const bool &Settings::saveLogBeforeExit() const
{
    return _saveLogBeforeExit;
}

QString &Settings::autoSaveLogDir()
{
    return _autoSaveLogDir;
}

bool &Settings::saveLogBeforeExit()
{
    return _saveLogBeforeExit;
}

QString &Settings::theme()
{
    return _theme;
}

const QString &Settings::theme() const
{
    return _theme;
}

std::atomic<Settings *> Settings::_instance{nullptr};
static std::mutex __mutW;

Settings *Settings::instance()
{
    if (Settings::_instance == nullptr)
    {
        std::lock_guard<std::mutex> lock{ __mutW };
        if (Settings::_instance == nullptr)
            Settings::_instance = new Settings;
    }
    return Settings::_instance;
}

bool &Settings::protectMouse()
{
    return _protectMouse;
}

const bool &Settings::protectMouse() const
{
    return _protectMouse;
}

bool &Settings::protectKeyboard()
{
    return _protectKeyboard;
}

const bool &Settings::protectKeyboard() const
{
    return _protectKeyboard;
}

WindowProperties &Settings::mainwindowProperties()
{
    return _mainwindowProperties;
}

const WindowProperties &Settings::mainwindowProperties() const
{
    return _mainwindowProperties;
}

Logger::Level &Settings::logLevel()
{
    return _logLevel;
}

const Logger::Level &Settings::logLevel() const
{
    return _logLevel;
}

QString &Settings::language()
{
    return _language;
}

const QString &Settings::language() const
{
    return _language;
}

void Settings::saveToIni(const QString &filePath) const
{
    QSettings ini(filePath, QSettings::IniFormat);
    ini.beginGroup("mainwindow");
    ini.setValue("startState", int(_mainwindowProperties.startState));
    if (_mainwindowProperties.size)
    {
        ini.setValue("width", _mainwindowProperties.size.get().width());
        ini.setValue("height", _mainwindowProperties.size.get().height());
    }
    if (_mainwindowProperties.position)
    {
        ini.setValue("x", _mainwindowProperties.position.get().x());
        ini.setValue("y", _mainwindowProperties.position.get().y());
    }
    ini.endGroup();
    ini.beginGroup("fonts");
    ini.setValue("ui", _uiFont.toString());
    ini.setValue("monospaced", _monospacedFont.toString());
    ini.endGroup();
    ini.beginGroup("usbconfigs");
    ini.setValue("protectMouse", _protectMouse);
    ini.setValue("protectKeyboard", _protectKeyboard);
    ini.endGroup();
    ini.beginGroup("userconfigs");
    ini.setValue("logLevel", int(_logLevel));
    ini.setValue("saveLogBeforeExit", _saveLogBeforeExit);
    ini.setValue("autoSaveLogDir", _autoSaveLogDir);
    ini.setValue("language", _language);
    ini.setValue("startWithRandomTheme", _startWithRandomTheme);
    ini.setValue("theme", _theme);
    ini.endGroup();
}

void Settings::loadFromIni(const QString &filePath)
{
    QSettings ini(filePath, QSettings::IniFormat);
    ini.beginGroup("mainwindow");
    _mainwindowProperties.startState = WindowStartStates(
                ini.value("startState", int(WindowStartStates::CENTER_DEFAULT_SIZE)).toInt());
    if (ini.contains("width") && ini.contains("height"))
        _mainwindowProperties.size.set(
                    ini.value("width", 0).toInt(),
                    ini.value("height", 0).toInt());
    else
        _mainwindowProperties.size.setNone();
    if (ini.contains("x") && ini.contains("y"))
        _mainwindowProperties.position.set(
                    ini.value("x", 0).toInt(),
                    ini.value("y", 0).toInt());
    else
        _mainwindowProperties.position.setNone();
    ini.endGroup();
    ini.beginGroup("fonts");
    _uiFont.fromString(ini.value("ui", QFontDatabase::systemFont(
                                     QFontDatabase::GeneralFont).toString()).toString());
    _monospacedFont.fromString(ini.value("monospaced", QFontDatabase::systemFont(
                                             QFontDatabase::FixedFont).toString()).toString());
    ini.endGroup();
    ini.beginGroup("usbconfigs");
    _protectMouse = ini.value("protectMouse", true).toBool();
    _protectKeyboard = ini.value("protectKeyboard", true).toBool();
    ini.endGroup();
    ini.beginGroup("userconfigs");
    _logLevel = Logger::Level(ini.value("logLevel", int(Logger::Level::Info)).toInt());
    _saveLogBeforeExit = ini.value("saveLogBeforeExit", false).toBool();
    _autoSaveLogDir = ini.value("autoSaveLogDir", Tools::getConfigFolder()).toString();
    _language = ini.value("language", "en_US").toString();
    _startWithRandomTheme = ini.value("startWithRandomTheme", false).toBool();
    _theme = ini.value("theme", "Aqua").toString();
    ini.endGroup();
}

Settings &settings()
{
    return *Settings::instance();
}
