#include "youtubeinterface.h"
#include "youtube.h"
#include  <QtCore>
#include <pluginloader.h>
#include  <QtGui>


YouTubeInterface::YouTubeInterface(QObject * object)
{
}


PlexyDesk::BasePlugin * YouTubeInterface::instance()
{
	qDebug("Calling YouTubeInterface::instance");
	return new YouTube(this);
}

Q_EXPORT_PLUGIN2(YouTubeInterface,YouTubeInterface)
#include "youtubeinterface.moc"
