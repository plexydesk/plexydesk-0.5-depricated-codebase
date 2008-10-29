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
    proxy.setHostName("vsnlproxy.iitk.ac.in");
    proxy.setPort(3128);
    proxy.setUser("shankar");
    proxy.setPassword("LINKINpark");

    service->setProxy(proxy);
    service->connectToHost(server,port);

    connect(service,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(errorHandler(QAbstractSocket::SocketError)));
    connect(service,SIGNAL(connected()),SLOT(setConnected()));

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

// void IrcData::init()
// {
//     service->write("NICK sharpBot\r\n");
//     service->write("USER sharpBot 7 * :Mani Shankar's BOT\r\n");
//     service->write("JOIN #plexydesk\r\n");
// }

int i=1;
void IrcData::parse()
{
    char buffer[1024];
    char tempBuffer[1024];
    bool append;
    QString *currentLine;
    QString *host;
    while(service->readLine(buffer,sizeof(buffer)) > 0){
        if(QString(buffer).endsWith("\r\n")){
            if(append){
                currentLine = new QString(tempBuffer);
                currentLine->append(QString(buffer));
                append = 0;
            }
            else {
                currentLine = new QString(buffer);
            }
        }
        else{
            for(int i=0; i<1024;i++)
                tempBuffer[i] = buffer[i];
            append = 1;
            continue;
        }
        QString *arg1;
        QString *arg2;
        QString *arg3;
        if((currentLine->left(1)).compare(":")==0){
                currentLine->remove(0,1);   // remove the prefix :
                QRegExp argRegExp("([^\\s]*)[\\s].*");
                int pos = argRegExp.indexIn(*currentLine);
                if(pos>-1){
                    arg1 = new QString(argRegExp.cap(1));
                }
                QString *restLine;
                QRegExp restRegExp("[^\\s]*[\\s](.*)");
                pos = restRegExp.indexIn(*currentLine);
                if(pos>-1){
                    restLine = new QString(restRegExp.cap(1));
                }
                pos = argRegExp.indexIn(*restLine);
                if(pos>-1){
                    arg2 = new QString(argRegExp.cap(1));
                }
                if(i==4 || i==5){
                    host = new QString(*arg1);
                }
                if( arg1->compare(*host) == 0)
                {
                    switch(arg2->toInt()){
                        case 1: emit userResponse(UserOK, "User OK");
                        emit nickResponse(NickOK, "Nick OK");
                        break;
                        case 431: emit nickResponse(NoNickGiven,"No Nick Given");
                        break;
                        case 433: emit nickResponse(NickInUse,"Nick in Use");
                        break;
                        case 462: emit userResponse(UserAlreadyRegistered,"User already registered");
                        break;
                        default: break;
                    }
                }
        }
        i++;
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
    emit(connectResponse(ConnectOK,"Connect OK"));
    Connected = 1;
}

bool IrcData::isConnected()
{
    return Connected;
}


