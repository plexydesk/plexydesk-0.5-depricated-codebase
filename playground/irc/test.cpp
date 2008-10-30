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
    connect(irc,SIGNAL(channelResponse(ChannelResponseType,QString,QStringList)),
            SLOT(channelHandler(ChannelResponseType,QString,QStringList)));

//     irc->writeMessage("#plexydesk", "Hi Folks, Our IRC engine works!");
//     irc->writeMessage("zakirs", "Hi Folks, Our IRC engine works!");
//     irc->writeMessage("sira", "Hi Folks, Our IRC engine works!");
    irc->writeMessage("dA_ShArP", "Hi Folks, Our IRC engine works!");
    connect(irc,SIGNAL(messageResponse(MessageResponseType,QString)),SLOT(messageHandler(MessageResponseType,QString)));

    irc->partChannel("#plexydesk", "Even Bots can type PART messages!");
    connect(irc,SIGNAL(partResponse(PartResponseType,QString)),SLOT(partHandler(PartResponseType,QString)));
}

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    Tester *test = new Tester();
    app.exec();
}
