#include <imagecache.h>
#include <QSvgRenderer>
#include <QtDebug>
#include <QObject>

namespace PlexyDesk
{

class ImageCache::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    QHash<QString, QPixmap> map;
    QHash<QString, QString> fileHash;
    QSvgRenderer render;

};

void ImageCache::clear()
{
    d->map.clear();
}

ImageCache::ImageCache(QDeclarativeImageProvider::ImageType type) :
    QDeclarativeImageProvider(type), d(new Private)
{
    load("default");
}

ImageCache::~ImageCache()
{
    delete d;
}

QPixmap ImageCache::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (size->width() <= 0 && size->height() <= 0) {
        return get(id);
    }
    QPixmap rv = get(id).scaled(requestedSize.width(), requestedSize.height(),
                            Qt::KeepAspectRatioByExpanding,
                            Qt::SmoothTransformation);
    return rv;
}
void ImageCache::load(const QString &themename)
{
    QString prefix = QDir::toNativeSeparators(Config::getInstance()->plexydeskBasePath() +
            QLatin1String("/share/plexy/themepack/") +
            themename
            + QLatin1String("/resources/"));

    QDir dir(prefix);
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); i++)
    {
        QFileInfo file = list.at(i);
        d->map[file.completeBaseName()] = QPixmap(QDir::toNativeSeparators(file.absoluteFilePath()));
        d->fileHash[file.completeBaseName()] = file.absoluteFilePath();
    }
}

QPixmap ImageCache::get(const QString &name)
{
    return d->map[name];
}

bool ImageCache::drawSvg(QPainter *p, QRectF rect, const QString &file)
{
    QString svgFile = d->fileHash[file];
    qDebug() << Q_FUNC_INFO << svgFile;
    QFileInfo fileInfo (svgFile);
    if (fileInfo.exists()) {
        d->render.load(svgFile);
        d->render.render(p, rect);
        return true;
    }
    return false;
}
} //namespace
