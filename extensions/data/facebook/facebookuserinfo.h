#ifndef FACEBOOKUSERINFO_H
#define FACEBOOKUSERINFO_H
#include <plexy.h>

#include <pendingjob.h>

#include <QNetworkAccessManager>

class FacebookUserInfo : public PlexyDesk::PendingJob
{
    Q_OBJECT
public:
    explicit FacebookUserInfo(QNetworkAccessManager *manager, const QString &facebookID, const QString &facebookToken, QObject *parent = 0);
    virtual ~FacebookUserInfo();

public Q_SLOTS:
    void onUserInfoReady();

private:
    class PrivateFacebookUserInfo;
    PrivateFacebookUserInfo *const d;
};

#endif // FACEBOOKUSERINFO_H
