#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <widgetplugin.h>

class YouTube : public PlexyDesk::WidgetPlugin
{
    Q_OBJECT

public:
    YouTube(QObject *object = 0);
    virtual ~YouTube();
    QGraphicsItem *view();
};

#endif
