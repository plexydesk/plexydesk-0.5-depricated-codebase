#ifndef PLEXY_WIDGET_DEMO_H
#define PLEXY_WIDGET_DEMO_H

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>


class VISIBLE_SYM Clock :public PlexyDesk::AbstractPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
   void  init() {} 
    Clock(QObject * object = 0);
    virtual ~Clock();
    virtual QGraphicsItem * item();// {};
};


#endif
