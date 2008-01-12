#include "backdrop.h"
#include "backdropitem.h"


ClassicBackdrop::ClassicBackdrop(QObject * object):QObject(object)
{
    bg = QPixmap("/usr/share/plexy/skins/default/default.png");

    width = QDesktopWidget().availableGeometry().width();
    height = QDesktopWidget().availableGeometry().height();
    
    QImage img(width,height,QImage::Format_ARGB32_Premultiplied);
    QPainter p;
    p.begin(&img);
    p.drawPixmap(QRect(0,0,width,height),bg);
    p.end();

    paint.setTextureImage(img);
}

ClassicBackdrop::~ClassicBackdrop()
{

}

QGraphicsItem * ClassicBackdrop::backdrop()
{
return new  PlexyDesk::BackdropItem(QRectF(0,0,200,200));//QGraphicsPixmapItem(QPixmap("/home/siraj/downloads-torrents/wallpaper/water.png"));
}

void ClassicBackdrop::render(QPainter *p,QRectF r)
{
    QRect er = p->matrix().inverted().mapRect(QRect(r.x(),r.y(),r.width(),r.height())).adjusted(-1, -1, 1, 1);
    p->fillRect(er.x(),er.y(),er.width(),er.height(),paint);
}

Q_EXPORT_PLUGIN2(ClassicBackdrop,ClassicBackdrop)
#include "backdrop.moc"
