#ifndef PLEXY_BACK_DROP_ITEM_CLASSIC_H
#define PLEXY_BACK_DROP_ITEM_CLASSIC_H

#include <QtCore>
#include <QtGui>
#include <plexy.h>
#include <desktopwidget.h>
#include "customproxy.h"

namespace PlexyDesk
{

    class WidgetItem: public DesktopWidget
    {

    Q_OBJECT
        public:
        WidgetItem(const QRectF &rect, QWidget *embeddedWidget = 0);
        virtual ~WidgetItem();
        void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QRectF boundingRect() const;
        private:
        CustomProxy *proxyWidget;
	QImage dateImg;

    };


} // namespace PlexyDesk

#endif
