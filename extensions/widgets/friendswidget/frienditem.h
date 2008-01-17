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

#ifndef FRIEND_ITEM_H
#define FRIEND_ITEM_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QImage>

namespace PlexyDesk
{

class FriendItem : public QObject, public QGraphicsItem
 {

 Q_OBJECT

 public:
     FriendItem(QGraphicsItem * parent);
     virtual ~FriendItem();
     QRectF boundingRect() const;
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

     void mousePressEvent ( QGraphicsSceneMouseEvent * event );
     void setName(QString n);
     QString getName();
     void setIcon(QPixmap m);

	signals:
	void clicked();

 private:
     int shade;
     QImage panel;
     QRectF rect;

     QString name;
     QPixmap icon;
 };

}

#endif
