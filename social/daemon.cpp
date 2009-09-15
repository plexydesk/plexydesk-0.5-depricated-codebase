#include "daemon.h"
#include <QtDBus>
#include <QDBusConnection>



SocialDaemon::SocialDaemon(QApplication *parent=0) : QDBusAbstractAdaptor(parent)
{

    QDBusConnection::sessionBus().registerService("org.plexydesk.social");
}

QString SocialDaemon::display()
{
    return "Hi";
}
