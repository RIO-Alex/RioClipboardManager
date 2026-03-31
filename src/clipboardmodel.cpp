#include "clipboardmodel.h"
#include "databasemanager.h"

ClipboardModel::ClipboardModel(DatabaseManager* Database, QObject *parent)
    : QAbstractListModel{parent}, db(Database)
{}

int ClipboardModel::rowCount(const QModelIndex& parent) const
{
    return Data.size();
}

QVariant ClipboardModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (Data[row].ID == -1)
    {
        const_cast<ClipboardModel*>(this)->fetchRange(row);

        if (role == PreviewRole)
            return QString("Загрузка...");

        return QVariant();
    }

    const ClipboardEntry& entry = Data[row];

    if (role == PreviewRole)
        return entry.PreviewText;

    if (role == TimeRole)
        return GetTimeString(entry.Timestamp);

    if (role == Qt::UserRole)
        return entry.ID;

    return QVariant();
}

void ClipboardModel::SetSearchText(const QString &text)
{
    if (!db)
        return;

    if (CurrentSearchText == text)
        return;

    CurrentSearchText = text;

    int newCount = db->GetTotalCount(CurrentSearchText);

    beginResetModel();

    Data.clear();
    Data.resize(newCount);

    endResetModel();
}

QString ClipboardModel::GetTimeString(const QDateTime &ForTime) const
{
    QDateTime now = QDateTime::currentDateTime();
    qint64 delta = ForTime.secsTo(now);

    if (delta < 60)
        return QString("%1 сек. назад").arg(delta);

    if (delta < 3600)
        return QString("%1 мин. назад").arg(delta / 60);

    if (delta < 86400)
        return QString("%1 ч. назад").arg(delta / 3600);

    if (delta < 2592000)
        return QString("%1 д. назад").arg(delta / 86400);

    // if delta > 30 days, return date
    return ForTime.toString("dd.MM.yyyy");
}

void ClipboardModel::fetchRange(int startRow)
{
    if (!db)
        return;

    // Hard limit for cache size
    int limit = 500;

    QList<ClipboardEntry> batch = db->FetchHistory(limit, startRow, CurrentSearchText);

    for (int i = 0; i < batch.size(); ++i)
    {
        int targetIndex = startRow + i;
        if (targetIndex < Data.size())
            Data[targetIndex] = batch[i];
    }

    emit dataChanged(index(startRow), index(startRow + batch.size() - 1));
}

void ClipboardModel::Refresh()
{
    if (!db)
        return;

    int newCount = db->GetTotalCount(CurrentSearchText);

    beginResetModel();
    Data.clear();
    Data.resize(newCount);

    endResetModel();
}