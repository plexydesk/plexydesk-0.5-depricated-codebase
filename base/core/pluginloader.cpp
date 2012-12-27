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

#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>
#include <QtCore/QSettings>

#include "pluginloader.h"
#include "dataplugininterface.h"
#include "controllerplugininterface.h"
#include "desktopviewplugininterface.h"

namespace PlexyDesk
{
PluginLoader *PluginLoader::mInstance = 0;

class PluginLoader::Private
{
public:
    typedef QHash <QString, DataPluginInterface*> EnginePlugins;
    typedef QHash <QString, ControllerPluginInterface *> ControllerPlugins;
    typedef QHash <QString, DesktopViewPluginInterface *> DesktopViewPlugins;

    Private() {
    }
    ~Private() {
        mDict.clear();
    }

    void addToDict(const QString &interface, const QString &pluginName);

    Interface mPluginGroups;

    EnginePlugins mEngines;
    ControllerPlugins mControllers;
    DesktopViewPlugins mDesktopViews;

    QString mPluginPrefix;
    QString mPluginInfoPrefix;
    QHash<QString, QStringList> mDict;
    QHash<QString, QString> mPluginNames;
};

PluginLoader::PluginLoader() : d(new Private)
{
}

PluginLoader::~PluginLoader()
{
    delete d;
}

PluginLoader *PluginLoader::getInstanceWithPrefix(const QString &desktopPrefix, const QString &libPrefix)
{
    if (!mInstance) {
        mInstance = new PluginLoader();
        mInstance->setPluginPrefix(libPrefix);
        mInstance->setPluginInfoPrefix(desktopPrefix);
        mInstance->scanForPlugins();
    }
    return mInstance;
}

PluginLoader *PluginLoader::getInstance()
{
    if (!mInstance) {
        mInstance = new PluginLoader();
    }
    return mInstance;
}

QStringList PluginLoader::listPlugins(const QString &types)
{
    return d->mDict[types];
}

QObject *PluginLoader::instance(const QString &name)
{
    if (d->mPluginGroups.contains(name)) {
        return d->mPluginGroups[name]->instance();
    }  else {
        return 0;
    }
}

QSharedPointer<DataSource> PluginLoader::engine(const QString &name)
{
    if (d->mEngines[name]) {
        return d->mEngines[name]->model();
    } else {
        load("Engine", name);
        if (d->mEngines[name]) {
            return d->mEngines[name]->model();
        }
    }

    return QSharedPointer<DataSource>();
}

ControllerPtr PluginLoader::controller(const QString &name)
{
    if (d->mControllers[name]) {
        return d->mControllers[name]->controller();
    } else {
        load("Widget", name);
        if (d->mControllers[name]) {
            return d->mControllers[name]->controller();
        }
    }

    return QSharedPointer<ControllerInterface>();
}

QString PluginLoader::controllerName(const QString &key) const
{
    return d->mPluginNames[key];
}

QSharedPointer<DesktopViewPlugin> PluginLoader::view(const QString &name)
{
    if (d->mDesktopViews[name]) {
        return d->mDesktopViews[name]->view();
    } else {
        load("DesktopView", name);
        if (d->mDesktopViews[name]) {
            return d->mDesktopViews[name]->view();
        }
    }

    return QSharedPointer<DesktopViewPlugin>();
}

void PluginLoader::load(const QString &interface, const QString &pluginName)
{
#ifdef Q_OS_MAC
    QPluginLoader loader (d->mPluginPrefix + QLatin1String("lib") + pluginName + QLatin1String(".dylib") );
#endif

#ifdef Q_OS_LINUX
    QPluginLoader loader (d->mPluginPrefix + QLatin1String("lib") + pluginName + ".so" );
#endif

#ifdef Q_WS_WIN
    QPluginLoader loader (d->mPluginPrefix + pluginName + ".dll" );
#endif

#ifdef Q_WS_QPA
    QPluginLoader loader (d->mPluginPrefix + QLatin1String("lib") + pluginName + ".so" );
#endif


    if (interface.toLower() == "engine") {
        QObject *plugin = loader.instance();

        if (plugin) {
            DataPluginInterface *Iface = 0;
            Iface = qobject_cast<DataPluginInterface *> (plugin);
            d->mEngines[pluginName] = Iface;
        }else
            qWarning() << Q_FUNC_INFO << ":" << interface << ":" << "pluginName" << loader.errorString();
    }

    if (interface.toLower() == "widget")  {

        QObject *plugin = loader.instance();

        if (plugin) {
            ControllerPluginInterface *Iface = 0;
            Iface = qobject_cast<ControllerPluginInterface *> (plugin);
            d->mControllers[pluginName] = Iface;
        }
        else {
            qWarning() << Q_FUNC_INFO << loader.errorString();
        }
    }

    if (interface.toLower() == "desktopview")  {

        QObject *plugin = loader.instance();

        if (plugin) {
            DesktopViewPluginInterface *Iface = 0;
            Iface = qobject_cast<DesktopViewPluginInterface *> (plugin);
            d->mDesktopViews[pluginName] = Iface;
       } else
            qWarning() << Q_FUNC_INFO << loader.errorString();
    }
}

void PluginLoader::scanForPlugins()
{
    if (d->mPluginInfoPrefix.isEmpty() || d->mPluginInfoPrefix.isNull()) {
        qWarning() << Q_FUNC_INFO << "Prefix undefined"
                   << " try running PluginLoader::getInstanceWithPrefix with the correct path first";
    }

    QDir dir(d->mPluginInfoPrefix);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        loadDesktop(d->mPluginInfoPrefix + fileInfo.fileName());
    }
}

void PluginLoader::setPluginPrefix(const QString &path)
{
    d->mPluginPrefix = path;
}

void PluginLoader::setPluginInfoPrefix(const QString &path)
{
    d->mPluginInfoPrefix = path;
}

QString PluginLoader::pluginInforPrefix() const
{
    return d->mPluginInfoPrefix;
}

QString PluginLoader::pluginPrefix() const
{
    return d->mPluginPrefix;
}

void PluginLoader::loadDesktop(const QString &path)
{
    QSettings desktopFile(path, QSettings::IniFormat, this);
    desktopFile.beginGroup("Desktop Entry");
    d->addToDict(desktopFile.value("Type").toString(), desktopFile.value("X-PLEXYDESK-Library").toString());
    d->mPluginNames[desktopFile.value("X-PLEXYDESK-Library").toString()] = desktopFile.value("Name").toString();
    desktopFile.endGroup();
}

void PluginLoader::Private::addToDict(const QString &interface, const QString &pluginName)
{
    const QStringList dictKeys = mDict.keys();
    if (!dictKeys.contains(interface)) {
        QStringList list;
        list << pluginName;
        mDict[interface] = list;
    } else {
        QStringList list;
        list = mDict[interface];
        list << pluginName;
        mDict[interface] = list;
    }
}

}
