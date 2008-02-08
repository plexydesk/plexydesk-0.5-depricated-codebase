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
        PluginLoader();
        QStringList interfaces(const QString& types);
        AbstractPluginInterface  * load(const QString& name);
        
        virtual ~PluginLoader();

    };

} // namespace PlexDesk

#endif
