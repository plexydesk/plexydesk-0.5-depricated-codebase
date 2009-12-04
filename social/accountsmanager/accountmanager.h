#include <QObject>
#include <QApplication>
#include <QVariantMap>
#include <QDBusObjectPath>
#include <QtDBus/QDBusContext>
#include <QStringList>
#include <QMap>

struct CreateAccountRequestData;
namespace PlexyDesk
{
    class SocialAccountsManagerAdaptor;
    class SocialAccount;
    class StorageLayerInterface;

    class SocialAccountsManager : public QObject, protected QDBusContext
    {
        Q_OBJECT;

        public:
            SocialAccountsManager(QApplication *parent=0);
            QDBusObjectPath getAccount(const QString &protocol, const QString &userName);
            StorageLayerInterface* storageLayer() const;
            void loadAccounts();

        signals:
            void AccountValidityChanged(const QDBusObjectPath &account, bool Valid);
            void callDoRegisterDBusObjects();

        private:
            QList<QDBusObjectPath> m_validAccounts;
            QList<QDBusObjectPath> m_invalidAccounts;
            QStringList m_interfaces;
            QStringList m_supportedAccountProperties;
            QDBusConnection *m_dbusConnection;
            QMap<QString, SocialAccount*> m_pendingAccounts;
            QMap<QString, SocialAccount*> m_readyAccounts;
            QMap<QString, CreateAccountRequestData*> m_pendingAccountsRequestData;
            bool m_dbusObjectsRegistered;
            bool m_storageLayerOpened;
            SocialAccountsManagerAdaptor *m_adaptor;
            CreateAccountRequestData *m_requestData;
            StorageLayerInterface *m_storageLayer;

            QString objectPathEscape(QString path) const;
            void doLoadAccounts();

        //public slots
        public slots:
             QDBusObjectPath createAccount(const QString &socialServiceName, const QString &displayName,
                                            const QString &userName, QVariantMap &properties);

        private slots:
             void onAccountReady(bool success);
             void onAccountValidityChanged();
             void doRegisterDBusObjects();
             void doCreateAccount();
             void onAccountRemoved();
             void onStorageLayerOpened(bool success);

    };
}//end of namespace PlexyDesk
