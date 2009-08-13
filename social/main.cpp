#include <QApplication>
#include <QDBusConnection>
#include "daemon.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    new SocialDaemon(&app);
    QDBusConnection::sessionBus().registerObject("/services", &app);
    return app.exec();
}
