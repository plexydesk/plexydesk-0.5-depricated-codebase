/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Dariusz Mikulski <dariusz.mikulski@gmail.com>
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

#ifndef PRESENCEINTERFACE_H_
#define PRESENCEINTERFACE_H_

#include <QObject>

#include <datainterface.h>
#include <dataplugin.h>
#include "flicker.h"

class PresenceInterface : public PlexyDesk::DataInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

public:
    PresenceInterface(QObject * object = 0);
    virtual ~PresenceInterface() {}

    /* this will return a valid data plugin pointer*/
    PlexyDesk::DataPlugin * instance();
};


#endif //PRESENCEINTERFACE_H_