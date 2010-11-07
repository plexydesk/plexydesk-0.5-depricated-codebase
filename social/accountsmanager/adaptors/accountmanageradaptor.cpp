#include "../accountmanager.h"
#include "accountmanageradaptor.h"

#include <QDebug>

namespace PlexyDesk
{
SocialAccountsManagerAdaptor::SocialAccountsManagerAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent)
{

}

QDBusObjectPath SocialAccountsManagerAdaptor::createAccount(const QString &socialServiceName, const QString &displayName,
        const QString &userName, QVariantMap properties)
{
    qDebug() << " SocialAccountsManagerAdaptor::createAccount() inside";
    SocialAccountsManager *accMan = static_cast<SocialAccountsManager*> (parent());
    accMan->createAccount(socialServiceName, displayName, userName, properties);
    return QDBusObjectPath("/dummy/object/path/to/suppress/qdbus/warning/accmanadaptor"); // return irrelevant
}

QString SocialAccountsManagerAdaptor::testSlot()
{
    return "return from SocialAccountsManagerAdaptor::testSlot()";
}

}
