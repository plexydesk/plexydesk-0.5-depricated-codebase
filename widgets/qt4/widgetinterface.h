
#ifndef PLEXY_WIDGET_INTERFACE
#define PLEXY_WIDGET_INTERFACE

#include <QtCore>
#include <QtGui>
#include <abstractplugininterface.h>

namespace PlexyDesk
{
    class WidgetInterface : public AbstractPluginInterface 
    {
        public:
	void init() {}
            WidgetInterface(){}
            ~WidgetInterface(){}
           virtual  QGraphicsItem * backdrop() = 0;
    };
} // namespace PlexyDesk

Q_DECLARE_INTERFACE(PlexyDesk::WidgetInterface, "org.plexydesk.WidgetInterface")
#endif
