/***************************************************************************
 *   Copyright (C) 2008 by Lahiru Lakmal Priyadarshana                     *
 *   llahiru@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef PLEXY_WIDGET_FRIENDS_H
#define PLEXY_WIDGET_FRIENDS_H

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>


class VISIBLE_SYM Friends : public QObject , public PlexyDesk::WidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::WidgetInterface)

    public:
    Friends(QObject * object = 0);
    virtual ~Friends();
    virtual QGraphicsItem * backdrop();// {};
};


#endif
