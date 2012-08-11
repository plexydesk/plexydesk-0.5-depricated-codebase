/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : *
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
#include "timer.h"
#include <desktopwidget.h>
#include <plexyconfig.h>
#include <QTimer>
#include <viewcontrollerplugin.h>

class TimerData::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    QTimer *mTimer;
};

TimerData::TimerData(QObject *object) : PlexyDesk::DataSource(object), d(new Private)
{
    startTimer(1000);
}

void TimerData::init()
{
}

TimerData::~TimerData()
{
    delete d;
}

void TimerData::setArguments(QVariant &arg)
{
}

QVariantMap TimerData::readAll()
{
    QVariant timeVariant;
    QVariantMap dataMap;

    timeVariant.setValue(QTime::currentTime());
    dataMap["currentTime"] = timeVariant;

    return dataMap;
}

void TimerData::timerEvent(QTimerEvent *event)
{
    Q_EMIT sourceUpdated(readAll());
}
