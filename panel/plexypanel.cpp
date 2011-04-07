#include "plexypanel.h"
#include <QPushButton>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QDebug>
#include <plexyconfig.h>

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
    // setup the button : only a temp button for testing

    QPushButton *changeLayer = new QPushButton(this);
    connect(changeLayer, SIGNAL(clicked()), this, SLOT(slt()));
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(changeLayer);
    this->setLayout(layout);
}

void PlexyPanel::switchLayer()
{
    QDBusMessage busMsg = QDBusMessage::createMethodCall("org.PlexyDesk.Config", "/Configuration", "local.PlexyDesk.Config", "changeLayer");
    QDBusConnection bus = QDBusConnection::connectToBus(QDBusConnection::SessionBus, "PlexyDesk");
    bus.call(busMsg);
}
