#ifndef PLEXY_BACK_DROP_ITEM_CLASSIC_H
#define PLEXY_BACK_DROP_ITEM_CLASSIC_H

#include <QtCore>
#include <QtGui>
#include <plexy.h>


namespace PlexyDesk
{

    class WidgetItem: public QObject, public QGraphicsRectItem
    {

    Q_OBJECT
        public:
        WidgetItem(const QRectF &rect, QWidget *embeddedWidget = 0);
        virtual ~WidgetItem();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QRectF boundingRect() const;
        private:
        QGraphicsProxyWidget *proxyWidget;

    };


} // namespace PlexyDesk

#endif
