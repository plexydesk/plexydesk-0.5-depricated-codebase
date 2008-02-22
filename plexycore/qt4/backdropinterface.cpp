#include <backdropinterface.h>
#include <plexy.h>
#include <config.h>
#include <pluginloader.h>
#include <datainterface.h>

namespace PlexyDesk
{
    DataInterface *  BackdropInterface::loadData(const QString& name)
    {
     DataInterface * data = (DataInterface*)PluginLoader::getInstance()->instance(name);

              if (data) {
                  return data;
              } else {
                  qDebug()<<"Null data found"<<endl;
              }

              return 0;
    }
} // namespace PlexyDesk

