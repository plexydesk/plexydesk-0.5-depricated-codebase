
#include <QtCore>
#include <QtGui>
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
    QPluginLoader loader ("/usr/local/lib/plexyext/widgets/libclassicbackdrop.so");
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
