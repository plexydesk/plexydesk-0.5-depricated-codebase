#include "compwindow.h"

extern "C" {
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xcomposite.h>
}
#include <QX11Info>

class CompWindow::Private
{
    public:
        Private(){}
        ~Private(){}
        Display* mDisplay;
        Window  mRootWindow;

        Window mMainWin;
        Window mMainwinParent;
        Window mOverlay;

        bool mCompositing;
        bool mManging;

};

CompWindow::CompWindow():d(new Private)
{
    d->mDisplay =  QX11Info::display();
    d->mRootWindow = QX11Info::appRootWindow();
    //register
    if (!isWmRunning()) {
        registerWindowManager();
    } else  {
        qDebug()<<"Window manager already running.."<<endl;
    }
}

CompWindow::~CompWindow()
{
    delete d;
}

//utility

bool CompWindow::isWmRunning()
{
   Atom wmAtom;
   wmAtom  = XInternAtom(d->mDisplay, "WM_S0",false);
   bool hasWm =  XGetSelectionOwner(d->mDisplay, wmAtom) != None;
   return  hasWm;
}

bool CompWindow::registerWindowManager()
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

