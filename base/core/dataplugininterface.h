#ifndef DATAPLUGININTERFACE_H
#define DATAPLUGININTERFACE_H

#include <plexy.h>

#include <QSharedPointer>
#include <QObject>

#include <datasource.h>

namespace PlexyDesk {
class DataPluginInterface : public QObject
{
    Q_OBJECT

public:
    DataPluginInterface(QObject *parent = 0);
    virtual QSharedPointer<DataSource> model() = 0;
};

}

Q_DECLARE_INTERFACE(PlexyDesk::DataPluginInterface, "org.plexydesk.DataPluginInterface")
#endif // DATAPLUGININTERFACE_H
