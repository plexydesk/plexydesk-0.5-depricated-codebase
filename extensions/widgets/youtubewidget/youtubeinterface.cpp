#include "youtubeinterface.h"
#include "youtube.h"
#include  <QtCore>
#include  <QtGui>


YouTubeInterface::YouTubeInterface(QObject * object)
{
}


PlexyDesk::BasePlugin * YouTubeInterface::instance()
{
	return new YouTube();
}

Q_EXPORT_PLUGIN2(YouTubeInterface,YouTubeInterface)
#include "youtubeinterface.moc"
