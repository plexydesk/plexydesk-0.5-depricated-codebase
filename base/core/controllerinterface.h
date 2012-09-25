
#ifndef PLEXY_VIEW_CONTROLLER_PLUGIN_H
#define PLEXY_VIEW_CONTROLLER_PLUGIN_H

#include <datasource.h>
#include <plexy.h>

class QGraphicsItem;
class QDropEvent;

/*!

  \class PlexyDesk::ViewControllerPlugin

  \brief Base class for writing view plugins

  \fn PlexyDesk::ViewControllerPlugin::visibleActions

  \brief Actions supported by the plugin

  \paragraph Widget Plugins can optionaly provide actions which
  it can perform on the view and the data source. So ViewControllerPlugins
  mostly act as a controller which knows about the data and the view
  and provides suitable actions which the user can perform.

  \paragraph If your plugin doesn't provide any actions which the user can
  execute, for instance a dash board widget plugin which displays read-only
  data source doesn't need to provide any user actions. In such cases plugin
  authors should not worry about overiding this method in their plugin class.

  \paragraph But if you plugin needs user actions such as adding a clock with a different
  Time Zone, then you can return the actions you want to perform by overiding
  PlexyDesk::ViewControllerPlugin::visibleActions method and returning the string lables for your action.
  once you complete the user action remember to emit PlexyDesk::ViewControllerPlugin::actionCompleted signal
  so that the action requester can notify the user about what happened to the action.

  \returns A list of action label supported by the widget  plugin
*/
namespace PlexyDesk
{
class AbstractDesktopWidget;

class PLEXYDESK_EXPORT ControllerInterface : public QObject
{
    Q_OBJECT

public:
    ControllerInterface(QObject *parent = 0);
    virtual ~ControllerInterface();

    virtual AbstractDesktopWidget *defaultView() = 0;

    virtual QStringList visibleActions() const;
    virtual void requestAction(const QString& actionName, const QVariantMap &args);

    virtual void handleDropEvent(AbstractDesktopWidget *widget, QDropEvent *event);

    virtual DataSource *dataSource();

protected:
    virtual bool connectToDataSource(const QString &source);

Q_SIGNALS:
    void actionComleted(bool, const QString &error);
    void data(const DataSource *source);
    void spawnView(AbstractDesktopWidget *widget);

private Q_SLOTS:
    virtual void onReady();

private:
    class PrivateViewControllerPlugin;
    PrivateViewControllerPlugin *const d;
};
}
#endif
