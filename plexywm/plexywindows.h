/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef PLEXYWINDOWS_H
#define PLEXYWINDOWS_H

#include <QtCore>
#include <QtGui>
#include <QWidget>

extern "C" {
#include <X11/Xlib.h>
#include <X11/extensions/Xdamage.h>
}

class PlexyWindows : public QObject
{
    Q_OBJECT
public:
    PlexyWindows(Display*d, Window w, XWindowAttributes *attr, QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~PlexyWindows() {}

    void Destroyed ();
    void Mapped (bool override_redirect);
    void Unmapped ();
    void RedirectWindow ();
    void bind();
    void PropertyChanged (Atom prop, bool deleted);
    void Configured (bool isNotify,
                     int x, int y,
                     int width, int height,
                     int border,
                     PlexyWindows *aboveWin,
                     bool override_redirect);
    void Damaged(XRectangle *rect);
private:
    class Private;
    Private * const d;
};

#endif // PLEXYWINDOWS_H
