/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal <llahiru@gmail.com>
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

#include "authplugin.h"
#include <qwebviewitem.h>

AuthPlugin::AuthPlugin(QObject *object)
{
     mWidget = new PlexyDesk::AuthWidget(QRectF(0, 0, 480, 320));
     mWidget->setController(this);
}

AuthPlugin::~AuthPlugin()
{
    if (mWidget)
        delete mWidget;
}

PlexyDesk::AbstractDesktopWidget *AuthPlugin::defaultView()
{
    return mWidget;
}

void AuthPlugin::revokeSession(const QVariantMap &args)
{
    if(mWidget)
        mWidget->revokeSession();
}

void AuthPlugin::setViewRect(const QRectF &rect)
{
    if (mWidget)
        mWidget->setPos(rect.x(), rect.y());
}
