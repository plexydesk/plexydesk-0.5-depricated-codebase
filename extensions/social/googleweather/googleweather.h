#ifndef PLEXY_GOOGLEWEATHER_H
#define PLEXY_GOOGLEWEATHER_H

#include <plexy.h>
#include <widgetinterface.h>
#include <widgetplugin.h>

class VISIBLE_SYM googleweather:public PlexyDesk::WidgetPlugin
{
    Q_OBJECT

    public:
        googleweather(QObject * object = 0);
        virtual ~googleweather();
        QGraphicsItem * item();
    private :
         QWidget *base;
};

#endif
