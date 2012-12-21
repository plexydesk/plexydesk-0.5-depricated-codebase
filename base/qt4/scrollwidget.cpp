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

#include <QGraphicsSceneWheelEvent>
#include "scrollwidget.h"

namespace PlexyDesk
{

class ScrollWidget::Private
{

public:
    Private() {}
    ~Private() {}

    QGraphicsItem *mChildItem;
};

ScrollWidget::ScrollWidget(const QRectF &rect, QGraphicsObject *parent)
    : DesktopWidget(rect,parent), d (new Private)
{
    d->mChildItem = 0;
    setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

ScrollWidget::~ScrollWidget()
{
    delete d;
}

void ScrollWidget::addWidget(QGraphicsItem *widget)
{
   if (!widget) {
       return;
   }

   if (widget == d->mChildItem)
       return;

   if (d->mChildItem != 0) {
       d->mChildItem->setParentItem(0);
       delete d->mChildItem;
   }

   if (widget) {
       widget->setParentItem(this);
       d->mChildItem = widget;
   }
}

void ScrollWidget::scrollBy(int x, int y)
{
    if (d->mChildItem) {
        //resetric to viewport
        int y_pos = d->mChildItem->y() + y;
        int view_height = this->boundingRect().height();
        int y_max = (-1) * (d->mChildItem->boundingRect().height() - view_height);

        if (y_pos < (this->y()) && y_pos > y_max)
            d->mChildItem->setPos(d->mChildItem->x() + x, d->mChildItem->y() + y);
        else if ( !(y_pos < (this->y())) )
            d->mChildItem->setPos(d->mChildItem->x() + x, this->y());
    }
}

void ScrollWidget::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (event->delta() < 0) {
        scrollBy (0, -10);
    } else {
        scrollBy (0, 10);
    }
    event->accept();
}

void ScrollWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void ScrollWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void ScrollWidget::dragMoveEvent(QGraphicsSceneDragDropEvent * /*event*/)
{
    qDebug() << Q_FUNC_INFO;
}

} // namespace PlexyDesk
