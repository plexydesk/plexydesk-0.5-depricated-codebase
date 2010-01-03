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
#include "widget.h"
#include "widgetitem.h"
//#include <QtWebKit>


WidgetDemo::WidgetDemo(QObject * object)
{

}

WidgetDemo::~WidgetDemo()
{

}

QGraphicsItem * WidgetDemo::item()
{
    QCalendarWidget *date = new QCalendarWidget(0);
    date->setWindowOpacity(0.2);
    date->resize(300, 200);
    date->move(20, 20);

    return new  PlexyDesk::WidgetItem(QRectF(0, 0, 340, 240), date);
}

Q_EXPORT_PLUGIN2(WidgetDemo, WidgetDemo)
#include "widget.moc"
