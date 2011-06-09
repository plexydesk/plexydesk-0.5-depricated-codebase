#include "themepackloader.h"
#include <config.h>
#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QDesktopWidget>
#include <QRect>

const QString themePackPath = QString("%1/%2").arg(PLEXPREFIX).arg("/share/plexy/themepack");

class ThemepackLoader::ThemepackLoaderPrivate
{
public:
    ThemepackLoaderPrivate() {}
    ~ThemepackLoaderPrivate() {}

    QSettings *mSettings;
    QString mThemeCfgFile;
    QString mBasePath;
    QString mThemeName;
};

ThemepackLoader::ThemepackLoader(const QString &themeName, QObject *parent) :
    QObject(parent), d (new ThemepackLoaderPrivate)
{
    d->mThemeName = themeName;
    QDir mainConfig(QString("%1/%2/").arg(themePackPath).arg(themeName));
    d->mSettings = new QSettings(QDir::toNativeSeparators(
                                 mainConfig.absoluteFilePath("main.cfg")),
                             QSettings::IniFormat, parent);

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

    QDir
        prefix(QString("%1/%2/%3").arg(themePackPath).arg(d->mThemeName).arg(QLatin1String("resources")));

    return QDir::toNativeSeparators(prefix.absoluteFilePath(rv));
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

QPoint ThemepackLoader::widgetPos(const QString &name)
{
    d->mSettings->beginGroup(name);
    int x = 0;
    int y = 0;
    QRect screenRect = QDesktopWidget().availableGeometry();

    QString x_value = d->mSettings->value("x").toString();
    QString y_value = d->mSettings->value("y").toString();

    QRegExp rx("(\\d+)");
    rx.indexIn(x_value, 0);

    qDebug() << Q_FUNC_INFO << "Parsed Value" << rx.cap(1); 
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
