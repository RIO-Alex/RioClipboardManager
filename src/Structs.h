#pragma once

#include "QString"
#include "QVariant"
#include "QDateTime"
#include <QKeySequence>
#include <windows.h>

// Enum for future: nee to add images, files, etc.
enum class ContentType { Text };

struct ClipboardEntry {
    ContentType type = ContentType::Text;
    int ID = -1;
    QString PreviewText = "";
    QDateTime Timestamp;
};

struct MySettings
{
    int HistoryLimit = 1000;
    bool StartWithSystem = false;
    QString HotKey = "ALT+C";
    QList<QString> IgnoredApps = {};
};

struct HotKey
{
    UINT modifiers;
    UINT vk;
    bool valid = false;
};