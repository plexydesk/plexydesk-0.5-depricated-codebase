#ifndef PLEXY_WIDGET_VIDEO_H
#define PLEXY_WIDGET_VIDEO_H

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>


class VISIBLE_SYM VideoWidget :public PlexyDesk::WidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::WidgetInterface)

    public:
    VideoWidget(QObject * object = 0);
    virtual ~VideoWidget();
    virtual QGraphicsItem * item();// {};


};


#endif
