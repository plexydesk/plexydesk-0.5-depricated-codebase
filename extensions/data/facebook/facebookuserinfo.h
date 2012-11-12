#ifndef FACEBOOKUSERINFO_H
#define FACEBOOKUSERINFO_H
#include <plexy.h>

#include <pendingjob.h>

#include <QNetworkAccessManager>
#include <QVariantMap>

class FacebookUserInfo : public PlexyDesk::PendingJob
{
    Q_OBJECT
public:
    explicit FacebookUserInfo(QNetworkAccessManager *manager, const QString &facebookID, const QString &facebookToken, QObject *parent = 0);
    virtual ~FacebookUserInfo();
    QVariantMap userInfo() const;

public Q_SLOTS:
    void onUserInfoReady();
    void onAvatarImageReady();

Q_SIGNALS:
    void finished(FacebookUserInfo *);

private:
    class PrivateFacebookUserInfo;
    PrivateFacebookUserInfo *const d;
};

#endif // FACEBOOKUSERINFO_H
