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
#include "plugin.h"
#include <plexy.h>
#include <config.h>
#include <QtCore>
#include<QtGui>


Welcome::Welcome(QObject * object)
{
    icon1 = new WellcomeItem(QRect(0,0,128,256));
    icon1->setName("");
    icon1->setIcon(QPixmap(QString(PLEXPREFIX)+"/share/plexy/skins/default/welcome/keditbookmarks.png"));
}

Welcome::~Welcome()
{

}

QGraphicsItem * Welcome::item()
{
   return  icon1; 
}

#include "plugin.moc"
