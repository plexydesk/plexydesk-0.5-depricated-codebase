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

#ifndef PLEXY_DATA_PLUGIN_H
#define PLEXY_DATA_PLUGIN_H

#include <plexy.h>
#include <QObject>
#include <QVariantMap>
#include <QStringList>

/*!
   \class PlexyDesk::DataSource

   \brief Class which abstracts a data source

    \section a PlexyDesk Data Plugin
      PlexyDesk data plugins are meant to abstract data source, such as data comming from
      flicker, a website or even your own local disk. The plugin would emit dataReady() signal
      when the data has been collected or if a data item is ready. once it's ready the client
      can read the abstracted data from readAll() function which returns a QVariantMap of data.

      When you are required to pass data to the plugin you should use pushData method provided by
      the plugin, this can be used for telling the plugin which youtube feeds you are interested in,
      or telling it which images to display from flicker. Code snippets below illustrates how
      plugins can be used from a client application.

    @verbatim
        youtubeSource = qobject_cast<PlexyDesk::DataSource*>(
        PlexyDesk::PluginLoader::getInstance()->instance("utubeengine"));

        if (youtubeSource) {
            connect(youtubeSource, SIGNAL(ready()), this, SLOT(onReady()));
        }
    @endverbatim

    \section b How to write your own plugins
     When you need to provide a plugin of you own, simply inhert this and write a class and wrapp
     the class in a DataSource interface. Emit dataReady() signal when you data source is ready to
     be used, and provide the data from readAll method. look at the numours examples shiped with
     plexydesk extenstions folder to get started with writting your own.

     \fn PlexyDesk::DataSource::readAll()
     \brief Provides the data of the plugin as a QVariantMap
     \returns The data as a Map of QVariant's
     \sa dataReady()

     \fn PlexyDesk::DataSource::ready()
     \brief Signal emited when data is ready to be read
     \sa DataSource::readAll()

     \fn PlexyDesk::DataSource::setArguments();
     \brief Arguments to be suppied for the Data Source

     \paragraph This method essentially provides a way to send Data or arguments to the
     Data source. For instance, if the data source handles a web services which
     requries a user name and a password, the delegate can use this method to
     send the required paramets to the data source so that it can begin it's
     operation.

     \param args The argument to be passed to the data source, The data source should
     define the protocol to be used.
 **/
namespace PlexyDesk
{

class PLEXYDESKCORE_EXPORT DataSource : public QObject
{
    Q_OBJECT

public:
    DataSource(QObject *object = 0) : QObject(object){
    }

    virtual ~DataSource() {
    }

    virtual QVariantMap readAll() = 0;

public Q_SLOTS:
    virtual void setArguments(QVariant &args) = 0;

Q_SIGNALS:
    void ready();

    void sourceUpdated(const QVariantMap &sourceUpdated);
};

}
#endif
