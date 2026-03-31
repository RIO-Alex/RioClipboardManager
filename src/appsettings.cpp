#include "appsettings.h"

#include <QSettings>
#include <QDir>
#include <QCoreApplication>

AppSettings::AppSettings(QObject *parent)
    : QObject{parent}
{
    ReadSettings();
}

void AppSettings::ReadSettings()
{
    QSettings CurrentSettings;

    CurrentSettings.beginGroup("ClipboardManagerSettings");

    Settings.HistoryLimit = CurrentSettings.value("HistoryLimit", 1000).toInt();
    if (!CurrentSettings.contains("HistoryLimit"))
        CurrentSettings.setValue("HistoryLimit", Settings.HistoryLimit);

    Settings.StartWithSystem = CurrentSettings.value("StartWithSystem", false).toBool();
    if (!CurrentSettings.contains("StartWithSystem"))
        CurrentSettings.setValue("StartWithSystem", Settings.StartWithSystem);

    Settings.HotKey = CurrentSettings.value("HotKey", "ALT+C").toString();
    if (!CurrentSettings.contains("HotKey"))
        CurrentSettings.setValue("HotKey", Settings.HotKey);

    if (!CurrentSettings.contains("IgnoredApps"))
        CurrentSettings.setValue("IgnoredApps", QList<QString>());

    int size = CurrentSettings.beginReadArray("IgnoredApps");
    for (int i = 0; i < size; ++i)
    {
        CurrentSettings.setArrayIndex(i);
        Settings.IgnoredApps.push_back(CurrentSettings.value("AppName").toString());
    }
    CurrentSettings.endArray();
    CurrentSettings.endGroup();
}

void AppSettings::UpdateSettings(MySettings& NewSettings)
{
    Settings = NewSettings;
    QSettings CurrentSettings;

    CurrentSettings.beginGroup("ClipboardManagerSettings");

    bool StartWithSystem = CurrentSettings.value("StartWithSystem").toBool();

    CurrentSettings.setValue("HistoryLimit", Settings.HistoryLimit);
    CurrentSettings.setValue("StartWithSystem", Settings.StartWithSystem);
    CurrentSettings.setValue("HotKey", Settings.HotKey);

    CurrentSettings.beginWriteArray("IgnoredApps");
    for (int i = 0; i < Settings.IgnoredApps.size(); ++i)
    {
        CurrentSettings.setArrayIndex(i);
        CurrentSettings.setValue("AppName", Settings.IgnoredApps[i]);
    }

    CurrentSettings.endArray();
    CurrentSettings.endGroup();

    if (StartWithSystem != NewSettings.StartWithSystem)
        UpdateStartup(NewSettings.StartWithSystem);

    emit onSettingsUpdated();
}

MySettings AppSettings::GetSettings()
{
    return Settings;
}

void AppSettings::UpdateStartup(bool Add)
{
#ifdef Q_OS_WIN32

    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (Add)
    {
        QString appPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        settings.setValue("RioClipboardManager", QString("\"%1\"").arg(appPath));
    }
    else
        settings.remove("RioClipboardManager");

    settings.sync();

#endif
}