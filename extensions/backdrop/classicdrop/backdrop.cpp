#include "backdrop.h"


ClassicBackdrop::ClassicBackdrop(QObject * object):QObject(object)
{

}

ClassicBackdrop::~ClassicBackdrop()
{

}

QGraphicsItem * backdrop()
{
return new QGraphicsPixmapItem(QPixmap("/home/siraj/downloads-torrents/wallpaper/water.png"));
}

#include "backdrop.moc"
