/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : PhobosK <phobosk@kbfx.net>
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

#include <QtCore>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QDebug>

#include "plexytray.h"
#include <plexyconfig.h>

static const QString aboutText="<br><b>" + QString(PLEXYNAME)
                            + "</b> <i>v" + QString(PLEXYVERSION)
                            + "</i> (" + QString(PLEXYDATE) + ")"
                            + "<br><b>License:</b> <i>LGPLv3</i>"
                            + "<br><b>Homepage:</b> <a href='http://www.plexyplanet.org'>http://www.plexyplanet.org</a>";
static const QString trayTooltip=QString(PLEXYNAME) + " - Running...";


PlexyTray::PlexyTray(QObject *parent, QIcon &icon) : QSystemTrayIcon(parent)
{
    appnameAction = new QAction(PLEXYNAME, parent);
    QFont itemFont;
    itemFont.setBold(true);
    itemFont.setItalic(true);
    appnameAction->setFont(itemFont);
    appnameAction->setEnabled(false);

    aboutAction = new QAction("&About", parent);
    aboutAction->setIcon(icon);
    connect(aboutAction,
                SIGNAL(triggered()),
                this,
                SLOT(showAbout())
            );

    quitAction = new QAction("&Quit", parent);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(appQuit()));

    trayIconMenu = new QMenu(QString(PLEXYNAME));
    trayIconMenu->addAction(appnameAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(parent);
    trayIcon->setToolTip(trayTooltip);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(icon);

    trayIcon->show();

    connect(trayIcon,
                SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this,
                SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
            );
}

PlexyTray::~PlexyTray()
{
    delete trayIcon;
    delete trayIconMenu;
    delete quitAction;
    delete aboutAction;
}

void PlexyTray::showAbout()
{
    // TODO: Beautify the about box
    QMessageBox aboutBox;
    aboutBox.setIconPixmap((this->icon()).pixmap(64));
    aboutBox.setWindowTitle("About " + QString(PLEXYNAME));
    aboutBox.setText(aboutText);
    aboutBox.setStandardButtons(QMessageBox::Close);
    aboutBox.exec();
}

void PlexyTray::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        trayIconMenu->exec(QCursor::pos());
}

void PlexyTray::appQuit()
{
    qDebug()<< "Tray: Quit requested...";
    QCoreApplication::quit();
}
