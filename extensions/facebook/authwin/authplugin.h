#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <viewcontrollerplugin.h>

class AuthPlugin : public PlexyDesk::ViewControllerPlugin
{
    Q_OBJECT

public:
    AuthPlugin(QObject *object = 0);
    virtual ~AuthPlugin();
    QGraphicsItem *view();
};

#endif
