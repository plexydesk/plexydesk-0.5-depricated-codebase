#include "youtubeinterface.h"
#include "youtube.h"
#include <pluginloader.h>

#include <QtPlugin>

YouTubeInterface::YouTubeInterface(QObject *object)
{
}

QObject *YouTubeInterface::instance()
{
    return new YouTube(this);
}

Q_EXPORT_PLUGIN2(plexyyoutube, YouTubeInterface)
