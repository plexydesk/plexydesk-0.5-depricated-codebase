#ifndef FACEBOOKSESSION_H
#define FACEBOOKSESSION_H

#include <QObject>

/**
  Lightweight Class for Handling Facebook Session data
 **/
class FacebookSession : public QObject
{
    Q_OBJECT
public:
    explicit FacebookSession(QObject *parent = 0);
    void requestFriendsList();
    bool hasToken() const;
    QString token() const;
    void makeRequest(const QString &url);
    QByteArray rawData() const;

Q_SIGNALS:
    void ready();
    void tokenReady();

public Q_SLOTS:
    void onReadyRead();

private:
    class Private;
    Private *const d;
};

#endif // FACEBOOKSESSION_H
