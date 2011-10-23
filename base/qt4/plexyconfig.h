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

#ifndef PLEXY_CONIFG_LINUX_QT_H
#define PLEXY_CONIFG_LINUX_QT_H

#include <QDeclarativeEngine>
#include <QSettings>
#include <QNetworkProxy>
#include <QStringList>

#include <plexy.h>
#include <imagecache.h>


#define QmlEngine() PlexyDesk::Config::qmlEngine();

namespace PlexyDesk
{
class PLEXYDESK_EXPORT Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString wallpaper READ wallpaper WRITE setWallpaper NOTIFY wallpaperChanged)
    Q_PROPERTY(QString wallpaperMode READ wallpaperMode WRITE setWallpaperMode NOTIFY wallpaperChanged)
    Q_PROPERTY(QString photo READ photo WRITE setPhoto NOTIFY photoChanged)
    Q_PROPERTY(bool proxy READ isProxyOn WRITE setProxyOn NOTIFY proxyChanged)
    Q_PROPERTY(int port READ proxyPort WRITE setProxyPort NOTIFY proxyPortChanged)
    Q_PROPERTY(QString proxyURL READ proxyURL WRITE setProxyURL NOTIFY proxyURLChanged)
    Q_PROPERTY(QString proxyUser READ proxyUser WRITE setProxyUser NOTIFY proxyUserChanged)
    Q_PROPERTY(QString proxyPasswd READ proxyPasswd WRITE setProxyPasswd NOTIFY proxyPasswdChanged)
    Q_PROPERTY(QString themepackName READ themepackName WRITE setThemepackName NOTIFY themepackNameChanged)
    Q_PROPERTY(QString iconTheme READ iconTheme WRITE setIconTheme NOTIFY iconThemechanged)
    Q_PROPERTY(bool openGL READ isOpenGL WRITE setOpenGL NOTIFY openGLChanged)

public:
    static Config *getInstance();
    static QDeclarativeEngine *qmlEngine();
    static QDeclarativeEngine *newQmlEngine();

    virtual ~Config();

    void read();
    void writeToFile();

    QString wallpaper() const;
    QString wallpaperMode() const;
    QString photo() const;
    bool isProxyOn() const;
    int proxyPort() const;
    QString proxyURL() const;
    QString proxyUser() const;
    QString proxyPasswd() const;
    QString themepackName() const;
    QString iconTheme() const;
    bool isOpenGL() const;
    QSettings *coreSettings();

    void setProxyOn(bool enable);
    void setProxyPort(int port);
    void setProxyURL(const QString &url);
    void setProxyUser(const QString &user);
    void setProxyPasswd(const QString &pass);
    void setThemepackName(const QString &name);
    void setIconTheme(const QString &name);
    void setOpenGL(bool enable);

    QString plexydeskBasePath();

public Q_SLOTS:
    void setWallpaper(const QString &str);
    void setWallpaperMode(const QString &str);
    void setPhoto(const QString &str);
    void addWidget(const QString &widget);
    void changeLayer();

Q_SIGNALS:
    void configChanged();
    void wallpaperChanged();
    void photoChanged();
    void widgetAdded();
    void layerChange();
    void proxyChanged();
    void proxyPortChanged();
    void proxyURLChanged();
    void proxyUserChanged();
    void proxyPasswdChanged();
    void widgetListChanged();
    void themepackNameChanged();
    void iconThemechanged();
    void openGLChanged();

private:
    Config(const QString &organization, const QString &application = QString(), QObject *parent = 0);
    Config();
    Config(Config &config);
    static Config *config;
    static QDeclarativeEngine *engine;

    class Private;
    Private *const d;

    Config &operator=(const Config &);
};
} // namespace PlexyDesk
#endif
