#include "clipboardmanager.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>
#include "databasemanager.h"
#include <psapi.h>
#include <QFileInfo>

ClipboardManager::ClipboardManager(QObject *parent)
    : QObject{parent}
{
    db = new DatabaseManager();
    db->Open("history.db");

    QClipboard* clipboard = QGuiApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged, this, &ClipboardManager::OnClipboardDataChanged);
}

ClipboardManager::~ClipboardManager()
{
    delete db;
}

void ClipboardManager::OnClipboardDataChanged()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    if (!clipboard)
        return;

    QString ProcessName = GetProcessName();
    if (IgnoredApps.contains(ProcessName))
        return;

    const QMimeData* data = clipboard->mimeData();
    if (!data)
        return;

    // only text for now. need to add another types later
    if (!data->hasText())
        return;

    // additional check for files path
    if (clipboard->text().startsWith("file:///"))
        return;

    AddToHistory(clipboard->text());
}

void ClipboardManager::AddToHistory(const QString& NewText)
{
    // Here LastSelected equals to PreviewText. And I need to construct PreviewText first
    QString PreviewText = NewText.size() <= 45 ? NewText.left(45).simplified() : NewText.left(42).simplified() + "...";

    if (NewText == LastTextInClipboard || PreviewText == LastSelected)
        return;

    ClipboardEntry Target;
    Target.type = ContentType::Text;
    Target.Timestamp = QDateTime::currentDateTime();
    Target.PreviewText = PreviewText;

    db->AddNewEntry(NewText, Target);

    LastTextInClipboard = NewText;
    LastSelected = "";

    emit OnHistoryUpdated();
}

void ClipboardManager::SetLastSelectedString(const QString& NewString)
{
    LastSelected = NewString;
}

DatabaseManager* ClipboardManager::GetDatabase()
{
    return db;
}

void ClipboardManager::SetIgnoredApps(QStringList NewApps)
{
    IgnoredApps = NewApps;
}

QString ClipboardManager::GetProcessName()
{
    QString result = "Unknow";
    HWND hwnd = GetOpenClipboardWindow();
    if (!hwnd)
        hwnd = GetForegroundWindow();
    if (!hwnd)
        return result;

    hwnd = GetAncestor(hwnd, GA_ROOTOWNER);
    if (!hwnd)
        return result;

    DWORD ProcessID = 0;
    GetWindowThreadProcessId(hwnd, &ProcessID);
    if (!ProcessID)
        return result;

    HANDLE Process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, ProcessID);
    if (!Process)
        return result;

    WCHAR ProcessName[MAX_PATH];
    DWORD size = MAX_PATH;
    if (QueryFullProcessImageNameW(Process, 0, ProcessName, &size))
    {
        QString fullPath = QString::fromWCharArray(ProcessName);
        result = QFileInfo(fullPath).fileName().toLower();
        result = result.left(result.size() - 4); // remove .exe
    }

    CloseHandle(Process);
    return result;
}