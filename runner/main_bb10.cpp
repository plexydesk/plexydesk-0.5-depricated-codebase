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

#include <config.h>

#include <plexy.h>

#include <plexyconfig.h>
#include <pluginloader.h>
#include <desktopbaseui.h>

#include <debug.h>

#ifdef Q_WS_QPA
#include <bb/cascades/AbstractPane>
#include <bb/system/SystemDialog>

#include "bbmobileui.h"

using namespace bb::cascades;
using ::bb::cascades::Application;
#endif


Q_DECL_EXPORT int main( int argc, char *argv[])
{
    BBMobileUi app(argc, argv);

    PlexyDesk::PluginLoader *loader = 0;
    loader =
        PlexyDesk::PluginLoader::getInstanceWithPrefix(QDir::toNativeSeparators(PlexyDesk::Config::getInstance()->plexydeskBasePath() +
            QLatin1String("/share/plexy/ext/groups/")),
                QDir::toNativeSeparators(PlexyDesk::Config::getInstance()->plexydeskBasePath() +
                    QLatin1String("/lib/plexyext/")));

    return BBMobileUi::exec();
}
