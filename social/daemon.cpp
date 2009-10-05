#include "daemon.h"
/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Sri Lanka Institute of Information Technology
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

#include <QtDBus>
#include <QDBusConnection>
#include <pluginloader.h>
#include <QDebug>
#include <QStringList>

namespace PlexyDesk
{
    SocialDaemon::SocialDaemon(QApplication *parent=0) : QDBusAbstractAdaptor(parent)
    {

        QDBusConnection::sessionBus().registerService("org.plexydesk.social");
    }

    SocialPlugin* SocialDaemon::getPluginInstance(const QString &pluginName)
    {
        QObject *pluginInstance = (PluginLoader::getInstance()->instance(pluginName));
        SocialPlugin *socioPlugin = static_cast<SocialPlugin *>(pluginInstance);
        return socioPlugin;
    }

    QStringList SocialDaemon::getPluginsList()
    {
        return PluginLoader::getInstance()->listPlugins("social");
    }

    QStringList SocialDaemon::supportedMethods(const QString &pluginName)
    {
        SocialPlugin *instance = getPluginInstance(pluginName);
       // return instance->supportedMethods();//SocialPlugin.h must contain a method QStringList supportedMethods()
    }

    QVariantMap SocialDaemon::data(const QString &pluginName, const QString &methodName, QVariantMap args)
    {
        SocialPlugin *socioPlugin = getPluginInstance(pluginName);
        //socioPlugin->data(methodName, args);//SocialPlugin.h must contain a method QVariantMap data(const QString &methodName, QVariantMap args)
    }
}//namespace PlexyDesk

