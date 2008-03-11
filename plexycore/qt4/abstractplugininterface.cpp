#include <abstractplugininterface.h>
#include <plexy.h>
#include <config.h>
#include <pluginloader.h>

namespace PlexyDesk
{
    AbstractPluginInterface::AbstractPluginInterface(QObject *parent /* = 0 */) : QObject(parent) {}
    DataInterface * AbstractPluginInterface::loadData(const QString& name)
    {
/*
        DataInterface * data = (DataInterface*)PluginLoader::getInstance()->instance(name);

        if (data) {
            return data;
        } else {
            qDebug()<<"AbstractPluginInterface::"<<"Null data found"<<endl;
        }
*/
        return 0;
    }

}
