/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
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
