/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal <llahiru@gmail.com>
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

#ifndef YOUTUBE_WIDGET_H
#define YOUTUBE_WIDGET_H

#include "videoentity.h"

#include <datainterface.h>
#include <pluginloader.h>

#include <qimage.h>
#include <desktopwidget.h>
#include <QPaintDevice>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QPaintEvent>
#include <X11/Xlib.h>
#include <X11/extensions/composite.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xdamage.h>
#include <QPainter>
#include <QTime>
#include <QX11Info>
#include <QWidget>
#include <QColor>

class QTimer;

namespace PlexyDesk
{


class YouTubeWidget:public DesktopWidget
{

    Q_OBJECT

	public:
        YouTubeWidget(const QRectF &rect, QWidget *widget = 0);
        virtual ~YouTubeWidget();
	virtual void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem * e, QWidget *);
	void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
	void setPath(QString);
	void drawWidget();;
        /*
         virtual void mousePressEvent( QMouseEvent *e)
	{
		clickPos = e->pos();
		setCursor(Qt::SizeAllCursor);
        }
        virtual void mouseMoveEvent( QMouseEvent *e)
	{
	move( e->globalPos() - clickPos );
	}
	virtual void mouseReleaseEvent(QMouseEvent *e)
	{
		setCursor(Qt::ArrowCursor);
	}
*/
    
    public slots:
        void data(QVariant&);
        void drawItems();
    signals:
        void dataChanged();
	
	private:

	int shade;
	
	QImage m_bg;
    QImage mItem_bg;

	QString prefix;
	QPoint clickPos;
    
    PlexyDesk::DataInterface * utubeEngine;
    
    QList<VideoEntity> mVideos;
    QMap<QString,QVariant> mVariantMap;
};

} // namespace PlexyDesk
#endif

