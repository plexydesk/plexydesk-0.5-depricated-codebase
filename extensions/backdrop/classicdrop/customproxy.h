#ifndef CUSTOMPROXY_H
#define CUSTOMPROXY_H

#include <QtCore/qtimeline.h>
#include <QtGui/qgraphicsproxywidget.h>

class CustomProxy : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    CustomProxy(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);

    QRectF boundingRect() const;
    void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                          QWidget *widget);                          


protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
                                          
private slots:
    void updateStep(qreal step);
    void stateChanged(QTimeLine::State);

private:
    QTimeLine *timeLine;
};

#endif
