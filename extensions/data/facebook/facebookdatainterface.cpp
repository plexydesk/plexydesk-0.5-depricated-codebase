#include "facebookdatainterface.h"
#include "facebooksession.h"

FacebookDataInterface::FacebookDataInterface(QObject *parent) :
    PlexyDesk::DataPluginInterface(parent)
{
}

QSharedPointer<PlexyDesk::DataSource> FacebookDataInterface::model()
{
    QSharedPointer<PlexyDesk::DataSource> obj =
            QSharedPointer<PlexyDesk::DataSource>(new FacebookSession(), &QObject::deleteLater);

    return obj;
}

Q_EXPORT_PLUGIN2(facebookengine, FacebookDataInterface)
