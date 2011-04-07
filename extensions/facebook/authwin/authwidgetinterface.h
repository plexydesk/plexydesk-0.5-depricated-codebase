#ifndef PLEXY_FB_AUTH_I
#define PLEXY_FB_AUTH_I

#include <plexy.h>
#include <abstractplugininterface.h>
#include <datainterface.h>
#include <widgetinterface.h>


class PLEXYDESK_EXPORT AuthInterface : public PlexyDesk::AbstractPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

public :
        AuthInterface(QObject *object = 0);
    PlexyDesk::BasePlugin *instance();
};

#endif
