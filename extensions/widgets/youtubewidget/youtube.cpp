/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal <llahiru@gmail.com>
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
#include "youtube.h"
#include "youtubewidget.h"
#include <QtCore>
#include <QtGui>

YouTube::YouTube(QObject * object)
{

}

YouTube::~YouTube()
{

}

QGraphicsItem * YouTube::item()
{
	qDebug("Calling Youtube::item");
    return new PlexyDesk::YouTubeWidget(QRectF(0,0,210,210),0);
}

//Q_EXPORT_PLUGIN2(YouTube,YouTube)
#include "youtube.moc"
