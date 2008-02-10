#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <plexy.h>
#include "abstractplugininterface.h"
#include <QtCore>
#include <QtGui>


namespace PlexyDesk
{
    class PluginLoader:public QObject
    {

    Q_OBJECT
        public:
        typedef QHash <QString,AbstractPluginInterface*> Interface;
        typedef QHash <QString,Interface*> Dict;
        PluginLoader();
        virtual ~PluginLoader();
        QStringList interfaces(const QString& types);

        protected:
            void scanDisk();
            void loadDesktop(const QString& path);
            void  load(const QString& interface,const QString& plugin);
 
        private:
            class Private;
            Private * const d ;


    };

} // namespace PlexDesk

#endif
