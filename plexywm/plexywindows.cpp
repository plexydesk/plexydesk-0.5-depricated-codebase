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

#include "plexywindows.h"
#include <QApplication>

#include "XAtoms.h"

extern "C" {
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

}
#include <QX11Info>

class PlexyWindows::Private
{
public:
    Private() {}
    ~Private() {}
    Damage damage;
    Window window;
    XWindowAttributes * attrib;
    Display * display;
    Pixmap pixmap;
    bool isRedirected;

};

PlexyWindows::PlexyWindows(Display* dsp, Window win, XWindowAttributes* attr, QWidget *parent, Qt::WindowFlags f  )
        :QObject(parent), d(new Private)
{
    XSelectInput (dsp, win, (PropertyChangeMask | EnterWindowMask | FocusChangeMask));
    XShapeSelectInput (dsp, win, ShapeNotifyMask);
    d->window = win;
    d->display = dsp;
    d->pixmap = None;
    d->attrib = attr;
    d->damage = XDamageCreate (dsp, win, XDamageReportRawRectangles);
    d->isRedirected = false;

    if (attr->map_state == IsViewable) {
        attr->map_state == IsUnmapped;
        Mapped(attr->override_redirect);
    }

    qDebug()<<Q_FUNC_INFO<<endl;
}

void PlexyWindows::Destroyed ()
{
}
void PlexyWindows::Mapped (bool override_redirect)
{
    bind();
}

void PlexyWindows::bind()
{
    RedirectWindow();

    if (!d->pixmap) {
        XGrabServer(d->display);
        XGetWindowAttributes(d->display, d->window, d->attrib);
        if (d->attrib->map_state == IsViewable) {
            d->pixmap = XCompositeNameWindowPixmap (d->display, d->window);
            if (d->pixmap == None) {
                qDebug()<<"Bad Pixmap not created"<<endl;
            } else {
                qDebug()<<"Goodpixmap"<<endl;
                QPixmap   pixmap = QPixmap::fromX11Pixmap(d->pixmap);
                QImage img = pixmap.toImage();
                img.save("snap.png");
            }
        }
        XUngrabServer (d->display);
    }
}

void PlexyWindows::RedirectWindow ()
{
    if (d->isRedirected) {
        return;
    }

    XCompositeRedirectWindow (d->display, d->window, CompositeRedirectManual);
    d->isRedirected = true;
    qDebug()<<Q_FUNC_INFO<<endl;
}

void PlexyWindows::Unmapped ()
{
}
void PlexyWindows::PropertyChanged (Atom prop, bool deleted)
{
}

void PlexyWindows::Configured (bool isNotify,int x, int y,int width, int height,int border, PlexyWindows *aboveWin, bool override_redirect)
{
}


