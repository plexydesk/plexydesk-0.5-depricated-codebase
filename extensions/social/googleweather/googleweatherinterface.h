#ifndef PLEXY_GOOGLEWEATHER_I
#define PLEXY_GOOGLEWEATHER_I

#include <plexy.h>

#include <abstractplugininterface.h>
#include <datainterface.h>
#include <widgetinterface.h>


class VISIBLE_SYM googleweatherInterface :public PlexyDesk::AbstractPluginInterface
{
	Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
    	googleweatherInterface(QObject * object = 0);
    	PlexyDesk::BasePlugin * instance () ;
};

#endif
