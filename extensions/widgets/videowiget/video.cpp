#include "video.h"
#include "videoitem.h"
#include <QtWebKit>


VideoWidget::VideoWidget(QObject * object):QObject(object)
{

}

VideoWidget::~VideoWidget()
{

}

QGraphicsItem * VideoWidget::backdrop()
{

return new  PlexyDesk::VideoItem(QRectF(0,0,380,300),0);
}

Q_EXPORT_PLUGIN2(VideoWidget,VideoWidget)
#include "video.moc"
