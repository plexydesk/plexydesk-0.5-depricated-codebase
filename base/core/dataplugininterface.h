#ifndef DATAPLUGININTERFACE_H
#define DATAPLUGININTERFACE_H

#include <plexy.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QObject>

#include <datasource.h>

namespace PlexyDesk {
class DataPluginInterface
{
public:
    DataPluginInterface() {};
    virtual ~DataPluginInterface() {}
    virtual QSharedPointer<DataSource> model() = 0;
};

}

Q_DECLARE_INTERFACE(PlexyDesk::DataPluginInterface, "org.plexydesk.DataPluginInterface")
#endif // DATAPLUGININTERFACE_H
