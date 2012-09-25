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
    : PlexyDesk::ControllerInterface(object),
      mThemePack(new PlexyDesk::ThemepackLoader("default", this))
{
    // TODO: bug#112
    // read the theme name from settings
    mBackgroundRender = new ClassicBackgroundRender(QRectF(0.0, 0.0, 0.0, 0.0), 0,
                                                    QImage(QDir::toNativeSeparators(PlexyDesk::Config::getInstance()->wallpaper())));
}

BackgroundController::~BackgroundController()
{
    if (mBlurAnimation) {
        delete mBlurAnimation;
    }

    if (mBlurEffect) {
        delete mBlurEffect;
    }

    if (mBackgroundRender)
        delete mBackgroundRender;
}

PlexyDesk::AbstractDesktopWidget *BackgroundController::defaultView()
{
    return mBackgroundRender;

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

void BackgroundController::handleDropEvent(PlexyDesk::AbstractDesktopWidget *widget, QDropEvent *event)
{
    ///TODO create a New Image Browser UI
    //for now just set the pixmap picture

    if ( event->mimeData()->urls().count() >= 0 ) {
        const QString droppedFile = event->mimeData()->urls().value(0).toLocalFile();

        QFileInfo info(droppedFile);

        if ( !info.isDir()) {
            mBackgroundRender->setBackgroundImage(droppedFile);
        }
      }
}

