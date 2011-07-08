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

#ifndef PLEXY_BACK_PLUGIN_H
#define PLEXY_BACK_PLUGIN_H

#include <baseplugin.h>
#include <config.h>
#include <plexy.h>

class QPainter;
class QVariant;
class QRectF;

namespace PlexyDesk
{
class PLEXYDESK_EXPORT BackdropPlugin : public BasePlugin
{
    Q_OBJECT

public:
    BackdropPlugin(QObject *object = 0) : BasePlugin(object)
    {
    }

    virtual ~BackdropPlugin()
    {
    }

    virtual QGraphicsItem *item() = 0;

public Q_SLOTS:
    virtual void pushData(QVariant &value) = 0;

Q_SIGNALS:
    void data(QVariant &value);
};
}

#endif
