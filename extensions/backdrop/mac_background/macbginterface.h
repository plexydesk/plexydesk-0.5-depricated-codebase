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
#ifndef PLEXY_MAC_BACKDROP_I
#define PLEXY_MAC_BACKDROP_I

#include <plexy.h>
#include <backdropinterface.h>
#include <datainterface.h>
#include <pluginloader.h>
#include <abstractplugininterface.h>


class MacBackgroundPlugin : public PlexyDesk::AbstractPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

public:
    MacBackgroundPlugin(QObject *object = 0);
    virtual ~MacBackgroundPlugin();
    virtual PlexyDesk::AbstractSource *instance();
};

#endif
