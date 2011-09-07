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

#include <QDesktopWidget>
#include <QStringList>
#include <QDir>
#include <QRect>

#include <QDebug>

#include "themepackloader.h"


namespace PlexyDesk
{

class ThemepackLoader::ThemepackLoaderPrivate
{
public:
    ThemepackLoaderPrivate() {}
    ~ThemepackLoaderPrivate() {}

    QSettings *mSettings;
    QString mThemeCfgFile;
    QString mBasePath;
    QString mThemeName;
    QString mThemePackPath;
};

ThemepackLoader::ThemepackLoader(const QString &themeName, QObject *parent) :
    QObject(parent), d (new ThemepackLoaderPrivate)
{
    d->mThemePackPath =
        QString("%1/%2").arg(PlexyDesk::Config::getInstance()->plexydeskBasePath()).arg("/share/plexy/themepack");
    d->mThemeName = themeName;
    QDir mainConfig(QString("%1/%2/").arg(d->mThemePackPath).arg(themeName));
    d->mSettings = new QSettings(QDir::toNativeSeparators(
                                 mainConfig.absoluteFilePath("main.cfg")),
                             QSettings::IniFormat, this);

    d->mThemeCfgFile = QDir::toNativeSeparators(d->mSettings->fileName());
    d->mBasePath = QDir::toNativeSeparators(mainConfig.absolutePath());

    qDebug() << Q_FUNC_INFO << themeName;
    qDebug() << Q_FUNC_INFO << d->mThemeCfgFile;
}

ThemepackLoader::~ThemepackLoader()
{
    delete d;
}

QString ThemepackLoader::wallpaper()
{
    QString rv;
    d->mSettings->beginGroup(QLatin1String("main"));
    rv = d->mSettings->value("wallpaper").toString();
    d->mSettings->endGroup();

    QDir
        prefix(QString("%1/%2/%3").arg(d->mThemePackPath).arg(d->mThemeName).arg(QLatin1String("resources")));

    return QDir::toNativeSeparators(prefix.absoluteFilePath(rv));
}

QString ThemepackLoader::qmlBackdropFromTheme()
{
    QString rv;
    d->mSettings->beginGroup(QLatin1String("main"));
    rv = d->mSettings->value("qml_wallpaper").toString();
    d->mSettings->endGroup();

    QDir
        prefix(QString("%1/%2/%3").arg(d->mThemePackPath).arg(d->mThemeName).arg(QLatin1String("backdrop/")));

    return QDir::toNativeSeparators(prefix.absoluteFilePath(rv));
}

QStringList ThemepackLoader::widgets(const QString &expType)
{
    QStringList rv;
    Q_FOREACH(const QString &key, d->mSettings->childGroups()) {
        qDebug() << Q_FUNC_INFO << key;
        if (key == QLatin1String("main") || key == QLatin1String("hidden")) {
            continue;
        }
        d->mSettings->beginGroup(key);
        QString type = d->mSettings->value(QLatin1String("type")).toString();
        if (type != expType) {
            d->mSettings->endGroup();
            continue;
        }
        d->mSettings->endGroup();
        rv << key;
    }

    return rv;
}

QString ThemepackLoader::hiddenQmlWidgets(const QString &name)
{
    QString rv;
    d->mSettings->beginGroup(QLatin1String("hidden"));
    rv = d->mSettings->value(name).toString();
    d->mSettings->endGroup();

    QDir
        prefix(QString("%1/%2/").arg(d->mThemePackPath).
                arg(d->mThemeName));

    return QDir::toNativeSeparators(prefix.absoluteFilePath(rv));
}

QVariant ThemepackLoader::getProperty(const QString &widgetName, const QString &prop)
{
    QVariant rv;
    d->mSettings->beginGroup(widgetName);
    rv = d->mSettings->value(prop);
    d->mSettings->endGroup();

    return rv; 
}

QString ThemepackLoader::qmlFilesFromTheme(const QString &name)
{
    d->mSettings->beginGroup(name);
    QString fileName = d->mSettings->value("mainfile").toString();
    QString baseFolder = d->mSettings->value("basefolder").toString();
    qDebug() << Q_FUNC_INFO << name << ": " << fileName;
    d->mSettings->endGroup();

    QDir path(QString("%1/%2").arg(d->mBasePath).arg(baseFolder));
    return (QDir::toNativeSeparators(QDir::cleanPath(
                                            path.absoluteFilePath(fileName))
                                    ));
}

QPoint ThemepackLoader::widgetPos(const QString &name)
{
    // TODO: Do the same with the width and height of the widget
    // getting relative width/height and returning either QPoint or width/height pair
    d->mSettings->beginGroup(name);
    int x = 0;
    int y = 0;
    QRect screenRect = QDesktopWidget().screenGeometry();
#ifdef Q_WS_WIN
    // A 1px hack to make the widget fullscreen and not covering the toolbar on Win
    screenRect.setHeight(screenRect.height()-1);
#endif

    QString x_value = d->mSettings->value("x").toString();
    QString y_value = d->mSettings->value("y").toString();

    QRegExp rx("(\\d+)");
    rx.indexIn(x_value, 0);

    qDebug() << Q_FUNC_INFO << "Parsed Value: " << rx.cap(1);
    x = rx.cap(1).toUInt();
    x = (screenRect.width()/100) * x;


    rx.indexIn(y_value, 0);
    y = rx.cap(1).toUInt();
    y = (screenRect.height()/100) * y;

    qDebug() << Q_FUNC_INFO << x << ": " << y;

    d->mSettings->endGroup();

    return QPoint(x, y);
}

PlexyDesk::DesktopWidget::State ThemepackLoader::widgetView(const QString &name)
{
    PlexyDesk::DesktopWidget::State default_state = PlexyDesk::DesktopWidget::NORMALSIDE;
    d->mSettings->beginGroup(name);
    QString state = d->mSettings->value("view").toString().toUpper();
    if (!state.isEmpty() && !state.isNull()) {
        if (state == "NORMALSIDE") {
           d->mSettings->endGroup();
           return default_state;
        } else if (state == "BACKSIDE") {
            d->mSettings->endGroup();
            return PlexyDesk::DesktopWidget::BACKSIDE;
        } else if (state == "DOCK") {
            d->mSettings->endGroup();
            return PlexyDesk::DesktopWidget::DOCK;
        }

    }

    d->mSettings->endGroup();
    return default_state;
}


void ThemepackLoader::setThemeName(const QString &name)
{
    Q_UNUSED(name);
}

} // namespace plexydesk
