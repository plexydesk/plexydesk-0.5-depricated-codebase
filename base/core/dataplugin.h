
#ifndef PLEXY_DATA_PLUGIN_H
#define PLEXY_DATA_PLUGIN_H

#include <baseplugin.h>
#include <plexy.h>

#include <QVariantMap>

/*!
   \class PlexyDesk::DataPlugin

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

      \code
        utubeEngine = qobject_cast<PlexyDesk::DataPlugin*>(
        PlexyDesk::PluginLoader::getInstance()->instance("utubeengine"));
        if (utubeEngine) {
         connect(utubeEngine, SIGNAL(dataReady()), this, SLOT(onDataReady()));
         }
    \endcode

    \section b How to write your own plugins
     When you need to provide a plugin of you own, simply inhert this and write a class and wrapp
     the class in a DataPlugin interface. Emit dataReady() signal when you data source is ready to
     be used, and provide the data from readAll method. look at the numours examples shiped with
     plexydesk extenstions folder to get started with writting your own.

     \fn PlexyDesk::DataPlugin::readAll()
     \brief Provides the data of the plugin as a QVariantMap
     \returns The data as a Map of QVariant's
     \sa dataReady()

     \fn PlexyDesk::DataPlugin::dataReady()
     \brief Signal emited when data is ready to be read
     \sa DataPlugin::readAll()
 **/
namespace PlexyDesk
{

class PLEXYDESK_EXPORT DataSource : public BasePlugin
{
    Q_OBJECT

public:
    DataSource(QObject *object = 0) {
        Q_UNUSED(object);
    }
    virtual ~DataSource() {
    }
    virtual QVariantMap readAll() = 0;

public Q_SLOTS:
    virtual void pushData(QVariant &) = 0;

Q_SIGNALS:
    void dataReady();
};

}
#endif
