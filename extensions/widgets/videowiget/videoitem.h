#ifndef PLEXY_BACK_DROP_ITEM_CLASSIC_H
#define PLEXY_BACK_DROP_ITEM_CLASSIC_H

#include <QtCore>
#include <QtGui>
#include <plexy.h>
#include <desktopwidget.h>
#include "vplayer.h"

namespace PlexyDesk
{

    class VideoItem: public DesktopWidget
    {

    Q_OBJECT
        public:
        VideoItem(const QRectF &rect, QWidget *embeddedWidget = 0);
        virtual ~VideoItem();
        void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
        QRectF boundingRect() const;
public slots:
    void getImage(QImage & img);
    void loop();
    
        private:
	QImage dateImg;
	VPlayer *vid;
	QImage *vidsurf;
	QBrush brush;
        QPixmap snap;
        int frameno;
        bool snapped;

    };


} // namespace PlexyDesk

#endif
