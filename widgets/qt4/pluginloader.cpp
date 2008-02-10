#include <QtCore>
#include <QtGui>
#include <config.h>
#include "pluginloader.h"
#include "extensionfactory.h"

namespace PlexyDesk
{

  class PluginLoader::Private
  {
  public:
    Private ()
    {
    }
     ~Private ()
    {
    }
    Interface groups;
    QString prefix;
  };

PluginLoader::PluginLoader ():d (new Private)
  {
    d->prefix = QString (PLEXPREFIX) + "/ext/groups/";
    scanDisk ();
  }

  PluginLoader::~PluginLoader ()
  {
    delete d;
  }

  void PluginLoader::load (const QString & interface,
					       const QString & pluginName)
  {
    QPluginLoader loader (QString (PLEXPREFIX) + "/lib/plexyext/lib" + pluginName + ".so");
    QObject *plugin = loader.instance ();
    if (plugin)
      {
        AbstractPluginInterface * Iface  = 0; //=  qobject_cast<AbstractPluginInterface*> (plugin);
        ExtensionProducer<AbstractPluginInterface> factory;
        Iface = factory.instance(interface,plugin);
      }
    else
      {
	qDebug () << loader.errorString () << endl;;
//	d->currentDrop = 0;

      }
  }


  void PluginLoader::scanDisk ()
  {
    QDir dir (d->prefix);
    dir.setFilter (QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting (QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList ();
    for (int i = 0; i < list.size (); ++i)
      {
	QFileInfo fileInfo = list.at (i);
	loadDesktop (d->prefix + fileInfo.fileName ());
      }

  }				// namespace PlexDesk

  void PluginLoader::loadDesktop (const QString & path)
  {
    qDebug () << path << endl;

    QSettings desktopFile (path, QSettings::IniFormat, this);

    desktopFile.beginGroup ("Desktop Entry");

    load (desktopFile.value ("Type").toString (),desktopFile.value ("X-PLEXYDESK-Library").toString ());

    desktopFile.endGroup ();

  }
}

#include "pluginloader.moc"
