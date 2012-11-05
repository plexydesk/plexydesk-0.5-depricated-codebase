/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLEXYDESK_SCROLL_WIDGET_H
#define PLEXYDESK_SCROLL_WIDGET_H
#include <plexy.h>
#include <config.h>
#include <desktopwidget.h>

#include <QGraphicsItem>
#include <QGraphicsObject>

namespace PlexyDesk
{

class ScrollWidget : public DesktopWidget
{
   Q_OBJECT
public:
   ScrollWidget(const QRectF &rect, QGraphicsObject *parent = 0);
   virtual ~ScrollWidget();

   void addWidget(QGraphicsItem *widget);
   void scrollBy(int x, int y);

private:
   virtual void wheelEvent (QGraphicsSceneWheelEvent * event);

   class Private;
   Private *const d;
};
}

#endif
