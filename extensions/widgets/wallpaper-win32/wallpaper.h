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
#ifndef PLEXY_WALLPAPER_CHANGE_H
#define PLEXY_WALLPAPER_CHANGE_H

#include <QtCore>
#include <plexy.h>
#include <datainterface.h>

class VISIBLE_SYM WallpaperChange : public PlexyDesk::DataPlugin
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

public :
        void init() {
    }
    WallpaperChange(QObject *object = 0);
    virtual ~WallpaperChange();
    virtual QGraphicsItem *item(); // {};
public slots:
    void pushData(QVariant &);
    void changeWallpaper(bool);
signals:
    void data(QVariant &);
};

#endif // PLEXY_WALLPAPER_CHANGE_H
