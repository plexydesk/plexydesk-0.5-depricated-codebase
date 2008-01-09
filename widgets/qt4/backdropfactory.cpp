
#include <QtCore>
#include <QtGui>
#include "backdropfactory.h"

namespace PlexyDesk
{

class BackdropFactory::Private
{
    public:
        Private() {}
        ~Private() {}
        BackdropInterface * currentDrop;
};


BackdropFactory::BackdropFactory(QObject * parent):QObject(parent),d(new Private)
{
    QPluginLoader loader ("/usr/local/lib/plexyext/libclassicbackdrop.so");
    QObject * plugin =  loader.instance();
    if (plugin) {
        d->currentDrop = qobject_cast<BackdropInterface*>(plugin);
    }else {
        qDebug()<<loader.errorString()<<endl;;
        d->currentDrop = 0;
    }
}

BackdropFactory::~BackdropFactory()
{
    delete d;
}

BackdropInterface * BackdropFactory::instance ()
{
return d->currentDrop ;
}

} // namespace PlexyDesk


#include "backdropfactory.moc"
