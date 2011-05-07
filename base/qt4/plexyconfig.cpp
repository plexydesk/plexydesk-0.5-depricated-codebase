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
#include <QDeclarativeEngine>

#ifdef Q_WS_X11
#include <configadaptor.h>
#endif

namespace PlexyDesk
{
Config *Config::config = 0;

QDeclarativeEngine *Config::engine = 0;

Config *Config::getInstance()
{
    if (config == 0) {
        config = new Config("plexydesk", "plexydesktop");
        return config;
    } else {
        return config;
    }
}

QDeclarativeEngine *Config::qmlEngine()
{
    if (engine == 0) {
        engine = new QDeclarativeEngine;
        return engine;
    } else {
        return engine;
    }
}

Config::Config(const QString &organization,
     const QString &application, QObject *parent) :
    QSettings(organization, application, parent)
{
    m_proxyOn = false;
    proxyURL = "";
    proxyUser = "";
    proxyPasswd = "";
    m_proxyPort = 3128;
    openGL = false;
    read();

    if (value("CurrentWallpaper").toString().isNull()) {
        CurrentWallpaper = DesktopWidget::applicationDirPath() + "/share/plexy/skins/default/default.png";
    }

    if (value("iconTheme").toString().isNull()) {
        iconTheme = "default";
    }

    m_collisionOn = false;

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
    m_proxyOn = value("proxyOn").toInt();
    proxyURL = value("proxyURL").toString();
    proxyUser = value("proxyUser").toString();
    proxyPasswd = value("proxyPasswd").toString();
    m_proxyPort = value("proxyPort").toInt();
    CurrentWallpaper = value("CurrentWallpaper").toString();
    widgetList = value("widgetList").toStringList();
    iconTheme = value("iconTheme").toString();
    openGL = value("openGL").toBool();

    m_collisionOn = false;
}

void Config::writeToFile()
{
    setValue("proxyOn", m_proxyOn);
    setValue("proxyURL", proxyURL);
    setValue("proxyUser", proxyUser);
    setValue("ProxyPasswd", proxyPasswd);
    setValue("proxyPort", m_proxyPort);
    setValue("CurrentWallpaper", CurrentWallpaper);
    setValue("collisionOn", m_collisionOn);
    setValue("widgetList", widgetList);
    setValue("iconTheme", iconTheme);
    setValue("openGL", openGL);
    sync();
}

void Config::setWallpaper(const QString &str)
{
    CurrentWallpaper = str;
    setValue("CurrentWallpaper", CurrentWallpaper);
    Q_EMIT configChanged();
}

void Config::addWidget(const QString &widget)
{
    //TODO: Check if this is a valid widgetname
    widgetList.append(widget);
    Q_EMIT configChanged();
    Q_EMIT widgetAdded();
    writeToFile();
}

void Config::changeLayer()
{
    qDebug() << Q_FUNC_INFO;
    Q_EMIT layerChange();
}
}
