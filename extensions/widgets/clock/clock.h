#ifndef PLEXY_WIDGET_DEMO_H
#define PLEXY_WIDGET_DEMO_H

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>


class VISIBLE_SYM Clock : public QObject , public PlexyDesk::WidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::WidgetInterface)

    public:
    Clock(QObject * object = 0);
    virtual ~Clock();
    virtual QGraphicsItem * backdrop();// {};
};


#endif
