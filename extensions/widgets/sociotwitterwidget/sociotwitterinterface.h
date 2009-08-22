#ifndef PLEXY_YOUTUBE_I
#define PLEXY_YOUTUBE_I

#include <plexy.h>

#include <abstractplugininterface.h>
#include <datainterface.h>
#include <widgetinterface.h>


class VISIBLE_SYM SocioTwitterInterface :public PlexyDesk::AbstractPluginInterface
{
	Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
    	SocioTwitterInterface(QObject * object = 0);
    	PlexyDesk::BasePlugin * instance () ;
};

#endif
