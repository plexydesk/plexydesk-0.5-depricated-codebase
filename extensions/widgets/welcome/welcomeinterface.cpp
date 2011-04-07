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
#include "welcomeinterface.h"
#include "plugin.h"
#include  <QtCore>
#include  <QtGui>


WelcomeInterface::WelcomeInterface(QObject *object)
{
}

PlexyDesk::BasePlugin *WelcomeInterface::instance()
{
// return new Clock(this);
    return new Welcome(this);
}

Q_EXPORT_PLUGIN2(WelcomeInterface, WelcomeInterface)
#include "welcomeinterface.moc"
