#pragma once

#ifndef TRAYCONTROLLER_H
#define TRAYCONTROLLER_H

#include <QObject>
#include <QSystemTrayIcon>
#include "Structs.h"

class TrayController : public QObject
{
    Q_OBJECT
public:
    explicit TrayController(QObject* parent = nullptr);

    ~TrayController();

    void OnShowAppWindowTriggered(QSystemTrayIcon::ActivationReason Reason);

    void ShowAppWindow();

    void ExitApp();

    class AppSettings* GetSettings();

    void InitSettings();

    HotKey GetHotkeyFromString(QString HotKeyStr);

signals:

protected:
    class ClipboardManager* ClipManager;
    class QuickMenu* AppMenu;
    QSystemTrayIcon* TrayIcon;
    class QMenu* TrayMenu;
    class AppSettings* Settings;

    void OpenSettings();
};

#endif // TRAYCONTROLLER_H
