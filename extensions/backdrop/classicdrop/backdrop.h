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

#include <config.h>

#include <QDesktopWidget>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>

#include <plexy.h>
#include <backdropplugin.h>
#include <themepackloader.h>
#include <qmldesktopwidget.h>

class BgPlugin : public PlexyDesk::BackgroundSource
{
    Q_OBJECT

public:
    BgPlugin(QObject *object = 0);
    virtual ~BgPlugin();
    QGraphicsItem *item();
    void setRect (const QRect &rect);

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
    QPropertyAnimation *mBlurAnimation;
    QGraphicsBlurEffect *mBlurEffect;
    PlexyDesk::ThemepackLoader *mThemePack;
    PlexyDesk::QmlDesktopWidget *mBackgroundItem;
    QGraphicsPixmapItem *mBackgroundItemPixmap;
    QRect mDesktopScreenRect;
};

#endif //PLEXY_CLASSIC_BACKDROP_H
