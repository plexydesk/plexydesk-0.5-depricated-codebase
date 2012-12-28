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

#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNamedNodeMap>

#include <plexyconfig.h>

#include "themepackloader.h"

namespace PlexyDesk
{

class ThemepackLoader::ThemepackLoaderPrivate
{
public:
    ThemepackLoaderPrivate() {}
    ~ThemepackLoaderPrivate()
    {
        if (mXmlRawFile)
            delete mXmlRawFile;
    }

    QSettings *mSettings;
    QString mThemeCfgFile;
    QString mBasePath;
    QString mThemeName;
    QString mThemePackPath;
    QRectF mScreenRect;

    QString mXmlConfigFile;
    QDomDocument mXmlDocumentRoot;
    QFile *mXmlRawFile;

    //todo until we write a proper render tree.
    QHash<QString, QPoint> mWidgetPosition;
    QHash<QString, QString> mWidgetFeatures;
};

ThemepackLoader::ThemepackLoader(const QString &themeName, QObject *parent) :
    QObject(parent), d (new ThemepackLoaderPrivate)
{
    d->mThemePackPath =
        QDir::toNativeSeparators(
                QString("%1/%2")
                .arg(PlexyDesk::Config::getInstance()->plexydeskBasePath())
                .arg("/share/plexy/themepack")
        );

    d->mThemeName = themeName;
    QDir mainConfig(QDir::toNativeSeparators(QString("%1/%2/").arg(d->mThemePackPath).arg(themeName)));
    d->mSettings = new QSettings(QDir::toNativeSeparators(
                                 mainConfig.absoluteFilePath("main.cfg")),
                             QSettings::IniFormat, this);

    d->mThemeCfgFile = QDir::toNativeSeparators(d->mSettings->fileName());
    d->mBasePath = QDir::toNativeSeparators(mainConfig.absolutePath());

    d->mXmlConfigFile = mainConfig.absoluteFilePath("layout.xml");
    d->mXmlRawFile = new QFile(d->mXmlConfigFile);

    if (!d->mXmlRawFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << Q_FUNC_INFO << "Failed to open " << d->mXmlConfigFile;
    } else {
         if (!d->mXmlDocumentRoot.setContent(d->mXmlRawFile)) {
             qWarning() << Q_FUNC_INFO << "Failed to load the xml file";
         }
    }
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
    d->mSettings->endGroup();

    QDir path(QString("%1/%2").arg(d->mBasePath).arg(baseFolder));
    return (QDir::toNativeSeparators(QDir::cleanPath(
                                     path.absoluteFilePath(fileName))
                                    ));
}

void ThemepackLoader::setRect (const QRectF &rect)
{
    d->mScreenRect = rect;
}

QPoint ThemepackLoader::widgetPos(const QString &name)
{
    // TODO: Do the same with the width and height of the widget
    // getting relative width/height and returning either QPoint or width/height pair
    d->mSettings->beginGroup(name);
    int x = 0;
    int y = 0;
    QRectF screenRect = d->mScreenRect;
#ifdef Q_WS_WIN
    // A 1px hack to make the widget fullscreen and not covering the toolbar on Win
    screenRect.setHeight(screenRect.height()-1);
#endif

    QString x_value = d->mSettings->value("x").toString();
    QString y_value = d->mSettings->value("y").toString();

    QRegExp rx("(\\d+)");
    rx.indexIn(x_value, 0);

    x = rx.cap(1).toUInt();
    x = (screenRect.width()/100) * x;

    rx.indexIn(y_value, 0);
    y = rx.cap(1).toUInt();
    y = (screenRect.height()/100) * y;

    d->mSettings->endGroup();

    return QPoint(x, y);
}

PlexyDesk::AbstractDesktopWidget::State ThemepackLoader::widgetView(const QString &name)
{
    PlexyDesk::AbstractDesktopWidget::State default_state = PlexyDesk::AbstractDesktopWidget::VIEW;
    d->mSettings->beginGroup(name);
    QString state = d->mSettings->value("view").toString().toUpper();
    if (!state.isEmpty() && !state.isNull()) {
        if (state == "VIEW") {
           d->mSettings->endGroup();
           return default_state;
        } else if (state == "BACK") {
            d->mSettings->endGroup();
            return PlexyDesk::AbstractDesktopWidget::ROTATED;
        } else if (state == "DOCK") {
            d->mSettings->endGroup();
            return PlexyDesk::AbstractDesktopWidget::DOCKED;
        }

    }

    d->mSettings->endGroup();
    return default_state;
}

QString ThemepackLoader::loadSessionFromDisk() const
{
    QString homePath = QDir::homePath();
    QString result;

    homePath.append("/.plexydesk/session.xml");

    QFileInfo info(homePath);

    if (info.exists()) {
        QFile sessionFile(homePath);

        if (sessionFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

            while (!sessionFile.atEnd()) {
                  QByteArray line = sessionFile.readLine();
                  result.append(line);
              }
        }
    }

    return result;
}

void ThemepackLoader::saveSessionToDisk(const QString &data)
{
    QString homePath = QDir::toNativeSeparators(QDir::homePath() + "/.plexydesk/");
    QFileInfo fileInfo(homePath);

    if (!fileInfo.exists()) {
        QDir::home().mkpath(homePath);
    }

    QFile file(QDir::toNativeSeparators(QDir::homePath() + "/.plexydesk/session.xml"));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << data;

    file.close();
}

void ThemepackLoader::setThemeName(const QString &name)
{
    Q_UNUSED(name);
}

QStringList ThemepackLoader::desktopWidgets() const
{
    QStringList rv;
    if (!d->mXmlDocumentRoot.hasChildNodes())
        return rv;

    QDomNodeList widgetNodeList = d->mXmlDocumentRoot.documentElement().elementsByTagName("widget");

    for(int index = 0;index < widgetNodeList.count(); index++) {
        QDomElement widgetElement = widgetNodeList.at(index).toElement();

        QString currentWidgetStringName = widgetElement.attribute("name");
        rv.append(currentWidgetStringName);

        QDomNode widgetEntries= widgetElement.firstChild();

//        while(!widgetEntries.isNull()) {
//            QDomElement subEntry = widgetEntries.toElement();
//            //QString tag = subEntry.tagName();

//            widgetEntries = widgetEntries.nextSibling();
//        }
    }

    return rv;
}

QString ThemepackLoader::desktopBackgroundController() const
{
    QString rv;
    if (!d->mXmlDocumentRoot.hasChildNodes())
        return rv;

    QDomNodeList widgetNodeList = d->mXmlDocumentRoot.documentElement().elementsByTagName("background");

    if (widgetNodeList.count() <= 0)
        return rv;

    QDomElement widgetElement = widgetNodeList.at(0).toElement();

    rv = widgetElement.attribute("name");
    return rv;
}

bool ThemepackLoader::queryMultiScreen(const QString & /*name*/)
{
    return false;
}

//TODO:
// Refactor these two methods
QRectF ThemepackLoader::positionForWidget(const QString &name, const QRectF &screen_rect)
{
    QDomNodeList widgetNodeList = d->mXmlDocumentRoot.documentElement().elementsByTagName("widget");
    QRectF rect;

    for(int index = 0; index < widgetNodeList.count(); index++) {
        QDomElement widgetElement = widgetNodeList.at(index).toElement();

        if (widgetElement.attribute("name") != name)
            continue;

        if (widgetElement.hasChildNodes()) {
            QDomElement rectElement = widgetElement.firstChildElement("rect");

            QDomAttr x = rectElement.attributeNode("x");
            QDomAttr y = rectElement.attributeNode("y");

            QDomAttr widthAttr = rectElement.attributeNode("width");
            QDomAttr heightAttr = rectElement.attributeNode("height");

            QString widthString = widthAttr.value();
            QString heightString = heightAttr.value();

            float width = 0.0f;
            float height = 0.0f;
            float x_coord = 0.0f;
            float y_coord = 0.0f;

            if (widthString.contains("%")) {
                width = toScreenValue(widthString, screen_rect.width());
            } else
                x_coord = x.value().toFloat();

            if (heightString.contains("%")) {
                height = toScreenValue(heightString, screen_rect.height());
            } else
                y_coord = x.value().toFloat();

            if (x.value().contains("%")) {
                x_coord = toScreenValue(x.value(), screen_rect.width());
            }

            if (y.value().contains("%")) {
                y_coord = toScreenValue(y.value(), screen_rect.height());
            }

            if (widthString == "device-width") {
                width = screen_rect.width();
            }

            if (heightString == "device-height") {
                height = screen_rect.height();
            }

            rect = QRectF( x_coord + screen_rect.x(), y_coord + screen_rect.y(), width, height);
        }
    }

    return rect;
}

QRectF ThemepackLoader::positionForBackground(const QString &name, const QRectF &screen_rect)
{
    QDomNodeList widgetNodeList = d->mXmlDocumentRoot.documentElement().elementsByTagName("background");
    QRectF rect;

    for(int index = 0; index < widgetNodeList.count(); index++) {
        QDomElement widgetElement = widgetNodeList.at(index).toElement();

        if (widgetElement.attribute("name") != name)
            continue;

        if (widgetElement.hasChildNodes()) {
            QDomElement rectElement = widgetElement.firstChildElement("rect");

            QDomAttr x = rectElement.attributeNode("x");
            QDomAttr y = rectElement.attributeNode("y");

            QDomAttr widthAttr = rectElement.attributeNode("width");
            QDomAttr heightAttr = rectElement.attributeNode("height");

            QString widthString = widthAttr.value();
            QString heightString = heightAttr.value();

            float width = 0.0f;
            float height = 0.0f;
            float x_coord = 0.0f;
            float y_coord = 0.0f;

            if (widthString.contains("%")) {
                width = toScreenValue(widthString, screen_rect.width());
            } else
                x_coord = x.value().toFloat();

            if (heightString.contains("%")) {
                height = toScreenValue(heightString, screen_rect.height());
            } else
                y_coord = x.value().toFloat();

            if (x.value().contains("%")) {
                x_coord = toScreenValue(x.value(), screen_rect.width());
            }

            if (y.value().contains("%")) {
                y_coord = toScreenValue(y.value(), screen_rect.height());
            }

            if (widthString == "device-width") {
                width = screen_rect.width();
            }

            if (heightString == "device-height") {
                height = screen_rect.height();
            }

            rect = QRectF( x_coord + screen_rect.x(), y_coord + screen_rect.y(), width, height);
        }
    }

    return rect;
}

int ThemepackLoader::toScreenValue(const QString &val, int max_distance)
{
    QRegExp rx("(\\d+)");

    rx.indexIn(val, 0);
    int _x = rx.cap(1).toUInt();
    _x = (max_distance/100) * _x;

    return _x;
}

} // namespace plexydesk
