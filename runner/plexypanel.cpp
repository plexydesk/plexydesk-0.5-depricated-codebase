#include "plexypanel.h"
#include <QDesktopWidget>
#include <QDebug>

#if defined(Q_WS_X11) // && defined(Q_WS_MAC) ??
#include <X11/Xlib.h>
#include <QX11Info>
#include <netwm.h>
#endif


PlexyPanel::PlexyPanel(QWidget *parent, Qt::WindowFlags wl) : QWidget(parent, wl)
{
    setup();
}

PlexyPanel::~PlexyPanel()
{
}

void PlexyPanel::setup()
{
    move(0, 0);
    resize(QDesktopWidget().geometry().width(), 32);
    setWindowOpacity(0.5);

#if defined(Q_WS_X11) // && defined(Q_WS_MAC) ??
    NETWinInfo info(QX11Info::display(), winId(), QX11Info::appRootWindow(), NET::Dock);
    info.setWindowType(NET::Dock);
    info.setState(winId(), NET::Sticky | NET::StaysOnTop | NET::KeepAbove);
    info.setDesktop(NETWinInfo::OnAllDesktops);
#endif
}
