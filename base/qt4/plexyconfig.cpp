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
#include <QDir>

#ifdef Q_WS_X11
#include <configadaptor.h>
#endif

namespace PlexyDesk
{

class Config::Private
{
public:
    Private() {}
    ~Private() {}

    QSettings *mSettings;

};

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
        engine->addImportPath(DesktopWidget::applicationDirPath() +
                QDir::toNativeSeparators("/lib/qt4/imports/"));
        return engine;
    } else {
        return engine;
    }
}

Config::Config(const QString &organization,
     const QString &application, QObject *parent) :
    d(new Private),
    QObject(parent)
{
    d->mSettings = new QSettings(organization, application, this);

    m_proxyOn = false;
    proxyURL = "";
    proxyUser = "";
    proxyPasswd = "";
    m_proxyPort = 3128;
    openGL = false;
    read();

    if (d->mSettings->value("CurrentWallpaper").toString().isNull()) {
        CurrentWallpaper = QDir::toNativeSeparators(
                DesktopWidget::applicationDirPath() + 
                QLatin1String("/share/plexy/skins/default/default.png"));
    }


    if (d->mSettings->value("iconTheme").toString().isNull()) {
        iconTheme = QLatin1String("default");
    }

    if (d->mSettings->value(QLatin1String("themepack")).toString().isEmpty()) {
        themepackName = QLatin1String("default");
    }

    m_collisionOn = false;

    if (widgetList.count() <= 0) {
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
    m_proxyOn = d->mSettings->value("proxyOn").toInt();
    proxyURL = d->mSettings->value("proxyURL").toString();
    proxyUser = d->mSettings->value("proxyUser").toString();
    proxyPasswd = d->mSettings->value("proxyPasswd").toString();
    m_proxyPort = d->mSettings->value("proxyPort").toInt();
    CurrentWallpaper = d->mSettings->value("CurrentWallpaper").toString();
    widgetList = d->mSettings->value("widgetList").toStringList();
    iconTheme = d->mSettings->value("iconTheme").toString();
    openGL = d->mSettings->value("openGL").toBool();
    themepackName = d->mSettings->value("themepack").toString();

    m_collisionOn = false;
}

void Config::writeToFile()
{
    d->mSettings->setValue("proxyOn", m_proxyOn);
    d->mSettings->setValue("proxyURL", proxyURL);
    d->mSettings->setValue("proxyUser", proxyUser);
    d->mSettings->setValue("ProxyPasswd", proxyPasswd);
    d->mSettings->setValue("proxyPort", m_proxyPort);
    d->mSettings->setValue("CurrentWallpaper", CurrentWallpaper);
    d->mSettings->setValue("collisionOn", m_collisionOn);
    d->mSettings->setValue("widgetList", widgetList);
    d->mSettings->setValue("iconTheme", iconTheme);
    d->mSettings->setValue("openGL", openGL);
    d->mSettings->setValue("themepack", themepackName);
    sync();
}

void Config::setWallpaper(const QString &str)
{
    CurrentWallpaper = str;
    d->mSettings->setValue("CurrentWallpaper", CurrentWallpaper);
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
    Q_EMIT wallpaperChanged();
}
}
