#ifndef PLEXY_WIDGET_DEMO_H
#define PLEXY_WIDGET_DEMO_H

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>


class VISIBLE_SYM WidgetDemo :public PlexyDesk::WidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::WidgetInterface)

    public:
    WidgetDemo(QObject * object = 0);
    virtual ~WidgetDemo();
    virtual QGraphicsItem * item();// {};
};


#endif
