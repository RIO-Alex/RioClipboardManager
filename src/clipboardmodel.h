#pragma once

#ifndef CLIPBOARDMODEL_H
#define CLIPBOARDMODEL_H

#include <QAbstractListModel>
#include "Structs.h"

class ClipboardModel : public QAbstractListModel
{
public:
    explicit ClipboardModel(class DatabaseManager* Databse, QObject *parent = nullptr);

    enum Roles
    {
        PreviewRole = Qt::UserRole + 1,
        TimeRole,
        FullTextRole
    };

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    void SetSearchText(const QString &text);
    QString GetTimeString(const QDateTime &ForTime) const;
    void fetchRange(int startRow);
    void Refresh();

protected:
    QList<ClipboardEntry> Data;
    QString CurrentSearchText;
    class DatabaseManager* db;
};

#endif // CLIPBOARDMODEL_H
