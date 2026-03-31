#pragma once

#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <QObject>

class ClipboardManager : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardManager(QObject *parent = nullptr);
    ~ClipboardManager();

    void OnClipboardDataChanged();
    void AddToHistory(const QString& NewText);
    void SetLastSelectedString(const QString& NewString);
    class DatabaseManager* GetDatabase();
    void SetIgnoredApps(QStringList NewApps);

signals:
    void OnHistoryUpdated();

protected:
    QString LastSelected;
    QString LastTextInClipboard;
    QStringList IgnoredApps;
    int HistoryLimit = 1000;
    class DatabaseManager* db;

    QString GetProcessName();
};

#endif // CLIPBOARDMANAGER_H
