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

#include "imageplugin.h"
#include <pluginloader.h>
#include <desktopwidget.h>
#include <QDeclarativeContext>

PhotoFrameController::PhotoFrameController(QObject * /*object*/) :
    mFrameParentitem(0)
{
}

PhotoFrameController::~PhotoFrameController()
{
    if (mFrameParentitem)
        delete mFrameParentitem;
}

PlexyDesk::AbstractDesktopWidget *PhotoFrameController::defaultView()
{
   if (mFrameParentitem == NULL) {
       mFrameParentitem = new PhotoWidget(QRectF(0.0, 0.0, 320.0, 240.0));
       mFrameParentitem->enableDefaultBackground(true);
       mFrameParentitem->setController(this);
   }

   return mFrameParentitem;
}

void PhotoFrameController::revokeSession(const QVariantMap &args)
{
    mFrameParentitem->setContentImage(args["src"].toString());
}

void PhotoFrameController::handleDropEvent(PlexyDesk::AbstractDesktopWidget *widget, QDropEvent *event)
{
    if ( event->mimeData()->urls().count() >= 0 ) {
        const QString droppedFile = event->mimeData()->urls().value(0).toLocalFile();

        QFileInfo info(droppedFile);
        QPixmap droppedPixmap(droppedFile);

        if ( !info.isDir() && !droppedPixmap.isNull() ) {
            mFrameParentitem->setContentImage(droppedPixmap);
            if (viewport()) {
                viewport()->sessionDataForController(controllerName(),"src", droppedFile);
            } else
                qDebug() << Q_FUNC_INFO << "Saving session Failed";
        }
    }
}

void PhotoFrameController::setViewRect(const QRectF &rect)
{
    if (mFrameParentitem)
        mFrameParentitem->setPos(rect.x(), rect.y());
}
