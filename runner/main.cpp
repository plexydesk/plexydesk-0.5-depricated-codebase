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

//siraj@kde.org GPL2
//
#include <QtCore>
#include <QtGui>

#ifdef Q_WS_X11
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>
#include <X11/Xlib.h>
#include <QX11Info>
#include <netwm.h>
#endif

#include <QtWebKit/QWebView>

//plexy
#include <plexy.h>
#include <baserender.h>
#include <desktopview.h>
#include <backdropfactory.h>
#include <widgetfactory.h>
#include <pluginloader.h>

using namespace PlexyDesk;

int main( int argc, char ** argv )
{
    QApplication app(argc,argv);
/*
    QDir d(QDir::homePath());
    QDirModel *model = new QDirModel;
    model->index(QDir::homePath());
    //qDebug()<<model->mimeTypes()<<endl;
    for(int i = 0 ; i < model->rawCount();i++)
    {
        qDebug()<<model->mimeType(i)<<endl;
    }
  */  qDebug() << PlexyDesk::PluginLoader::getInstance()->listPlugins("Desktop");
    qDebug() << PlexyDesk::PluginLoader::getInstance()->listPlugins("Media");

    QGraphicsScene scene;

    scene.setSceneRect(QDesktopWidget().availableGeometry());
    scene.setBackgroundBrush(Qt::NoBrush);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    DesktopView *view = new DesktopView(&scene);
    view->resize(QDesktopWidget().availableGeometry().size());
    //view->setWindowOpacity(0.1);

#ifdef Q_WS_X11
    NETWinInfo info(QX11Info::display(), view->winId(), QX11Info::appRootWindow(), NET::WMDesktop );
    info.setDesktop(NETWinInfo::OnAllDesktops);
    info.setWindowType(NET::Desktop);
#endif

//    view->addExtension("plexyfriends");
//    view->addExtension("plexyclock");
//    view->addData("flickerengine");
    view->show();

    return app.exec();
}
