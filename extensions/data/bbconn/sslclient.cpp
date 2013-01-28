#include "sslclient.h"
#include <QMessageBox>
#include <QHostAddress>

SSLClient::SSLClient(QObject *parent) :
    QObject(parent)
{
    connect(&socket, SIGNAL(encrypted()),
            this, SLOT(connectedToServer()));
    connect(&socket, SIGNAL(sslErrors(const QList<QSslError> &)),
            this, SLOT(sslErrors(const QList<QSslError> &)));
    connect(&socket, SIGNAL(disconnected()),
            this, SLOT(connectionClosed()));
    connect(&socket, SIGNAL(readyRead()),
            this, SLOT(receiveMessage()));
    connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError()));
}

bool SSLClient::connectToHost(const QString &host, const QString &port)
{
    socket.connectToHostEncrypted(host, port.toLong(), host);

    return true;
}

void SSLClient::connectedToServer()
{
    qDebug() << Q_FUNC_INFO;
}

void SSLClient::sslErrors(const QList<QSslError> &errors)
{
    Q_FOREACH (QSslError error, errors) {
        if (error.error() == QSslError::SelfSignedCertificate) {
            QMessageBox::StandardButton result =
                    QMessageBox::question(0,"SSL Error",
                                          QString("PlexyDesk is trying to connect to %1 with a Self Signed Certificate, Approve ?").
                                          arg(socket.peerAddress().toString()),
                                          QMessageBox::Yes | QMessageBox::No);

            if (result == QMessageBox::Yes)
                socket.ignoreSslErrors();
            return;
        }
    }
}

void SSLClient::receiveMessage()
{
    qDebug() << Q_FUNC_INFO;
}

void SSLClient::connectionClosed()
{
    qDebug() << Q_FUNC_INFO;
}

void SSLClient::socketError()
{
    qDebug() << Q_FUNC_INFO << socket.errorString();
    socket.close();
}
