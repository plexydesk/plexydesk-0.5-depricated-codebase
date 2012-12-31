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

#ifndef PLEXY_DESKTOP_PLUGIN_H
#define PLEXY_DESKTOP_PLUGIN_H

#include <plexy.h>
#include <abstractdesktopview.h>

class QPainter;
class QVariant;
class QRectF;

namespace PlexyDesk
{
class PLEXYDESKCORE_EXPORT DesktopViewPlugin : public QObject
{
    Q_OBJECT

public:
    DesktopViewPlugin(QObject *object = 0) : QObject(object){}

    virtual ~DesktopViewPlugin(){}

    virtual QObject *view(QGraphicsScene *scene) = 0;

    virtual void setRect (const QRect &rect) = 0;

Q_SIGNALS:
    void data(QVariant &value);
};
}

#endif
