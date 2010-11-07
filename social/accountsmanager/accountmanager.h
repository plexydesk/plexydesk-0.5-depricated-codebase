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

#include <QApplication>
#include <QDBusObjectPath>
#include <QMap>
#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QtDBus/QDBusContext>

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
