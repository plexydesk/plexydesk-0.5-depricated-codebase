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
#include <QtGui>


Welcome::Welcome(QObject * object)
{
    frm = new Frame(QRect(0,0,140 * 4 ,128*3));
    icon1 = new WellcomeItem(QRect(0,0,128,256),frm);
    icon1->setName("");
    icon1->setIcon(QPixmap(applicationDirPath() + "/share/plexy/skins/default/welcome/kfm_home.png"));
    icon1->setPos(60,70);

    icon2 = new WellcomeItem(QRect(0,0,128,256),frm);
    icon2->setName("");
    icon2->setIcon(QPixmap(applicationDirPath() + "/share/plexy/skins/default/welcome/colors.png"));
    icon2->setPos(220,70);

    icon3 = new WellcomeItem(QRect(0,0,128,256),frm);
    icon3->setName("");
    icon3->setIcon(QPixmap(applicationDirPath() + "/share/plexy/skins/default/welcome/gnome_apps.png"));
    icon3->setPos(380,70);


}

Welcome::~Welcome()
{

}

QGraphicsItem * Welcome::item()
{
   return  frm; 
}

#include "plugin.moc"
