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

#include <plexy.h>
#include "youtubewidget.h"
#include <math.h>

#include <QMatrix>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>
#include <QGraphicsSceneWheelEvent>
#include <QStyleOptionGraphicsItem>

namespace PlexyDesk
{

YouTubeWidget::YouTubeWidget(const QRectF &rect, QWidget *widget):
        ListView(rect, widget)
{
    utubeEngine = qobject_cast<PlexyDesk::DataPlugin*>(
                      PlexyDesk::PluginLoader::getInstance()->instance("utubeengine"));

    if (utubeEngine) {
        connect(utubeEngine, SIGNAL(dataReady()), this, SLOT(onDataReady()));
    } else {
        qDebug("DataSource Was Null");
    }
}

YouTubeWidget::~YouTubeWidget()
{}

void YouTubeWidget::onDataReady()
{
    QVariant data = utubeEngine->readAll()["data"];
    mVariantMap = data.toMap();

    VideoEntity videoentity;
    videoentity.title = (mVariantMap["title"]).toString();
    videoentity.link = (mVariantMap["link"]).toString();
    videoentity.desc = (mVariantMap["description"]).toString();
    videoentity.thumb = (mVariantMap["thumb"]).toString();

    mVideos.append(videoentity);
    view.setHeight(mVideos.size()*mItem_bg.height());

    ListItem * item = new ListItem();
    item->title = videoentity.title;
    item->link = videoentity.link;
    item->desc = videoentity.desc;
    item->thumb = videoentity.thumb;

    insert(item);

    emit dataChanged();
}


} // namespace PlexyDesk
