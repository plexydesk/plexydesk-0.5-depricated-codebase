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
    NickNoNickGiven,
    ErroneusNick,
    NickCollision,
    NickUnavailResource,
    NickRestricted,
} NickResponseType;

typedef enum{
    UserOK,
    UserAlreadyRegistered,
    UserNeedMoreParams
} UserResponseType;

typedef enum{
    ChannelOK,
    BannedFromChannel,
    ChannelNoChannelModes,
    InviteOnlyChannel,
    BadChannelKey,
    ChannelIsFull,
    ChannelBadChannelMask,
    ChannelNoSuchChannel,
    TooManyChannels,
    TooManyTargets,
    Topic,  //topic stored in QString error
    UserList,
    ChannelUnavailResource,
    ChannelNeedMoreParams
} ChannelResponseType;

typedef enum{
    NoRecipient,
    NoTextToSend,
    CannotSendToChannel,
    NoTopLevel,
    WildTopLevel,
//     TooManyTargets,
    MessageNoSuchNick,
    AwayMessage
} MessageResponseType;

typedef enum{
    PartNeedMoreParams,
    PartNoSuchChannel,
    PartNotOnChannel
} PartResponseType;

typedef enum{
    InviteNeedMoreParams,
    InviteNoSuchNick,
    InviteNotOnChannel,
    InviteUserOnChannel,
    InviteChannelOpNeeded,
    InviteOK,
    InviteAwayMessage
} InviteResponseType;

typedef enum{
    KickNeedMoreParams,
    KickNoSuchChannel,
    KickBadChannelMask,
    KickChannelOpNeeded,
    KickNotInChannel,
    KickNotOnChannel
} KickResponseType;

typedef enum{
    WhoisNoNickGiven,
    WhoisNoSuchNick,
    WhoisOK
} WhoisResponseType;

typedef enum{
    Away,
    UnAway
} AwayResponseType;

typedef enum{
    OperNeedMoreParams,
    NoOperHost,
    OperPasswordMismatch,
    OperOK
} OperResponseType;

typedef enum{
} ModeResponseType;

typedef enum{
    TopicNeedMoreParams,
    TopicNotOnChannel,
    TopicChannelOpNeeded,
    TopicNoChannelModes,
    TopicSet,
    TopicNotSet
} TopicResponseType;

typedef enum{
    ListOK
} ListResponseType;

typedef enum{
} NoticeResponseType;

typedef enum{
    NamesUserList
} NamesResponseType;

typedef enum{
    PRIVMSG,
    JOIN,
    PART,
    QUIT,
    KICK,
    NICK
} ResponseType;
class IrcData : public QObject
{
    Q_OBJECT
    public:
        IrcData(QObject *p =0);
        IrcData(QString server, qint16 port);
        bool isConnected();

        /*!
        Asynchonously emits connectResponse(ConnectResponseType response,QString error)
        */
        void connectToServer();

        /*!
        Doesn't emit anything
        \param pass password for a registration, don't comfuse this with NickServ password
        */
        void setPassword(QString pass);

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

        /*!
        Asynchronousy emits messageResponse(MessageResponseType,QString)
        \param channel The channel to which the message is to be delivered or the nick of the person to PVT with
        \param message The message that should be delivered :P
        */
        void writeMessage(QString channel,QString message);

        /*!
        Asynchronousy emits partResponse(PartResponseType,QString)
        \param channel Channel to part from
        \param partMessage Part Message
        */
        void partChannel(QString channel, QString partMessage);

        /*!
        Doesn't emit anything
        \param quitMessage Quit Message
        */
        void quit(QString quitMessage);

        /*!
        Asynchronousy emits inviteResponse(InviteResponseType,QString)
        \param nick Nick to invite
        \param channel Invite to channel
        */
        void invite(QString nick, QString channel);

        /*!
        Asynchronously emits kickResponse(KickResponseType,QString)
        \param channel Which channel to kick from
        \param nick The person to kick
        \param comment Kick comment
        */
        void kick(QString channel,QString nick,QString comment);

        /*!
        Asynchronously emits whoisResponse(WhoisResponseType,QString,User)
        \param nick The nick of who you need whois information
        */
        void whois(QString nick);

        /*!
        Asynchronously emits whoisResponse(WhoisResponseType,QString,User)
        \param nick The nick of who you need whois information
        */
        void away(QString message);

        /*!
        Asynchronously emits operResponse(OperResponseType,QString)
        \param nick The nick of yourself
        \param password Your password
         */
        void oper(QString nick,QString password);

        /*!
        Asynchronously emits modeResponse(ModeResponseType,QString)
        \param nick Nick of the person
        \param mode The mode string he wants to change to, say -o+n
        */
        void mode(QString nick,QString mode);

        /*!
        Asynchronously emits topicResponse(TopicResponseType,QString)
        \param channel The channel whose topic you want to change
        \param topic The new topi you want to set to
        */
        void topic(QString channel,QString topic);

        /*!
        Asynchronously emits listResponse(ListResponseType,QString,QStringList)
        */
        void list();

        /*!
        Asynchronously emits noticeResponse(NoticeResponseType,QString)
        \param target The channel/nick to whom you want to send the notice to
        \param text The actual text you want to send as notice
         */
        void notice(QString target,QString text);

        /*!
        Asynchronously emits namesResponse(NamesResponseType,QString,QStringList)
        \param channel The channel whose user list you want to know
         */
        void names(QString channel);

//         service(QString nickname,QString reserved, QString distribution,QString type,QString reserved,QString info);

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
        \param userList List of all users in that channel, if the ChannelResponseType is OK,
                        OPs start with @ and moderators start with +
                        further for every ChannelResponseType the first string or userList is the channel name
         */
        void channelResponse(ChannelResponseType response,QString error,QStringList userList);

        /*!
        \param response Response code for the request setUser(QString) check UserResponseType (irc.h)
        \param error if error, the error is retured in this, else empty(not null)
        */
        void userResponse(UserResponseType response,QString error);

        /*!
        \param response Response code for the request writeMessage(QString,QString) check UserResponseType (irc.h)
        \param error if error, the error is retured in this, else empty(not null)
         */
        void messageResponse(MessageResponseType response,QString error);

        /*!
        \param response Response code for the request partChannel(QString,QString) check PartResponseType (irc.h)
        \param error if error, the error is retured in this, else empty(not null)
        */
        void partResponse(PartResponseType response,QString error);

        /*!
        \param response Response code for the request invite(QString,QString) check InviteResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
        */
        void inviteResponse(InviteResponseType response,QString error);

        /*!
        \param response Response code for the request kick(QString,QString,QString) check KickResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
        */
        void kickResponse(KickResponseType,QString);

        /*!
        \param response Response code for the request whois(QString) check WhoisResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
        \param message The actual whois reponse ! Nick, User, Host, Real Name, Server, Server Info, Away Message(if away else skipped) ,
        \param channels The channels the user has joined
        */
        void whoisResponse(WhoisResponseType response,QString error,User user);

        /*!
        \param response Response code for the request away(QString) check AwayResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
        */
        void awayResponse(AwayResponseType response,QString error);

        /*!
        \param response Response code for the request oper(QString,QString) check OperResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
         */
        void operResponse(OperResponseType response,QString error);

        /*!
        \param response Response code for the request mode(QString,QString) check ModeResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
         */
        void modeResponse(ModeResponseType response,QString error);

        /*!
        \param response Response code for the request topic(QString,QString) check TopicResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
         */
        void topicResponse(TopicResponseType response,QString error);

        /*!
        \param response Response code for the request list() check ListResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
         */
        void listResponse(ListResponseType response,QString error,QStringList listOfServer);

        /*!
        \param response Response code for the request notice(QString,QString) check NoticeResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
         */
        void noticeResponse(NoticeResponseType response,QString error);

        /*!
        \param response Response code for the request names(QString) check NamesResponseType (irc.h)
        \param error If error, the error is returned int this, else the info for the corresponding response
         */
        void namesResponse(NamesResponseType response,QString error,QStringList namesList);

        /*!
        This response Potentially holds *everything else*. We strongly recommed you to check ResponseType enum!
        \param response The generic response type
        \param arg1 Since this is generic, giving a flexibility to send messages using arguments
        \param arg2 Since this is generic, giving a flexibility to send messages using arguments
        \param arg3 Since this is generic, giving a flexibility to send messages using arguments
        \param arg3 Since this is generic, giving a flexibility to send messages using arguments
        */
        void genericResponse(ResponseType response,QString arg1,QString arg2,QString arg3,QString arg4);

    public slots:

        /*!
        emits connectResponse(ConnectOK,"Connected OK")
        */
        void setConnected();

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
