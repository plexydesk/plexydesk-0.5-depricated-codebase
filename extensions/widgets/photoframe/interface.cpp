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

#include <QtPlugin>

#include "interface.h"
#include "imageplugin.h"
#include "imageplugin.h"

QSharedPointer<PlexyDesk::ControllerInterface> Interface::controller()
{
    QSharedPointer<PlexyDesk::ControllerInterface> obj =
            QSharedPointer<PlexyDesk::ControllerInterface>(new PhotoFrameController(this), &QObject::deleteLater);

    return obj;
}

Q_EXPORT_PLUGIN2(photoframe, Interface)
