#include "facebookuserinfo.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QImage>
#include <QPixmap>
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
    d->data["command"] = QString("userinfo");

    d->manager = manager;


    QUrl url (QString("https://graph.facebook.com/%1?fields=name,picture&access_token=%2").arg(d->mFacebookID, d->mToken));
    QNetworkReply *reply = d->manager->get(QNetworkRequest(url));

    connect(reply, SIGNAL(finished()), this, SLOT(onUserInfoReady()));
}

FacebookUserInfo::~FacebookUserInfo()
{
    delete d;
}

QVariantMap FacebookUserInfo::userInfo() const
{
    return d->data;
}

void FacebookUserInfo::onUserInfoReady()
{
    if (sender()) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender());

        if (reply) {
            QString data = reply->readAll();
            //qDebug() << Q_FUNC_INFO << data;
            Json::Value root;
            Json::Reader jsonReader;

            bool parsingSuccessful = jsonReader.parse(data.toStdString(), root);

            if (parsingSuccessful) {
                const Json::Value data_list = root["picture"]["data"];
                //qDebug() <<  "Request" << data_list["url"].asCString();
                d->data["id"] = d->mFacebookID;
                d->data["name"] = root["name"].asCString();
                //qDebug() << d->data;
                QUrl url (QString(data_list["url"].asCString()));
                QNetworkReply *reply = d->manager->get(QNetworkRequest(url));

                connect(reply, SIGNAL(finished()), this, SLOT(onAvatarImageReady()));


            }
        }
    }
}


void FacebookUserInfo::onAvatarImageReady()
{
    if (sender()) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender());


        if (reply) {

            if (reply->error() != QNetworkReply::NoError) {
                  qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
                  return;
              }

            QString header = reply->header(QNetworkRequest::ContentTypeHeader).toString();

            const QByteArray data = reply->readAll();
            int bufferSize = reply->bytesAvailable();

            QPixmap pixmap;
            pixmap.loadFromData(data);

            if(pixmap.isNull()) {
                qDebug() << Q_FUNC_INFO << "Null Image data from : " << reply->url() << " : was finished ?" << reply->isFinished();
            } else {
                d->data["picture"] = QVariant(pixmap);
                Q_EMIT finished(this);
            }
        }

        reply->deleteLater();
    }
}
