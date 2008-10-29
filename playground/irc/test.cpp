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
        case NickInUse: qDebug() << "Duplicate Nick";
                break;
        default: break;
    }
}

Tester::Tester(QObject *p) : QObject(p)
{
    Test = 0;
    IrcData *irc = new IrcData("64.161.254.20",6667);
    irc->connectToServer();
    connect(irc,SIGNAL(connectResponse(ConnectResponseType,QString)),SLOT(connectHandler(ConnectResponseType,QString)));
    irc->setNick("sharpBot");
    connect(irc,SIGNAL(nickResponse(NickResponseType,QString)),SLOT(nickHandler(NickResponseType,QString)));
    irc->setUser("sharpBot", 7, "*", "Mani Shankar's BOT");
    connect(irc,SIGNAL(userResponse(UserResponseType,QString)),SLOT(userHandler(UserResponseType,QString)));
    irc->joinChannel("#plexy");
}

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    Tester *test = new Tester();
    app.exec();
}
