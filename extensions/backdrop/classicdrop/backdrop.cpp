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

#include <QDir>
#include <QPixmapCache>
#include <QDeclarativeContext>

#include <QtDebug>

#include "backdrop.h"
#include <desktopwidget.h>
#include <plexyconfig.h>

BackgroundController::BackgroundController(QObject *object)
    : PlexyDesk::ViewControllerPlugin(object),
      mQtDesktopWidget(new QDesktopWidget),
      mThemePack(new PlexyDesk::ThemepackLoader("default", this)),
      mBackgroundItem(0)
{
    // TODO: bug#112
    // read the theme name from settings

    mBackgroundPixmap =
      new QPixmap(QDir::toNativeSeparators(PlexyDesk::Config::getInstance()->wallpaper()));

    mDesktopScreenRect = mQtDesktopWidget->screenGeometry(0);
#ifdef Q_WS_WIN
    // A 1px hack to make the widget fullscreen and not covering the toolbar on Win
    mDesktopScreenRect.setHeight(desktopScreenRect.height()-1);
#endif
}

BackgroundController::~BackgroundController()
{
    if (mBackgroundItem) {
        delete mBackgroundItem;
    }

    if (mBackgroundPixmap) {
        delete mBackgroundPixmap;
    }

    if (mBlurAnimation) {
        delete mBlurAnimation;
    }

    if (mBlurEffect) {
        delete mBlurEffect;
    }

    if (mQtDesktopWidget)
        delete mQtDesktopWidget;
}

QGraphicsItem *BackgroundController::view()
{
    if (mBackgroundItem == NULL) {
        QSize desktopSize;

        desktopSize = mDesktopScreenRect.size();
#ifdef Q_WS_WIN
        // A 1px hack to make the widget fullscreen and not covering the toolbar on Win
        desktopSize.setHeight(desktopSize.height()-1);
#endif
        mBackgroundCache = mBackgroundPixmap->scaled(desktopSize.width(), desktopSize.height(),
               Qt::IgnoreAspectRatio,
               Qt::SmoothTransformation);

        connect(PlexyDesk::Config::getInstance(), SIGNAL(wallpaperChanged()),
                this, SLOT(changeWallpaperItem()));

        mBackgroundItemPixmap = new QGraphicsPixmapItem(mBackgroundCache);
        mBackgroundItemPixmap->setCacheMode(QGraphicsItem::NoCache);

        delete mBackgroundPixmap;
        mBackgroundPixmap = 0;
        return mBackgroundItemPixmap;
    }
    
    return NULL;
}

QStringList BackgroundController::visibleActions() const
{
    QStringList actions;
    actions << QLatin1String ("Change Background");

    return actions;
}

void BackgroundController::requestAction(const QString &actionName, const QVariantMap &args)
{
    if (actionName == "Change Background") {
        //TODO change background
    }
}

void BackgroundController::handleDropEvent(QDropEvent *event)
{
    ///TODO create a New Image Browser UI
    //for now just set the pixmap picture

    if ( event->mimeData()->urls().count() >= 0 ) {
        const QString droppedFile = event->mimeData()->urls().value(0).toLocalFile();

        QFileInfo info(droppedFile);

        if ( !info.isDir() && !QPixmap(droppedFile).isNull() ) {
            if (mBackgroundPixmap)
                delete mBackgroundPixmap;

            QSize desktopSize = mDesktopScreenRect.size();
#ifdef Q_WS_WIN
            // A 1px hack to make the widget fullscreen and not covering the toolbar on Win
            desktopSize.setHeight(desktopSize.height() - 1);
#endif
            mBackgroundItemPixmap->setCacheMode(QGraphicsItem::NoCache);
            mBackgroundPixmap = new QPixmap(desktopSize.width(), desktopSize.height());
            mBackgroundPixmap->load(droppedFile);
            mBackgroundItemPixmap->setPixmap(mBackgroundPixmap->scaled(desktopSize.width(), desktopSize.height(),
                                                                       Qt::IgnoreAspectRatio,
                                                                       Qt::SmoothTransformation));

            delete mBackgroundPixmap;
            mBackgroundPixmap = 0;
        }
      }
}

