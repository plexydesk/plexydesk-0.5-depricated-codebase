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
#include "clock.h"
#include "clockwidget.h"
#include <abstractdesktopview.h>
#include <controllerinterface.h>

Clock::Clock(QObject *parent) : PlexyDesk::ControllerInterface (parent)
{
    clock = new ClockWidget(QRectF(0, 0, 210, 210));
    clock->setController(this);

    if (connectToDataSource("timerengine")) {
        connect(dataSource(), SIGNAL(sourceUpdated(QVariantMap)), this, SLOT(onDataUpdated(QVariantMap)));
    }
}

Clock::~Clock()
{
    if (clock)
        delete clock;

}

PlexyDesk::AbstractDesktopWidget *Clock::defaultView()
{
    return clock;
}

void Clock::revokeSession(const QVariantMap & /*args*/)
{
}

void Clock::setViewRect(const QRectF &rect)
{
    if (clock)
        clock->setPos(rect.x(), rect.y());
}

bool Clock::deleteWidget(PlexyDesk::AbstractDesktopWidget *widget)
{
    disconnect(dataSource(), SIGNAL(sourceUpdated(QVariantMap)));
    int index = 0;

    if (widget == clock) {
        delete clock;
        clock = 0;
        return 1;
    }

    Q_FOREACH (ClockWidget *_clock, mClocks) {
        if (_clock && _clock == widget) {
            mClocks.removeAt(index);
            delete _clock;
            _clock = 0;
            return 1;
        }

        index++;
    }

    return 1;
}

QStringList Clock::actions() const
{
    QStringList actionList;
    actionList << "Add Clock";

    return actionList;
}

void Clock::requestAction(const QString &actionName, const QVariantMap &args)
{
    if (actionName == "Add Clock") {
        ClockWidget *clock_widget = new ClockWidget(QRectF(0, 0, 210, 210));
        clock_widget->setController(this);
        mClocks.append(clock_widget);
        if(viewport()) {
            viewport()->addWidgetToView(clock_widget);
        }
    }
}

void Clock::onDataUpdated(const QVariantMap &data)
{
    if (clock)
        clock->updateTime(data);

    Q_FOREACH (ClockWidget *_clock, mClocks) {
        if (_clock)
            _clock->updateTime(data);
    }
}
