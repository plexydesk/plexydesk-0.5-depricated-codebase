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
#include "plexyconfig.h"
#include <desktopwidget.h>

#ifdef Q_WS_X11
#include <configadaptor.h>
#endif

namespace PlexyDesk
{
Config * Config::config = 0;

Config * Config::getInstance()
{
    if (config == 0) {
        config =  new Config("plexydesk", "plexydesktop");
        return config;
    } else {
        return config;
    }
}

Config::Config(const QString &organization,
        const QString &application, QObject *parent):
    QSettings(organization, application, parent)
{
    proxyOn = false;
    proxyURL = "";
    proxyUser = "";
    proxyPasswd = "";
    proxyPort = 3128;
    read();

    if (value("CurrentWallpaper").toString().isNull()) {
        CurrentWallpaper =  DesktopWidget::applicationDirPath() + "/share/plexy/skins/default/default.png";
    }

    if (value("iconTheme").toString().isNull()) {
        iconTheme = "default";
    }

    collitionOn = false;

    if (widgetList.count() < 0) {
        writeToFile();
    }
#ifdef Q_WS_X11
    // register  with dbus
    new ConfigAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Configuration", this);
    dbus.registerService("org.PlexyDesk.Config");
#endif
}

void Config::read()
{
    proxyOn = value("proxyOn").toInt();
    proxyURL = value("proxyURL").toString();
    proxyUser = value("proxyUser").toString();
    proxyPasswd = value("proxyPasswd").toString();
    proxyPort = value("proxyPort").toInt() ;
    CurrentWallpaper = value("CurrentWallpaper").toString();
    widgetList = value("widgetList").toStringList();
    iconTheme = value("iconTheme").toString();
    collitionOn = false;
}

void Config::writeToFile()
{
    setValue("proxyOn", proxyOn);
    setValue("proxyURL", proxyURL);
    setValue("proxyUser", proxyUser);
    setValue("ProxyPasswd", proxyPasswd);
    setValue("proxyPort", proxyPort);
    setValue("CurrentWallpaper", CurrentWallpaper);
    setValue("collitionOn", collitionOn);
    setValue("widgetList", widgetList);
    setValue("iconTheme", iconTheme);
    sync();
}

void Config::setWallpaper(const QString& str)
{
    CurrentWallpaper = str;
    setValue("CurrentWallpaper", CurrentWallpaper);
    emit configChanged();
}

void Config::addWidget(const QString& widget)
{
    //TODO: Check if this is a valid widgetname
    widgetList.append(widget);
    emit configChanged();
    emit widgetAdded();
    writeToFile();
}
}
