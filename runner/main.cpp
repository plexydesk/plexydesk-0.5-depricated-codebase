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

#include <config.h>

#include <QtCore>
#include <QtGui>
#include <QDeclarativeComponent>
#include <QIcon>

#include <desktopbaseui.h>
#include <plexy.h>
// We do not use this for now. Commented.
//#include "plexyeventhandler.h"
#include <plexyconfig.h>
#include <baserender.h>
#include <pluginloader.h>
#include <datainterface.h>
#include <debug.h>


#ifdef Q_WS_X11
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <X11/Xlib.h>
#include <QX11Info>

#include <netwm.h>
#endif


int main( int argc, char * *argv )
{
#ifndef Q_WS_MAC
    QApplication::setGraphicsSystem(QLatin1String("raster"));
#endif
    QByteArray debug_settings = qgetenv("PLEXYDESK_CONSOLE_DEBUG").toLower();

    if (debug_settings != "enable" && debug_settings != "true" && debug_settings != "1" ) {
       qInstallMsgHandler(plexyWindowsLogger);
    }

    QApplication app(argc, argv);

    QString appIconPath = PlexyDesk::Config::getInstance()->plexydeskBasePath() + 
        "/share/plexy/plexydesk.png";
    QIcon appIcon = QIcon(QDir::toNativeSeparators(appIconPath));
    app.setWindowIcon(appIcon);
    app.setApplicationName(QString(PLEXYNAME));

#ifdef Q_WS_WIN
    QString pluginPath = PlexyDesk::Config::getInstance()->plexydeskBasePath() 
        + "/lib/qt4/plugins/imageformats";
    app.addLibraryPath(QDir::toNativeSeparators(pluginPath));
#endif

#ifdef Q_WS_MAC
    PlexyDesk::Config::getInstance()->setOpenGL(true);
#endif
    PlexyDesk::PluginLoader *loader = PlexyDesk::PluginLoader::getInstance();
    loader->scanDisk();
    // Set this to false if you need a "close to tray" functionality when systray exists
    QApplication::setQuitOnLastWindowClosed(true);

    DesktopBaseUi * ui = new DesktopBaseUi();

    return app.exec();
}
