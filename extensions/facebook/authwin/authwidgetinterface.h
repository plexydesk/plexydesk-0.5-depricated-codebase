#ifndef PLEXY_FB_AUTH_I
#define PLEXY_FB_AUTH_I

#include <plexy.h>
#include <abstractplugininterface.h>
#include <controllerinterface.h>
#include <controllerplugininterface.h>

class AuthInterface : public QObject, public PlexyDesk::ControllerPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::ControllerPluginInterface)

public :
    AuthInterface(QObject *parent = 0) : PlexyDesk::ControllerPluginInterface() {}
    virtual ~AuthInterface() {}

    QSharedPointer<PlexyDesk::ControllerInterface> controller();

};

#endif
