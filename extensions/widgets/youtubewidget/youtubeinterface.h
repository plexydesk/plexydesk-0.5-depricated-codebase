#ifndef PLEXY_YOUTUBE_I
#define PLEXY_YOUTUBE_I

#include <plexy.h>

#include <controllerplugininterface.h>


class YouTubeInterface : public QObject, public PlexyDesk::ControllerPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::ControllerPluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.youtubewidget")

public :
    YouTubeInterface(QObject *object = 0);

    QSharedPointer<PlexyDesk::ControllerInterface> controller();


};

#endif
