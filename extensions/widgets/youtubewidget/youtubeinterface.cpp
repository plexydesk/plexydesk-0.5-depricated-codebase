#include "youtubeinterface.h"
#include "youtube.h"
#include <pluginloader.h>

#include <QtPlugin>

YouTubeInterface::YouTubeInterface(QObject *object)
{
}

QSharedPointer<PlexyDesk::ControllerInterface> YouTubeInterface::controller()
{
    QSharedPointer<PlexyDesk::ControllerInterface> obj =
            QSharedPointer<PlexyDesk::ControllerInterface>(new YouTube(this), &QObject::deleteLater);

    return obj;
}

Q_EXPORT_PLUGIN2(plexyyoutube, YouTubeInterface)
