
#include <QtCore>
#include <QtGui>
#include <config.h>
#include "widgetfactory.h"

namespace PlexyDesk
{

class WidgetFactory::Private
{
    public:
        Private() {}
        ~Private() {}
        WidgetInterface * currentDrop;
};


WidgetFactory::WidgetFactory(QObject * parent):QObject(parent),d(new Private)
{
   QString ext(".so");
#ifdef Q_WS_MAC
    QPluginLoader loader (QString(PLEXYPREFIX) +"/lib/plexyext/widgets/libwidgetdemo.dylib");
#endif

#ifdef Q_WS_X11
    QPluginLoader loader (QString(PLEXYPREFIX) +"/lib/plexyext/widgets/libwidgetdemo.so");
#endif

#ifdef Q_WS_WIN32
    QPluginLoader loader (QString(PLEXYPREFIX) +"\\lib\\plexyext\\widgets\\libwidgetdemo.dll");
#endif



    QObject * plugin =  loader.instance();
    if (plugin) {
        d->currentDrop = qobject_cast<WidgetInterface*>(plugin);
    }else {
        qDebug()<<loader.errorString()<<endl;;
        d->currentDrop = 0;
    }
}

WidgetFactory::~WidgetFactory()
{
    delete d;
}

WidgetInterface * WidgetFactory::instance ()
{
return d->currentDrop ;
}

} // namespace PlexyDesk


#include "widgetfactory.moc"
