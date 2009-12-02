#include "twitinterface.h"
#include "twit.h"
#include <pluginloader.h>

#include <QtPlugin>

TwitInterface::TwitInterface(QObject * object)
{
}

PlexyDesk::BasePlugin * TwitInterface::instance()
{
	return new Twit(this);
}

Q_EXPORT_PLUGIN2(TwitInterface,TwitInterface)
