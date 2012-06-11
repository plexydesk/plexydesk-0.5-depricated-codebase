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

#include <svgprovider.h>


namespace PlexyDesk
{

class SvgProvider::Private
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

void SvgProvider::clear()
{
    d->map.clear();
    d->fileHash.clear();
}

SvgProvider::SvgProvider(QDeclarativeImageProvider::ImageType type) :
    QDeclarativeImageProvider(type), d(new Private)
{
    load("default");
}

SvgProvider::~SvgProvider()
{
    delete d;
}

QPixmap SvgProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (size->width() <= 0 && size->height() <= 0) {
        return get(id, requestedSize);
    }
    QPixmap rv = get(id, requestedSize);
    QSize imgSize = rv.size();
    size = &imgSize;
    return rv;
}

void SvgProvider::load(const QString &themename)
{
    QString prefix = QDir::toNativeSeparators(Config::getInstance()->plexydeskBasePath() +
            QLatin1String("/share/plexy/themepack/") +
            themename
            + QLatin1String("/resources/"));

    QDir dir(prefix);
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList() << "*.svg" << "*.svgz");
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); i++)
    {
        QFileInfo file = list.at(i);
        d->map[file.completeBaseName()] = QPixmap(QDir::toNativeSeparators(file.absoluteFilePath()));
        d->fileHash[file.completeBaseName()] = file.absoluteFilePath();
    }
}

void SvgProvider::addToCached(QString &imgfile, QString &filename, QString &themename)
{
    QString prefix = QDir::toNativeSeparators(Config::getInstance()->plexydeskBasePath() +
            QLatin1String("/share/plexy/themepack/") +
            themename
            + QLatin1String("/resources/"));

    QFileInfo file = prefix + imgfile;
    d->map[filename] = QPixmap(QDir::toNativeSeparators(file.absoluteFilePath()));
    d->fileHash[filename] = file.absoluteFilePath();
}


QPixmap SvgProvider::get(const QString &name, const QSize &size)
{
    QStringList itemList = name.split('#');
    QPixmap rv (size);

    if (itemList.count() < 1) {
      return rv; 
    } else if (itemList.count() == 1) {
      return d->map[name];
    }

    rv.fill(Qt::transparent);
    QPainter painter;

    painter.begin(&rv);

    drawSvg(&painter, QRectF (0.0, 0.0, size.width(), size.height()),
        itemList.value(0), itemList.value(1));

    painter.end();
    return rv;
}

bool SvgProvider::isCached(QString &filename) const
{
    if ((d->fileHash[filename]).isNull())
        return false;

    return true;
}

bool SvgProvider::drawSvg(QPainter *p, QRectF rect, const QString &file, const QString &elementId)
{
    QString svgFile = d->fileHash[file];

    QFileInfo fileInfo (svgFile);
    if (fileInfo.exists()) {
        d->render.load(svgFile);
        d->render.render(p, elementId,  rect);
        return true;
    }

    return false;
}
} //namespace
