#include "googleweatherinterface.h"
#include "googleweather.h"
#include <pluginloader.h>

#include <QtPlugin>

googleweatherInterface::googleweatherInterface(QObject * object)
{
}

PlexyDesk::BasePlugin * googleweatherInterface::instance()
{
	return new googleweather(this);
}

Q_EXPORT_PLUGIN2(googleweatherInterface,googleweatherInterface)
