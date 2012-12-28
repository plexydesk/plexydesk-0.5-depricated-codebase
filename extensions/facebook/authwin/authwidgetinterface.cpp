#include "authwidgetinterface.h"
#include "authplugin.h"
#include <QtPlugin>

QSharedPointer<PlexyDesk::ControllerInterface> AuthInterface::controller()
{
    QSharedPointer<PlexyDesk::ControllerInterface> obj =
            QSharedPointer<PlexyDesk::ControllerInterface>(new AuthPlugin(this), &QObject::deleteLater);

    return obj;
}

