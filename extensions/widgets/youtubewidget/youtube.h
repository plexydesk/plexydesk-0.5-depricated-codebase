#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <viewcontrollerplugin.h>

class YouTube : public PlexyDesk::ViewControllerPlugin
{
    Q_OBJECT

public:
    YouTube(QObject *object = 0);
    virtual ~YouTube();
    QGraphicsItem *view();
};

#endif
