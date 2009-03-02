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

class CompWindow : public QWidget
{
    Q_OBJECT
    public:
        CompWindow();
        ~CompWindow();

        bool isWmRunning();

        void addWindow(Window id);
        bool registerWindowManager(Window newOwner, Atom atom);
	bool registerCompositeManager();
	void registerAtoms();
	bool startOverlay();
	bool checkExtensions();
    private:
        void init();//setups  Atoms, registering ..etc
        //utility 
        //data
        class Private;
        Private * const d;
};

#endif