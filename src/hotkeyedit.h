#pragma once

#ifndef HOTKEYEDIT_H
#define HOTKEYEDIT_H

#include <QKeySequenceEdit>
#include <windows.h>

class HotKeyEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    explicit HotKeyEdit(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    HKL EnglishLayout = nullptr;

    HKL FindEnglishLayout();
};

#endif // HOTKEYEDIT_H
