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
#include "customproxy.h"

#include <QtGui>

CustomProxy::CustomProxy(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsProxyWidget(parent, wFlags)
{
    timeLine = new QTimeLine(250, this);
    connect(timeLine, SIGNAL(valueChanged(qreal)),
            this, SLOT(updateStep(qreal)));
    connect(timeLine, SIGNAL(stateChanged(QTimeLine::State)),
            this, SLOT(stateChanged(QTimeLine::State)));


}

QRectF CustomProxy::boundingRect() const
{
    return QRectF(0,0,QDesktopWidget().screenGeometry().width() ,240); //QGraphicsProxyWidget::boundingRect().adjusted(0, 0, 0, 0);
}

void CustomProxy::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                   QWidget *widget)
{
    QRectF br = boundingRect();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 64));
//    painter->drawRect(boundingRect());
    
}

void CustomProxy::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsProxyWidget::hoverEnterEvent(event);
    scene()->setActiveWindow(this);
    if (timeLine->direction() != QTimeLine::Forward)
        timeLine->setDirection(QTimeLine::Forward);
    if (timeLine->state() == QTimeLine::NotRunning) 
        timeLine->start();
}
        
void CustomProxy::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsProxyWidget::hoverLeaveEvent(event);
    if (timeLine->direction() != QTimeLine::Backward)
        timeLine->setDirection(QTimeLine::Backward);
    if (timeLine->state() == QTimeLine::NotRunning) 
        timeLine->start();
}
        
void CustomProxy::updateStep(qreal step)
{
    QRectF r = boundingRect();
  /* 
    setTransform(QTransform().rotate(step * 30, Qt::XAxis)
                 .rotate(step * 10, Qt::YAxis)
                 .rotate(step * 10, Qt::ZAxis));
   /              */

//   setTransform(QTransform().rotate(step * 30, Qt::XAxis));
}

void CustomProxy::stateChanged(QTimeLine::State state)
{
    if (state == QTimeLine::Running) {
	if (timeLine->direction() == QTimeLine::Forward)
	    setCacheMode(NoCache);
    } else if (state == QTimeLine::NotRunning) {
	if (timeLine->direction() == QTimeLine::Backward)
	    setCacheMode(DeviceCoordinateCache);
    }
}


#include "customproxy.moc"
