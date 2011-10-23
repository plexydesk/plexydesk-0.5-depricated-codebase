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

signals:
    void ready();

public Q_SLOTS:


};

#endif // FACEBOOKSESSION_H
