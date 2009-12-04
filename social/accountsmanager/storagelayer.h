#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QVariant>
#include "storagelayerinterface.h"

namespace PlexyDesk
{
    class SocialStorageLayer : public StorageLayerInterface
    {
        Q_OBJECT;
        public:
            SocialStorageLayer(QObject *parent=0);
            void open();
            void close() ;

            QStringList listAccounts() ;
            QVariantMap readAccount(const QString &id);
            bool removeAccount(const QString &id);
            bool writeAccount(const QString &id, const QVariantMap &data);
            bool updateAccount(const QString &id, const QString &propertyName, const QVariant &value);

        private:
            void setDatabasePath(const QString &path);
            void createTables();

            //QString generatePasswordHash();
    };
}
