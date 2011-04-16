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
#ifndef PLEXY_WALLPAPER_WIDGET_H
#define PLEXY_WALLPAPER_WIDGET_H

#include <QObject>
#include <QRectF>

#include <desktopwidget.h>
#include <datainterface.h>

#include "config.h"

class QWidget;
class QStyleOptionGraphicsItem;
class QPainter;

class WALLPAPERCHANGE_EXPORT WallpaperWidget : public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    WallpaperWidget(const QRectF &rect, QWidget *embeddedWidget = 0);
    virtual ~WallpaperWidget();

    void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem *e, QWidget *);
    void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void paintExtBackFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *);
    QRectF boundingRect() const;
};

#endif // PLEXY_WALLPAPER_WIDGET_H
