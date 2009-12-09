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
 *Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "account.h"
#include "adaptors/accountadaptor.h" //TODEO: remove this
#include "accountmanager.h"
#include "storagelayerinterface.h"
#include <QTimer>
#include <QDebug>

//using namespace PlexyDesk;
namespace PlexyDesk
{

    SocialAccount::SocialAccount(const QString &accountId, QDBusConnection *dbusConnection, QObject *parent) : QObject(parent),
        m_adapter(new SocialAccountAdaptor(this)),
        m_id(accountId),
        m_dbusConnection(dbusConnection)
    {
        connect(this, SIGNAL(AccountPropertyChanged(const QString&)), this, SLOT(onAccountPropertyChanged(const QString&)));
        QTimer::singleShot(0, this, SLOT(loadAccountData()));
    }

    SocialAccount::~SocialAccount()
    {
        qDebug() << "SocialAccount::~SocialAccount(): Destroying account:" << m_id;
        m_dbusConnection->unregisterObject(m_id);
        delete m_adapter;
    }

    QString SocialAccount::id() const
    {
        return m_id;
    }

    void SocialAccount::setDisplayName(const QString &displayName)
    {
        this->m_displayName = displayName;
        emit AccountPropertyChanged("DISPLAY_NAME");
    }

    void SocialAccount::setUserName(const QString &userName)
    {
        this->m_userName = userName;
        emit AccountPropertyChanged("USERNAME");
    }

    void SocialAccount::setPassword(const QString &password)
    {
        m_password = password;
        emit AccountPropertyChanged("PASSWORD");
    }

    void SocialAccount::setEnabled(bool enable)
    {
        this->m_enabled = enable;
        emit AccountPropertyChanged("ENABLED");
    }

    void SocialAccount::setValid(bool valid)
    {
        this->m_valid = valid;
        emit validityChanged(m_valid);
        emit AccountPropertyChanged("VALID");
    }

    void SocialAccount::setParameters(const QVariantMap &set, const QStringList &unSet)
    {
        //TODO: Implement the method body
    }

    void SocialAccount::loadAccountData()
    {
        QVariantMap accountData = getAccount(m_id); //TODO: implement 'getAccount()' method

        // Load property: valid
        m_valid = true;
        if(accountData.contains("VALID"))
        {
            if(accountData.value("VALID").type() == QVariant::Bool)
            {
                m_valid = accountData.value("VALID").toBool();
            }
        }

        // Load private member: socialService
        if(accountData.contains("SOCIAL_SERVICE"))
        {
            m_socialService = accountData.value("SOCIAL_SERVICE").toString();
        }
        else
        {
            qDebug() << "Account::loadAccountData(): Account does not contain a socialService. Making it invalid.";
            m_valid = false;
        }

        /* Load property: parameters
        if(accountData.contains("PARAMETERS"))
        {
            m_parameters = accountData.value("PARAMETERS").toMap();
        }
        else
        {
            qDebug() << "Account::loadAccountData(): Account does not contain parameters. Making it invalid.";
            m_valid = false;
        }*///TODO: implement parameter handling

        // Load property: display name
        // FIXME: Is an account required to have a display name?
        if(accountData.contains("DISPLAY_NAME"))
        {
            m_displayName = accountData.value("DISPLAY_NAME").toString();
        }
        else
        {
            qDebug() << "Account::loadAccountData(): Account does not contain a displayname. Making it invalid.";
            m_valid = false;
        }

        // Newly created accounts default to being enabled.
        if(accountData.contains("ENABLED"))
        {
            m_enabled = accountData.value("ENABLED").toBool();
        }
        else
        {
            m_enabled = true;
        }

        if(accountData.contains("USERNAME"))
        {
            m_userName = accountData.value("USERNAME").toString();
        }
        else
        {
            m_userName = "unknown username";
        }

        //TODO: add a method to get supported interfaces

        // Register the dbus object.
        if(!m_dbusConnection->registerObject(m_id, this)) {
            qDebug() << "Account::loadAccountData() Registering Account:" << m_id << " DBUS Object failed. Die.";
            Q_EMIT ready(false);
            return;
        }

        Q_EMIT ready(true);
    }

    QVariantMap SocialAccount::getAccount(const QString &accountHandle)
    {
        //TODO: implement parameter support
        StorageLayerInterface *storageLayer = (qobject_cast<SocialAccountsManager*> (parent()))->storageLayer();
        return storageLayer->readAccount(accountHandle);
    }

    void SocialAccount::removeAccount()
    {
        //TODO: remove from storage layer
        StorageLayerInterface *storageLayer = (qobject_cast<SocialAccountsManager*> (parent()))->storageLayer();
        if(storageLayer->removeAccount(m_id))
            emit Removed();
    }

    /*--------------------------public get methods------------------------*/
    QString SocialAccount::getUserName()
    {
        return m_userName;
    }

    QString SocialAccount::getPassword()
    {
        return m_password;
    }

    QString SocialAccount::getDisplayName()
    {
        return m_displayName;
    }

    bool SocialAccount::isEnabled()
    {
        return m_enabled;
    }

    bool SocialAccount::isValid()
    {
        return m_valid;
    }

    QVariantMap SocialAccount::getParameters()
    {
        return m_parameters;
    }
    /*--------------------------------------------------------------------------*/

    void SocialAccount::onAccountPropertyChanged(const QString &property)
    {
        StorageLayerInterface *storageLayer = (qobject_cast<SocialAccountsManager*> (parent()))->storageLayer();
        qDebug() << "SocialAccount::onAccountPropertyChanged():update status" << storageLayer->updateAccount(m_id, property, this->property(property.toAscii()));
    }

}
