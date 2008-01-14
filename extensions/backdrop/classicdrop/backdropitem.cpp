#include "backdropitem.h"
#include "customproxy.h"
#include "pictureflow.h"

namespace PlexyDesk
{
    BackdropItem::BackdropItem(const QRectF &rect):QGraphicsRectItem(rect)
    {
        PictureFlow* w = new PictureFlow; 
        w->setSlideSize(QSize(320/2, 240/2));
        w->resize(QDesktopWidget().screenGeometry().width() ,240);
        w->setBackgroundColor(Qt::white);
        bg = QPixmap("/usr/share/plexy/skins/default/default.png"); 
        CustomProxy *proxyWidget  = new   CustomProxy ( this);
        proxyWidget->setWidget(w);
        proxyWidget->setGeometry(boundingRect().adjusted(25, 25, -25, -25));
        proxyWidget->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        for (int i = 0; i < 10 ; i++) {
            w->addSlide(QPixmap("/usr/share/plexy/skins/default/default.png"));
        }
        w->setCenterIndex(w->slideCount()/2);
        w->show();
    }
    
    BackdropItem::~BackdropItem()
    {

    }

    void BackdropItem::paint(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget *)
    {
        //   p->setOpacity(0.5);
         //  p->drawPixmap(QRect(0,0,159,174) ,QPixmap("/usr/share/plexy/skins/default/buttons/ikon-4.png"));
    }

    QRectF BackdropItem::boundingRect() const
    {
        return QRectF(0,0,QDesktopWidget().screenGeometry().width() ,240); 
    }

} // namespace PlexyDesk

#include "backdropitem.moc"
