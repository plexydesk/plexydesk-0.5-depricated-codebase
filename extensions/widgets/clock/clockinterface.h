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
#ifndef PLEXY_CLOCK_I
#define PLEXY_CLOCK_I

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>
#include "clock.h"
#include "clockwidget.h"

class ClockInterface : public PlexyDesk::WidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

public :
    ClockInterface(QObject *object = 0)
    {
        Q_UNUSED(object);
    }

    PlexyDesk::AbstractSource *instance()
    {
        return new Clock(this);
    }
};

Q_EXPORT_PLUGIN2(ClockInterface, ClockInterface)
#endif
