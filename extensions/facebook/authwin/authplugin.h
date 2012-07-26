#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <widgetplugin.h>

class AuthPlugin : public PlexyDesk::WidgetPlugin
{
    Q_OBJECT

public:
    AuthPlugin(QObject *object = 0);
    virtual ~AuthPlugin();
    QGraphicsItem *item();
};

#endif
