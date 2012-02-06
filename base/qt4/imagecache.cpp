/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Siraj Razick <siraj@kde.org>
*                 PhobosK <phobosk@kbfx.net>
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#include <QSvgRenderer>
#include <QtDebug>
#include <QObject>

#include <imagecache.h>


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
    QObject (0),
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

    Q_EMIT ready();
}

void ImageCache::addToCached(QString &imgfile, QString &filename, QString &themename)
{
    QString prefix = QDir::toNativeSeparators(Config::getInstance()->plexydeskBasePath() +
            QLatin1String("/share/plexy/themepack/") +
            themename
            + QLatin1String("/resources/"));

    QFileInfo file = prefix + imgfile;
    d->map[filename] = QPixmap(QDir::toNativeSeparators(file.absoluteFilePath()));
    d->fileHash[filename] = file.absoluteFilePath();
}


QPixmap ImageCache::get(const QString &name)
{
    return d->map[name];
}

bool ImageCache::isCached(QString &filename) const
{
    if ((d->fileHash[filename]).isNull())
        return false;

    return true;
}

bool ImageCache::drawSvg(QPainter *p, QRectF rect, const QString &file, const QString &elementId)
{
    QString svgFile = d->fileHash[file];
    qDebug() << Q_FUNC_INFO << svgFile;
    QFileInfo fileInfo (svgFile);
    if (fileInfo.exists()) {
        d->render.load(svgFile);
        d->render.render(p, elementId,  rect);
        return true;
    }

    return false;
}
} //namespace
