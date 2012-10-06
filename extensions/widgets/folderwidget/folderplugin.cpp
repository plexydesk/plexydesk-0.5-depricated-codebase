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

#define ADD_DIR "addDirectory"
#define CREATE_DIR "createDirectory"

DirectoryController::DirectoryController(QObject *object) :
    PlexyDesk::ControllerInterface (object)
{
   mThemePack = new PlexyDesk::ThemepackLoader("default", this);

   IconWidgetView *view = new IconWidgetView(QRectF(0.0, 0.0, 480.0, 320.0));

   view->enableDefaultBackground(true);
   view->setDirectoryPath(QDir::homePath() + QLatin1String("/Desktop/"));
   view->setController(this);
   mFolderViewList.append(view);
}

DirectoryController::~DirectoryController()
{
    if (mThemePack) {
        delete mThemePack;
    }
}

PlexyDesk::AbstractDesktopWidget *DirectoryController::defaultView()
{
    return mFolderViewList.at(0);
}

void DirectoryController::revokeSession(const QString &key, const QString &value)
{
}

QStringList DirectoryController::visibleActions() const
{
    QStringList actions;
    actions << ADD_DIR << CREATE_DIR;

    return actions;
}

void DirectoryController::requestAction(const QString &actionName, const QVariantMap &args)
{
    qDebug() << actionName << args;

    if (actionName == CREATE_DIR) {
        qDebug() << "Not supported yet";
    } else if (actionName == ADD_DIR) {
        IconWidgetView *view = new IconWidgetView(QRectF(0.0, 0.0, 480.0, 320.0));

        view->enableDefaultBackground(true);
        view->setDirectoryPath(args["path"].toString());
        view->setController(this);
        mFolderViewList.append(view);
        Q_EMIT spawnView(view);
    }
}

void DirectoryController::handleDropEvent(PlexyDesk::AbstractDesktopWidget *widget, QDropEvent *event)
{
    qDebug() << Q_FUNC_INFO;

    const QString droppedFile = event->mimeData()->urls().value(0).toLocalFile();
    QFileInfo fileInfo (droppedFile);

    if (fileInfo.isDir()) {
        QVariant path = droppedFile;
        QVariantMap args;
        args["path"] = path;
        requestAction(ADD_DIR, args);
    }
}
