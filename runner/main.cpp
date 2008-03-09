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
#include <fakemime.h>

using namespace PlexyDesk;

int main( int argc, char ** argv )
{
    QApplication app(argc,argv);

#ifdef Q_WS_WIN
    QString pluginPath = app.applicationDirPath() + "/plugins/imageformats";
    app.addLibraryPath(pluginPath);
#endif

    qDebug() << PlexyDesk::PluginLoader::getInstance()->listPlugins("Desktop");
    qDebug() << PlexyDesk::PluginLoader::getInstance()->listPlugins("Media");
    qDebug() << PlexyDesk::FakeMime::getInstance()->getType("hello.png")<<endl;
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

    view->show();

    view->addExtension("plexyfriends");
    view->addExtension("imagepil");
    view->addExtension("plexyclock");
    view->addExtension("plexyyoutube");
    view->addExtension("plexycpu");
    view->addExtension("wallpaperchange-win32");
//    view->addData("flickerengine");

    return app.exec();
}
