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

void IrcData::writeMessage(QString channel,QString message)
{
    service->write(QString("PRIVMSG %1 :%2\r\n").arg(channel).arg(message).toAscii());
}

void IrcData::partChannel(QString channel,QString message)
{
    service->write(QString("PART %1 :%2\r\n").arg(channel).arg(message).toAscii());
}

// void IrcData::init()
// {
//     service->write("NICK sharpBot\r\n");
//     service->write("USER sharpBot 7 * :Mani Shankar's BOT\r\n");
//     service->write("JOIN #plexydesk\r\n");
// }

int i=1;
int j =0;
int listFinish = 1;
QStringList empty;
bool append = 0;
char buffer[1024];
char tempBuffer[1024];
void IrcData::parse()
{
    QString *currentLine;
    QString *host;
//     qDebug() << service->readAll();
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
        QString *arg4;
        QString *arg5;
//         qDebug() << *currentLine;
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
//                     qDebug() << *arg2;
                    switch(arg2->toInt()){
                        case 1: 
                            emit userResponse(UserOK, "User OK");
                            emit nickResponse(NickOK, "Nick OK");
                            break;
                        case 301:
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            emit messageResponse(AwayMessage,*restLine);
                            break;
                        case 332:
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            restLine->remove(0,1);
                            emit channelResponse(Topic,*restLine,empty << *arg4);
                            break;
                        case 353:
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg5 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            restLine->remove(0,1);
                            restLine->remove("\r\n");
//                             qDebug() << QString("%1 %2 %3 %4 %5 :").arg(*arg1).arg(*arg2).arg(*arg3).arg(*arg4).arg(*arg5);
//                             while(j<10){
                            while(restLine->compare("")!=0 || restLine->compare(" ")!=0){
                                pos = argRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    empty << QString(argRegExp.cap(1));
//                                     qDebug() << QString(argRegExp.cap(1));
                                }
                                pos = restRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    restLine = new QString(restRegExp.cap(1));
//                                     qDebug() << *restLine;
                                }
                                else {break;}
                                j++;
                            }
//                             qDebug() << empty << "HHHH******";
                            break;
                        case 366:
                            listFinish = 1;
                            emit channelResponse(UserList,"user List",empty);
//                             qDebug() << empty;
                            break;
                        case 401: 
//                             pos = restRegExp.indexIn(*restLine);
//                             if(pos>-1){
//                                 restLine = new QString(restRegExp.cap(1));
//                             }
//                             pos = argRegExp.indexIn(*restLine);
//                             if(pos>-1){
//                                 arg3 = new QString(argRegExp.cap(1));
//                             }
//                             pos = restRegExp.indexIn(*restLine);
//                             if(pos>-1){
//                                 restLine = new QString(restRegExp.cap(1));
//                             }
//                             pos = argRegExp.indexIn(*restLine);
//                             if(pos>-1){
//                                 arg4 = new QString(argRegExp.cap(1));
//                             }
                            emit messageResponse(NoSuchNick,"No Such Nick");
                            break;
                        case 403: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(NoSuchChannel,"No Such Channel",empty << *arg4);
                            break;
                        case 404: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit messageResponse(CannotSendToChannel,QString("Cannot Send To Channel :%1").arg(*arg4));
                            break;
                        case 405:
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(TooManyChannels,"Too Many Channels",empty << *arg4);
                            break;
                        case 407: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(TooManyTargets,"Too Many Channels",empty << *arg4);
                            break;
                        case 411: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            restLine->remove(0,1);
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit messageResponse(NoRecipient,QString("No Recipient :%1").arg(*arg4));
                            break;
                        case 412:
                            emit messageResponse(NoTextToSend,"No Text To Send");
                            break;
                        case 413:
                            emit messageResponse(NoTopLevel,"No Top Level");
                            break;
                        case 414:
                            emit messageResponse(WildTopLevel,"Wild Top Level");
                            break;
                        case 431: 
                            emit nickResponse(NoNickGiven,"No Nick Given");
                            break;
                        case 432: 
                            emit nickResponse(ErroneusNick,"Erroneus Nick");
                            break;
                        case 433: 
                            emit nickResponse(NickInUse,"Nick in Use");
                            break;
                        case 436: 
                            emit nickResponse(NickCollision,"Nick Collision");
                            break;
                        case 437:
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            if(arg4->startsWith("#")){
                                pos = restRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    restLine = new QString(restRegExp.cap(1));
                                }
                                pos = argRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    arg3 = new QString(argRegExp.cap(1));
                                }
                                pos = restRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    restLine = new QString(restRegExp.cap(1));
                                }
                                pos = argRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    arg4 = new QString(argRegExp.cap(1));
                                }
                                emit channelResponse(ChannelUnavailResource,"Channel Unavailable Resource",empty << *arg4);
                            }
                            else
                                emit nickResponse(NickUnavailResource,"Nick Unavailable Resource");
                            break;
                        case 442:
                            emit partResponse(PartNotInChannel,"Part Not In Channel");
                            break;
                        case 461: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            if(arg4->compare("USER")==0)
                                emit userResponse(UserNeedMoreParams,"User Need More Params");
                            if(arg4->compare("JOIN")==0){
                                pos = restRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    restLine = new QString(restRegExp.cap(1));
                                }
                                pos = argRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    arg3 = new QString(argRegExp.cap(1));
                                }
                                pos = restRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    restLine = new QString(restRegExp.cap(1));
                                }
                                pos = argRegExp.indexIn(*restLine);
                                if(pos>-1){
                                    arg4 = new QString(argRegExp.cap(1));
                                }
                                emit channelResponse(ChannelNeedMoreParams,"Channel Need More Params",empty << *arg4);
                            }
                            if(arg4->compare("PART")==0)
                                emit partResponse(PartNeedMoreParams,"Part Need More Params");
                            break;
                        case 462: 
                            emit userResponse(UserAlreadyRegistered,"User already registered");
                            break;
                        case 471: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(ChannelIsFull,"Channel Is Full",empty << *arg4);
                            break;
                        case 473: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(InviteOnlyChannel,"Invite Only Channel",empty << *arg4);
                            break;
                        case 474: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(BannedFromChannel,"Banned From Channel",empty << *arg4);
                            break;
                        case 475: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(BadChannelKey,"Bad Channel Key",empty << *arg4);
                            break;
                        case 476: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(BadChannelMask,"Bad Channel Mask",empty << *arg4);
                            break;
                        case 477: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(NoChannelModes,"No Channel Modes",empty << *arg4);
                            break;
                        case 478: 
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg3 = new QString(argRegExp.cap(1));
                            }
                            pos = restRegExp.indexIn(*restLine);
                            if(pos>-1){
                                restLine = new QString(restRegExp.cap(1));
                            }
                            pos = argRegExp.indexIn(*restLine);
                            if(pos>-1){
                                arg4 = new QString(argRegExp.cap(1));
                            }
                            emit channelResponse(BannedFromChannel,"Banned From Channel",empty << *arg4);
                            break;
                        case 484:
                            emit nickResponse(NickRestricted,"Nick Restricted");
                            break;
                        default: 
                            break;
                    }
                }
        }
        else{
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

            if(arg1->compare("PING")==0){
                pos = argRegExp.indexIn(*restLine);
                if(pos>-1){
                    arg2 = new QString(argRegExp.cap(1));
                }
                arg2->remove(0,1);
                service->write(QString("PONG %1").arg(*arg2).toAscii());
            }
            else if(arg1->compare("NOTICE")==0){
                pos = argRegExp.indexIn(*restLine);
                if(pos>-1){
                    arg2 = new QString(argRegExp.cap(1));
                }
                pos = restRegExp.indexIn(*restLine);
                if(pos>-1){
                    restLine = new QString(restRegExp.cap(1));
                }
                restLine->remove(0,1);
                qDebug() << QString("%1 %2 :%3").arg(*arg1).arg(*arg2).arg(*restLine);
            }
        }
        i++;
    } // End While
}

void IrcData::errorHandler(QAbstractSocket::SocketError err)
{
    switch(err){
        case QAbstractSocket::ConnectionRefusedError:
            emit(connectResponse(ConnectionRefusedError,"ConnectionRefusedError"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            emit(connectResponse(RemoteHostClosedError,"RemoteHostClosedError"));
            break;
        case QAbstractSocket::HostNotFoundError:
            emit(connectResponse(HostNotFoundError,"HostNotFoundError"));
            break;
        case QAbstractSocket::SocketAccessError:
            emit(connectResponse(SocketAccessError,"SocketAccessError"));
            break;
        case QAbstractSocket::SocketResourceError:
            emit(connectResponse(SocketResourceError,"SocketResourceError"));
            break;
        case QAbstractSocket::SocketTimeoutError:
            emit(connectResponse(SocketTimeoutError,"SocketTimeoutError"));
            break;
        case QAbstractSocket::DatagramTooLargeError:
            emit(connectResponse(DatagramTooLargeError,"DatagramTooLargeError"));
            break;
        case QAbstractSocket::NetworkError:
            emit(connectResponse(NetworkError,"NetworkError"));
            break;
        case QAbstractSocket::AddressInUseError:
            emit(connectResponse(AddressInUseError,"AddressInUseError"));
            break;
        case QAbstractSocket::SocketAddressNotAvailableError:
            emit(connectResponse(SocketAddressNotAvailableError,"SocketAddressNotAvailableError"));
            break;
        case QAbstractSocket::UnsupportedSocketOperationError:
            emit(connectResponse(UnsupportedSocketOperationError,"UnsupportedSocketOperationError"));
            break;
        case QAbstractSocket::ProxyAuthenticationRequiredError:
            emit(connectResponse(ProxyAuthenticationRequiredError,"ProxyAuthenticationRequiredError"));
            break;
        case QAbstractSocket::SslHandshakeFailedError: 
            emit(connectResponse(SslHandshakeFailedError,"SslHandshakeFailedError"));
            break;
        case QAbstractSocket::UnknownSocketError: 
            emit(connectResponse(UnknownSocketError,"UnknownSocketError"));
            break;
        case QAbstractSocket::UnfinishedSocketOperationError:
            emit(connectResponse(UnfinishedSocketOperationError,"UnfinishedSocketOperationError"));
            break;
        default: 
            break;
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


