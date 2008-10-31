#include <QApplication>
#include "test.h"

void Tester::connectHandler(ConnectResponseType response,QString error)
{
    if(response == ConnectOK)
        qDebug() << error;
    else
    {
        qDebug() << error;
    }
}

void Tester::userHandler(UserResponseType response,QString error)
{
    if(response == UserOK)
        qDebug() << error;
    else
    {
        qDebug() << error;
    }
}

void Tester::nickHandler(NickResponseType response,QString error)
{
    switch(response){
        case NickOK: qDebug() << error;
                break;
        case NickInUse: qDebug() << error;
                break;
        default: break;
    }
}

void Tester::channelHandler(ChannelResponseType response,QString error,QStringList userList)
{
    if(response == UserList)
        qDebug() << userList;
    else
    {
        qDebug() << error << " :"<<userList.takeAt(0);
    }
}

void Tester::messageHandler(MessageResponseType response,QString error)
{
    if(response == AwayMessage)
        qDebug() << error;
    else
    {
        qDebug() << error ;
    }
}

void Tester::partHandler(PartResponseType response,QString error)
{
    qDebug() << error ;
}

void Tester::inviteHandler(InviteResponseType response,QString error)
{
    qDebug() << error ;
}

void Tester::kickHandler(KickResponseType response,QString error)
{
    qDebug() << error ;
}

void Tester::whoisHandler(WhoisResponseType response,QString error,User who)
{
    qDebug() << error ;
    qDebug() << who.nick << " " << who.user << " " << who.host << " " << who.realName << " " << who.server << " " << who.serverInfo << " " << who.awayMessage << " " << who.channels ;
}

void Tester::awayHandler(AwayResponseType response,QString error)
{
    qDebug() << error ;
}

Tester::Tester(QObject *p) : QObject(p)
{
    Test = 0;
    IrcData *irc = new IrcData("64.161.254.20",6667);
    irc->connectToServer();
    connect(irc,SIGNAL(connectResponse(ConnectResponseType,QString)),
            SLOT(connectHandler(ConnectResponseType,QString)));
    irc->setNick("spamBot");
    connect(irc,SIGNAL(nickResponse(NickResponseType,QString)),SLOT(nickHandler(NickResponseType,QString)));

    irc->setUser("spamBot", 7, "*", "Mani Shankar's BOT");
    connect(irc,SIGNAL(userResponse(UserResponseType,QString)),SLOT(userHandler(UserResponseType,QString)));

    irc->joinChannel("#plexydesk");
    connect(irc,SIGNAL(channelResponse(ChannelResponseType,QString,QStringList)),
            SLOT(channelHandler(ChannelResponseType,QString,QStringList)));

//     irc->away("Yay!");
//     connect(irc,SIGNAL(awayResponse(AwayResponseType,QString)),SLOT(awayHandler(AwayResponseType,QString)));

//     irc->writeMessage("#plexydesk", "Hi Folks, Our IRC engine works!");
//     irc->writeMessage("zakirs", "Hi Folks, Our IRC engine works!");
//     irc->writeMessage("sira", "Hi Folks, Our IRC engine works!");
//     irc->writeMessage("sadasdasd", "Hi Folks, Our IRC engine works!");
//     connect(irc,SIGNAL(messageResponse(MessageResponseType,QString)),SLOT(messageHandler(MessageResponseType,QString)));

//     irc->invite("dA_ShArP","#plexydesk");
//     connect(irc,SIGNAL(inviteResponse(InviteResponseType,QString)),SLOT(inviteHandler(InviteResponseType,QString)));

//     irc->kick("#plexydesk","dA_ShArP", "Even Bots can Kick!");
//     connect(irc,SIGNAL(kickResponse(KickResponseType,QString)),SLOT(kickHandler(KickResponseType,QString)));

//     irc->whois("siraj");
//     connect(irc,SIGNAL(whoisResponse(WhoisResponseType,QString,User)),SLOT(whoisHandler(WhoisResponseType,QString,User)));

//     irc->partChannel("#plexydesk", "Even Bots can type PART messages!");
//     connect(irc,SIGNAL(partResponse(PartResponseType,QString)),SLOT(partHandler(PartResponseType,QString)));

//     irc->quit("Even BOTs can give Quit messages!");
}

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    Tester *test = new Tester();
    app.exec();
}
