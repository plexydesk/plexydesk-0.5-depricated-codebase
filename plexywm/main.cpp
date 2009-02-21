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

//plexy
#include <plexy.h>
#include <baserender.h>
#include <desktopview.h>
#include <pluginloader.h>
#include <fakemime.h>
#include <datainterface.h>
#include <canvas.h>
#include <plexyconfig.h>

/// internals
#include "compwindow.h"
using namespace PlexyDesk;

int main( int argc, char ** argv )
{
   QApplication app(argc,argv);
   CompWindow win;
   
   if (win.isWmRunning()) 
   {
       qDebug()<<"Window manager Already running"<<endl;
    //   return 0;
   }
   return app.exec();
}
