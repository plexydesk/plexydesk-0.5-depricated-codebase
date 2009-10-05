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
#ifndef COM_WINDOW_H
#define COM_WINDOW_H

#include <QtCore>
#include <QtGui>

extern "C" {
#include <X11/Xlib.h>
}

class CompWindow : public QApplication
{
    Q_OBJECT
public:
    CompWindow(int & argc, char ** argv);
    ~CompWindow();
    bool isWmRunning();
    void ReplaceSelectionOwner(Window newOwner, Atom atom);
    void addWindow(Window id);
    bool registerWindowManager(Window newOwner, Atom atom);
    bool registerCompositeManager();
    void registerAtoms();
    bool startOverlay();
    void setupWindows();
    bool checkExtensions();
    Window GetEventXWindow (XEvent *xev);
private:
    void destroyNotify(XEvent* event);
    void configureRequest(XEvent* event);
    void mapRequest(XEvent* e);
    void createNotify(XEvent* e);
    void clientMsgNotify(XEvent* e);
    void configureNotify(XEvent* event);
    void init();//setups  Atoms, registering ..etc
    bool x11EventFilter( XEvent* );
    //utility
    //data
    class Private;
    Private * const d;
};

#endif
