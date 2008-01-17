#ifndef FRIEND_ITEM_H
#define FRIEND_ITEM_H

#include <QGraphicsItem>

namespace PlexyDesk
{

class FriendItem : public QObject, public QGraphicsItem
 {

 Q_OBJECT

 public:
     FriendItem(QGraphicsItem * parent);
     virtual ~FriendItem();
     QRectF boundingRect() const;
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

 private:
     int shade;
     QImage panel;
     QRectF rect;
 };

}

#endif
