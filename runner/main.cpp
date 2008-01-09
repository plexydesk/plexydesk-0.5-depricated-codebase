
//siraj@kde.org GPL2
//
#include <QtCore>
#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <X11/Xlib.h>
//#include <fixx11h.h>
#include <QtDBus/QtDBus>
#include <QtWebKit/QWebView>

//plexy
#include <plexy.h>
#include <baserender.h>
#include <desktopview.h>
#include <backdropfactory.h>

#include <QX11Info>
#include <netwm.h>

int main( int argc, char ** argv )
{
QApplication app(argc,argv);

/*
QWebView *view = new QWebView(0);
view->load(QUrl("http://www.google.com/ig"));
view->show();
*/

PlexyDesk::BackdropInterface * bg = 0;

PlexyDesk::BackdropFactory * bgfact = new PlexyDesk::BackdropFactory(0);

qDebug()<<"Plugin Before Address"<<bg<<endl;

bg = bgfact->instance(); 

qDebug()<< "plugin After "<<bg<<endl;



using namespace PlexyDesk;
QGraphicsScene scene;

DesktopView * view = new DesktopView(&scene);
scene.addItem(bgfact->instance()->backdrop());

NETWinInfo info( QX11Info::display(), view->winId(), QX11Info::appRootWindow(), NET::WMDesktop );
info.setDesktop( NETWinInfo::OnAllDesktops );
info.setWindowType(NET::Desktop);

view->show();


return app.exec();

}
