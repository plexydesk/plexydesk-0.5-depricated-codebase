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

  //  setCacheMode(DeviceCoordinateCache);
    setCacheMode(NoCache);
//    setCacheMode(QGraphicsItem::ItemCoordinateCache);
/*
    setTransform(QTransform().rotate( 30, Qt::XAxis)
                 .rotate( 10, Qt::YAxis)
                 .rotate( 10, Qt::ZAxis));
                 */
}

QRectF CustomProxy::boundingRect() const
{
    return QGraphicsProxyWidget::boundingRect().adjusted(0, 0, 0, 0);
}

void CustomProxy::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                   QWidget *widget)
{
    QRectF br = boundingRect();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 64));
   painter->drawRect(boundingRect());
    
}

void CustomProxy::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    /*
    QGraphicsProxyWidget::hoverEnterEvent(event);
    scene()->setActiveWindow(this);
    if (timeLine->direction() != QTimeLine::Forward)
        timeLine->setDirection(QTimeLine::Forward);
    if (timeLine->state() == QTimeLine::NotRunning) 
        timeLine->start();
        */
}
        
void CustomProxy::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    /*
    QGraphicsProxyWidget::hoverLeaveEvent(event);
    if (timeLine->direction() != QTimeLine::Backward)
        timeLine->setDirection(QTimeLine::Backward);
    if (timeLine->state() == QTimeLine::NotRunning) 
        timeLine->start();
        */
}
        
void CustomProxy::updateStep(qreal step)
{
    QRectF r = boundingRect();
  /* 
    setTransform(QTransform().rotate(step * 30, Qt::XAxis)
                 .rotate(step * 10, Qt::YAxis)
                 .rotate(step * 10, Qt::ZAxis));
   /              */

// setTransform(QTransform().rotate(step * 30, Qt::XAxis));
}

void CustomProxy::stateChanged(QTimeLine::State state)
{
    /*
    if (state == QTimeLine::Running) {
	if (timeLine->direction() == QTimeLine::Forward)
	    setCacheMode(NoCache);
    } else if (state == QTimeLine::NotRunning) {
	if (timeLine->direction() == QTimeLine::Backward)
	    setCacheMode(DeviceCoordinateCache);
    }
    */
}


#include "customproxy.moc"
