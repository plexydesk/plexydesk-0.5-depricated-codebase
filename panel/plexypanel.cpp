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
//    setup();
    test();
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
    qDebug() << Q_FUNC_INFO << this->size();
}

void PlexyPanel::test()
{
    QPushButton *changeLayer = new QPushButton("change Layer");
//	connect(changeLayer,SIGNAL(clicked()),PlexyDesk::Config::getInstance(),SLOT(changeLayer()));
    connect(changeLayer, SIGNAL(clicked()), this, SLOT(slt()));
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(changeLayer);

    QWidget *wd = new QWidget();
    wd->setLayout(layout);
    wd->resize(QDesktopWidget().geometry().width(), 32);
    wd->show();
    qDebug()<<Q_FUNC_INFO;
}

void PlexyPanel::slt()
{
    QDBusMessage busMsg = QDBusMessage::createMethodCall("org.PlexyDesk.Config", "/Configuration", "local.PlexyDesk.Config", "changeLayer");
    QDBusConnection bus = QDBusConnection::connectToBus(QDBusConnection::SessionBus, "PlexyDesk");
    bus.call(busMsg);
}
