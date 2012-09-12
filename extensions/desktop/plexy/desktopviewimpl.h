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
#ifndef PLEXYDESK_DESKTOP_VIEW_PLUGIN_IMPL
#define PLEXYDESK_DESKTOP_VIEW_PLUGIN_IMPL

#include <QDesktopWidget>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>

#include <config.h>
#include <plexy.h>

#include <desktopviewplugin.h>
#include <themepackloader.h>
#include <qmldesktopwidget.h>

#include "desktopview.h"
#include "plexydesktopview.h"
#include "plexytray.h"

class DesktopViewPluginImpl: public PlexyDesk::DesktopViewPlugin
{
    Q_OBJECT

public:
    DesktopViewPluginImpl(QObject *object = 0);

    virtual ~DesktopViewPluginImpl();

    QObject *view(QGraphicsScene *scene);

    void setRect (const QRect &rect);

Q_SIGNALS:
    void dataChange();

private:
    QList<PlexyDesktopView *> mViewList;
    PlexyTray *mTray;
};

#endif //PLEXYDESK_DESKTOP_VIEW_PLUGIN_IMPL
