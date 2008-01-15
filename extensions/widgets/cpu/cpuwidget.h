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

#ifndef CPU_WIDGET_H
#define CPU_WIDGET_H

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


class CpuWidget:public DesktopWidget
{

    Q_OBJECT

	public:
	CpuWidget(const QRectF &rect, QWidget *widget = 0);
	virtual ~CpuWidget();
	virtual void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem * e, QWidget *);
	void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
	void setPath(QString);
	void drawCpuWidget();
	void getBaseData();
	void getNxtData();
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
	void drawMeter();
	
	private:
	QTimer  *meter_timer;
	long user_a;
	long nice_a;
	long sys_a;
	long idle_a;

	long user_b;
	long nice_b;
	long sys_b;
	long idle_b;

	long usage;
	long total;

	double percen;

	int shade;
	
	QPixmap  _meter_hand;
	QPixmap thedot;
	QImage _cpu_bg;
	QImage gloss;

	QString prefix;
	QPoint clickPos;
};

} // namespace PlexyDesk
#endif

