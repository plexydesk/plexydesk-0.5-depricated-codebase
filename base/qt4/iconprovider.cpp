#include "iconprovider.h"
#include <plexyconfig.h>
#include <QPixmap>
#include <QSettings>
#include <QString>

namespace PlexyDesk
{
class IconProvider::Private
{
public:
    Private() {}
    ~Private() {}
    QStringList iconpaths;
    QList<Requests> tasks;

};

/*
IconProvider* IconProvider::obj = 0;

IconProvider* IconProvider::instance()
{
    //if(!obj) {
      new IconProvider();
    //}

    //return obj;
}
*/

IconProvider::IconProvider():d(new Private)
{
    /*Order is according to the freedesktop icon theme sepc ,
      if adding more paths append to the end. do not alter
      this layout.
      */
    d->iconpaths << QDir::homePath()+"/.icons/"+ Config::getInstance()->iconTheme + "/"
    ;

    QStringList xdg = QString(qgetenv("XDG_DATA_DIRS")).split(':');
    foreach(QString path, xdg) {
        d->iconpaths << path +"/icons/"+Config::getInstance()->iconTheme + "/";
    }
    d->iconpaths << "/usr/share/pixmaps"
    << "/usr/share/app-install/icons/";

    connect(this,SIGNAL(requestAdded()), this, SLOT(loadIcons()));
}

void IconProvider::requestIcon(const QString& name, const QString& size)
{
    Requests r;
    r.name = name;
    r.size = size;
    d->tasks.append(r);
    emit requestAdded();
}

void IconProvider::loadIcons()
{
    QPixmap pixmap ;
    foreach(QString path, d->iconpaths) {
        QDir dir(path);
        if (dir.exists()) {
            if (!d->tasks.isEmpty()) {
                Requests current = d->tasks.first();
                if(current.name.isEmpty()) {
                        d->tasks.removeFirst();
                    return;
                }
                QString subpath = current.size + "/" + current.name + ".png";
                QStringList iconlist = getSubDir(path);
                foreach(QString icon, iconlist) {
                  QFile iconfile(icon+"/"+current.name+".png");
                  if(iconfile.exists() ){
                      pixmap = QPixmap(icon+"/"+current.name);
                  }
                }

            }
        }
    }

    if (pixmap.isNull()) {
        qDebug()<<"Bad icon"<<endl;
    } else
        qDebug()<<"=========================== YAY icon found"<<endl;

    emit iconPixmap(pixmap);
    d->tasks.removeFirst();
}

QStringList IconProvider::getSubDir(const QString& path)
{
    QStringList rpaths;
    QStringList paths;
    QFile file (path+"/"+"index.theme");
    if(file.exists()) {
      QSettings indexfile(path+"/"+"index.theme", QSettings::IniFormat);
       qDebug()<<"Getting index.theme  from:"<<path+"/"+"index.theme"<<endl;
       indexfile.beginGroup("Icon Theme");
        rpaths = indexfile.value("Directories","").toStringList();
        indexfile.endGroup();
    }

    foreach (QString _rpath, rpaths) {
     paths.append(path +"/"+_rpath);
    }
    paths.append("/usr/share/app-install/icons/");
    return paths;
}

}
