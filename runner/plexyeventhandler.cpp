/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : PhobosK <phobosk@kbfx.net>
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

#include <QDebug>
#include <QApplication>

#include "desktopview.h"
#include "plexyeventhandler.h"

#ifdef Q_WS_WIN
// Keep that if we want other native direct manipulations on Windows
//#include <windows.h>
#endif

bool PlexyEventHandler::eventFilter(QObject *obj, QEvent *event)
{
    DesktopView *mainwidget = qobject_cast<DesktopView *>(obj);

/*
    QApplication *qapplication = qobject_cast<QApplication *>(obj);
    // This is for event debugging. Produces a lot of output. Disabled
    if (mainwidget != 0){
           qDebug()<< mainwidget << "...." << event->type();
    } else if (qapplication != 0 ){
           qDebug()<< qapplication << "...." << event->type();
    } else {
           qDebug()<< obj->objectName() << "...." << event->type();
    }
*/

    if (event->type() == QEvent::WindowActivate) {
        if (mainwidget != 0){
#ifdef Q_WS_WIN
            // Windows native direct window manipulation
            ::SetWindowPos( mainwidget->winId(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );
            qDebug()<< "Making DesktopView window bottom...";
#endif
            qDebug()<< "DesktopView: ..." << mainwidget;
        }

        qDebug() << "Activate event dropped...";
        return true;
    }

    if (event->type() == QEvent::WindowStateChange) {
        qDebug("StateChange event dropped...");
        return true;
    }

    if (event->type() == QEvent::Hide) {
        qDebug("Hide event dropped...");
        return true;
    }


    return QObject::eventFilter(obj, event);
}
