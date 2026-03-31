#pragma once

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include "Structs.h"

class DatabaseManager
{
public:
    DatabaseManager();
    bool Open(const QString& path);

    void AddNewEntry(const QString& FullText, ClipboardEntry& Entry);
    QString GetFullText(int ForID);
    QList<ClipboardEntry> FetchHistory(int Limit, int Offset, const QString& Filter);
    int GetTotalCount(const QString& Filter);
    void ResetHistory();
    void SetHistoryLimit(int NewLimit);
    void DeleteEntry(int ForID);

protected:
    QSqlDatabase db_;
    int HistoryLimit = 1000;
};

#endif // DATABASEMANAGER_H
