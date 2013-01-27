#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <QObject>
#include <QSslSocket>

class SSLClient : public QObject
{
    Q_OBJECT
public:
    explicit SSLClient(QObject *parent = 0);
    
signals:
    
public Q_SLOTS:
    void connectedToServer();
    void sslErrors(const QList<QSslError> &errors);
    void receiveMessage();
    void connectionClosed();
    void socketError();
    
private:
    QSslSocket socket;
};

#endif // SSLCLIENT_H
