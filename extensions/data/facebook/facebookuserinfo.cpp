#include "facebookuserinfo.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <json.h>

class FacebookUserInfo::PrivateFacebookUserInfo
{
public:
    PrivateFacebookUserInfo() {}
    ~PrivateFacebookUserInfo() {}

    QString mFacebookID;
    QString mToken;

    QNetworkAccessManager *manager;
    QVariantMap data;
};

FacebookUserInfo::FacebookUserInfo(QNetworkAccessManager *manager, const QString &facebookID, const QString &facebookToken, QObject *parent) :
    PlexyDesk::PendingJob(parent),
    d (new PrivateFacebookUserInfo)
{
    d->mFacebookID = facebookID;
    d->mToken = facebookToken;

    d->manager = manager;
    connect(d->manager, SIGNAL(finished(QNetworkReply *)),
     this, SLOT(onFinished(QNetworkReply *)));
    connect(d->manager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), this,
     SLOT(handleAuth(QNetworkReply *, QAuthenticator *)));

    QUrl url (QString("https://graph.facebook.com/%1?fields=name,picture&access_token=%2").arg(d->mFacebookID, d->mToken));
    QNetworkReply *reply = d->manager->get(QNetworkRequest(url));

    connect(reply, SIGNAL(finished()), this, SLOT(onUserInfoReady()));
}

FacebookUserInfo::~FacebookUserInfo()
{
    delete d;
}

void FacebookUserInfo::onUserInfoReady()
{
    if (sender()) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender());

        if (reply) {
            QString data = reply->readAll();
            qDebug() << Q_FUNC_INFO << data;
        }
    }
}
