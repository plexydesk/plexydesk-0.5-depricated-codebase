#include "datasource.h"
#include <QVariantHash>

namespace PlexyDesk
{

class DataSource::PrivateDataSource
{
public:
    PrivateDataSource(){}
    ~PrivateDataSource(){}

    QVariantHash mSettingsData;
};

DataSource::DataSource(QObject *object) : QObject(object),
    d(new PrivateDataSource)
{
}

DataSource::~DataSource()
{
    delete d;
}

QVariant DataSource::settings(const QString &key) const
{
    return d->mSettingsData[key];
}

void DataSource::addSetting(const QString &key, const QVariant &value)
{
    d->mSettingsData[key] = value;
}

void DataSource::requestData(QVariant args)
{
    setArguments(args);
}

}
