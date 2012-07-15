#include "qmlpixmapprovider.h"

#include <imagecache.h>

class QmlPixmapProvider::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    PlexyDesk::ImageCache *mPixmapource;

};

QmlPixmapProvider::QmlPixmapProvider(ImageType type) : QDeclarativeImageProvider(type), d(new Private)
{
    d->mPixmapource = new PlexyDesk::ImageCache();
}

QmlPixmapProvider::~QmlPixmapProvider()
{
   delete d;
}

QPixmap QmlPixmapProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if(d->mPixmapource)
        return d->mPixmapource->requestPixmap(id, size, requestedSize);

    return QPixmap();
}
