#include "databasemanager.h"
#include "QtSql/QSqlError"

DatabaseManager::DatabaseManager() {}

bool DatabaseManager::Open(const QString& path)
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(path);

    if (!db_.open())
        return false;

    QSqlQuery Query(db_);
    Query.exec("CREATE TABLE IF NOT EXISTS history ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "preview TEXT, "
               "full_content TEXT, "
               "lower_content TEXT, "
               "timestamp DATETIME)");

    db_.commit();

    return true;
}

void DatabaseManager::AddNewEntry(const QString& FullText, ClipboardEntry& Entry)
{
    QSqlQuery Query(db_);
    Query.prepare("INSERT INTO history (preview, full_content, lower_content, timestamp) "
                  "VALUES (:preview, :full, :lower, :time)");
    Query.bindValue(":preview", Entry.PreviewText);
    Query.bindValue(":full", FullText);
    Query.bindValue(":lower", FullText.toLower());
    Query.bindValue(":time", Entry.Timestamp.toString(Qt::ISODate));

    if (Query.exec())
        Entry.ID = Query.lastInsertId().toInt();
    else
        qDebug() << "Insert error:" << Query.lastError().text();

    Query.prepare("DELETE FROM history WHERE id NOT IN "
                   "(SELECT id FROM history ORDER BY timestamp DESC LIMIT :limit)");
    Query.bindValue(":limit", HistoryLimit);
    Query.exec();

    db_.commit();
}

QString DatabaseManager::GetFullText(int ForID)
{
    QSqlQuery Query(db_);
    Query.prepare("SELECT full_content FROM history WHERE id = :id");
    Query.bindValue(":id", ForID);
    Query.exec();

    if (Query.next())
        return Query.value(0).toString();

    return QString();
}

QList<ClipboardEntry> DatabaseManager::FetchHistory(int Limit, int Offset, const QString& Filter)
{
    QList<ClipboardEntry> Result;
    QSqlQuery Query(db_);

    QString QueryString = "SELECT id, preview, timestamp FROM history ";

    if (!Filter.isEmpty()) {
        QueryString += "WHERE lower_content LIKE :search ";
    }

    QueryString += "ORDER BY timestamp DESC LIMIT :limit OFFSET :offset";

    Query.prepare(QueryString);

    if (!Filter.isEmpty())
        Query.bindValue(":search", "%" + Filter.toLower() + "%");

    Query.bindValue(":limit", Limit);
    Query.bindValue(":offset", Offset);

    if (Query.exec())
        while (Query.next()) {
            ClipboardEntry Entry;
            Entry.ID = Query.value(0).toInt();
            Entry.PreviewText = Query.value(1).toString();
            Entry.Timestamp = QDateTime::fromString(Query.value(2).toString(), Qt::ISODate);

            Result.append(Entry);
        }
    else
        qDebug() << "Fetch error:" << Query.lastError().text();


    return Result;
}

int DatabaseManager::GetTotalCount(const QString& Filter)
{
    QSqlQuery Query(db_);
    QString QueryString = "SELECT COUNT(*) FROM history";

    if (!Filter.isEmpty())
        QueryString += " WHERE lower_content LIKE :search";


    Query.prepare(QueryString);
    if (!Filter.isEmpty())
        Query.bindValue(":search", "%" + Filter.toLower() + "%");


    if (Query.exec() && Query.next())
        return Query.value(0).toInt();

    return 0;
}

void DatabaseManager::ResetHistory()
{
    QSqlQuery Query(db_);
    Query.exec("DELETE FROM history");

    db_.commit();
}

void DatabaseManager::SetHistoryLimit(int NewLimit)
{
    HistoryLimit = NewLimit;
}

void DatabaseManager::DeleteEntry(int ForID)
{
    QSqlQuery Query(db_);
    Query.prepare("DELETE FROM history WHERE id = :id");
    Query.bindValue(":id", ForID);

    if (!Query.exec())
        qWarning() << "Delete failed:" << Query.lastError().text();
}