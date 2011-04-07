#include <QDBusAbstractAdaptor>
#include <QObject>
#include <QVariantMap>

namespace PlexyDesk
{
class SocialAccountAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT;
    Q_CLASSINFO("D-Bus Interface", "local.socioplexy.account");

public:
    SocialAccountAdaptor(QObject *parent = 0);

public slots:
    //public get methods
    QString getDisplayName();
    QString getUserName();
    bool isEnabled();
    bool isValid();
    QVariantMap getParameters();

    //public set methods
    void setDisplayName(const QString &displayName);
    void setUserName(const QString &userName);
    void setEnabled(bool enable);
    void setValid(bool valid);
    void setParameters(const QVariantMap &set, const QStringList &unSet);
    void removeAccount();
};
} //end of namespace PlexyDesk
