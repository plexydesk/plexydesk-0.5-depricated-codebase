#include <controllerinterface.h>
#include <pluginloader.h>
#include <QDropEvent>
#include <QDebug>

namespace PlexyDesk
{

class ControllerInterface::PrivateViewControllerPlugin
{
public:
    PrivateViewControllerPlugin() {}
    ~PrivateViewControllerPlugin() {}
    QSharedPointer<DataSource> mDataSource;
    AbstractDesktopView *mViewport;
    QString mName;
};

ControllerInterface::ControllerInterface(QObject *parent) : QObject(parent), d(new PrivateViewControllerPlugin)
{
    d->mViewport = 0;
}

ControllerInterface::~ControllerInterface()
{
    delete d;
}

void ControllerInterface::setViewport(AbstractDesktopView *view)
{
    qDebug() << Q_FUNC_INFO ;
    d->mViewport = view;

    handleViewport();
}

AbstractDesktopView *ControllerInterface::viewport()
{
    return d->mViewport;
}

void ControllerInterface::handleViewport()
{
}

void ControllerInterface::firstRun()
{
}

QStringList ControllerInterface::actions() const
{
    return QStringList();
}

void ControllerInterface::requestAction(const QString & /*actionName*/, const QVariantMap & /*args*/)
{
    Q_EMIT actionComleted(false, QString("Invalid Action"));
}

void ControllerInterface::handleDropEvent(AbstractDesktopWidget * /*widget*/, QDropEvent * /*event*/)
{
}

DataSource *ControllerInterface::dataSource()
{
    return d->mDataSource.data();
}

void ControllerInterface::setControllerName(const QString &name)
{
    d->mName = name;
}

QString ControllerInterface::controllerName() const
{
    return d->mName;
}

bool ControllerInterface::connectToDataSource(const QString &source)
{
   d->mDataSource = PluginLoader::getInstance()->engine(source);

   if (!d->mDataSource.data())
       return 0;

   connect(d->mDataSource.data(), SIGNAL(ready()), this, SLOT(onReady()));

   return true;
}

bool ControllerInterface::disconnectFromDataSource()
{
    //disconnect(d->mDataSource.data(), SIGNAL(sourceUpdated(QVariantMap)));
    return false;
}

void ControllerInterface::onReady()
{
    if(d->mDataSource)
        Q_EMIT data(d->mDataSource.data());
}

}
