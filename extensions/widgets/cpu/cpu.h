#ifndef PLEXY_WIDGET_CPU_H
#define PLEXY_WIDGET_CPU_H

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>


class VISIBLE_SYM Cpu :public PlexyDesk::AbstractPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
    void  init() {} 
    Cpu(QObject * object = 0);
    virtual ~Cpu();
    virtual QGraphicsItem * item();// {};
};


#endif
