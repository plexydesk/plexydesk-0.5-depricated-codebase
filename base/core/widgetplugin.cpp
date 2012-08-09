#include <widgetplugin.h>
#include <pluginloader.h>

namespace PlexyDesk
{

class WidgetPlugin::PrivateWidgetPlugin
{
public:
    PrivateWidgetPlugin() {}
    ~PrivateWidgetPlugin() {}
    DataSource *mDataSource;
};

WidgetPlugin::WidgetPlugin(QObject *parent) : AbstractSource(parent), d(new PrivateWidgetPlugin)
{
    d->mDataSource = 0;
}

WidgetPlugin::~WidgetPlugin()
{
    delete d;
}

QStringList WidgetPlugin::visibleActions() const
{
    return QStringList();
}

void WidgetPlugin::requestAction(const QString &actionName)
{
    Q_EMIT actionComleted(false, QString("Invalid Action"));
}

DataSource *WidgetPlugin::dataSource()
{
    return d->mDataSource;
}

bool WidgetPlugin::connectToDataSource(const QString &source)
{
   d->mDataSource =
           qobject_cast<PlexyDesk::DataSource*> (PluginLoader::getInstance()->instance(source));

   if (!d->mDataSource)
       return 0;

   connect(d->mDataSource, SIGNAL(ready()), this, SLOT(onReady()));

   return true;
}

void WidgetPlugin::onReady()
{
    if(d->mDataSource)
       Q_EMIT data(d->mDataSource);
}

}
