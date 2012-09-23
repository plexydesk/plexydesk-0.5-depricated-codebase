#include <viewcontrollerplugin.h>
#include <pluginloader.h>
#include <QDropEvent>

namespace PlexyDesk
{

class ViewControllerPlugin::PrivateViewControllerPlugin
{
public:
    PrivateViewControllerPlugin() {}
    ~PrivateViewControllerPlugin() {}
    DataSource *mDataSource;
};

ViewControllerPlugin::ViewControllerPlugin(QObject *parent) : QObject(parent), d(new PrivateViewControllerPlugin)
{
    d->mDataSource = 0;
}

ViewControllerPlugin::~ViewControllerPlugin()
{
    delete d;
}

QStringList ViewControllerPlugin::visibleActions() const
{
    return QStringList();
}

void ViewControllerPlugin::requestAction(const QString &actionName, const QVariantMap &args)
{
    Q_EMIT actionComleted(false, QString("Invalid Action"));
}

void ViewControllerPlugin::handleDropEvent(AbstractDesktopWidget *widget, QDropEvent *event)
{
}

DataSource *ViewControllerPlugin::dataSource()
{
    return d->mDataSource;
}

bool ViewControllerPlugin::connectToDataSource(const QString &source)
{
   d->mDataSource =
           qobject_cast<PlexyDesk::DataSource*> (PluginLoader::getInstance()->instance(source));

   if (!d->mDataSource)
       return 0;

   connect(d->mDataSource, SIGNAL(ready()), this, SLOT(onReady()));

   return true;
}

void ViewControllerPlugin::onReady()
{
    if(d->mDataSource)
       Q_EMIT data(d->mDataSource);
}

}
