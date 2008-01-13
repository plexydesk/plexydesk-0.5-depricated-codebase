
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
#include <widgetfactory.h>

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
PlexyDesk::WidgetInterface * widget = 0;

PlexyDesk::BackdropFactory * bgfact = new PlexyDesk::BackdropFactory(0);
PlexyDesk::WidgetFactory * widgetfact = new PlexyDesk::WidgetFactory(0);

qDebug()<<"Plugin Before Address"<<bg<<endl;

bg = bgfact->instance(); 

qDebug()<< "plugin After "<<bg<<endl;



using namespace PlexyDesk;
QGraphicsScene scene;
scene.setSceneRect(QDesktopWidget().availableGeometry());
scene.setBackgroundBrush(Qt::NoBrush);
scene.setItemIndexMethod(QGraphicsScene::NoIndex);
QPushButton * btn= new QPushButton("ClickME");;
DesktopView * view = new DesktopView(&scene);
view->resize( QDesktopWidget().availableGeometry().size());
NETWinInfo info( QX11Info::display(), view->winId(), QX11Info::appRootWindow(), NET::WMDesktop );
info.setDesktop( NETWinInfo::OnAllDesktops );
info.setWindowType(NET::Desktop);

scene.addItem(bgfact->instance()->backdrop());
scene.addItem(widgetfact->instance()->backdrop());
widgetfact->instance()->backdrop()->setZValue(-100);
view->show();


return app.exec();

}
