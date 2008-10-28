#include "irc.h"
#include <QApplication>
#include <QNetworkProxy>

IrcData::IrcData(QObject *p) : QObject(p)
{
    this->Connected = 0;
}

IrcData::IrcData(QString server_arg, qint16 port_arg)
{
    Connected = 0;
    server = server_arg;
    port = port_arg;
}

void IrcData::connectToServer()
{
    service = new QTcpSocket();

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("bsnlproxy.iitk.ac.in");
    proxy.setPort(3128);
    proxy.setUser("shankar");
    proxy.setPassword("LINKINpark");

    service->setProxy(proxy);
    service->connectToHost(server,port);
    service->connectToHost(server,port);
    connect(service,SIGNAL(error(QAbstractSocket::SocketError)),SIGNAL(error(QAbstractSocket::SocketError)));
    connect(service,SIGNAL(connected()),SIGNAL(connected()));
    connect(service,SIGNAL(connected()),SLOT(init()));
    connect(service,SIGNAL(connected()),SLOT(setConnected()));
    connect(service,SIGNAL(readyRead()),SLOT(parse()));
}

void IrcData::init()
{
    service->write("NICK spamBOT_sharp\r\n");
    service->write("USER spamBOT_sharp localhost localhost :spamBOT_sharp\r\n");
    service->write("JOIN #spamBOT_sharp\r\n");
}

void IrcData::parse()
{
    QByteArray ba;
    QString* currentLine;
    while(1){
        ba = service->readLine();
        if(!ba.isEmpty()){
            currentLine = new QString(ba);
            if((currentLine->left(1)).compare(":")==0){
                currentLine->remove(0,1);
                QString host = currentLine->left(currentLine->indexOf(" "));
                currentLine->remove(host,Qt::CaseSensitive);
                emit sample(host);
            }
        }
        else break;
    }
}

// void IrcData::setNick(QString nick)
// {
//     service->write("");
// }

void IrcData::ErrorHandler(QAbstractSocket::SocketError err)
{
//     *i = err;
}

void IrcData::setConnected()
{
//     this->Connected = 1;
    qDebug("Connected called from irc");
}

bool IrcData::isConnected()
{
    return Connected;
}


