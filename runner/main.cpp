
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

using namespace PlexyDesk;
qDebug()<< "plugin After "<<bg<<endl;
/*WidgetInterface * currentDrop=0;

 QPluginLoader loader ("/usr/local/lib/plexyext/widgets/libvideowidget.so");
 QObject * plugin =  loader.instance();
    if (plugin) {
       currentDrop = qobject_cast<WidgetInterface*>(plugin);
    }else {
        qDebug()<<loader.errorString()<<endl;;
        currentDrop = 0;
    }
*/
WidgetInterface * clock=0;

 QPluginLoader loaderClock ("/usr/local/lib/plexyext/widgets/libplexyclock.so");
 QObject * pluginClock =  loaderClock.instance();
    if (pluginClock) {
       clock = qobject_cast<WidgetInterface*>(pluginClock);
    }else {
        qDebug()<<loaderClock.errorString()<<endl;;
        clock = 0;
    }

WidgetInterface * cpu=0;

 QPluginLoader loaderCpu ("/usr/local/lib/plexyext/widgets/libplexycpu.so");
 QObject * pluginCpu =  loaderCpu.instance();
    if (pluginCpu) {
       cpu = qobject_cast<WidgetInterface*>(pluginCpu);
    }else {
        qDebug()<<loaderCpu.errorString()<<endl;;
        cpu = 0;
    }

WidgetInterface * friends=0;

 QPluginLoader loaderFriends ("/usr/local/lib/plexyext/widgets/libplexyfriends.so");
 QObject * pluginFriends =  loaderFriends.instance();
    if (pluginFriends) {
        friends = qobject_cast<WidgetInterface*>(pluginFriends);
    }else {
        qDebug()<<loaderFriends.errorString()<<endl;;
        friends = 0;
    }

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
//scene.addItem(currentDrop->backdrop());
scene.addItem(clock->backdrop());
scene.addItem(cpu->backdrop());
scene.addItem(friends->backdrop());

widgetfact->instance()->backdrop()->setZValue(-100);
view->show();


return app.exec();

}
