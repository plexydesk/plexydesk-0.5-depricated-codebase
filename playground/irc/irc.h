#include <QtCore>
#include <QTcpSocket>

#ifndef IRC_H
#define IRC_H
class IrcData : public QObject
{
    Q_OBJECT
    public:
        IrcData(QObject *p =0);
        void connectToServer();
        IrcData(QString server, qint16 port);
        bool isConnected();
        void setNick();
    public slots:
        void ErrorHandler(QAbstractSocket::SocketError);
        void setConnected();
        void init();
        void parse();
    signals:
        void error(QAbstractSocket::SocketError);
        void connected();
        void sample(QString);
        void nickSet();
        void duplicateNick();
    private:
        QTcpSocket *service;
        QString server;
        qint16 port;
    public:
        bool Connected;
};
#endif
