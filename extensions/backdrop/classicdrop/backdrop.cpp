/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
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
#include "backdrop.h"
#include <desktopwidget.h>
#include <plexyconfig.h>
#include <QDir>

BgPlugin::BgPlugin(QObject *object)
    : BackdropPlugin(object)
{
    mBackgroundPixmap =
     new QPixmap(QDir::toNativeSeparators(PlexyDesk::Config::getInstance()->wallpaper()));
    qDebug() << Q_FUNC_INFO << QDir::toNativeSeparators(PlexyDesk::Config::getInstance()->wallpaper());
}

BgPlugin::~BgPlugin()
{
}

void BgPlugin::data(QVariant &data)
{
    QImage wall = data.value<QImage>();
    if (wall.isNull()) {
        wall = QImage::fromData(data.toByteArray());
	return;
    }

    emit dataChange();
}

void BgPlugin::render(QPainter *p, QRectF r)
{
    if (mBackgroundCache.isNull()) {
        mBackgroundCache = mBackgroundPixmap->scaled(r.width(), r.height(),
                Qt::KeepAspectRatioByExpanding,
                Qt::SmoothTransformation);
        delete mBackgroundPixmap;
    }

    const QSize imageSize = mBackgroundCache.size();

    p->save();
    p->setRenderHint(QPainter::SmoothPixmapTransform, false);
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setRenderHint(QPainter::NonCosmeticDefaultPen, false);
    p->drawPixmap(r.x(), r.y(), imageSize.width(), imageSize.height(), mBackgroundCache);
    p->restore();
}
