#ifndef FACEBOOKDATAINTERFACE_H
#define FACEBOOKDATAINTERFACE_H

#include <QtCore>
#include <plexy.h>
#include <dataplugininterface.h>
#include <QtNetwork>

class FacebookDataInterface : public QObject, public PlexyDesk::DataPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::DataPluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.facebookData")

public:
    explicit FacebookDataInterface(QObject *parent = 0);

    QSharedPointer<PlexyDesk::DataSource> model();
};

#endif // FACEBOOKDATAINTERFACE_H
