#ifndef PLEXY_FB_AUTH_I
#define PLEXY_FB_AUTH_I

#include <plexy.h>
#include <abstractplugininterface.h>

class PLEXYDESK_EXPORT AuthInterface : public PlexyDesk::AbstractPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

public :
    PlexyDesk::AbstractSource *instance();
};

#endif
