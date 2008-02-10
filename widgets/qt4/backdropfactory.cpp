
#include <QtCore>
#include <QtGui>
#include <config.h>
#include <abstractplugininterface.h>
#include "backdropfactory.h"

namespace PlexyDesk
{

class BackdropFactory::Private
{
    public:
        Private() {}
        ~Private() {}
       AbstractPluginInterface  * currentDrop;
};


BackdropFactory::BackdropFactory(QObject * parent):QObject(parent),d(new Private)
{

   
   
#ifdef Q_WS_MAC
    QPluginLoader loader (QString(PLEXPREFIX) +"/lib/plexyext/libclassicbackdrop.dylib");
#endif    

#ifdef Q_WS_X11
    QPluginLoader loader (QString(PLEXPREFIX)+"/lib/plexyext/libclassicbackdrop.so");
#endif 

#ifdef Q_WS_WIN32
    QPluginLoader loader (QString(PLEXPREFIX) +"\\lib\\plexyext\\libclassicbackdrop.dll");
#endif 


    QObject * plugin =  loader.instance();
    
    if (plugin) {
        d->currentDrop = qobject_cast<AbstractPluginInterface*>(plugin);
    }else {
        qDebug()<<loader.errorString()<<endl;;
        d->currentDrop = 0;
    }
   
}

BackdropFactory::~BackdropFactory()
{
    delete d;
}

AbstractPluginInterface * BackdropFactory::instance ()
{
return d->currentDrop ;
}

} // namespace PlexyDesk


#include "backdropfactory.moc"
