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
};

ThemepackLoader::ThemepackLoader(const QString &themeName,
        Format format,
        QObject *parent) :
    QSettings( QLatin1String(PLEXPREFIX) +
            QLatin1String("/share/plexy/themepack/") + themeName +
            QLatin1String("/main.cfg"), format, parent),
    d (new ThemepackLoaderPrivate)

{
    d->mThemeCfgFile = fileName();
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
            continue;
        }
        endGroup();
        rv << key;
    }

    return rv;
}


void ThemepackLoader::setThemeName(const QString &name)
{
    Q_UNUSED(name);
}



