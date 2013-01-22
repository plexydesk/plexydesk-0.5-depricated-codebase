/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : *
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
#include "bbconn.h"
#include <desktopwidget.h>
#include <plexyconfig.h>
#include <QTimer>
#include <controllerinterface.h>
#include <QCryptographicHash>

#include "connection.h"
#include "client.h"

class BBConnData::Private
{
public:
    Private() {
    }

    ~Private() {
        if (mClient)
            delete mClient;
    }

    QTimer *mBBConn;
    Client *mClient;
    QString mToken;
};

void BBConnData::startService(const QString &token)
{
    if (d->mClient)
        return;

    d->mClient = new Client(token);

    connect(d->mClient, SIGNAL(newMessage(QString,QString)),
            this, SLOT(onNewMessage(QString,QString)));

    connect(d->mClient, SIGNAL(newParticipant(QString)),
            this, SLOT(newParticipant(QString)));

    connect(d->mClient, SIGNAL(participantLeft(QString)),
            this, SLOT(participantLeft(QString)));

    connect(d->mClient, SIGNAL(greet(QString, Connection *)),
            this, SLOT(onGreet(QString, Connection *)));
}

BBConnData::BBConnData(QObject *object) : PlexyDesk::DataSource(object), d(new Private)
{
    d->mClient = 0;
}

void BBConnData::init()
{
}

BBConnData::~BBConnData()
{
    delete d;
}

void BBConnData::setArguments(QVariant arg)
{
    qDebug() << Q_FUNC_INFO << arg;
    QVariantMap map = arg.toMap();

    if (!map["key"].toString().isEmpty()) {
        d->mToken = map["key"].toString();
        if (!d->mToken.isNull()) {
           d->mToken = encrypt(map["key"].toString());
           startService(d->mToken);
        }
    }
}

void BBConnData::onNewMessage(const QString &from, const QString &message)
{
    qDebug() << "from: " << from << "Message: " << message;
}

void BBConnData::newParticipant(const QString &nick)
{
    qDebug() << Q_FUNC_INFO;
    QVariant timeVariant;
    QVariantMap dataMap;

    timeVariant.setValue(QTime::currentTime());
    dataMap["state"] = "new";
    dataMap["Nick"] = nick;

    Q_EMIT sourceUpdated(dataMap);
}

void BBConnData::participantLeft(const QString &nick)
{
}

QString BBConnData::encrypt(const QString &token) const
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(token.toUtf8());
    qDebug () << Q_FUNC_INFO << "Token" << (QString(hash.result().toHex()));

    return QString(hash.result().toHex());
}

void BBConnData::onGreet(const QString &token, Connection *conn)
{
    qDebug() << Q_FUNC_INFO << "-----" << token;

    bool approval = (token == d->mToken);
    d->mClient->approveGreeting(conn, approval);
}

QVariantMap BBConnData::readAll()
{
    QVariant timeVariant;
    QVariantMap dataMap;

    timeVariant.setValue(QTime::currentTime());
    dataMap["currentTime"] = timeVariant;

    return dataMap;
}

void BBConnData::timerEvent(QTimerEvent *event)
{
    //Q_EMIT sourceUpdated(readAll());
}
