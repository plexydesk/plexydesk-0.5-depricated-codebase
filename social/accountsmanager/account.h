#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include <QString>
#include <QStringList>
#include <QDBusConnection>

namespace PlexyDesk
{
    class SocialAccountAdaptor;

    class SocialAccount : public QObject
    {
        Q_OBJECT;
        
        Q_PROPERTY(QString DISPLAY_NAME READ getDisplayName WRITE setDisplayName)
        Q_PROPERTY(QString USERNAME READ getUserName WRITE setUserName)
        Q_PROPERTY(QString PASSWORD READ getPassword WRITE setPassword)
        Q_PROPERTY(bool ENABLED READ isEnabled WRITE setEnabled)
        Q_PROPERTY(bool VALID READ isValid WRITE setValid)
        
        public:
            SocialAccount(const QString &accountId, QDBusConnection *dbusConnection, QObject *parent=0);
            ~SocialAccount();
            //set methods
            void setDisplayName(const QString &displayName);
            void setUserName(const QString &userName);
            void setPassword(const QString &password);
            void setEnabled(bool enable);
            void setValid(bool valid);
            void setParameters(const QVariantMap &set, const QStringList &unSet); //TODO: implement parameter handling
            void removeAccount();

            //public get methods
            QString getUserName();
            QString getDisplayName();
            QString getPassword();
            bool isEnabled();
            bool isValid();
            QVariantMap getParameters();

            QString id() const;
            QVariantMap getAccount(const QString &accountHandle);

        private:
            //member valiables
            const QString m_id;
            QDBusConnection *m_dbusConnection;
            SocialAccountAdaptor *m_adapter;
            QString m_displayName;
            QString m_userName;
            QString m_password;
            //QString avatar; TODO: Implement avatar handling
            bool m_enabled;
            bool m_valid;
            bool m_hasBeenOnline;
            QVariantMap m_parameters;
            QString m_socialService;

            //loaded from storage layer
            QString socialService;

        public slots:
            //void UpdateParameters(const QVariantMap &Set, const QStringList &Unset); TODO: implement

        signals:
            void AccountPropertyChanged(const QString &property);
            void Removed();

            void ready(bool success);
            void validityChanged(bool valid);

        private slots:
            void loadAccountData();
            void onAccountPropertyChanged(const QString &property);

    };
}
