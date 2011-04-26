#include <imagecache.h>
#include <QSvgRenderer>
#include <QtDebug>

namespace PlexyDesk
{

ImageCache *ImageCache::staticObject = NULL;

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

ImageCache *ImageCache::instance() {
    if (!staticObject)
        return new ImageCache();

    return staticObject;
}

ImageCache::ImageCache() : d(new Private)
{
    load("default");
}

ImageCache::~ImageCache()
{
    delete d;
}

void ImageCache::load(const QString &themename)
{
    QString prefix = QString(PLEXPREFIX) + "/theme/"+ themename +"/";

    QDir dir(prefix);
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); i++)
    {
        QFileInfo file = list.at(i);
        d->map[file.completeBaseName()] = QPixmap(file.absoluteFilePath());
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
