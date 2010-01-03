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
#include "twit.h"
#include "twitwidget.h"

Twit::Twit(QObject * object)
{

}

Twit::~Twit()
{

}

QGraphicsItem * Twit::item()
{
//   PlexyDesk::AbstractPluginInterface * utubeEngine = (PlexyDesk::AbstractPluginInterface*)
//   PlexyDesk::PluginLoader::getInstance()->instance("restengine");
//   qDebug() << Q_FUNC_INFO << utubeEngine->instance();
    return new PlexyDesk::TwitWidget(QRectF(0, 0, 480, 320), 0);
}
