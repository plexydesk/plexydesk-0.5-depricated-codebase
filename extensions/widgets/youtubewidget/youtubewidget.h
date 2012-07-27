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

#ifndef PLEXY_YOUTUBE_WIDGET_H
#define PLEXY_YOUTUBE_WIDGET_H

#include "videoentity.h"

#include <pluginloader.h>

#include <desktopwidget.h>
#include <scrollwidget.h>
#include <QGraphicsTextItem>
#include <datasource.h>

class QTimer;

namespace PlexyDesk
{

class YouTubeWidget : public ScrollWidget
{

    Q_OBJECT

public:
    YouTubeWidget(const QRectF &rect);
    virtual ~YouTubeWidget();

public Q_SLOTS:
    void onDataReady();

Q_SIGNALS:
    void dataChanged();

private:

    //refactor
    QRectF clip;
    QRectF view;


    int shade;
    int moveY;
    QImage m_bg;
    QImage mItem_bg;

    QString prefix;
    QPoint clickPos;

    PlexyDesk::DataSource *utubeEngine;

    QList<VideoEntity> mVideos;
    QMap<QString, QVariant> mVariantMap;
    QGraphicsTextItem *mView;
};

} // namespace PlexyDesk
#endif

