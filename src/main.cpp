#include "traycontroller.h"

#include <QApplication>
#include <QFontDatabase>
#include <QFile>

int main(int argc, char *argv[])
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    // For use QSettings
    QCoreApplication::setOrganizationName("RIO");
    QCoreApplication::setApplicationName("RioClipboardManager");

    QFile f(":/styles/dark_pro.qss");
    f.open(QFile::ReadOnly);
    qApp->setStyleSheet(f.readAll());

    int id = QFontDatabase::addApplicationFont(":/fonts/Inter-Regular.otf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    QFont font(family);
    font.setPixelSize(14);
    app.setFont(font);

    TrayController tray;

    return app.exec();
}
