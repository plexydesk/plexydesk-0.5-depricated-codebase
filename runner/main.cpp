
//siraj@kde.org GPL2
//
#include <QtCore>
#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <fixx11h.h>
#include <QtDBus/QtDBus>
#include <kdeversion.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <kaboutdata.h>
#include <kconfig.h>


//plexy
#include <plexy.h>
#include <baserender.h>

static const char description[] =
        I18N_NOOP("Kandy Panel For KDE");

int main( int argc, char ** argv )
{
QApplication app(argc,argv);

PlexyDesk::BaseRender * r = new PlexyDesk::BaseRender(0);
r->resize(640,480);
r->show();


return app.exec();

}
