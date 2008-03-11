/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <plexy.h>
#include <abstractplugininterface.h>
#include <baseplugin.h>
#include <QtCore>
#include <QtGui>


namespace PlexyDesk
{
    class VISIBLE_SYM PluginLoader:public QObject
    {

    Q_OBJECT
        public:
        typedef QHash <QString,AbstractPluginInterface*> Interface;
        typedef QHash <QString,BasePlugin*> Dict;
        PluginLoader();
        virtual ~PluginLoader();
        QStringList listPlugins(const QString& types);
        BasePlugin * instance(const QString& name);
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
            void load(const QString& _interface, const QString& plugin);
 
        private:
            class Private;
            Private * const d ;
#ifdef Q_WS_WIN
            static PluginLoader * mInstance;
#else
            static VISIBLE_SYM PluginLoader * mInstance;
#endif
            Interface groups;


    };

} // namespace PlexDesk


#endif
