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

Clock::Clock()
{
    connectToDataSource("timer");
    connect(this, SIGNAL(data(const DataSource*)), this, SLOT(onData(DataSource*)));
    mTimer = new QTimer();
    clock = new ClockWidget(QRectF(0, 0, 210, 210));

    connect(mTimer, SIGNAL(timeout()), clock, SLOT(testSlot()));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(goData()));
    mTimer->start(1000);
}

Clock::~Clock()
{
}

QGraphicsItem *Clock::view()
{
    return clock;
}

void Clock::onData(PlexyDesk::DataSource *source)
{
    qDebug() << Q_FUNC_INFO << "Timer Data Source Recived";
    connect(dataSource(), SIGNAL(data(const QVariantMap &)), this, SLOT(onDataUpdated(PlexyDesk::DataSource*)));
}

void Clock::onDataUpdated(const QVariantMap &data)
{
    qDebug() << Q_FUNC_INFO << data;
}

void Clock::goData()
{
    qDebug() << Q_FUNC_INFO;
}
