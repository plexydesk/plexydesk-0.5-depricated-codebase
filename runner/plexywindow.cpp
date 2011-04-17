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

#include "plexywindow.h"
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
    Private() {
    }
    ~Private() {
    }
    Damage damage;
    Window window;
    XWindowAttributes attrib;
    Display *display;
    Pixmap pixmap;
    bool isRedirected;
    QPixmap plexypixmap;
    XWindowChanges changeSet;
};

PlexyWindows::PlexyWindows(Display *dsp, Window win, XWindowAttributes *attr, QWidget *parent, Qt::WindowFlags f  )
    : PlexyDesk::DesktopWidget(QRect(0, 0, 200, 200)), d(new Private)
{
    XSelectInput (dsp, win, (PropertyChangeMask | EnterWindowMask | FocusChangeMask));
    XShapeSelectInput (dsp, win, ShapeNotifyMask);
    d->window = win;
    d->display = dsp;
    d->pixmap = None;
    d->attrib = *attr;
    d->damage = XDamageCreate (dsp, win, XDamageReportNonEmpty);
    d->isRedirected = false;

    if (attr->map_state == IsViewable) {
        attr->map_state == IsUnmapped;
        Mapped(attr->override_redirect);
    }

}

void PlexyWindows::Destroyed ()
{
}

void PlexyWindows::Mapped (bool override_redirect)
{
    qDebug() << Q_FUNC_INFO << override_redirect;
    int num;
    Atom *atoms = XListProperties(d->display, d->window, &num);
    for (int i = 0 ; i < num ; i++) {
        qDebug() << Q_FUNC_INFO << XGetAtomName(d->display, atoms[i]);
        //XGetWindowProperty(d->display, d->window, atoms[i], 32, 32, FALSE
    }
    qDebug() << Q_FUNC_INFO <<  *atoms << num;
    d->attrib.map_state = IsViewable;
    d->attrib.override_redirect = override_redirect;
    bind();
}

void PlexyWindows::bind()
{
    qDebug() <<  Q_FUNC_INFO<<endl;
    RedirectWindow();
    qDebug() <<  Q_FUNC_INFO<< " Try grabbing pixmap " << d->window << endl;
    if (!d->pixmap) {
        XGrabServer(d->display);
        qDebug() <<  Q_FUNC_INFO<< "Grabbing Xserver " << endl;
        XWindowAttributes attr;
        XGetWindowAttributes(d->display, d->window, &d->attrib);
        setRect(d->attrib.x, d->attrib.y, d->attrib.width/5, d->attrib.height/5);
        if (d->attrib.map_state == IsViewable) {
            d->pixmap = XCompositeNameWindowPixmap (d->display, d->window);
            if (d->pixmap == None) {
                qDebug()<<"Bad Pixmap not created"<<endl;
            } else {
                qDebug()<<"Goodpixmap"<<endl;
                d->plexypixmap = QPixmap::fromX11Pixmap(d->pixmap);
            }
        }
        XUngrabServer (d->display);
    }
}

void PlexyWindows::RedirectWindow ()
{
    qDebug() <<  Q_FUNC_INFO<<endl;
    // if (d->isRedirected) {
    //   return;
    //}

    XCompositeRedirectWindow (d->display, d->window, CompositeRedirectAutomatic);
    d->isRedirected = true;
}

void PlexyWindows::Unmapped ()
{
    if (d->attrib.map_state != IsViewable)
        return;

    d->attrib.map_state = IsUnmapped;
    ReleaseWindow ();

}
void PlexyWindows::PropertyChanged (Atom prop, bool deleted)
{
}

void PlexyWindows::Configured (bool isNotify, int x, int y, int width, int height, int border, PlexyWindows *aboveWin, bool override_redirect)
{
    d->attrib.override_redirect = override_redirect;
    if (isNotify) {
        Resized (x, y, width, height, border);
        unsigned changeMask = CWX | CWY | CWWidth| CWHeight;
        XConfigureWindow (d->display, d->window, changeMask, &d->changeSet);
        if (d->attrib.border_width != 0)
            qDebug()<<Q_FUNC_INFO<<"Resize"<<endl;
    }
}

void PlexyWindows::Damaged(XRectangle *rect)
{
    bind();
    if (!rect) {
        XRectangle allrect = { d->attrib.x, d->attrib.y, d->attrib.width, d->attrib.height };
        Damaged (&allrect);
        return;
    }

    if (d->pixmap) {
        d->plexypixmap = QPixmap::fromX11Pixmap(d->pixmap);
        //  update(QRect(rect->x,rect->y,rect->width,rect->height));
        update();
        qDebug()<<Q_FUNC_INFO<<endl;
    }
}

void PlexyWindows::paintViewSide(QPainter *painter, const QRectF &rect)
{
    DesktopWidget::paintViewSide(painter, rect);
    painter->save();
    float width = d->plexypixmap.width() / 5;
    float height = d->plexypixmap.height() / 5;
    painter->drawPixmap(rect.x(), rect.y(), width, height, d->plexypixmap);
    painter->restore();
}

void PlexyWindows::ClientMessaged (Atom type, int format, long *data /*[5]*/)
{
    qDebug()<<Q_FUNC_INFO<<endl;
}


void PlexyWindows::Resized(int x, int y, int width, int height, int border)
{
    qDebug()<<x<<y<<width<<height<<border<<endl;
    qDebug()<<d->attrib.width<<d->attrib.height<<d->attrib.border_width<<endl;

    if (width != d->attrib.width || height != d->attrib.height || border != d->attrib.border_width || d->attrib.override_redirect) {
        qDebug()<<Q_FUNC_INFO<<endl;
        if (d->isRedirected) {
            qDebug()<<Q_FUNC_INFO<<endl;
            ReleaseWindow ();
            d->pixmap = XCompositeNameWindowPixmap (d->display, d->window);
            if (d->pixmap == None) {
                return;
            }

            Damaged(NULL);
        }
    }
    d->attrib.height = height;
    d->attrib.width = width;
    d->attrib.border_width = border;
    d->attrib.x = x;
    d->attrib.y = y;

    d->changeSet.height = height;
    d->changeSet.width = width;
    d->changeSet.border_width = border;
    d->changeSet.x = x;
    d->changeSet.y = y;
    ////setRect(x, y, width, height);
}

void PlexyWindows::ReleaseWindow ()
{
    if (d->pixmap) {
        XFreePixmap(d->display, d->pixmap);
        d->pixmap = None;
        qDebug()<<Q_FUNC_INFO<<endl;
    }
}
