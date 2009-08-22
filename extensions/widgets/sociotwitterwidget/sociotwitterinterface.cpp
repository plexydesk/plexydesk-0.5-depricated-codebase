#include "youtubeinterface.h"
#include "sociotwitter.h"
#include <pluginloader.h>

#include <QtPlugin>

YouTubeInterface::SocioTwitterInterface(QObject * object)
{
}

PlexyDesk::BasePlugin * SocioTwitterInterface::instance()
{
	return new SocioTwitter(this);
}

Q_EXPORT_PLUGIN2(SocioTwitterInterface,SocioTwitterInterface)
