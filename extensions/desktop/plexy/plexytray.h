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

#ifndef PLEXYTRAY_H
#define PLEXYTRAY_H

#include "config.h"

#include <QSystemTrayIcon>

#include <plexy.h>


class QMenu;
class QAction;

class PlexyTray : public QSystemTrayIcon
{
    Q_OBJECT

public:
    PlexyTray(QObject *parent, QIcon &icon);
    virtual ~PlexyTray();

private Q_SLOTS:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void appQuit();
    void showAbout();

private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *appnameAction;
    QAction *aboutAction;
    QAction *quitAction;
};

#endif // PLEXYTRAY_H
