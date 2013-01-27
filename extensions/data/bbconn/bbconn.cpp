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
#include "sslserver.h"
#include "sslclient.h"

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
    SslServer mServer;
    QList<QSslSocket *> mSocketList;

    QString mCert;
    QString mPrivatekey;
    QList<SSLClient *> mSSLClientList;

};

void BBConnData::startService(const QString &token)
{
    if (d->mClient)
        return;

    d->mClient = new Client(token);
    d->mServer.listen(QHostAddress::Any, 32234);

    connect(&d->mServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));

    connect(d->mClient, SIGNAL(newMessage(QString,QString)),
            this, SLOT(onNewMessage(QString,QString)));

    connect(d->mClient, SIGNAL(newParticipant(QString)),
            this, SLOT(newParticipant(QString)));

    connect(d->mClient, SIGNAL(participantLeft(QString)),
            this, SLOT(participantLeft(QString)));

    connect(d->mClient, SIGNAL(greet(QString, Connection *)),
            this, SLOT(onApprovalRequested(QString, Connection *)));
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
    QStringList hostInfo = message.split(":");

    qDebug() << Q_FUNC_INFO << hostInfo;

    if (hostInfo.count() != 2)
        return;

    SSLClient *client = new SSLClient (this);

    d->mSSLClientList.append(client);

    client->connectToHost(hostInfo[0], hostInfo[1]);
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

void BBConnData::onApprovalRequested(const QString &token, Connection *conn)
{
    bool approval = (token == d->mToken);

    d->mClient->approveGreeting(conn, approval);

    if (approval) {
        if (conn) {
            d->mClient->sendMessage(d->mServer.serverAddress().toString() + ":" + QString::number(d->mServer.serverPort(), 10));
        }
    }
}

void BBConnData::loadKeys()
{
    QString homePath = QDir::homePath();
    QString result;

    QString cert = homePath + QLatin1String("/.plexydesk/plexydesk-server.crt");
    QString privateKey = homePath + QLatin1String("/.plexydesk/plexydesk-private-key.rsa");

    QFileInfo certInfo(cert);
    QFileInfo privateKeyInfo(privateKey);

    if (certInfo.exists() && privateKeyInfo.exists()) {
        d->mCert = cert;
        d->mPrivatekey = privateKey;
    }
}

void BBConnData::acceptConnection()
{
    qDebug() << Q_FUNC_INFO;

    QSslSocket *socket = dynamic_cast<QSslSocket *>(d->mServer.nextPendingConnection());


    // QSslSocket emits the encrypted() signal after the encrypted connection is established
    connect(socket, SIGNAL(encrypted()), this, SLOT(handshakeComplete()));

    // Report any SSL errors that occur
    connect(socket, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(sslErrors(const QList<QSslError> &)));

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionFailure()));

    QString key, certificate;

    loadKeys();

    socket->setPrivateKey(d->mPrivatekey);
    socket->setLocalCertificate(d->mCert);

    socket->setPeerVerifyMode(QSslSocket::QueryPeer);
    socket->startServerEncryption();
}

void BBConnData::handshakeComplete()
{
    QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());

    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

    d->mSocketList.push_back(socket);
}

void BBConnData::sslErrors(const QList<QSslError> &errors)
{
    QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());

    QString errorStrings;

    foreach (QSslError error, errors)
    {
      errorStrings += error.errorString();
      if (error != errors.last())
      {
        errorStrings += ';';
      }
    }

    qDebug() << Q_FUNC_INFO << (QString("[%1] %2:%3 reported the following SSL errors: %4")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
                            .arg(socket->peerAddress().toString())
                            .arg(socket->peerPort())
                            .arg(errorStrings));
}

void BBConnData::receiveMessage()
{
}

void BBConnData::connectionClosed()
{
}

void BBConnData::connectionFailure()
{
    qDebug() << Q_FUNC_INFO;
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
