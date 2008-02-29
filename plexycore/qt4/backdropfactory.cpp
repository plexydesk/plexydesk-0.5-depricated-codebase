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

#include <QtCore>
#include <QtGui>
#include <config.h>
#include <abstractplugininterface.h>
#include "backdropfactory.h"

namespace PlexyDesk
{

class BackdropFactory::Private
{
    public:
        Private() {}
        ~Private() {}
       AbstractPluginInterface  * currentDrop;
};


BackdropFactory::BackdropFactory(QObject * parent):QObject(parent),d(new Private)
{

   
   
#ifdef Q_WS_MAC
    QPluginLoader loader (QString(PLEXPREFIX) +"/lib/plexyext/libclassicbackdrop.dylib");
#endif    

#ifdef Q_WS_X11
    QPluginLoader loader (QString(PLEXPREFIX)+"/lib/plexyext/libclassicbackdrop.so");
#endif 

#ifdef Q_WS_WIN32
    QPluginLoader loader (QString(PLEXPREFIX) +"\\lib\\plexyext\\libclassicbackdrop.dll");
#endif 


    QObject * plugin =  loader.instance();
    
    if (plugin) {
        d->currentDrop = qobject_cast<AbstractPluginInterface*>(plugin);
    }else {
        qDebug()<<loader.errorString()<<endl;;
        d->currentDrop = 0;
    }
   
}

BackdropFactory::~BackdropFactory()
{
    delete d;
}

AbstractPluginInterface * BackdropFactory::instance ()
{
return d->currentDrop ;
}

} // namespace PlexyDesk


#include "backdropfactory.moc"
