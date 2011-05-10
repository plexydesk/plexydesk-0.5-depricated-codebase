#include "themepackloader.h"
#include <config.h>
#include <QDebug>
#include <QStringList>
#include <QDir>

const QString themePackPath = QString("%1/%2").arg(PLEXPREFIX).arg("share/plexy/themepack");

class ThemepackLoader::ThemepackLoaderPrivate
{
public:
    ThemepackLoaderPrivate() {}
    ~ThemepackLoaderPrivate() {}

    QSettings *mSettings;
    QString mThemeCfgFile;
    QString mBasePath;
};

ThemepackLoader::ThemepackLoader(const QString &themeName, QSettings::Format format, QObject *parent) :
    QObject(parent), d (new ThemepackLoaderPrivate)
{
    QDir mainConfig(QString("%1/%2/").arg(themePackPath).arg(themeName));
    d->mSettings = new QSettings(QDir::toNativeSeparators(
                                 mainConfig.absoluteFilePath("main.cfg")),
                             format, parent);

    d->mThemeCfgFile = QDir::toNativeSeparators(d->mSettings->fileName());
    d->mBasePath = QDir::toNativeSeparators(mainConfig.absolutePath());

    qDebug() << Q_FUNC_INFO << d->mThemeCfgFile;
}

QString ThemepackLoader::wallpaper()
{
    QString rv;
    d->mSettings->beginGroup(QLatin1String("main"));
    rv = d->mSettings->value("wallpaper").toString();
    d->mSettings->endGroup();
    return rv;
}

QStringList ThemepackLoader::widgets(const QString &expType)
{
    QStringList rv;
    Q_FOREACH(const QString &key, d->mSettings->childGroups()) {
        qDebug() << Q_FUNC_INFO << key;
        if (key == QLatin1String("main")) {
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

QString ThemepackLoader::qmlFilesFromTheme(const QString &name)
{
    d->mSettings->beginGroup(name);
    QString fileName = d->mSettings->value("mainfile").toString();
    QString baseFolder = d->mSettings->value("basefolder").toString();
    qDebug() << Q_FUNC_INFO << name << ":" << fileName;
    d->mSettings->endGroup();

    QDir path(QString("%1/%2").arg(d->mBasePath).arg(baseFolder));
    return (QDir::toNativeSeparators(path.absoluteFilePath(fileName)));
}


void ThemepackLoader::setThemeName(const QString &name)
{
    Q_UNUSED(name);
}
