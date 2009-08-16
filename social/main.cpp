#include <QApplication>
#include <QDBusConnection>
#include "daemon.h"
//dbus-send --session --print-reply --dest=org.plexydesk.social /services
//local.socioplexy.Configuration.display

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    new SocialDaemon(&app);
    QDBusConnection::sessionBus().registerObject("/services", &app);
    return app.exec();
}
