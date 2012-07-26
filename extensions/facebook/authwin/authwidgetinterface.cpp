#include "authwidgetinterface.h"
#include "authplugin.h"
#include <QtPlugin>


PlexyDesk::AbstractSource *AuthInterface::instance()
{
    return new AuthPlugin(this);
}

Q_EXPORT_PLUGIN2(plexyauth, AuthInterface)
