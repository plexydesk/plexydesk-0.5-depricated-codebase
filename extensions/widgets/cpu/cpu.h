#ifndef PLEXY_WIDGET_CPU_H
#define PLEXY_WIDGET_CPU_H

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>


class VISIBLE_SYM Cpu : public QObject , public PlexyDesk::WidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::WidgetInterface)

    public:
    Cpu(QObject * object = 0);
    virtual ~Cpu();
    virtual QGraphicsItem * item();// {};
};


#endif
