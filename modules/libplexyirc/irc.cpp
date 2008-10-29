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

    connect(service,SIGNAL(error(QAbstractSocket::SocketError)),SIGNAL(errorHandler(QAbstractSocket::SocketError)));
    connect(service,SIGNAL(connected()),SLOT(setConnected()));
    connect(service,SIGNAL(connected()),SLOT(init()));
    connect(service,SIGNAL(readyRead()),SLOT(parse()));
}

void IrcData::setNick(QString nick)
{
    service->write(QString("NICK %1\r\n").arg(nick).toAscii());
}

void IrcData::setUser(QString user,qint16 mode,QString unused,QString realName)
{
    service->write(QString("USER %1 %2 %3 :%4\r\n").arg(user).arg(mode).arg(unused).arg(realName).toAscii());
}

void IrcData::joinChannel(QString channel)
{
    service->write(QString("JOIN %1\r\n").arg(channel).toAscii());
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
    QString host;
    int i=1;    // to retrieve first 5 reponse lines
    while(1){
        ba = service->readLine();
        if(!ba.isEmpty()){
            currentLine = new QString(ba);
            QString arg1;
            QString arg2;
            QString arg3;
            if((currentLine->left(1)).compare(":")==0){
                currentLine->remove(0,1);   // remove the prefix :
                QRegExp argRegExp("([^\\s]*)[\\s].*");
                int pos = argRegExp.indexIn(*currentLine);
                if(pos>-1){
                    arg1 = argRegExp.cap(1);
                }
                QRegExp restRegExp("[^\\s]*[\\s](.*)");
                pos = restRegExp.indexIn(*currentLine);
                if(pos>-1){
                    *currentLine = restRegExp.cap(1);
                }
                pos = argRegExp.indexIn(*currentLine);
                if(pos>-1){
                    arg2 = argRegExp.cap(1);
                }
                if(i==1){
                    host = arg1;
                }
                if( arg1.compare(host) == 0)
                {
                    switch(arg2.toInt()){
                        case 433: emit nickResponse(DuplicateNick,"Duplicate Nick");
                                    i++;
                                    break;
                        case 451: emit ;//notRegistered();
                                    i++;
                                    break;
//                         case 3: break;
//                                     i++;
//                         case 4: break;
//                                     i++;
//                         case 5: break;
//                                     i++;
                        default: break;
                    }
                }
//                 emit sample(arg1);
            }
        }
        else break;
    }
}

void IrcData::errorHandler(QAbstractSocket::SocketError err)
{
    switch(err){
        case QAbstractSocket::ConnectionRefusedError:
            emit(connectResponse(ConnectionRefusedError,"ConnectionRefusedError")); break;
        case QAbstractSocket::RemoteHostClosedError:
            emit(connectResponse(RemoteHostClosedError,"RemoteHostClosedError")); break;
        case QAbstractSocket::HostNotFoundError:
            emit(connectResponse(HostNotFoundError,"HostNotFoundError")); break;
        case QAbstractSocket::SocketAccessError:
            emit(connectResponse(SocketAccessError,"SocketAccessError")); break;
        case QAbstractSocket::SocketResourceError:
            emit(connectResponse(SocketResourceError,"SocketResourceError")); break;
        case QAbstractSocket::SocketTimeoutError:
            emit(connectResponse(SocketTimeoutError,"SocketTimeoutError")); break;
        case QAbstractSocket::DatagramTooLargeError:
            emit(connectResponse(DatagramTooLargeError,"DatagramTooLargeError")); break;
        case QAbstractSocket::NetworkError:
            emit(connectResponse(NetworkError,"NetworkError")); break;
        case QAbstractSocket::AddressInUseError:
            emit(connectResponse(AddressInUseError,"AddressInUseError")); break;
        case QAbstractSocket::SocketAddressNotAvailableError:
            emit(connectResponse(SocketAddressNotAvailableError,"SocketAddressNotAvailableError")); break;
        case QAbstractSocket::UnsupportedSocketOperationError:
            emit(connectResponse(UnsupportedSocketOperationError,"UnsupportedSocketOperationError")); break;
        case QAbstractSocket::ProxyAuthenticationRequiredError:
            emit(connectResponse(ProxyAuthenticationRequiredError,"ProxyAuthenticationRequiredError")); break;
        case QAbstractSocket::SslHandshakeFailedError: 
            emit(connectResponse(SslHandshakeFailedError,"SslHandshakeFailedError")); break;
        case QAbstractSocket::UnknownSocketError: 
            emit(connectResponse(UnknownSocketError,"UnknownSocketError")); break;
        case QAbstractSocket::UnfinishedSocketOperationError:
            emit(connectResponse(UnfinishedSocketOperationError,"UnfinishedSocketOperationError")); break;
        default: break;
    }
}

void IrcData::setConnected()
{
    emit(connectResponse(ConnectOK,"Connected OK"));
    Connected = 1;
}

bool IrcData::isConnected()
{
    return Connected;
}


