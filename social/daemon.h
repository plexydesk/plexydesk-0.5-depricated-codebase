/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Sri Lanka Institute of Information Technology
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
#include <QApplication>
#include <QDBusAbstractAdaptor>
#include <QObject>
#include <QDBusVariant>
#include <QList>
#include <QStringList>
#include <QVariantMap>
#include <socialplugin.h>
//#include <SocialPlugin.h>  <-----**********To be implemented*****************

namespace PlexyDesk
{
    class SocialDaemon : QDBusAbstractAdaptor
    {
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", "local.socioplexy.Configuration")
        //Q_PROPERTY(QString display READ display)

    private:
        SocialPlugin* getPluginInstance(const QString &pluginName);
    public:
        SocialDaemon(QApplication *parent);
    public slots:
        QStringList getPluginsList();
        QStringList supportedMethods(const QString &pluginName);
        QVariantMap data(const QString &pluginName, const QString &methodName, QVariantMap args);


    };
} //namespace PlexyDesk
