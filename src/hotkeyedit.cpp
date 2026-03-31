#include "hotkeyedit.h"
#include <QKeyEvent>

HotKeyEdit::HotKeyEdit(QWidget* parent)
    : QKeySequenceEdit(parent)
{
    EnglishLayout = FindEnglishLayout();
}

HKL HotKeyEdit::FindEnglishLayout()
{
    // Get Eng layout
    HKL layouts[64];
    int count = GetKeyboardLayoutList(64, layouts);

    for (int i = 0; i < count; ++i)
    {
        LANGID lang = LOWORD((quintptr)layouts[i]);
        if (PRIMARYLANGID(lang) == LANG_ENGLISH)
            return layouts[i];
    }

    return nullptr;
}

void HotKeyEdit::keyPressEvent(QKeyEvent* event)
{
    if (!EnglishLayout)
    {
        QKeySequenceEdit::keyPressEvent(event);
        return;
    }

    UINT vk = event->nativeVirtualKey();
    UINT scanCode = event->nativeScanCode();

    BYTE keyState[256] = {};
    WCHAR buf[8]       = {};

    int res = ToUnicodeEx(vk, scanCode, keyState, buf, 7, 0, EnglishLayout);

    if (res > 0)
    {
        QChar ch = QChar(buf[0]).toUpper();
        if (ch.isLetter())
        {
            Qt::Key Key = static_cast<Qt::Key>(ch.unicode());
            QKeyEvent NewEvent(event->type(), Key, event->modifiers(), QString(ch), event->isAutoRepeat());

            QKeySequenceEdit::keyPressEvent(&NewEvent);
            return;
        }
    }

    QKeySequenceEdit::keyPressEvent(event);
}