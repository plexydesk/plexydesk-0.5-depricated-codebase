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
#include "pluginloader.h"
#include "extensionfactory.h"

#include <QStringList>
#include <QPluginLoader>
#include <QDebug>
#include <QDir>
#include <QSettings>


namespace PlexyDesk
{
PluginLoader *PluginLoader::mInstance = 0;

class PluginLoader::Private
{
public:
    Private() {}
    ~Private() {}
    Interface groups;
    QString prefix;
};

PluginLoader::PluginLoader(): d(new Private)
{
    d->prefix = applicationDirPath() + "/ext/groups/";
}

PluginLoader::~PluginLoader()
{
    delete d;
}

PluginLoader* PluginLoader::getInstance()
{
    if (!mInstance) {
        mInstance = new PluginLoader();
        mInstance->scanDisk();
    }

    return mInstance;
}

QStringList PluginLoader::listPlugins(const QString& types)
{
    return d->groups.keys();
}

BasePlugin *PluginLoader::instance(const QString& name)
{
    if (d->groups.contains(name)) {
        return d->groups[name]->instance();
    }  else {
        return 0;
    }
}

void PluginLoader::load(const QString & interface, const QString & pluginName)
{
#ifdef Q_WS_MAC
    QPluginLoader loader(applicationDirPath() + "/lib/plexyext/lib" + pluginName + ".dylib");
#endif

#ifdef Q_WS_X11
    QPluginLoader loader(applicationDirPath() + "/lib/plexyext/lib" + pluginName + ".so");
#endif

#ifdef Q_WS_WIN
    QPluginLoader loader(applicationDirPath() + "/lib/plexyext/" + pluginName + ".dll");
#endif

    QObject *plugin = loader.instance();

    if (plugin) {
        AbstractPluginInterface *Iface = 0;
        Iface = qobject_cast<AbstractPluginInterface*> (plugin);
        d->groups[pluginName] = Iface;
        qDebug() << "PluginLoader::load" << "Loading.." << Iface << pluginName << endl;
    } else {
        qDebug() << loader.errorString() << endl;;
    }
}

void PluginLoader::scanDisk()
{
    QDir dir(d->prefix);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        loadDesktop(d->prefix + fileInfo.fileName());
    }
}

void PluginLoader::loadDesktop(const QString & path)
{

    QSettings desktopFile(path, QSettings::IniFormat, this);

    desktopFile.beginGroup("Desktop Entry");

    load(desktopFile.value("Type").toString(), desktopFile.value("X-PLEXYDESK-Library").toString());

    desktopFile.endGroup();
}

}
