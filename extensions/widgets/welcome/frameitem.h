
#ifndef FRAME_ITEM_H
#define FRAME_ITEM_H

#include <QtCore>
#include <QtGui>

class Frame :public  QGraphicsRectItem
{
    public:
        Frame (const QRectF& rect,QGraphicsItem * parent = 0):QGraphicsRectItem(rect,parent) {}
        void  paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
 //       virtual QRectF boundingRect() const { return QRectF(0,0,128*4,256); }

};

#endif 
