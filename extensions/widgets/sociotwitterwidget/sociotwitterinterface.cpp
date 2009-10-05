#include "sociotwitter.h"
#include <pluginloader.h>

#include <QtPlugin>

SocioTwitterInterface::SocioTwitterInterface(QObject * object)
{
}

PlexyDesk::BasePlugin * SocioTwitterInterface::instance()
{
	return new SocioTwitter(this);
}

Q_EXPORT_PLUGIN2(SocioTwitterInterface,SocioTwitterInterface)
