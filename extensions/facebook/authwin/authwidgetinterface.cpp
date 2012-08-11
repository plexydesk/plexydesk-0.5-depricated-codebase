#include "authwidgetinterface.h"
#include "authplugin.h"
#include <QtPlugin>

QObject *AuthInterface::instance()
{
    return new AuthPlugin();
}

Q_EXPORT_PLUGIN2(plexyauth, AuthInterface)
