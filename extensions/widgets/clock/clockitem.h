#ifndef PLEXY_CLOCK_H
#define PLEXY_CLOCK_H

#include <QtCore>
#include <QtGui>
#include <plexy.h>
#include <desktopwidget.h>

namespace PlexyDesk
{

    class ClockItem: public DesktopWidget
    {

    Q_OBJECT
        public:
        ClockItem(const QRectF &rect, QWidget *embeddedWidget = 0);
        virtual ~ClockItem();
        void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QRectF boundingRect() const;
        private:
	QImage dateImg;

    };


} // namespace PlexyDesk

#endif
