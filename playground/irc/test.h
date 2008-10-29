#include <QtCore>
#include "irc.h"


class Tester : public QObject
{
    Q_OBJECT
    public:
        Tester(QObject *p =0);
    public slots:
        void connectHandler(ConnectResponseType,QString);
        void userHandler(UserResponseType,QString);
        void nickHandler(NickResponseType,QString);
    private:
        int Test;
};

