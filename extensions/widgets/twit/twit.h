#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <widgetinterface.h>
#include <widgetplugin.h>

class VISIBLE_SYM Twit:public PlexyDesk::WidgetPlugin
{
    Q_OBJECT

    public:
        Twit(QObject * object = 0);
        virtual ~Twit();
        QGraphicsItem * item();
};

#endif
