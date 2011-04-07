
#ifndef FRAME_ITEM_H
#define FRAME_ITEM_H

#include <QtCore>
#include <QtGui>
#include <QSvgRenderer>
#include <config.h>

class Frame : public QGraphicsRectItem
{
public:
    Frame(const QRectF &rect, QGraphicsItem *parent = 0) : QGraphicsRectItem(rect, parent) {
        setFlag(QGraphicsItem::ItemIsMovable, true);
        render.load(QString(PLEXPREFIX) + "/share/plexy/skins/default/welcome/welcome.svg");
    }
    void  paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    QSvgRenderer render;
//       virtual QRectF boundingRect() const { return QRectF(0,0,128*4,256); }

};

#endif
