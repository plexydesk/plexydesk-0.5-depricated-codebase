/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Dariusz Mikulski <dariusz.mikulski@gmail.com>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#include "presence.h"

#include <TelepathyQt4/Client/Account>
#include <TelepathyQt4/Client/AccountManager>

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtDebug>
#include <QVariant>

class PresenceData::PresenceDataPrivate
{
    PresenceData *parent;
public:
    PresenceDataPrivate(PresenceData *p) : parent(p) {}

    Telepathy::Client::AccountManager * m_accountManager;

    void createAccountDataSource(const QString &path) {
        qDebug() << "createAccountDataSource called";
        qDebug() << path;
        Telepathy::Client::Account *account = accountFromPath(path);

        QString source;
        source = account->uniqueIdentifier();

        Telepathy::SimplePresence sp = account->currentPresence();

        QVariant vsp;
        vsp.setValue(sp);

        QVariantMap datas;
        datas.insert(source, vsp);

        QVariant sendData;
        sendData.setValue(datas);
        emit parent->data(sendData);
    }

    void removeAccountDataSource(const QString &path) {
        qDebug() << "removeAccountDataSource called";
        qDebug() << path;

        Telepathy::Client::Account *account = accountFromPath(path);
        QString identifier = account->uniqueIdentifier();
        //parent->removeSource(identifier);
    }

    Telepathy::Client::Account *accountFromPath(const QString &path) {
        return m_accountManager->accountForPath(path);
    }
};

PresenceData::PresenceData(QObject * parent)
        : d(new PresenceDataPrivate(this))
{
    // Register custom types:
    Telepathy::registerTypes();
}

/**
 * Class destructor
 */
PresenceData::~PresenceData()
{
    delete d;
}

/**
 * Initialize Presence.
 */
void PresenceData::init()
{
    qDebug() << "init() started";
    /*
     * check that we are connected to the session
     * bus OK.
     */
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "PresenceEngine::init(): cannot connect to session bus.";
    }

    /*
     * set up the dbus accountmanager object
     * which will provide all the data to this
     * data engine.
     */
    d->m_accountManager =
        new Telepathy::Client::AccountManager(QDBusConnection::sessionBus());

    /*
     * connect signal from the account manager
     * to waiting when it's ready
     */
    connect(d->m_accountManager->becomeReady(),
            SIGNAL(finished(Telepathy::Client::PendingOperation*)),
            this,
            SLOT(onAccountReady(Telepathy::Client::PendingOperation*))
           );

    /*
     * connect signals from the account manager
     * to slots within this data engine.
     *
     * we intentionally do this before processing
     * the accounts that are already created so
     * that if another is created while we are
     * processing them, we don't miss out on it.
     */
    connect(d->m_accountManager, SIGNAL(accountCreated(const QString &)),
            this, SLOT(accountCreated(const QString &)));
    connect(d->m_accountManager, SIGNAL(accountValidityChanged(const QString &, bool)),
            this, SLOT(accountValidityChanged(const QString &, bool)));
    connect(d->m_accountManager, SIGNAL(accountRemoved(const QString &)),
            this, SLOT(accountRemoved(const QString &)));
}

void PresenceData::onAccountReady(Telepathy::Client::PendingOperation *operation)
{
    qDebug() << "onAccountReady() called";
    if (operation->isError()) {
        qDebug() << operation->errorName() << ": " << operation->errorMessage();
        return;
    }

    QStringList pathList = d->m_accountManager->allAccountPaths();
    qDebug() << "All Account Paths: " << pathList.size();

    /*
     * get a list of all the accounts that
     * are all ready there
     */
    QList<Telepathy::Client::Account *> accounts = d->m_accountManager->allAccounts();
    qDebug() << "accounts: " << accounts.size();

    /*
     * create a datasource for each
     * of the accounts we got in the list.
     */
    foreach(const QString &path, pathList) {
        d->createAccountDataSource(path);
    }
}

/**
 *  Slot for new account.
 *
 * \param path QDBusObjectPath to created account.
 */
void PresenceData::accountCreated(const QString &path)
{
    qDebug() << "accountCreated() called";
    // Load the data for the new account. To avoid duplicating code, we treat
    // this just as if an account was updated, and call the method to handle
    // that.
    d->createAccountDataSource(path);
}

/**
 * Slot for account data changed.
 *
 * \param QDBusObjectPath Name of the account path.
 * \param valid true if the account is valid.
 */
void PresenceData::accountValidityChanged(const QString &path, bool valid)
{
    Q_UNUSED(valid);
    qDebug() << "accountValidityChanged() called";
    /*
     * slot called when an account has
     * been updated.
     */
    d->createAccountDataSource(path);
}

/**
 * Slot for account removed.
 *
 * \param QDBusObjectPath Name of the account path.
 */
void PresenceData::accountRemoved(const QString &path)
{
    qDebug() << "accountRemoved() called";
    /*
     * slot called when an account has been deleted
     *
     * remove that source.
     */
    d->removeAccountDataSource(path);
}

void PresenceData::pushData(QVariant& v)
{

}

