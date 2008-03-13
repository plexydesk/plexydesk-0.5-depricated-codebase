
#ifndef FRAME_ITEM_H
#define FRAME_ITEM_H

#include <QtCore>
#include <QtGui>
#include <QSvgRenderer>

class Frame :public  QGraphicsRectItem
{
    public:
        Frame (const QRectF& rect,QGraphicsItem * parent = 0):QGraphicsRectItem(rect,parent)
        {
           render.load(QString("/usr/local/share/plexy/skins/default/welcome/background.svg"));
        }
        void  paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    private:
        QSvgRenderer render;
 //       virtual QRectF boundingRect() const { return QRectF(0,0,128*4,256); }

};

#endif 
