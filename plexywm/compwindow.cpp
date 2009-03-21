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

/*special credit goes to Pyrodesk project, the logic of plexydeskwm is based on
  pyrodesk's compzilla windows manager */

#include "compwindow.h"
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
#include "plexywindows.h"

class CompWindow::Private
{
public:
    Private() {}
    ~Private() {}
    Display* mDisplay;
    Window  mRootWindow;

    Window mMainWin;
    Window mMainwinParent;
    Window mOverlay;

    bool mCompositing;
    bool mManging;
    XAtoms atoms;
    QMap<Window, PlexyWindows*> windowMap;
};

CompWindow::CompWindow(int & argc, char ** argv):QApplication(argc, argv), d(new Private)
{
    d->mDisplay =  QX11Info::display();
    d->mRootWindow = QApplication::desktop()->winId();
    init();
}

CompWindow::~CompWindow()
{
    delete d;
}

void CompWindow::addWindow(Window window)
{
    XWindowAttributes attrs;
    if (!XGetWindowAttributes(d->mDisplay, window, &attrs)) {
        qDebug()<<"Error adding windows, getting window attributes failed"<<endl;
        return;
    }else 
    qDebug()<<"Going -----------------"<<endl;
    PlexyWindows *  _window  = new PlexyWindows(d->mDisplay, window, &attrs);
    d->windowMap[window] = _window;
}

bool CompWindow::x11EventFilter( XEvent* event)
{


     bool nomap = false;
      foreach (PlexyWindows* _win, d->windowMap) {
      if(_win->winId() == event->xmap.window) {
          qDebug()<<"Already has a base"<<endl;
          nomap = true;
      }
      else {
          qDebug()<<"No Base";
      }
    }

    switch (event->type) {
    case ClientMessage:
        qDebug()<<"Client Message"<<
        event->xclient.data.l<<event->xclient.format<<event->xclient.message_type;
        break;
    case MapRequest:
        qDebug()<<"Map Requesti-------->"<<endl;
        //XMapWindow(d->mDisplay, event->xmaprequest.window);
        break;
    case LeaveNotify:
        qDebug()<<"Leave "<<endl;
        break;
    case EnterNotify:
        qDebug()<<"Enter"<<endl;
        break;
    case ReparentNotify:
        qDebug()<<"Reparent"<<endl;
        break;
    case CreateNotify :
        qDebug()<<"Create Notify";
        break;
    case  DestroyNotify:
        qDebug()<<"DestroyNotify";
       break;
    case ConfigureNotify:
        qDebug()<<"ConfigureNotify";
        break;
    case ConfigureRequest:
        qDebug()<<"ConfigureRequest";
        break;
    case MapNotify :
        qDebug()<<"MapNotify";
         if (!nomap)
             addWindow(event->xmap.window);
        break;
    case UnmapNotify :
        qDebug()<<"UnmapNotify";
        break;
    case PropertyNotify :
        qDebug()<<"PropertyNotify";
        break;
    case FocusIn :
        qDebug()<<"FocusIn";
        break;
    case FocusOut:
        qDebug()<<"FocusOut";
        break;
    case Expose:
        qDebug()<<"Expose";
        break;
    }
}

//utility

bool CompWindow::isWmRunning()
{
    Atom wmAtom;
    wmAtom  = XInternAtom(d->mDisplay, "WM_S0",false);
    bool hasWm =  XGetSelectionOwner(d->mDisplay, wmAtom) != None;
    return  hasWm;
}

void CompWindow::init()
{
    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()), KeyPressMask
                 | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask | PointerMotionMask |
                 EnterWindowMask | LeaveWindowMask | FocusChangeMask |
                 VisibilityChangeMask |
                 ExposureMask | StructureNotifyMask |
                 SubstructureRedirectMask | SubstructureNotifyMask);

    XClearWindow(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()));
    XSync(QX11Info::display(), false);

    if (!isWmRunning()) {
        XSetWindowAttributes attrs;
        attrs.override_redirect = True;
        attrs.event_mask = PropertyChangeMask;

        d->mMainWin = XCreateWindow (d->mDisplay,
                                     d->mRootWindow,
                                     -100, -100, 1, 1,
                                     0,
                                     CopyFromParent,
                                     CopyFromParent,
                                     (Visual *)CopyFromParent,
                                     CWOverrideRedirect | CWEventMask,
                                     &attrs);
        Atom wmAtom = XInternAtom(d->mDisplay, "WM_S0", false);
        if (!registerWindowManager(d->mMainWin, wmAtom)) {
            qDebug()<<"Register window failed"<<endl;
        }
        Xutf8SetWMProperties (d->mDisplay, d->mMainWin, "plexydeskwm",
                              "plexydeskwm", NULL, 0, NULL, NULL, NULL);
        Atom cmAtom = XInternAtom(d->mDisplay, "_NET_WM_CM_S0", false);
        if (!registerWindowManager(d->mMainWin, cmAtom)) {
            qDebug()<<"Register Compsite failed"<<endl;
        }
        registerAtoms();
        if (!checkExtensions()) {
            qDebug()<<"Some or all extensions are missing or out dated, upgrade and check again, thanks ";
        }
        //startOverlay();
        setupWindows();

        Cursor normal = XCreateFontCursor(d->mDisplay, XC_left_ptr);
        XDefineCursor(d->mDisplay, d->mRootWindow, normal);

    } else {
        qDebug()<<"Another Window manager already running.. "<<endl;
        qApp->quit();
    }
}

void CompWindow::registerAtoms()
{
    bool result = XInternAtoms (d->mDisplay,
                                atom_names, sizeof (atom_names) / sizeof (atom_names[0]),
                                false,
                                d->atoms.a);
    if (!result)
        qDebug()<<"Registration of atoms:" <<atom_names<<"Failed"<<endl;
}
bool CompWindow::registerWindowManager(Window getOwner, Atom wmAtom)
{
    /*  Atom wmAtom;
      XSetWindowAttributes attrs;
      attrs.override_redirect = True;
      attrs.event_mask = PropertyChangeMask;

      wmAtom = XInternAtom(d->mDisplay, "WM_S0", false);
      */
    Window  owner = XGetSelectionOwner(d->mDisplay, wmAtom);

    if (owner != None)
        XSelectInput (d->mDisplay, owner, StructureNotifyMask);

    XSetSelectionOwner(d->mDisplay, wmAtom, getOwner, CurrentTime);

    if (XGetSelectionOwner (d->mDisplay, wmAtom) != getOwner) {
        qDebug() << "Error registering Window Manager"<<endl;
        return false;
    }

    XClientMessageEvent cm;
    cm.window = d->mRootWindow;
    cm.message_type = XInternAtom(d->mDisplay, "MANAGER", false);
    cm.type =  ClientMessage;
    cm.format = 32;
    cm.data.l[0] = CurrentTime;
    cm.data.l[1] = wmAtom;

    XSendEvent(d->mDisplay, d->mRootWindow, false, StructureNotifyMask, (XEvent*)&cm);
    if (owner != None) {
        XEvent event;
        do {
            XWindowEvent (d->mDisplay, owner, StructureNotifyMask, &event);
            qDebug()<<"Waiting for current owner .."<<endl;
        } while (event.type != DestroyNotify);
    }
    return true;
}


bool CompWindow::registerCompositeManager()
{
    Atom wmAtom;
    XSetWindowAttributes attrs;
    attrs.override_redirect = True;
    attrs.event_mask = PropertyChangeMask;

    wmAtom = XInternAtom(d->mDisplay, "WM_S0", false);
    Window  owner = XGetSelectionOwner(d->mDisplay, wmAtom);
    Window  getOwner  = XCreateWindow(d->mDisplay, d->mRootWindow, -100, -100, 1, 1, 0, CopyFromParent,
                                      CopyFromParent, (Visual*) CopyFromParent, CWOverrideRedirect | CWEventMask, & attrs);

    if (owner != None)
        XSelectInput (d->mDisplay, owner, StructureNotifyMask);

    XSetSelectionOwner(d->mDisplay, wmAtom, getOwner, CurrentTime);

    if (XGetSelectionOwner (d->mDisplay, wmAtom) != getOwner) {
        qDebug() << "Error registering Window Manager"<<endl;
        return false;
    }

    XClientMessageEvent cm;
    cm.window = d->mRootWindow;
    cm.message_type = XInternAtom(d->mDisplay, "MANAGER", false);
    cm.type =  ClientMessage;
    cm.format = 32;
    cm.data.l[0] = CurrentTime;
    cm.data.l[1] = wmAtom;

    XSendEvent(d->mDisplay, d->mRootWindow, false, StructureNotifyMask, (XEvent*)&cm);
    if (owner != None) {
        XEvent event;
        do {
            XWindowEvent (d->mDisplay, owner, StructureNotifyMask, &event);
            qDebug()<<"Waiting for current owner .."<<endl;
        } while (event.type != DestroyNotify);
    }
    return true;
}


bool CompWindow::checkExtensions()
{
    int opcode;
    int composite_event, composite_error, xfixes_event, xfixes_error;
    int shape_event, shape_error, damage_event, damage_error;
    int composite_major, composite_minor;

    if (!XQueryExtension (d->mDisplay, COMPOSITE_NAME, &opcode,
                          &composite_event, &composite_error)) {
        qDebug()<<"missing composite extension\n";
        return false;
    }
    XCompositeQueryVersion (d->mDisplay, &composite_major, &composite_minor);
    if (composite_major == 0 && composite_minor < 2) {
        qDebug()<<"libXcomposite too old, upgrade your package";
        return false;
    }

    if (!XDamageQueryExtension (d->mDisplay, &damage_event, &damage_error)) {
        qDebug()<<"Missing damage extension";
        return false;
    }

    if (!XFixesQueryExtension (d->mDisplay, &xfixes_event, &xfixes_error)) {
        qDebug()<<"Missing XFixes extension";
        return false;
    }
#if HAVE_XSHAPE
    if (!XShapeQueryExtension (d->mDisplay, &shape_event, &shape_error)) {
        qDebug()<<"Missing Shaped window extension";
        return false;
    }
#endif

    return true;
}



bool CompWindow::startOverlay()
{
    d->mOverlay = XCompositeGetOverlayWindow (d->mDisplay, d->mRootWindow);
    if (!d->mOverlay) {
        qDebug()<<"Overly window can not start"<<endl;
    }

    XGCValues vals;
    int x, y;
    unsigned int cx, cy, cx_border, depth;

    if (!XGetGeometry(d->mDisplay, d->mOverlay, &d->mRootWindow, &x, &y, &cx, &cy, &cx_border, &depth)) {
     x = 0;  y =  0;
     cx = 800; cy = 480;
    }

    vals.foreground = BlackPixel(d->mDisplay, 0);
    vals.background = BlackPixel(d->mDisplay, 0);
    GC gc = XCreateGC(d->mDisplay,  d->mOverlay, GCForeground | GCBackground, &vals);
    XFlush(d->mDisplay);
    XReparentWindow (d->mDisplay, d->mMainWin, d->mOverlay, 0, 0);
    XserverRegion region;
    XRectangle rect = { 0, 0, DisplayWidth(d->mDisplay, 0), DisplayHeight(d->mDisplay, 0) };
    region = XFixesCreateRegion(d->mDisplay, &rect, 1);
    XFixesSetWindowShapeRegion(d->mDisplay, d->mOverlay, ShapeBounding, 0, 0, region);
    XFixesDestroyRegion(d->mDisplay, region);

}


void CompWindow::setupWindows()
{
    XGrabServer (d->mDisplay);

    long ev_mask = (SubstructureRedirectMask |
                    SubstructureNotifyMask |
                    StructureNotifyMask |
                    ResizeRedirectMask |
                    PropertyChangeMask
                    |
                    FocusChangeMask);
    XSelectInput (d->mDisplay, d->mRootWindow, ev_mask);


    ev_mask &= ~(SubstructureRedirectMask);
    XSelectInput (d->mDisplay, d->mRootWindow, ev_mask);
    Window root_notused, parent_notused;
    Window *children;
    unsigned int nchildren;

    XQueryTree (d->mDisplay,
                d->mRootWindow,
                &root_notused,
                &parent_notused,
                &children,
                &nchildren);

    for (int i = 0; i < nchildren; i++) {
        if (children[i]
                != d->mMainWin) {
            qDebug()<<"Mapping windows"<<endl;
            addWindow(children[i]);
        }

      //  XFree (children);
    }
    XUngrabServer (d->mDisplay);
}
