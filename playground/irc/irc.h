#include <QtCore>
#include <QTcpSocket>
#include "user.h"

#ifndef IRC_H
#define IRC_H

typedef enum{
    ConnectOK,
    ConnectionRefusedError,
    RemoteHostClosedError,
    HostNotFoundError,
    SocketAccessError,
    SocketResourceError,
    SocketTimeoutError,
    DatagramTooLargeError,
    NetworkError,
    AddressInUseError,
    SocketAddressNotAvailableError,
    UnsupportedSocketOperationError,
    ProxyAuthenticationRequiredError,
    SslHandshakeFailedError,
    UnknownSocketError,
    UnfinishedSocketOperationError
} ConnectResponseType;

typedef enum{
    NickOK,
    NickInUse,
    NoNickGiven,
    ErroneusNick,
    NickCollision,
    UnavailResource,
    Restricted
} NickResponseType;

typedef enum{
    UserNeedMoreParams,
    UserAlreadyRegistered
} UserResponseType;

typedef enum{
    ChannelOK,
    PrivateChannel,
    InviteRequired
} ChannelResponseType;

class IrcData : public QObject
{
    Q_OBJECT
    public:
        IrcData(QObject *p =0);
        IrcData(QString server, qint16 port);
        bool isConnected();
        void setConnected();

        /*!
        Asynchonously emits connectResponse(ConnectResponseType response,QString error)
        */
        void connectToServer();

        /*!
        Asynchronously emits nickResponce(NickResponseType)
        Here you have to note that if you send multiple setNick() request
        you will not be able to distinguish between the response signals
        So, better send one setNick() request at a time, don't spam :)
        \param nick Nick to set
        */
        void setNick(QString nick);

        /*!
        Asynchronously emits channelResponse(ChannelResponseType,QString,QStringList)
        \param channelName Channel name to join
        */
        void joinChannel(QString channelName);

        /*!
        Asynchronousy emits userResponse(UserResponseType,QString)
        \param user Username
        \param mode mode to set by default
        \param unused unused :)
        \param realName Real Name of the User
        */
        void setUser(QString user,qint16 mode,QString unused,QString realName);

    signals:

        /*!
        \param error Socket Error
        */
        void error(QAbstractSocket::SocketError error);

        /*!
        \param response Response code for the request connectToServer() check ConnectResponseType (irc.h)
        \param error if error, the error is retured in this, else empty(not null)
        */
        void connectResponse(ConnectResponseType response,QString error);

        /*!
        \param response Response code for the request setNick(QString) check NickResponseType (irc.h)
        \param error if error, the error is retured in this, else empty(not null)
        */
        void nickResponse(NickResponseType response,QString error);

        /*!
        \param response Response code for the request joinChannel(QString), check ChannelResponseType (irc.h)
        \param error if error, the error is retured in this, else empty(not null)
        \param channelName Since the reponse signals are asynchronous, to identify which channel emitted this response
        \param userList List of all users in that channel, if the ChannelResponseType is OK
         */
        void channelResponse(ChannelResponseType response,QString error,QString channelName ,QList<User> userList);

        /*!
        \param response Response code for the request setUser(QString) check UserResponseType (irc.h)
        \param error if error, the error is retured in this, else empty(not null)
        */
        void userResponse(UserResponseType response,QString error);

    public slots:

        void init();

        /*!
        handle socket errors and emit connectResponse()
        */
        void errorHandler(QAbstractSocket::SocketError);

        /*!
        Parse the server response and emit signals accordingly, this is where the Asynchronous-ness starts :)
        */
        void parse();

    private:
        QTcpSocket *service;
        QString server;
        qint16 port;
        bool Connected;
};
#endif
