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


#ifndef WEATHERINTERFACE_H
#define WEATHERINTERFACE_H

#include <QObject>
#include <QtPlugin>
#include <httpjobhandler.h>
class WeatherInterface
{

public:
    virtual ~WeatherInterface() {}
    virtual QVariantMap getweather()=0;
    virtual QVariantMap data(const QString &methodName, QVariantMap args)=0;
    virtual QStringList supportedMethods()=0;

};




#endif
