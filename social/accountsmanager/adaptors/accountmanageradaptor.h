#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QVariantMap>
#include <QDBusObjectPath>

namespace PlexyDesk
{
    class SocialAccountsManagerAdaptor : public QDBusAbstractAdaptor
    {
        Q_OBJECT;
        Q_CLASSINFO("D-Bus Interface", "local.socioplexy.manageAcounts");

        public:
            SocialAccountsManagerAdaptor(QObject *parent=0);
        public slots:
            QDBusObjectPath createAccount(const QString &socialServiceName, const QString &displayName,
                                            const QString &userName, QVariantMap properties);

    };
}
