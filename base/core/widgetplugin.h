
#ifndef PLEXY_WIDGET_PLUGIN_H
#define PLEXY_WIDGET_PLUGIN_H

#include <abstractsource.h>
#include <plexy.h>

class QGraphicsItem;

namespace PlexyDesk
{
class PLEXYDESK_EXPORT WidgetPlugin : public AbstractSource
{
    Q_OBJECT
public:
    virtual ~WidgetPlugin() {}
    virtual QGraphicsItem *item()
    {
        return NULL;
    }
    virtual void setData(const QVariantMap & /*data*/)
    {
    }
};
}
#endif
