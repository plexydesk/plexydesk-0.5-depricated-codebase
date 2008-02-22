#include "backdrop.h"
#include <desktopwidget.h>

ClassicBackdrop::ClassicBackdrop(QObject * object)
{
    bg = QPixmap("/usr/share/plexy/skins/default/default.png");
    width = QDesktopWidget().availableGeometry().width();
    height = QDesktopWidget().availableGeometry().height();
    img = QImage(width,height,QImage::Format_ARGB32_Premultiplied);
    QPainter p;
    p.begin(&img);
    p.drawPixmap(QRect(0,0,width,height),bg);
    paint.setTextureImage(img);
    loadData("flickerengine");
}

ClassicBackdrop::~ClassicBackdrop()
{

}

void ClassicBackdrop::data(QVariant& data)
{
    QImage wall(QImage::fromData(data.toByteArray()));
    QPainter p ;
    p.begin(&img);
    p.drawImage(QRect(0,0,width,height),wall);
    p.end();
    qDebug()<<img.size()<<endl;
    paint.setTextureImage(img);
}

void ClassicBackdrop::render(QPainter *p,QRectF r)
{
    p->fillRect(r.x(),r.y(),r.width(),r.height(),paint);
}

Q_EXPORT_PLUGIN2(ClassicBackdrop,ClassicBackdrop)
#include "backdrop.moc"
