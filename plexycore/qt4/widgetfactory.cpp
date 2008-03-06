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
#include "widgetfactory.h"

namespace PlexyDesk
{

class WidgetFactory::Private
{
    public:
        Private() {}
        ~Private() {}
        WidgetInterface * currentDrop;
};


WidgetFactory::WidgetFactory(QObject * parent):AbstractPluginInterface(parent),d(new Private)
{
   QString ext(".so");
#ifdef Q_WS_MAC
    QPluginLoader loader (QString(PLEXPREFIX) +"/lib/plexyext/widgets/libwidgetdemo.dylib");
#endif

#ifdef Q_WS_X11
    QPluginLoader loader (QString(PLEXPREFIX) +"/lib/plexyext/widgets/libwidgetdemo.so");
#endif

#ifdef Q_WS_WIN32
    QPluginLoader loader (QString(PLEXPREFIX) +"\\lib\\plexyext\\widgets\\libwidgetdemo.dll");
#endif



    QObject * plugin =  loader.instance();
    if (plugin) {
        d->currentDrop = qobject_cast<WidgetInterface*>(plugin);
    }else {
        qDebug()<<loader.errorString()<<endl;;
        d->currentDrop = 0;
    }
}

WidgetFactory::~WidgetFactory()
{
    delete d;
}

WidgetInterface * WidgetFactory::instance ()
{
return d->currentDrop ;
}

} // namespace PlexyDesk


#include <widgetfactory.moc>
