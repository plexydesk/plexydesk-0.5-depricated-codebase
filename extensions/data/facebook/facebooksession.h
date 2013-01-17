#ifndef FACEBOOKSESSION_H
#define FACEBOOKSESSION_H

#include <QObject>
#include <QtCore>
#include <plexy.h>
#include <datasource.h>
#include <pendingjob.h>
#include "facebookuserinfo.h"

class FacebookSession : public PlexyDesk::DataSource
{
    Q_OBJECT
public:
    explicit FacebookSession(QObject *parent = 0);

    virtual QVariantMap readAll();

public Q_SLOTS:
    void setArguments(QVariant args);
    void onFriendListReady();
    void onUserInfoReady(FacebookUserInfo *job);
    void onContactInfoReady();
    void onStatusReady();
    void onFeedPublished();
    void onFeedReady();

private:
    class Private;
    Private *const d;
};

#endif // FACEBOOKSESSION_H
