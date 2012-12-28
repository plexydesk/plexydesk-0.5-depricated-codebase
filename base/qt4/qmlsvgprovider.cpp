#include "qmlsvgprovider.h"

#include <QtDeclarative/QDeclarativeImageProvider>

#include <svgprovider.h>

class QmlSvgProvider::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    PlexyDesk::SvgProvider *mSvgSource;

};

QmlSvgProvider::QmlSvgProvider(QDeclarativeImageProvider::ImageType type) : QDeclarativeImageProvider(type),
    d(new Private)
{
    d->mSvgSource = new PlexyDesk::SvgProvider();
}

QmlSvgProvider::~QmlSvgProvider()
{
    delete d;
}

QPixmap QmlSvgProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if(d->mSvgSource) {
        return d->mSvgSource->requestPixmap(id, size, requestedSize);
    }

    return QPixmap();
}
