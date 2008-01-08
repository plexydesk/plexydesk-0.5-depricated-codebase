
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


int main( int argc, char ** argv )
{
QApplication app(argc,argv);

QWebView *view = new QWebView(0);
view->load(QUrl("http://www.google.com/"));
view->show();
/*
PlexyDesk::DesktopView * view = new PlexyDesk::DesktopView();
view->show();
*/

return app.exec();

}
