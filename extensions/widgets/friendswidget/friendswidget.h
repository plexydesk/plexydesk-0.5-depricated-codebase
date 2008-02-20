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
#include <QPainter>
#include <QTime>
#include <QWidget>
#include <QColor>
#include <QMap>

class QTimer;

namespace PlexyDesk {

class FriendsWidget : public DesktopWidget
{
    Q_OBJECT

    public:
        FriendsWidget(const QRectF &rect, QWidget *widget = 0);
        virtual ~FriendsWidget();
        virtual void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem *e, QWidget *w);
        virtual void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *e, QWidget *w);
        virtual void paintExtBackFace(QPainter *painter, const QStyleOptionGraphicsItem *e , QWidget *w);
        void setPath(const QString& path);
        void drawFriendsWidget();

    private:
        int shade;

        QPixmap  user;
        QImage _main_bg;

        QString prefix;
        QPoint clickPos;

        QMap<QString, QGraphicsItem*> items;
        QMap<QString, QGraphicsItem*> actions;
};

} // namespace PlexyDesk
#endif

