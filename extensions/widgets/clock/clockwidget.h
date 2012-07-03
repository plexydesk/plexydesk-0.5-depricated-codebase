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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
***************************************************************************/

#ifndef CLOCK
#define CLOCK

#include <desktopwidget.h>

#include <QTime>

class ClockWidget : public PlexyDesk::DesktopWidget
{
    Q_OBJECT

public:
    ClockWidget(const QRectF &rect, QWidget *widget = 0);
    virtual ~ClockWidget();

    void paintFrontView(QPainter *painter, const QRectF &r);
    void paintDockView(QPainter *painter, const QRectF &rect);
    void paintRotatedView(QPainter *painter, const QRectF &rect);

    void setPath(QString);
    void drawClockWidget();
public slots:
    void drawSeconds();

private:
    QTimer *sec_timer;
    QTimer *mins_timer;
    QTimer *hours_timer;

    QPixmap _secs_hand;
    QPixmap _mins_hand;
    QPixmap _hour_hand;

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
    QImage gloss;
    QPixmap thedot;
    int shade;
    QImage lens;
    QPixmap mLensePixmap;
    QPixmap mGlossPixmap;
    QPixmap date;

};
#endif

