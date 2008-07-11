#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>
#include <widgetplugin.h>

class VISIBLE_SYM YouTube:public PlexyDesk::WidgetPlugin
{
    Q_OBJECT

    public:
        YouTube(QObject * object = 0);
        virtual ~YouTube();
        virtual QGraphicsItem * item();
    
/*    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
    void  init() {} 
    YouTube(QObject * object = 0);
    virtual ~YouTube();
    virtual QGraphicsItem * item();// {};*/
};

#endif
