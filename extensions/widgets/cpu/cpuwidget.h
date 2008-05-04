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

#ifndef CPU_WIDGET_H
#define CPU_WIDGET_H

#include <QtCore>
#include <QtGui>

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

