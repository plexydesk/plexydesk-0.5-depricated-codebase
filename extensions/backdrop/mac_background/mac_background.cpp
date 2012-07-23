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

#include <QDir>
#include <QPixmapCache>
#include <QDeclarativeContext>

#include <QtDebug>

#include "mac_background.h"
#include <desktopwidget.h>
#include <plexyconfig.h>


MacBackgroundSource::MacBackgroundSource(QObject *object)
    : BackgroundSource(object)
{
    mMacDesktopRectItem = new QGraphicsRectItem();
}

MacBackgroundSource::~MacBackgroundSource()
{
    if (mMacDesktopRectItem)
        delete mMacDesktopRectItem;
}

void  MacBackgroundSource::setRect(const QRect &rect)
{
  mDesktopScreenRect = rect;
}

void MacBackgroundSource::changeWallpaperItem()
{

    QSize desktopSize = mDesktopScreenRect.size();

    mMacDesktopRectItem->setRect(0, 0, desktopSize.width(), desktopSize.height());
}

void MacBackgroundSource::data(QVariant &data)
{
}

QGraphicsItem *MacBackgroundSource::item()
{
    QSize desktopSize = mDesktopScreenRect.size();
    mMacDesktopRectItem->setRect(0, 0, desktopSize.width(), desktopSize.height());
    return mMacDesktopRectItem;
}
