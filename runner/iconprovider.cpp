#include "iconprovider.h"
#include <plexyconfig.h>

#include <QPixmap>
#include <QQueue>
#include <QSettings>
#include <QString>

namespace PlexyDesk
{
class IconProvider::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    QStringList iconpaths;

};

IconProvider::IconProvider() : d(new Private)
{
}

IconJobPtr IconProvider::requestIcon(const QString &name, const QString &size)
{
    QSharedPointer<IconJob> obj =
         QSharedPointer<IconJob>(new IconJob(this), &QObject::deleteLater);
    obj->requestIcon(name, size);
    return obj;
}

}
