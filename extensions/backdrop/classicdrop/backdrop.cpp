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
    p->fillRect(r.x(),r.y(),width,height,paint);
}

Q_EXPORT_PLUGIN2(ClassicBackdrop,ClassicBackdrop)
#include "backdrop.moc"
