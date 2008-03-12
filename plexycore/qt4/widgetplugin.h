
#ifndef PLEXY_WIDGET_PLUGIN_H
#define PLEXY_WIDGET_PLUGIN_H 

#include <plexy.h>
#include <config.h>
#include <baseplugin.h>
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
    class WidgetPlugin : public BasePlugin
    {
        Q_OBJECT
        public :
                WidgetPlugin(QObject  * object = 0){}
                virtual ~WidgetPlugin(){}
                virtual QGraphicsItem * item () = 0;

     };
}
#endif
