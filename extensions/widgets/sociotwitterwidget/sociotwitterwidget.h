#ifndef SOCIOTWITTERWIDGET_H
#define SOCIOTWITTERWIDGET_H

#include <QtGui/QWidget>
#include <pluginloader.h>
#include <desktopwidget.h>
#include "sociotwitterinterface.h"


namespace PlexyDesk
{
class SocioTwitterwidget : public DesktopWidget

{
    Q_OBJECT

public:
    SocioTwitterwidget(const QRectF &rect, QWidget *widget);
    virtual ~SocioTwitterwidget();
    //virtual void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem * e, QWidget *);
    //virtual void wheelEvent(QGraphicsSceneWheelEvent * event);
    //void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setPath(QString);
    void drawWidget();;

    //private:
    //  Ui::SocioTwitterClass *ui;
};
}
#endif // SOCIOTWITTER_H
