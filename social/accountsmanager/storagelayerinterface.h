#include <QObject>
#include <QVariant>
#include <QString>
#include <QVariantMap>

namespace PlexyDesk
{
    class StorageLayerInterface : public QObject
    {
        Q_OBJECT;

    public:
        StorageLayerInterface(QObject *parent = 0): QObject(parent) {}
        virtual ~StorageLayerInterface(){}

        virtual void open() = 0;
        virtual void close() = 0;

        virtual QStringList listAccounts() = 0;
        virtual QVariantMap readAccount(const QString& id) = 0;
        virtual bool removeAccount(const QString& id) = 0;
        virtual bool writeAccount(const QString& id, const QVariantMap& data) = 0;
        virtual bool updateAccount(const QString &id, const QString &propertyName, const QVariant &value) = 0;

    signals:
        void opened(bool success);
        void closed();
    };
}
