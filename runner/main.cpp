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
#include <debug.h>

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
#include "desktopview.h"
#include <pluginloader.h>
#include <datainterface.h>
#include <canvas.h>
#include <plexyconfig.h>

using namespace PlexyDesk;

int main( int argc, char * *argv )
{
#ifndef Q_WS_MAC
    QApplication::setGraphicsSystem(QLatin1String("raster"));
#elif Q_WS_WIN
    qInstallMsgHandler(plexyWindowsLogger);
#endif
    QApplication app(argc, argv);

#ifdef Q_WS_WIN
    QString pluginPath = QString(PLEXPREFIX) + "/plugins/imageformats";
    app.addLibraryPath(pluginPath);
#endif

    QGraphicsScene scene;
    scene.setBackgroundBrush(Qt::NoBrush);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    QSharedPointer<DesktopView> view = QSharedPointer<DesktopView>(new DesktopView(0));
    bool accel = PlexyDesk::Config::getInstance()->openGL;
    QSize desktopSize;
#ifdef Q_WS_MAC
    accel = true;
    desktopSize = QDesktopWidget().screenGeometry().size();
#else
    accel = PlexyDesk::Config::getInstance()->openGL;
    desktopSize = QDesktopWidget().availableGeometry().size();
#endif

    view->enableOpenGL(accel);

    view->setScene(&scene);

    QObject::connect(view.data(), SIGNAL(closeApplication()), &app, SLOT(quit()));
    QRect r = QDesktopWidget().availableGeometry();
    view->move(r.x(), r.y());
    view->resize(desktopSize);
    scene.setSceneRect(QDesktopWidget().availableGeometry()); //TODO Resolution changes ?
    view->setSceneRect(QDesktopWidget().availableGeometry());
    view->ensureVisible(QDesktopWidget().availableGeometry());
    view->setDragMode(QGraphicsView::RubberBandDrag);

#ifdef Q_WS_WIN
    /// \brief: remove plexy from taskbar
    view->move(0, 0);
#endif

#ifdef Q_WS_X11
    NETWinInfo info(QX11Info::display(), view->winId(), QX11Info::appRootWindow(), NET::WMDesktop );
    info.setDesktop(NETWinInfo::OnAllDesktops);
    info.setWindowType(NET::Desktop);
#endif

    PlexyDesk::PluginLoader *loader = PlexyDesk::PluginLoader::getInstance();
    loader->scanDisk();
    view->show();

    /* load all the widgets to a Widget Browser Layer */
    Q_FOREACH(const QString &pluginName, loader->listPlugins(QLatin1String("Widget"))) {
        qDebug() << "Lading Plugins to Hidden Browser Layer";
        view->addExtension(pluginName, QLatin1String("Browser"));
    }

    view->setThemePack(PlexyDesk::Config::getInstance()->themepackName);
    view->showLayer(QLatin1String("Widgets"));

    return app.exec();
}
