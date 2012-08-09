
#ifndef PLEXY_WIDGET_PLUGIN_H
#define PLEXY_WIDGET_PLUGIN_H

#include <abstractsource.h>
#include <datasource.h>
#include <plexy.h>

class QGraphicsItem;

/*!

  \class PlexyDesk::WidgetPlugin

  \brief Base class for writing view plugins

  \fn PlexyDesk::WidgetPlugin::visibleActions

  \brief Actions supported by the plugin

  \paragraph Widget Plugins can optionaly provide actions which
  it can perform on the view and the data source. So WidgetPlugins
  mostly act as a controller which knows about the data and the view
  and provides suitable actions which the user can perform.

  \paragraph If your plugin doesn't provide any actions which the user can
  execute, for instance a dash board widget plugin which displays read-only
  data source doesn't need to provide any user actions. In such cases plugin
  authors should not worry about overiding this method in their plugin class.

  \paragraph But if you plugin needs user actions such as adding a clock with a different
  Time Zone, then you can return the actions you want to perform by overiding
  PlexyDesk::WidgetPlugin::visibleActions method and returning the string lables for your action.
  once you complete the user action remember to emit PlexyDesk::WidgetPlugin::actionCompleted signal
  so that the action requester can notify the user about what happened to the action.

  \returns A list of action label supported by the widget  plugin
*/
namespace PlexyDesk
{
class PLEXYDESK_EXPORT WidgetPlugin : public AbstractSource
{
    Q_OBJECT

public:
    explicit WidgetPlugin(QObject *parent = 0);
    virtual ~WidgetPlugin();
    virtual QGraphicsItem *view() = 0;
    virtual QStringList visibleActions() const;
    virtual void requestAction(const QString& actionName);
    virtual DataSource *dataSource();

protected:
    virtual bool connectToDataSource(const QString &source);

Q_SIGNALS:
    void actionComleted(bool, const QString &error);
    void data(const DataSource *source);

private Q_SLOTS:
    virtual void onReady();

private:
    class PrivateWidgetPlugin;
    PrivateWidgetPlugin *const d;
};
}
#endif
