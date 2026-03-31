#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(class AppSettings* SettingsObject, QWidget *parent = nullptr);
    ~SettingsWindow();

private slots:
    void on_CancelButton_clicked();

    void on_SaveButton_clicked();

private:
    Ui::SettingsWindow *ui;
    class AppSettings* Settings;
};

#endif // SETTINGSWINDOW_H
