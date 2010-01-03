#ifndef SOCIOTWITTER_H
#define SOCIOTWITTER_H

#include <plexy.h>
#include <widgetinterface.h>
#include <widgetplugin.h>
#include "sociotwitterinterface.h"

class VISIBLE_SYM SocioTwitter: public PlexyDesk::WidgetPlugin
{
    Q_OBJECT

public:
    SocioTwitter(QObject * object = 0);
    virtual ~SocioTwitter();
    QGraphicsItem * item();
};

#endif

