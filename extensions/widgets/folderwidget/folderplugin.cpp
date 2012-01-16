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

#include "folderplugin.h"
#include <pluginloader.h>
#include <desktopwidget.h>
#include <QDeclarativeContext>

FolderPlugin::FolderPlugin(QObject *object) :
    mFrameParentitem(0)
{
   mThemePack = new PlexyDesk::ThemepackLoader("default", this);
}

FolderPlugin::~FolderPlugin()
{
    if (mThemePack) {
        delete mThemePack;
    }
}

void FolderPlugin::searchImage()
{
}


void FolderPlugin::onDataReady()
{
}

void FolderPlugin::setData(const QVariantMap &data)
{
    qDebug() << Q_FUNC_INFO << data;
      QString dir_path = data["dir_path"].toString();
      if (! dir_path.isEmpty() || ! dir_path.isNull()) {
          mImageSource = dir_path;
          emit dirSourceChanged();
      }
}

QGraphicsItem *FolderPlugin::item()
{
   if (mFrameParentitem == NULL) {
       mFrameParentitem = new PlexyDesk::QmlDesktopWidget(QRectF(0.0, 0.0, 400.0, 400.0));
       QDeclarativeContext *context = mFrameParentitem->engine()->rootContext();
       const QString qmlData = mThemePack->hiddenQmlWidgets(QLatin1String("folderview"));

       qDebug() << Q_FUNC_INFO << qmlData;
       context->setContextProperty("DirSource", this);
       mFrameParentitem->setSourceUrl (QUrl(qmlData));
   }

   return mFrameParentitem;
}
