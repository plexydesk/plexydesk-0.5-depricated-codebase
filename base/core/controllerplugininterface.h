#ifndef CONTROLLERPLUGININTERFACE_H
#define CONTROLLERPLUGININTERFACE_H

#include <plexy.h>

#include <QSharedPointer>
#include <QObject>

#include <controllerinterface.h>

namespace PlexyDesk {

class ControllerPluginInterface : public QObject
{
    Q_OBJECT

public:
    ControllerPluginInterface(QObject *parent = 0);
    ~ControllerPluginInterface();

    virtual QSharedPointer<ControllerInterface> controller() = 0;
};

}

Q_DECLARE_INTERFACE(PlexyDesk::ControllerPluginInterface, "org.plexydesk.ControllerPluginInterface")
#endif // CONTROLLERPLUGININTERFACE_H
