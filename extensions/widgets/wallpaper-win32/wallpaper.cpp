/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Dariusz Mikulski <dariusz.mikulski@gmail.com>
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
#include "wallpaper.h"
#include "wallpaperwidget.h"

#include <QPushButton>
#include <QSettings>
#include <QVariantMap>

WallpaperChange::WallpaperChange(QObject *object)
{
}

WallpaperChange::~WallpaperChange()
{

}

QGraphicsItem *WallpaperChange::item()
{
    QPushButton *button = new QPushButton("Change Wallpaper", 0);
    button->move(10, 10);
    connect(button, SIGNAL(clicked(bool)), this, SLOT(changeWallpaper(bool)));

    return new WallpaperWidget(QRectF(0, 0, 340, 240), button);
}

void WallpaperChange::pushData(QVariant &)
{

}

void WallpaperChange::changeWallpaper(bool)
{
    QString wallpaperRegPath("HKEY_CURRENT_USER\\Control Panel\\Desktop");
    QSettings settings(wallpaperRegPath, QSettings::NativeFormat);
    QString imagePath = settings.value("Wallpaper").toString();
    QImage img(imagePath);
    QVariant image(img);
    emit data(image);
}

QVariantMap WallpaperChange::readAll()
{
    return QVariantMap();
}
