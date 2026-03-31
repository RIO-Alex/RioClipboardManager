#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include "Structs.h"

class AppSettings : public QObject
{
    Q_OBJECT
public:
    explicit AppSettings(QObject *parent = nullptr);

    void UpdateSettings(MySettings& NewSettings);
    MySettings GetSettings();

signals:
    void onSettingsUpdated();

protected:
    MySettings Settings;
    void ReadSettings();
    void UpdateStartup(bool Add);
};

#endif // APPSETTINGS_H
