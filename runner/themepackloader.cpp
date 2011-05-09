#include "themepackloader.h"
#include <config.h>
#include <QDebug>
#include <QStringList>

class ThemepackLoader::ThemepackLoaderPrivate
{

public:
ThemepackLoaderPrivate() {}
~ThemepackLoaderPrivate() {}

  QString mThemeCfgFile;
  QString mBasePath;
};

ThemepackLoader::ThemepackLoader(const QString &themeName,
        Format format,
        QObject *parent) :
    QSettings(QLatin1String(PLEXPREFIX) +
            QLatin1String("/share/plexy/themepack/") + themeName +
            QLatin1String("/main.cfg"), format, parent),
    d (new ThemepackLoaderPrivate)

{
    d->mThemeCfgFile = fileName();
    d->mBasePath = QLatin1String(PLEXPREFIX) +
        QLatin1String("/share/plexy/themepack/") +
        themeName + QLatin1String("/");

    qDebug() << Q_FUNC_INFO << d->mThemeCfgFile;
}

QString ThemepackLoader::wallpaper()
{
    QString rv;
    beginGroup(QLatin1String("main"));
    rv = value("wallpaper").toString();
    endGroup();
    return rv;
}

QStringList ThemepackLoader::widgets(const QString &expType)
{
    QStringList rv;
    Q_FOREACH(const QString &key, childGroups()) {
        qDebug() << Q_FUNC_INFO << key;
        if (key == QLatin1String("main")) {
            continue;
        }
        beginGroup(key);
        QString type = value(QLatin1String("type")).toString();
        if (type != expType) {
            endGroup();
            continue;
        }
        endGroup();
        rv << key;
    }

    return rv;
}

QString ThemepackLoader::qmlFilesFromTheme(const QString &name)
{
    beginGroup(name);
    QString fileName = value("mainfile").toString();
    QString baseFolder = value("basefolder").toString();
    qDebug() << Q_FUNC_INFO << name << ":" << fileName;
    endGroup();

    return (d->mBasePath + baseFolder + QLatin1String("/") + fileName );
}


void ThemepackLoader::setThemeName(const QString &name)
{
    Q_UNUSED(name);
}
