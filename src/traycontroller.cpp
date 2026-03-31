#include "traycontroller.h"
#include <QMenu>
#include <QApplication>
#include "clipboardmanager.h"
#include "quickmenu.h"
#include "appsettings.h"
#include "databasemanager.h"

TrayController::TrayController(QObject *parent)
    : QObject{parent}
{
    Settings = new AppSettings(this);

    AppMenu = new QuickMenu(this, Settings);
    ClipManager = new ClipboardManager(this);

    if (AppMenu && ClipManager)
        AppMenu->InitList(ClipManager);

    // create tray icon
    TrayIcon = new QSystemTrayIcon(this);
    TrayIcon->setIcon(QIcon(":/icons/app_icon.png"));
    TrayIcon->setToolTip("Rio Clipboard Manager");
    TrayIcon->show();

    // connect callback for click on tray icon
    connect(TrayIcon, &QSystemTrayIcon::activated, this, &TrayController::OnShowAppWindowTriggered);

    // create context menu for tray
    TrayMenu = new QMenu();
    QAction* ShowAction = TrayMenu->addAction("Развернуть");
    QAction* SettingsAction = TrayMenu->addAction("Настройки");
    QAction* ExitAction = TrayMenu->addAction("Выход");

    TrayIcon->setContextMenu(TrayMenu);

    connect(ShowAction, &QAction::triggered, this, &TrayController::ShowAppWindow);
    connect(ExitAction, &QAction::triggered, this, &TrayController::ExitApp);
    connect(SettingsAction, &QAction::triggered, this, &TrayController::OpenSettings);

    InitSettings();
    connect(Settings, &AppSettings::onSettingsUpdated, this, &TrayController::InitSettings);
}

TrayController::~TrayController()
{
    delete TrayMenu;
    delete AppMenu;
}

void TrayController::OnShowAppWindowTriggered(QSystemTrayIcon::ActivationReason Reason)
{
    if (Reason == QSystemTrayIcon::Trigger && AppMenu)
        ShowAppWindow();
}

void TrayController::ExitApp()
{
    QApplication::quit();
}

void TrayController::ShowAppWindow()
{
    if (!AppMenu)
        return;

    AppMenu->ShowWindow();
}

AppSettings* TrayController::GetSettings()
{
    return Settings;
}

void TrayController::InitSettings()
{
    ClipManager->GetDatabase()->SetHistoryLimit(Settings->GetSettings().HistoryLimit);

    HotKey Key = GetHotkeyFromString(Settings->GetSettings().HotKey);
    AppMenu->BindKey(&Key);

    ClipManager->SetIgnoredApps(Settings->GetSettings().IgnoredApps);
}

HotKey TrayController::GetHotkeyFromString(QString HotKeyStr)
{
    HotKey result {0, 0, false};
    QStringList parsed = HotKeyStr.split("+");

    for (int i = 0; i < parsed.size(); ++i)
    {
        QString part = parsed[i].trimmed();

        if (part.toLower() == "alt") result.modifiers |= MOD_ALT;
        else if (part.toLower() == "ctrl") result.modifiers |= MOD_CONTROL;
        else if (part.toLower() == "shift") result.modifiers |= MOD_SHIFT;
        else if (part.toLower() == "win") result.modifiers |= MOD_WIN;

        // last element
        if (i == parsed.size() - 1)
        {
            // A-Z, А-Я or 0-9
            if (part.length() == 1 && (part[0].isLetter() || part[0].isDigit()))
            {
                result.vk = part[0].toLatin1();
                result.valid = true;
                continue;
            }

            // F1-F24
            if (part.startsWith("F"))
            {
                bool ok;
                int number = part.mid(1).toInt(&ok);
                if (ok && number > 0 && number <= 24)
                {
                    result.vk = VK_F1 + (number -1);
                    result.valid = true;
                    continue;
                }
            }
        }
    }

    return result;
}

void TrayController::OpenSettings()
{
    if (AppMenu)
        AppMenu->OpenSettings();
}