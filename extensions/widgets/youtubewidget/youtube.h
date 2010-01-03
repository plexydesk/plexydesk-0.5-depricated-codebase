#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <widgetinterface.h>
#include <widgetplugin.h>

class VISIBLE_SYM YouTube: public PlexyDesk::WidgetPlugin
{
    Q_OBJECT

public:
    YouTube(QObject * object = 0);
    virtual ~YouTube();
    QGraphicsItem * item();
};

#endif
