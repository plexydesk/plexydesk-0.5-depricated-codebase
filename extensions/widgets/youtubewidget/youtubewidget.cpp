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

YouTubeWidget::YouTubeWidget(const QRectF &rect, QWidget *embeddedWidget, QDeclarativeItem *parent) :
    DesktopWidget(rect, embeddedWidget, parent)
{
    setRect(rect);
    setContentRect(rect);
    mScroll = new ScrollWidget (QRectF(5.0, 5.0, rect.width() - 20, rect.height() - 20 ), embeddedWidget);
    mScroll->setPos(10.0, 10.0);
    mScroll->setRect(QRectF(10.0, 10.0, rect.width() - 50, rect.height() - 30));
    mScroll->setParentItem (this);
    mScroll->setFlag(QGraphicsItem::ItemIsMovable, false);

    utubeEngine = qobject_cast<PlexyDesk::DataPlugin *>(
         PlexyDesk::PluginLoader::getInstance()->instance("restengine"));

    QString account;
    QString pass;
    QVariantMap mMap;
    mMap.insert("url", QUrl("http://doc.qt.nokia.com/4.2/qgraphicstextitem.html"));
    mMap.insert("type", 1); //post
    mMap.insert("user", account);
    mMap.insert("pass", pass);
    QVariant arg = QVariant(mMap);
    utubeEngine->pushData(arg);

    if (utubeEngine) {
        connect(utubeEngine, SIGNAL(dataReady()), this, SLOT(onDataReady()));
    } else {
        qDebug("DataSource Was Null");
    }
}

YouTubeWidget::~YouTubeWidget()
{
}

void YouTubeWidget::onDataReady()
{
    QVariant data = utubeEngine->readAll()["data"];

    mView = new QGraphicsTextItem();
    mView->setHtml(data.toString());

    mScroll->addWidget (mView);
    emit dataChanged();
}

} // namespace PlexyDesk
