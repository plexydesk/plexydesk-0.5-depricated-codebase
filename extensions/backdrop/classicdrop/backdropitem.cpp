#include "backdropitem.h"

namespace PlexyDesk
{
    BackdropItem::BackdropItem(const QRectF &rect):QGraphicsRectItem(rect)
    {
    bg = QPixmap("/home/siraj/downloads-torrents/wallpaper/water.png"); 
    }
    
    BackdropItem::~BackdropItem()
    {

    }

    void BackdropItem::paint(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget *)
    {
           p->setOpacity(0.5);
           p->drawPixmap(QDesktopWidget().screenGeometry() ,bg);
    }

    QRectF BackdropItem::boundingRect() const
    {
        return QDesktopWidget().screenGeometry();
    }

} // namespace PlexyDesk

#include "backdropitem.moc"
