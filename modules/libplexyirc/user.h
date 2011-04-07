#ifndef USER_IRC_H
#define USER_IRC_H

class User
{
public:
    QString nick;
    QString user;
    QString host;

    User(QString nick, QString user, QString host) {
        this->nick = nick;
        this->user = user;
        this->host = host;
    }
};

#endif
