#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <plexy.h>
#include "abstractplugininterface.h"
#include <QtCore>
#include <QtGui>


namespace PlexyDesk
{
    class VISIBLE_SYM PluginLoader:public QObject
    {

    Q_OBJECT
        public:
        typedef QHash <QString,AbstractPluginInterface*> Interface;
        typedef QHash <QString,Interface*> Dict;
        PluginLoader();
        virtual ~PluginLoader();
        QStringList listPlugins(const QString& types);
        AbstractPluginInterface * instance(const QString& name);
        void scanDisk();
        static PluginLoader * getInstance()
        {
             if (!mInstance) {
                 mInstance = new PluginLoader();
                 mInstance->scanDisk();
             }

            return mInstance;
        }

        protected:
            void loadDesktop(const QString& path);
            void  load(const QString& interface,const QString& plugin);
 
        private:
            class Private;
            Private * const d ;
            static VISIBLE_SYM PluginLoader * mInstance;
            Interface groups;


    };

} // namespace PlexDesk


#endif
