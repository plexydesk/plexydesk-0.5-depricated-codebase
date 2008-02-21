#ifndef PLEXY_BASE_PLUGIN_h
#define PLEXY_BASE_PLUGIN_h


#include <plexy.h>
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
    class AbstractPluginInterface 
    {
        public:
           virtual void init() = 0;
           virtual  QGraphicsItem * item() {return new QGraphicsRectItem ();};
           virtual  void render(QPainter *p,QRectF r) {};


    };
} // namespace PlexyDesk

Q_DECLARE_INTERFACE(PlexyDesk::AbstractPluginInterface, "org.plexydesk.PluginInterface")
#endif
