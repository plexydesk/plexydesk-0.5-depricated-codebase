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
//LGPL siraj@kde.org Siraj Razick

#ifndef PLEXY_EXTENION_FACTORY_H
#define PLEXY_EXTENION_FACTORY_H
#include <backdropinterface.h>
#include <pluginloader.h>
#include <widgetinterface.h>
#include <plexy.h>


namespace PlexyDesk
{

template <class T>
class VISIBLE_SYM ExtensionProducer
{
    public:
         T * instance(const QString& name,QObject *obj)
         {
             return  qobject_cast<T*> (obj);
         }
};

} // namespace PlexyDesk

#endif
