#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <plexy.h>
#include <abstractplugininterface.h>


namespace PlexDesk
{
    class PluginLoader:public Object
    {

    Q_OBJECT
        public:
        typedef QHash <QString,AbstractPluginInterface*> Interface;
        typedef QHash <QString,Interface*> Dict;
        PluginLoader();
        virtual ~PluginLoader();
        QStringList interfaces(const QString& types);
        AbstractPluginInterface  * load(const QString& interface,const QString& widget);

    };

} // namespace PlexDesk

#endif
