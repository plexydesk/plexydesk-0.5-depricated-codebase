#include "sslclient.h"

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
    socket.connectToHost(host, port.toLong());

    return true;
}

void SSLClient::connectedToServer()
{
    qDebug() << Q_FUNC_INFO;
}

void SSLClient::sslErrors(const QList<QSslError> &errors)
{
    qDebug() << Q_FUNC_INFO;
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
    qDebug() << Q_FUNC_INFO;
}
