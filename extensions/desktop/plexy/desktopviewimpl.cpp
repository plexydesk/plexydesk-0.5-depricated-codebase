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

#include <QPixmapCache>
#include <QtDebug>
#include <QDir>
#include <QDeclarativeContext>

#include "desktopviewimpl.h"
#include "plexydesktopview.h"
#include <desktopwidget.h>
#include <plexyconfig.h>

DesktopViewPluginImpl::DesktopViewPluginImpl(QObject *object)
    : DesktopViewPlugin(object), mTray (0)
{
}

DesktopViewPluginImpl::~DesktopViewPluginImpl()
{
    qDeleteAll(mViewList.begin(), mViewList.end());
    mViewList.clear();

    if (mTray) {
        delete mTray;
    }
}

void  DesktopViewPluginImpl::setRect(const QRect &rect)
{
}

void DesktopViewPluginImpl::layout(int screen)
{
}

QObject  *DesktopViewPluginImpl::view(QGraphicsScene *scene)
{
    PlexyDesktopView *view = new PlexyDesktopView(scene);
    //view->addWallpaperItem();
    //view->setThemePack(PlexyDesk::Config::getInstance()->themepackName());
    //view->registerPhotoDialog();
    mViewList.append(view);

#ifdef Q_WS_WIN
    /* system tray */
       if (!mTray) {
           QString appIconPath =
               PlexyDesk::Config::getInstance()->plexydeskBasePath()
               + "/share/plexy/plexydesk.png";
           QIcon icon (QDir::toNativeSeparators(appIconPath));
           mTray = new PlexyTray(view->window(), icon);
    }
#endif
       return view;
}
