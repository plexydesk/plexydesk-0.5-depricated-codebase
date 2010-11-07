#include "../account.h"
#include "accountadaptor.h"

namespace PlexyDesk
{
SocialAccountAdaptor::SocialAccountAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent)
{

}
/*-----------------public get methods----------*/
QString SocialAccountAdaptor::getDisplayName()
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->getDisplayName();
}

QString SocialAccountAdaptor::getUserName()
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->getUserName();
}

bool SocialAccountAdaptor::isEnabled()
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->isEnabled();
}

bool SocialAccountAdaptor::isValid()
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->isValid();
}

QVariantMap SocialAccountAdaptor::getParameters()
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->getParameters();
}

/*-----------------public set methods-----------------*/
void SocialAccountAdaptor::setDisplayName(const QString &displayName)
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->setDisplayName(displayName);
}

void SocialAccountAdaptor::setEnabled(bool enable)
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->setEnabled(enable);
}

void SocialAccountAdaptor::setUserName(const QString &userName)
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->setUserName(userName);
}

void SocialAccountAdaptor::setParameters(const QVariantMap &set, const QStringList &unSet)
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->setParameters(set, unSet);
}

void SocialAccountAdaptor::setValid(bool valid)
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->setValid(valid);
}

void SocialAccountAdaptor::removeAccount()
{
    SocialAccount *account = static_cast<SocialAccount*> (parent());
    return account->removeAccount();
}
}
