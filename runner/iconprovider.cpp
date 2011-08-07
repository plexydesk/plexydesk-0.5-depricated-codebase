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

#include <QPixmap>
#include <QQueue>
#include <QSettings>
#include <QString>

#include "iconprovider.h"
#include <plexyconfig.h>


namespace PlexyDesk
{
class IconProvider::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    QStringList iconpaths;

};

IconProvider::IconProvider() : d(new Private)
{
}

IconJobPtr IconProvider::requestIcon(const QString &name, const QString &size)
{
    QSharedPointer<IconJob> obj =
         QSharedPointer<IconJob>(new IconJob(this), &QObject::deleteLater);
    obj->requestIcon(name, size);
    return obj;
}

}
