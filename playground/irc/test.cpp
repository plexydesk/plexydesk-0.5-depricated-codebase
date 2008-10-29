#include <QApplication>
#include "irc.h"
#include "test.h"

void Tester::connectionHandler()
{
    qDebug() << "Connected";
}

// void Tester::nickHandler(int code)
// {
//     switch(code){
//         case 0: qDebug() << "Nick Set";
//                 break;
//         case 1: qDebug() << "Duplicate Nick";
//                 break;
//         default: break;
//     }
// }

void Tester::sampleHandler(QString str)
{
    qDebug() << str;
}

Tester::Tester(QObject *p) : QObject(p)
{
    Test = 0;
    IrcData *irc = new IrcData("64.161.254.20",6667);
    irc->connectToServer();
    connect(irc,SIGNAL(connected()),SLOT(connectionHandler()));
    connect(irc,SIGNAL(sample(QString)),SLOT(sampleHandler(QString)));
}

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    Tester *test = new Tester();
    app.exec();
}
