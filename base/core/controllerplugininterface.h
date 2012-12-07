#ifndef CONTROLLERPLUGININTERFACE_H
#define CONTROLLERPLUGININTERFACE_H

#include <plexy.h>

#include <QSharedPointer>
#include <QObject>

#include <controllerinterface.h>

namespace PlexyDesk {

class ControllerPluginInterface
{
public:
    ControllerPluginInterface() {}
    virtual ~ControllerPluginInterface() {};

    virtual QSharedPointer<ControllerInterface> controller() = 0;
};

}

Q_DECLARE_INTERFACE(PlexyDesk::ControllerPluginInterface, "org.plexydesk.ControllerPluginInterface")
#endif // CONTROLLERPLUGININTERFACE_H
