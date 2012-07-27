#ifndef PLEXY_YOUTUBE_I
#define PLEXY_YOUTUBE_I

#include <plexy.h>

#include <abstractplugininterface.h>


class YouTubeInterface : public PlexyDesk::AbstractPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

public :
        YouTubeInterface(QObject *object = 0);
        PlexyDesk::AbstractSource *instance();
};

#endif
