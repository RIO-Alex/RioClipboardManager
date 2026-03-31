#pragma once

#ifndef QUICKMENU_H
#define QUICKMENU_H

#include <QWidget>
#include "Structs.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class QuickMenu;
}
QT_END_NAMESPACE

class QuickMenu : public QWidget
{
    Q_OBJECT

public:
    QuickMenu(class TrayController* Tray, class AppSettings* Settings, QWidget *parent = nullptr);
    ~QuickMenu();

    void HideWindow();
    void ShowWindow();
    void SetModel(class QSortFilterProxyModel* NewModel);
    void InitList(class ClipboardManager* NewManager);
    void BindKey(struct HotKey* NewKey);
    void OpenSettings();

private:
    Ui::QuickMenu *ui;

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
    void changeEvent(QEvent* event) override;
    void OnItemClicked(const QModelIndex& index);
    void RefreshModel();

    QWidget* SettingsRef = nullptr;
    class ClipboardManager* Manager;
    class ClipboardModel* Model;
    class QSortFilterProxyModel* Proxy;
    class ListItemDelegate* Delegate;
    class QTimer* Timer;
    class QTimer* SearchTimer;

    bool bIsKeyBinded = false;
    HotKey LastHotKey;

private slots:
    void on_CloseButton_clicked();
    void UpdateTimer();
    void on_clearButton_clicked();
    void on_SettingsButton_clicked();
};
#endif // QUICKMENU_H
