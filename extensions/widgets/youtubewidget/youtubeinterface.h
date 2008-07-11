#ifndef PLEXY_YOUTUBE_I
#define PLEXY_YOUTUBE_I


#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>

class VISIBLE_SYM YouTubeInterface :public PlexyDesk::AbstractPluginInterface
{
	Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
    	YouTubeInterface(QObject * object = 0);
    	PlexyDesk::BasePlugin * instance () ;
};

#endif
