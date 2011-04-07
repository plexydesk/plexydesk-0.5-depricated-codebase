#ifndef USER_IRC_H
#define USER_IRC_H

class User
{
public:
    QString nick;
    QString user;
    QString host;
    QString realName;
    QString server;
    QString serverInfo;
    QString awayMessage;
    QStringList channels;
    QString idleTime;
    bool isAway;
    bool isIdle;
    bool isOP;

    User(QString nick, QString user, QString host, QString realName, QString server, QString serverInfo,
     QString awayMessage, QStringList channels, QString idleTime, bool isAway, bool isIdle, bool isOP) {
        this->nick = nick;
        this->user = user;
        this->host = host;
        this->realName = realName;
        this->server = server;
        this->serverInfo = serverInfo;
        this->awayMessage = awayMessage;
        this->channels = channels;
        this->idleTime = idleTime;
        this->isAway = isAway;
        this->isIdle = isIdle;
        this->isOP = isOP;
    }
    User(){
    }
};

#endif
