/***************************************************************************
 *   Copyright (C) 2008 by Lahiru Lakmal Priyadarshana                     *
 *   llahiru@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef FRIENDS_WIDGET_H
#define FRIENDS_WIDGET_H

#include "frienditem.h"
#include <desktopwidget.h>
#include <QImage>
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
#include <QMap>

class QTimer;

namespace PlexyDesk
{


class FriendsWidget:public DesktopWidget
{

    Q_OBJECT

	public:
	FriendsWidget(const QRectF &rect, QWidget *widget = 0);
	virtual ~FriendsWidget();
	virtual void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem * e, QWidget *);
	virtual void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
	virtual void paintExtBackFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget *);
	void setPath(QString);
	void drawFriendsWidget();
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
	
	private:

	QMap<QString, QGraphicsItem*> items;
	QMap<QString, QGraphicsItem*> actions;

	int shade;

	QPixmap  user;
	QImage _main_bg;

	QString prefix;
	QPoint clickPos;
};

} // namespace PlexyDesk
#endif

