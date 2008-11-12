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

void Tester::operHandler(OperResponseType response,QString error)
{
    qDebug() << error ;
}

void Tester::namesHandler(NamesResponseType response,QString error ,QStringList list)
{
    qDebug() << list;
}

void Tester::listHandler(ListResponseType response,QString error,QStringList list)
{
    qDebug() << error ;
}

void Tester::topicHandler(TopicResponseType response,QString error)
{
    qDebug() << error ;
}

void Tester::noticeHandler(NoticeResponseType response,QString error)
{
    qDebug() << error ;
}

void Tester::genericHandler(ResponseType response,QString arg1,QString arg2,QString arg3,QString arg4)
{
    qDebug() << arg1 << " " << arg2 << " " << arg3 << " " << arg4;
}

Tester::Tester(QObject *p) : QObject(p)
{
    Test = 0;
    IrcData *irc = new IrcData("64.161.254.20",6667);
    irc->connectToServer();
    connect(irc,SIGNAL(connectResponse(ConnectResponseType,QString)),
            SLOT(connectHandler(ConnectResponseType,QString)));
    irc->setNick("sharpBot");
    connect(irc,SIGNAL(nickResponse(NickResponseType,QString)),SLOT(nickHandler(NickResponseType,QString)));

    irc->setUser("sharpBot", 7, "*", "Mani Shankar's BOT");
    connect(irc,SIGNAL(userResponse(UserResponseType,QString)),SLOT(userHandler(UserResponseType,QString)));

    irc->joinChannel("#plexydesk");
    irc->joinChannel("#fosskriti");
    connect(irc,SIGNAL(channelResponse(ChannelResponseType,QString,QStringList)),
            SLOT(channelHandler(ChannelResponseType,QString,QStringList)));

//     irc->oper("sharpBot","jajdasdas");
//     connect(irc,SIGNAL(operResponse(OperResponseType,QString)),SLOT(operHandler(OperResponseType,QString)));

//     irc->names("#plexydesk");
//     connect(irc,SIGNAL(namesResponse(NamesResponseType,QString,QStringList)),SLOT(namesHandler(NamesResponseType,QString,QStringList)));

//     irc->away("Yay!");
//     connect(irc,SIGNAL(awayResponse(AwayResponseType,QString)),SLOT(awayHandler(AwayResponseType,QString)));

    irc->writeMessage("#fosskriti", "Hi Folks, I'm sharp's BOT!");
    irc->writeMessage("#fosskriti", "And now I seem to work");
//     irc->writeMessage("zakirs", "Hi Folks, Our IRC engine works!");
//     irc->writeMessage("sira", "Hi Folks, Our IRC engine works!");
//     irc->writeMessage("sadasdasd", "Hi Folks, Our IRC engine works!");
    connect(irc,SIGNAL(messageResponse(MessageResponseType,QString)),SLOT(messageHandler(MessageResponseType,QString)));

//     irc->invite("dA_ShArP","#plexydesk");
//     connect(irc,SIGNAL(inviteResponse(InviteResponseType,QString)),SLOT(inviteHandler(InviteResponseType,QString)));

//     irc->kick("#plexydesk","dA_ShArP", "Even Bots can Kick!");
//     connect(irc,SIGNAL(kickResponse(KickResponseType,QString)),SLOT(kickHandler(KickResponseType,QString)));

//     irc->whois("siraj");
//     connect(irc,SIGNAL(whoisResponse(WhoisResponseType,QString,User)),SLOT(whoisHandler(WhoisResponseType,QString,User)));

//     irc->partChannel("#plexydesk", "Even Bots can type PART messages!");
//     connect(irc,SIGNAL(partResponse(PartResponseType,QString)),SLOT(partHandler(PartResponseType,QString)));

//     irc->quit("Even BOTs can give Quit messages!");

    connect(irc,SIGNAL(genericResponse(ResponseType,QString,QString,QString,QString)),SLOT(genericHandler(ResponseType,QString,QString,QString,QString)));
}

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    Tester *test = new Tester();
    app.exec();
}
