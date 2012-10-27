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

#include <config.h>

#include <QCoreApplication>
#include <QDir>
#include <QHash>
#include <QtDebug>
#include "plexyconfig.h"
#include <abstractdesktopwidget.h>
#include <QCoreApplication>

#ifdef Q_WS_X11
#include <configadaptor.h>
#endif


#ifdef Q_WS_MAC
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace PlexyDesk
{

class Config::Private
{
public:
    Private() {}
    ~Private() {}

    QSettings *mSettings;
    QHash<QString, QVariant> mData;
#ifdef Q_WS_X11
    ConfigAdaptor *mConfigAdaptor;
#endif
};

Config *Config::config = 0;

Config *Config::getInstance()
{
    if (config == 0) {
        config = new Config(QLatin1String("plexydesk"),
                QLatin1String("plexydesktop"));
        return config;
    } else {
        return config;
    }
}

Config::Config(const QString &organization,
     const QString &application, QObject *parent) :
    QObject(parent),
    d(new Private)
{
    d->mSettings = new QSettings(organization, application, this);

    d->mData["proxyOn"] = QVariant(false);
    d->mData["proxyURL"] = QVariant(QLatin1String(""));
    d->mData["proxyUser"] = QVariant(QLatin1String(""));
    d->mData["proxyPasswd"] = QVariant(QLatin1String(""));
    d->mData["proxyPort"] = QVariant(3128);
    d->mData["openGL"] = QVariant(false);
#ifdef Q_WS_MAC
    d->mData["openGL"] = QVariant(true);
#endif
    read();
    QString suffix="4x3";
    QSize desktopSize = QDesktopWidget().screenGeometry().size();
    float screenRatio = float(desktopSize.width())/float(desktopSize.height());
    if (screenRatio > 1.333) {
        suffix="16x9";
    }

    if (d->mSettings->value("CurrentWallpaper").toString().isEmpty()) {
        d->mData["CurrentWallpaper"] = QDir::toNativeSeparators(
                                            plexydeskBasePath()
                                            + "/" + QString(PLEXYRESOURCESDIR)
                                            + "/default-"
                                            + suffix
                                            + ".png");
    }

    if (d->mSettings->value("CurrentWallpaperMode").toString().isEmpty()) {
        d->mData["CurrentWallpaperMode"] = QString("IgnoreAspectRatio");
    }

    if (d->mSettings->value("photo").toString().isEmpty()) {
        d->mData["photo"] = QDir::toNativeSeparators(
                                            plexydeskBasePath()
                                            + "/" + QString(PLEXYRESOURCESDIR)
                                            + "/default-photo.png");
    }

    if (d->mSettings->value("iconTheme").toString().isEmpty()) {
        d->mData["iconTheme"] = QLatin1String("default");
    }

    if (d->mSettings->value(QLatin1String("themepack")).toString().isEmpty()) {
        d->mData["themepack"] = QLatin1String("default");
    }

    writeToFile();

#ifdef Q_WS_X11
    // register with dbus
    d->mConfigAdaptor = new ConfigAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/data", this);
    dbus.registerService("org.PlexyDesk.Config");
#endif

    qDebug() << Q_FUNC_INFO << d->mData;
}

Config::~Config()
{
    delete d;
}

void Config::read()
{
    d->mData["proxyOn"] = d->mSettings->value("proxyOn");
    d->mData["proxyURL"] = d->mSettings->value("proxyURL");
    d->mData["proxyUser"] = d->mSettings->value("proxyUser");
    d->mData["proxyPasswd"] = d->mSettings->value("proxyPasswd");
    d->mData["proxyPort"] =  d->mSettings->value("proxyPort");
    d->mData["CurrentWallpaper"] = d->mSettings->value("CurrentWallpaper");
    d->mData["CurrentWallpaperMode"] = d->mSettings->value("CurrentWallpaperMode");
    d->mData["photo"] = d->mSettings->value("photo");
    d->mData["iconTheme"] = d->mSettings->value("iconTheme");
    d->mData["openGL"] = d->mSettings->value("openGL");
    d->mData["themepack"] = d->mSettings->value("themepack");

    qDebug()  << Q_FUNC_INFO << "Read: " << d->mData;
}

void Config::writeToFile()
{
    d->mSettings->setValue("proxyOn", d->mData["proxyOn"].toBool());
    d->mSettings->setValue("proxyURL", d->mData["proxyURL"].toString());
    d->mSettings->setValue("proxyUser", d->mData["proxyUser"].toString());
    d->mSettings->setValue("ProxyPasswd", d->mData["proxyPasswd"].toString());;
    d->mSettings->setValue("proxyPort", d->mData["proxyPort"].toInt());
    d->mSettings->setValue("CurrentWallpaper", d->mData["CurrentWallpaper"].toString());
    d->mSettings->setValue("CurrentWallpaperMode", d->mData["CurrentWallpaperMode"].toString());
    d->mSettings->setValue("photo", d->mData["photo"].toString());
    d->mSettings->setValue("iconTheme", d->mData["iconTheme"].toString());
    d->mSettings->setValue("openGL", d->mData["openGL"].toBool());
    d->mSettings->setValue("themepack", d->mData["themepack"].toString());
    d->mSettings->sync();

}

void Config::setWallpaper(const QString &str)
{
    qDebug() << Q_FUNC_INFO << str;
    d->mData["CurrentWallpaper"] = QVariant(str);
    d->mSettings->setValue("CurrentWallpaper",
            d->mData["CurrentWallpaper"].toString());
    Q_EMIT wallpaperChanged();
}

void Config::setWallpaperMode(const QString &str)
{
    qDebug() << Q_FUNC_INFO << str;
    d->mData["CurrentWallpaperMode"] = QVariant(str);
    d->mSettings->setValue("CurrentWallpaperMode",
            d->mData["CurrentWallpaperMode"].toString());
    Q_EMIT wallpaperChanged();
}

void Config::setPhoto(const QString &str)
{
    qDebug() << Q_FUNC_INFO << str;
    d->mData["photo"] = QVariant(str);
    d->mSettings->setValue("photo",
            d->mData["photo"].toString());
    Q_EMIT photoChanged();
}

void Config::addWidget(const QString & /*widget*/)
{
    Q_EMIT configChanged();
    Q_EMIT widgetAdded();
    writeToFile();
}

void Config::changeLayer()
{
    Q_EMIT layerChange();
    Q_EMIT wallpaperChanged();
}

// getters

QString Config::wallpaper() const
{
    return d->mData["CurrentWallpaper"].toString();
}

QString Config::wallpaperMode() const
{
    return d->mData["CurrentWallpaperMode"].toString();
}

QString Config::photo() const
{
    return d->mData["photo"].toString();
}

bool Config::isProxyOn() const
{
    return d->mData["proxyOn"].toBool();
}

int Config::proxyPort() const
{
    return d->mData["proxyPort"].toInt();
}

QString Config::proxyURL() const
{
    return d->mData["proxyURL"].toString();
}

QString Config::proxyUser() const
{
    return d->mData["proxyUser"].toString();
}

QString Config::proxyPasswd() const
{
    return d->mData["proxyPasswd"].toString();
}

QString Config::themepackName() const
{
    return d->mData["themepack"].toString();
}

QString Config::iconTheme() const
{
    return d->mData["iconTheme"].toString();
}

bool Config::isOpenGL() const
{
    return d->mData["openGL"].toBool();
}

// setters
void Config::setProxyOn(bool enable)
{
    d->mData["proxyOn"] = enable;
}

void Config::setOpenGL(bool status)
{
    d->mData["openGL"] = QVariant(status);
}

void Config::setIconTheme(const QString &name)
{
    d->mData["iconTheme"] = QVariant(name);
}

void Config::setProxyPort(int port)
{
    d->mData["proxyPort"] = QVariant(port);
}

void Config::setProxyPasswd(const QString &pass)
{
    d->mData["proxyPasswd"] = QVariant(pass);
}

void Config::setThemepackName(const QString &name)
{
    d->mData["themepack"] = QVariant(name);
}

void Config::setProxyUser(const QString &user)
{
    d->mData["proxyUser"] = QVariant(user);
}

void Config::setProxyURL(const QString &url)
{
    d->mData["proxyURL"] = QVariant(url);
}


QSettings *Config::coreSettings()
{
   return d->mSettings;
}

QString Config::plexydeskBasePath()
{
#ifndef Q_WS_X11
    QDir binaryPath (QCoreApplication::applicationDirPath());
    if (binaryPath.cdUp()) {
        return QDir::toNativeSeparators(binaryPath.canonicalPath());
    }
#endif

#ifdef Q_WS_X11
    QString basePath(qgetenv("PLEXYDESK_DIR"));
    if (basePath.isEmpty() || basePath.isNull()) {
        return PLEXYPREFIX;
    }

    return basePath;
#endif

#ifdef Q_WS_MAC
    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef, kCFURLPOSIXPathStyle);
    const char *pathPtr = CFStringGetCStringPtr(macPath,CFStringGetSystemEncoding());
    qDebug("Path = %s", pathPtr);
    CFRelease(appUrlRef);
    CFRelease(macPath);
    return QLatin1String(pathPtr) + QString("/Contents/");
#endif

    return QString();
}

}
