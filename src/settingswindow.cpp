#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "appsettings.h"

#include <QKeySequence>
#include <QMessageBox>

SettingsWindow::SettingsWindow(AppSettings* SettingsObject, QWidget *parent)
    : QWidget(parent), Settings(SettingsObject)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    setWindowTitle("Rio Clipboard Manager");
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    ui->IgnoreApps->setPlaceholderText("application1\napplication2");

    if (Settings)
    {
        MySettings CurrentSettings = Settings->GetSettings();
        ui->HistoryLimitSpin->setValue(CurrentSettings.HistoryLimit);
        ui->StartWithWindowsCheck->setChecked(CurrentSettings.StartWithSystem);
        ui->Hotkey->setKeySequence(QKeySequence::fromString(CurrentSettings.HotKey));

        QString IgnoredApps = "";
        for (int i = 0; i < CurrentSettings.IgnoredApps.size(); ++i)
        {
            IgnoredApps += CurrentSettings.IgnoredApps[i];

            // for last element
            if (i != CurrentSettings.IgnoredApps.size() - 1)
                IgnoredApps += "\n";
        }

        ui->IgnoreApps->setPlainText(IgnoredApps);
    }
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_CancelButton_clicked()
{
    // close for delete
    close();
}


void SettingsWindow::on_SaveButton_clicked()
{
    if (Settings)
    {
        MySettings CurrentSettings;
        CurrentSettings.HistoryLimit = ui->HistoryLimitSpin->value();
        CurrentSettings.StartWithSystem = ui->StartWithWindowsCheck->isChecked();
        CurrentSettings.HotKey = ui->Hotkey->keySequence().toString();

        QStringList IgnoredApps = ui->IgnoreApps->toPlainText().split("\n");
        for (int i = 0; i < IgnoredApps.size(); ++i)
        {
            QString AppName = IgnoredApps[i].trimmed();
            if (AppName.isEmpty())
                continue;

            CurrentSettings.IgnoredApps.push_back(AppName);
        }

        Settings->UpdateSettings(CurrentSettings);

        QMessageBox::information(nullptr, "Rio Clipboard Manager", "Настройки успешно сохранены!");
    }

    close();
}

