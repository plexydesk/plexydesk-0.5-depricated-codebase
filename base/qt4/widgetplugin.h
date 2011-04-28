
#ifndef PLEXY_WIDGET_PLUGIN_H
#define PLEXY_WIDGET_PLUGIN_H

#include <baseplugin.h>
#include <plexy.h>

class QGraphicsItem;

namespace PlexyDesk
{
class PLEXYDESK_EXPORT WidgetPlugin : public BasePlugin
{
    Q_OBJECT
public:
    virtual ~WidgetPlugin() {}
    virtual QGraphicsItem *item()
    {
        return NULL;
    }
};
}
#endif
