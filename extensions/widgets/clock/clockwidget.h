/***************************************************************************
 *   Copyright (C) 2005,2006,2007 by Siraj Razick                          *
 *   siraj@kdemail.net                                                     *
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





#ifndef CLOCK
#define CLOCK


#include <qimage.h>
#include <desktopwidget.h>
#include <qpaintdevice.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qtimer.h>
//#include <q3mainwindow.h>
//Added by qt3to4:
#include <QPaintEvent>
#include <qtimer.h>
#include <QTime>
#ifdef Q_WS_X11
#include <QX11Info>
#endif
#include <QWidget>
#include <QColor>


class QTimer;

namespace PlexyDesk
{


class ClockWidget:public DesktopWidget
{

    Q_OBJECT

public:
	ClockWidget (const QRectF &rect, QWidget *widget = 0);
	virtual ~ClockWidget();
	virtual void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem * e, QWidget *);
	void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
	void setPath(QString);
	void drawClockWidget();
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
void drawSeconds();

private:


QTimer  * sec_timer;
QTimer  * mins_timer;
QTimer  * hours_timer;

QPixmap  _secs_hand;
QPixmap  _mins_hand;
QPixmap  _hour_hand;

QStringList _hour_path;
QStringList _mins_path;
QStringList _secs_path;

QImage _clock_bg;
QImage _bg;
QString prefix;

double _secs;
double _mins;
double _hour;


int seconds;
int s_a;
QTime time;
QPoint clickPos;
QPixmap face;
/*additional ClockWidget images : Opera ClockWidget seems to do so*/
QImage gloss;
QPixmap thedot;
int shade;
QImage lens;
QPixmap date;

};

} // namespace PlexyDesk
#endif

