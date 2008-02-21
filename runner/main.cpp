
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

    qDebug() << PlexyDesk::PluginLoader::getInstance()->listPlugins("Desktop");
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

    view->addExtension("plexyfriends");
    view->addExtension("plexyclock");
    view->addData("flickerengine");
    view->show();

    return app.exec();
}
