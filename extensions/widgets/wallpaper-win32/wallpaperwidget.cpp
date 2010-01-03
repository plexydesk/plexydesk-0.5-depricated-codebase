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
#include "WallpaperWidget.h"
#include <QtCore>
#include <QtGui>

WallpaperWidget::WallpaperWidget(const QRectF &rect, QWidget *widget)
        : PlexyDesk::DesktopWidget(rect, widget)
{
}

WallpaperWidget::~WallpaperWidget()
{

}

void WallpaperWidget::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
{
}

void WallpaperWidget::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
{
    p->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    p->setPen(QColor(255, 255, 255));
    p->setFont(QFont("Bitstream Charter", 9));
    p->drawText(QRect(8, 5, 64, 64), Qt::AlignCenter, "Wallpaper");
}

void WallpaperWidget::paintExtBackFace(QPainter *p, const QStyleOptionGraphicsItem * e, QWidget *widget)
{

}

QRectF WallpaperWidget::boundingRect() const
{
    return rect();
}

#include "wallpaperwidget.moc"
