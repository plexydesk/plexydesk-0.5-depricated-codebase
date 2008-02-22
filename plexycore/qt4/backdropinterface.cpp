#include <backdropinterface.h>
#include <plexy.h>
#include <config.h>
#include <pluginloader.h>
#include <datainterface.h>

namespace PlexyDesk
{
    DataInterface *  BackdropInterface::loadData(const QString& name)
    {
        qDebug()<<"oops"<<endl;
     DataInterface * data = (DataInterface*)PluginLoader().instance(name);

              if (data) {
                  return data;
              }

              return 0;
    }
} // namespace PlexyDesk

