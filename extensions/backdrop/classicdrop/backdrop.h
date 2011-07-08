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
#ifndef PLEXY_CLASSIC_BACKDROP_H
#define PLEXY_CLASSIC_BACKDROP_H

#include <QPainter>
#include <QDesktopWidget>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <plexy.h>
#include <config.h>
#include <backdropplugin.h>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>

class BgPlugin : public PlexyDesk::BackdropPlugin
{
    Q_OBJECT

public:
    BgPlugin(QObject *object = 0);
    virtual ~BgPlugin();
    QGraphicsItem *item();

public Q_SLOTS:
    void data(QVariant &);
    void pushData(QVariant &) {
    }
    void changeWallpaperItem();

Q_SIGNALS:
    void dataChange();

private:
    QPixmap mBackgroundCache;
    QPixmap *mBackgroundPixmap;
    QGraphicsPixmapItem *mBackgroundItem;
    QPropertyAnimation *mBlurAnimation;
    QGraphicsBlurEffect *mBlurEffect;
};

#endif //PLEXY_CLASSIC_BACKDROP_H
