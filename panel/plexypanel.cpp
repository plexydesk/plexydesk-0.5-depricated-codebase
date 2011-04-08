#include "plexypanel.h"
#include <QDesktopWidget>
#include <QDebug>

//includes needed for testing
#include <QPushButton>
#include <QVBoxLayout>
#include <QDBusConnection>
#include <QDBusMessage>

#include <X11/Xlib.h>
#include <QX11Info>
#include <netwm.h>


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
    NETWinInfo info(QX11Info::display(), winId(), QX11Info::appRootWindow(), NET::Dock);
    info.setWindowType(NET::Dock);
    info.setState(winId(), NET::Sticky | NET::StaysOnTop | NET::KeepAbove);
    info.setDesktop(NETWinInfo::OnAllDesktops);
    setWindowOpacity(0.5);

    /*setup test button Only for testing.. :)*/
    QPushButton *but = new QPushButton("ChangeLayer");
    connect(but, SIGNAL(clicked()), this, SLOT(switchLayer()));
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(but);
    this->setLayout(layout);
}

void PlexyPanel::switchLayer()
{
    QDBusMessage busMsg = QDBusMessage::createMethodCall("org.PlexyDesk.Config","/Configuration","local.PlexyDesk.Config","changeLayer");
    QDBusConnection bus = QDBusConnection::connectToBus(QDBusConnection::SessionBus,"PlexyDesk");
    bus.call(busMsg);
}
