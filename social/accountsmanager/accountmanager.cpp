/* This file is part of telepathy-accountmanager-kwallet
 *
 * Copyright (C) 2008-2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Mahesh Kaushalya 2008-2009 <wpmahesh@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 **Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "account.h"
#include "accountmanager.h"
#include "adaptors/accountmanageradaptor.h"
#include "storagelayer.h"

#include <QDBusMessage>
#include <QDBusPendingReply>
#include <QDebug>
#include <QMap>
#include <QMetaObject>
#include <QMetaType>
#include <QTimer>

/*------------------------ Structures -------------------------*/

struct CreateAccountRequestData
{
    QString socialService;
    QString displayName;
    QString userName;
    QVariantMap parameters;
    QDBusMessage message;

};
Q_DECLARE_METATYPE(CreateAccountRequestData);

namespace PlexyDesk
{



SocialAccountsManager::SocialAccountsManager(QApplication *parent) : QObject(parent),
                                                                     m_adaptor(new SocialAccountsManagerAdaptor(this)),
                                                                     m_storageLayer(new SocialStorageLayer(this)),
                                                                     m_storageLayerOpened(false)
{
    qRegisterMetaType<CreateAccountRequestData>();
    m_dbusConnection = new QDBusConnection(QDBusConnection::sessionBus());
    m_dbusConnection->registerService("org.plexydesk.SocialAccountsManager");
    m_dbusConnection->registerObject("/org/plexydesk/SocialAccountsManager", this);

    loadAccounts();
}

StorageLayerInterface *SocialAccountsManager::storageLayer() const
{
    if (m_storageLayerOpened)
        return m_storageLayer;
}

void SocialAccountsManager::loadAccounts()
{
    Q_ASSERT(false == m_storageLayerOpened);
    connect(m_storageLayer, SIGNAL(opened(bool)), this, SLOT(onStorageLayerOpened(bool)));
    m_storageLayer->open();

}

void SocialAccountsManager::onStorageLayerOpened(bool success)
{
    qDebug() << "SocialAccountsManager::onStorageLayerOpened()";
    Q_ASSERT(false == m_storageLayerOpened);
    if (success)
    {
        m_storageLayerOpened = true;
        doLoadAccounts();
        return;
    }
}

void SocialAccountsManager::doLoadAccounts()
{
    qDebug() << "SocialAccountsManager::doLoadAccounts()";
    QStringList accounts = m_storageLayer->listAccounts();
    qDebug() << "SocialAccountsManager::doLoadAccounts(): listAccounts value:" << accounts;
    QStringList::const_iterator end( accounts.end() );
    for ( QStringList::const_iterator itr( accounts.begin() ); itr != end; ++itr )
    {
        QString accountHandle((*itr));

        Q_ASSERT(!m_pendingAccounts.contains(accountHandle));
        Q_ASSERT(!m_readyAccounts.contains(accountHandle));
        Q_ASSERT(!m_validAccounts.contains(QDBusObjectPath(accountHandle)));
        Q_ASSERT(!m_invalidAccounts.contains(QDBusObjectPath(accountHandle)));

        m_pendingAccounts.insert(accountHandle, new SocialAccount(accountHandle, m_dbusConnection, this));
        connect(m_pendingAccounts.value(accountHandle), SIGNAL(ready(bool)),
             this, SLOT(onAccountReady(bool)));

    }
}

QDBusObjectPath SocialAccountsManager::createAccount(const QString &socialServiceName, const QString &displayName,
     const QString &userName, QVariantMap &properties)
{
    qDebug() <<" SocialAccountsManager::createAccount() called";
    //create the structure storing the account requested data
    CreateAccountRequestData *requestData = new CreateAccountRequestData;
    requestData->displayName = displayName;
    requestData->parameters = properties;
    requestData->socialService = socialServiceName;
    requestData->userName = userName;

    //indicate that the reply tho this method will be delayd
    requestData->message = message(); //This is from QDBusContext class
    setDelayedReply(true);
    //m_dbusConnection->send(requestData->message);

    //check whether the Social Service name is empty
    if (requestData->socialService.isEmpty())
    {
        QDBusMessage errorReply = requestData->message.createErrorReply(
             "org.plexydesk.SocialAccountsManager.Error.NotImplemented", "Empty social service parameter passed.");
        m_dbusConnection->send(errorReply);
        delete requestData;
        return QDBusObjectPath("/dummy/object/path/to/suppress/qdbus/warning");
    }

    if (requestData->displayName.isEmpty())
    {
        requestData->displayName = "Unknown Account";
    }

    //calls doCreateAccount slot once
    m_requestData = requestData;
    QTimer::singleShot(0, this, SLOT(doCreateAccount()));

    // Return value is irrelevant so default-constructed one is fine.
    return QDBusObjectPath("/dummy/object/path/to/suppress/qdbus/warning");
}

void SocialAccountsManager::doCreateAccount()
{
    if (!m_requestData)
    {
        qDebug() << "SocialAccountsManager::doCreateAccount() m_requestData is null";
        return;
    }
    /*--------------check whether the social service requested is supported by social daemon-----------*/

    QDBusMessage busMsg = QDBusMessage::createMethodCall("org.plexydesk.social", "/services", "local.socioplexy.Configuration", "getPluginsList");
    QDBusPendingReply<QStringList> msgRep = m_dbusConnection->asyncCall(busMsg, 1000);
    msgRep.waitForFinished();
    //if reply is valid only proceed the QDBus connection method calling
    if (msgRep.isValid())
    {
        QStringList supportedServices = msgRep.value();
        if (!supportedServices.contains(m_requestData->socialService))
        {
            // SocialDaemon doesn't support the social service specified. dbus error.
            QDBusMessage errorReply = m_requestData->message.createErrorReply(
                 "org.plexydesk.SocialAccountsManager.Error.NotImplemented",
                 "SocialDaemon does not support the specified service.");
            m_dbusConnection->send(errorReply);
            delete m_requestData;
            return;
        }
    }

    // TODO: Support for registering accounts on the server.
    // TODO: Check for unsupported parameters being passed.

    // Build the account identifier.
    QString accountHandle("/org/plexydesk/social/Accounts/");
    accountHandle.append(objectPathEscape(m_requestData->socialService));
    accountHandle.append('/');
    accountHandle.append(objectPathEscape(m_requestData->userName));

    //chech if the account identifier is already available. If so append a number to make it unique
    if (m_pendingAccounts.contains(accountHandle) || m_readyAccounts.contains(accountHandle))
    {
        int i = 2;
        while (true)
        {
            accountHandle.append("_");
            accountHandle.append(QString::number(i));

            if (!m_pendingAccounts.contains(accountHandle) && m_readyAccounts.contains(accountHandle))
                break;
            i++;
        }
    }

    // Make a map containing all the data to be stored in the storage layer.
    QVariantMap accountData;

    accountData.insert("SOCIAL_SERVICE", m_requestData->socialService);
    accountData.insert("DISPLAY_NAME", m_requestData->displayName);
    accountData.insert("USERNAME", m_requestData->userName);
    accountData.insert("VALID", true); //Newly created accounts VALID property set to true by default
    accountData.insert("ENABLED", true); ////Newly created accounts ENABLED property set to true by default
    // accountData.insert("parameters", requestData->parameters); TODO: Implement parameter support

    qDebug() << "SocialAccountsManager::doCreateAccount():write status:" <<m_storageLayer->writeAccount(accountHandle, accountData);

    //create the ACTUAL account object
    m_pendingAccounts.insert(accountHandle, new SocialAccount(accountHandle, m_dbusConnection, this));
    connect(m_pendingAccounts.value(accountHandle), SIGNAL(ready(bool)),
         this, SLOT(onAccountReady(bool)));
    Q_ASSERT(!m_pendingAccountsRequestData.contains(accountHandle));
    m_pendingAccountsRequestData.insert(accountHandle, m_requestData);
}

void SocialAccountsManager::onAccountReady(bool success)
{
    qDebug()<< "SocialAccountsManager::onAccountReady(): An account is ready!..";
    SocialAccount *account = qobject_cast<SocialAccount *>(sender());

    Q_ASSERT(0 != account);
    Q_ASSERT(m_pendingAccounts.contains(account->id()));
    Q_ASSERT(!m_readyAccounts.contains(account->id()));
    Q_ASSERT(!m_validAccounts.contains(QDBusObjectPath(account->id())));
    Q_ASSERT(!m_invalidAccounts.contains(QDBusObjectPath(account->id())));

    m_pendingAccounts.remove(account->id());
    if (success)
    {
        m_readyAccounts.insert(account->id(), account);

        if (account->isValid())
        {
            m_validAccounts.append(QDBusObjectPath(account->id()));
        }
        else
        {
            m_invalidAccounts.append(QDBusObjectPath(account->id()));
        }

        connect(account, SIGNAL(validityChanged(bool)),
             this, SLOT(onAccountValidityChanged()));
        connect(account, SIGNAL(Removed()),
             this, SLOT(onAccountRemoved()));
    }
    else
    {
        delete account;
    }

    // If it is a new account, we should reply to the method call that created it.
    if (m_pendingAccountsRequestData.contains(account->id())) {

        CreateAccountRequestData *requestData = m_pendingAccountsRequestData.value(account->id());

        if (success)
        {
            // Get the DBus reply object we are going to use.
            QDBusMessage reply = requestData->message.createReply();

            // Send the DBus reply.
            reply << QVariant::fromValue(QDBusObjectPath(account->id()));
            m_dbusConnection->send(reply);
        }
        else
        {
            QDBusMessage errorReply = requestData->message.createErrorReply(
                 "org.plexydesk.SocialAccountsManager.Error.NotImplemented", "Unknown error occured trying to create the account.");
            m_dbusConnection->send(errorReply);
        }

        // Remove the requestData struct for the map of pending account's ones.
        m_pendingAccountsRequestData.remove(account->id());
        Q_ASSERT(!m_pendingAccountsRequestData.contains(account->id()));
        // Delete the requestData struct now we are finished with it.
        delete requestData;

        // If we have already registered on dbus, we should emit a signal saying
        // that this account has been created.
        /* if(m_dbusObjectsRegistered)
           {
             Q_EMIT AccountValidityChanged(QDBusObjectPath(account->id()), account->isValid());
           }*/
    }

    /*  if((m_dbusObjectsRegistered == false) && (m_pendingAccounts.size() == 0))
       {
          Q_EMIT callDoRegisterDBusObjects();
       }*/                                                                                                                                        //CHECK: this is done @ constructor
}

void SocialAccountsManager::onAccountValidityChanged()
{
    SocialAccount *account = qobject_cast<SocialAccount *>(sender());
    Q_ASSERT(0 != account);
    Q_ASSERT(m_readyAccounts.contains(account->id()));

    if (account->isValid())
    {
        if (m_validAccounts.contains(QDBusObjectPath(account->id())))
        {
            qDebug () << "AccountManagerPrivate::onAccountValidityChanged(): Validity of account" << account->id() << "changed, but it already has that validity.";
            Q_ASSERT(!m_invalidAccounts.contains(QDBusObjectPath(account->id())));
        }
        else
        {
            qDebug() << "AccountManagerPrivate::onAccountValidityChanged(): Changing validity of acccount" << account->id();
            m_invalidAccounts.removeOne(QDBusObjectPath(account->id()));
            m_validAccounts.append(QDBusObjectPath(account->id()));
            Q_ASSERT(m_validAccounts.contains(QDBusObjectPath(account->id())));
            Q_ASSERT(!m_invalidAccounts.contains(QDBusObjectPath(account->id())));
            //Q_EMIT AccountValidityChanged(QDBusObjectPath(account->id()), account->isValid());
        }
    }
    else
    {
        if (m_invalidAccounts.contains(QDBusObjectPath(account->id())))
        {
            qDebug () << "AccountManagerPrivate::onAccountValidityChanged(): Validity of account" << account->id() << "changed, but it already has that validity.";
            Q_ASSERT(!m_validAccounts.contains(QDBusObjectPath(account->id())));
        }
        else
        {
            qDebug() << "AccountManagerPrivate::onAccountValidityChanged(): Changing validity of acccount" << account->id();
            m_validAccounts.removeOne(QDBusObjectPath(account->id()));
            m_invalidAccounts.append(QDBusObjectPath(account->id()));
            Q_ASSERT(m_invalidAccounts.contains(QDBusObjectPath(account->id())));
            Q_ASSERT(!m_validAccounts.contains(QDBusObjectPath(account->id())));
            //Q_EMIT AccountValidityChanged(QDBusObjectPath(account->id()), account->isValid());
        }
    }
}

void SocialAccountsManager::onAccountRemoved()
{
    SocialAccount *account = qobject_cast<SocialAccount *>(sender());
    Q_ASSERT(0 != account);

    Q_ASSERT(m_readyAccounts.contains(account->id()));
    Q_ASSERT(!m_pendingAccounts.contains(account->id()));

    m_readyAccounts.remove(account->id());

    QDBusObjectPath accountObjectPath(account->id());

    if (m_validAccounts.contains(accountObjectPath))
    {
        m_validAccounts.removeOne(accountObjectPath);
    }
    else if (m_invalidAccounts.contains(accountObjectPath))
    {
        m_invalidAccounts.removeOne(accountObjectPath);
    }

    // Q_EMIT AccountRemoved(accountObjectPath);

    Q_ASSERT(!m_readyAccounts.contains(account->id()));
    Q_ASSERT(!m_validAccounts.contains(accountObjectPath));
    Q_ASSERT(!m_invalidAccounts.contains(accountObjectPath));

    delete account;
}

void SocialAccountsManager::doRegisterDBusObjects()
{
    Q_ASSERT(0 == m_dbusObjectsRegistered);

    // Register dbus object
    qDebug() << "SocialAccountaManager::doRegisterDBusObjects(): Registering busname org.plexydesk.SocialAccountsManager";
    if (!m_dbusConnection->registerObject("/org/plexydesk/SocialAccountsManager", this)) {
        qDebug() << "SocialAccountsManager::doRegisterDBusObjects(): Registering /org/plexydesk/SocialAccountsManager failed. Die.";
        //deleteLater(); TODO: what is this?
    }

    m_dbusObjectsRegistered = true;
}


QString SocialAccountsManager::objectPathEscape(QString path) const
{
    // This method escapes any characters in a string that are not allowed
    // in dbus object paths.
    path.replace('-', '_');
    path.replace('@', '_');
    path.replace('.', '_');

    QRegExp invalidCharacters("\\W|\\d{0,0}");
    path = path.remove(invalidCharacters);

    return path;
}
}
