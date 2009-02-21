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
}

CompWindow::~CompWindow()
{
    delete d;
}
