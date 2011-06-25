#include "iconjob.h"
#include <plexyconfig.h>
#include <QPixmap>

namespace PlexyDesk
{
class IconJob::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    QStringList iconpaths;
    QString name;
    QString size;
    QPixmap pixmap;
};

IconJob::IconJob(QObject *parent) : PendingJob(parent), d(new Private)
{
    /*Order is according to the freedesktop icon theme sepc ,
            if adding more paths append to the end. do not alter
                 this layout.
     */
    const QLatin1String slash("/");
    d->iconpaths << QDir::homePath() + QLatin1String("/.icons/") +
        Config::getInstance()->iconTheme()
        + slash;

    QStringList xdg = QString(qgetenv("XDG_DATA_DIRS")).split(':');
    foreach(QString path, xdg) {
        d->iconpaths << path + QLatin1String("/icons/") + Config::getInstance()->iconTheme() + slash;
    }
    d->iconpaths << QLatin1String("/usr/share/pixmaps/")
                 << QLatin1String("/usr/share/icons/")
                 << QLatin1String("/usr/share/icons/") + Config::getInstance()->iconTheme() + slash
                 << QLatin1String("/usr/share/app-install/icons/")
                 << QLatin1String(PLEXPREFIX) + QLatin1String("/share/plexy/skins") +
                    slash + Config::getInstance()->iconTheme() + slash + QLatin1String("icons") + slash;
    connect(this, SIGNAL(newJob()), this, SLOT(handleJob()), Qt::DirectConnection);
}

IconJob::~IconJob()
{
    delete d;
}

QPixmap IconJob::iconPixmap() const
{
    return d->pixmap;
}

void IconJob::requestIcon(const QString &name, const QString &size)
{
    d->size = size;
    d->name = name;
    QMetaObject::invokeMethod(this, "newJob", Qt::DirectConnection);
}

void IconJob::handleJob()
{
    QString error, message;
    foreach(QString path, d->iconpaths)
    {
        QDir dir(path);
        if (dir.exists())
        {
            QStringList iconlist = getSubDir(path);
            foreach(QString icon, iconlist)
            {
                QString iconFilePath(icon + "/" + d->name + QLatin1String(".png"));
                QFileInfo iconfile(iconFilePath);
                if (iconfile.exists())
                {
                    d->pixmap = QPixmap(iconFilePath);
                    if (!d->pixmap.isNull())
                    {
                        setFinished(true, error, message);
                        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
                        return;
                    }
                }
            }
        }
    }

   setFinished(false, error, message);
   QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
}


QStringList IconJob::getSubDir(const QString &path)
{
    QStringList rpaths;
    QStringList paths;
    QFile file(path + "index.theme");
    if (file.exists()) {
        QSettings indexfile(path + "index.theme", QSettings::IniFormat);
        indexfile.beginGroup("Icon Theme");
        rpaths =
             indexfile.value("Directories", "").toStringList();
        indexfile.endGroup();
    }
    foreach(QString _rpath, rpaths)
    {
        if(_rpath.contains(d->size))
            paths.append(path + _rpath);
    }
    paths.append("/usr/share/app-install/icons/");
    paths.append(path);
    return paths;
}

}
