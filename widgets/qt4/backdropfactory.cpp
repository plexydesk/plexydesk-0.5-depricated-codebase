
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

   
   
#ifdef Q_WS_MAC
    QPluginLoader loader ("/usr/local/lib/plexyext/libclassicbackdrop.dylib");
#endif    

#ifdef Q_WS_X11
    QPluginLoader loader ("/usr/local/lib/plexyext/libclassicbackdrop.so");
#endif 
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
