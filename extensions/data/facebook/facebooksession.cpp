#include "facebooksession.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <json.h>
#include "facebookuserinfo.h"

class FacebookSession::Private {
public:
    Private(){}
    ~Private(){}

    QNetworkAccessManager *manager;
    QVariantMap data;
    QString mToken;
    int mContactCount;
};

FacebookSession::FacebookSession(QObject *parent) :
    PlexyDesk::DataSource(parent),
    d (new Private)
{
    d->mContactCount = 0;

    d->manager = new QNetworkAccessManager(this);
}

QVariantMap FacebookSession::readAll()
{
    return d->data;
}

void FacebookSession::setArguments(QVariant &args)
{
    QVariantMap param = args.toMap();

    QString command = param["command"].toString();

    if (command == "login") {
        // it's expected that the controller sends us the key
        // so we check if the key is emptry if not notify the controller
        // with a emptry response
        QString key = param["token"].toString();

        if (key.isEmpty() || key.isNull())  {
            //Notify the client about the missing key.
            QVariantMap response;
            response["command"] = QVariant("login");
            response["token"] = QVariant("");
            d->data = response;
            qDebug() << Q_FUNC_INFO << "No key found";
            Q_EMIT sourceUpdated(response);
        }
    }

    if (command == "friends") {
        QString key = param["token"].toString();

        if (key.isEmpty() || key.isNull())  {
            //Notify the client about the missing key.
            QVariantMap response;
            response["command"] = QVariant("login");
            response["token"] = QVariant("");
            d->data = response;
            qDebug() << Q_FUNC_INFO << "No key found";
            Q_EMIT sourceUpdated(response);
            return;
        }

        d->mToken = key;
        QUrl url ("https://graph.facebook.com/me/friends?access_token=" + key);
        QNetworkReply *reply = d->manager->get(QNetworkRequest(url));

        connect(reply, SIGNAL(finished()), this, SLOT(onFriendListReady()));
    }

    if (command == "user") {
        QString key = param["token"].toString();

        if (key.isEmpty() || key.isNull())  {
            //Notify the client about the missing key.
            QVariantMap response;
            response["command"] = QVariant("login");
            response["token"] = QVariant("");
            d->data = response;
            qDebug() << Q_FUNC_INFO << "No key found";
            Q_EMIT sourceUpdated(response);
            return;
        }

        QString id = param["id"].toString();

        //picture.type(small) | picture.type(large);
        QUrl url (QString("https://graph.facebook.com/%1/?fields=cover,hometown,location,first_name,last_name,picture,picture.type(normal)&limit=1&access_token=%2").arg(id, key));
        QNetworkReply *reply = d->manager->get(QNetworkRequest(url));

        connect(reply, SIGNAL(finished()), this, SLOT(onContactInfoReady()));
    }

    if (command == "status") {
        QString key = param["token"].toString();

        if (key.isEmpty() || key.isNull())  {
            //Notify the client about the missing key.
            QVariantMap response;
            response["command"] = QVariant("login");
            response["token"] = QVariant("");
            d->data = response;
            qDebug() << Q_FUNC_INFO << "No key found";
            Q_EMIT sourceUpdated(response);
            return;
        }

        QString id = param["id"].toString();
        QUrl url (QString("https://graph.facebook.com/%1/statuses?fields=message,from&limit=1&access_token=%2").arg(id, key));
        QNetworkReply *reply = d->manager->get(QNetworkRequest(url));

        connect(reply, SIGNAL(finished()), this, SLOT(onStatusReady()));
    }

    if (command == "wallpost") {
        QString key = param["token"].toString();

        if (key.isEmpty() || key.isNull())  {
            //Notify the client about the missing key.
            QVariantMap response;
            response["command"] = QVariant("login");
            response["token"] = QVariant("");
            d->data = response;
            qDebug() << Q_FUNC_INFO << "No key found";
            Q_EMIT sourceUpdated(response);
            return;
        }

       QString id = param["id"].toString();
//        QUrl url (QString("https://graph.facebook.com/%1/statuses?fields=message,from&limit=1&access_token=%2").arg(id, key));
//        QNetworkReply *reply = d->manager->get(QNetworkRequest(url));

//        connect(reply, SIGNAL(finished()), this, SLOT(onStatusReady()));
        QNetworkRequest request;
        request.setUrl(QString("https://graph.facebook.com/%1/feed/").arg(id));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");

        QByteArray postData;
        postData.append(QString("message=%1&").arg(param["message"].toString()));
        postData.append("name=plexydesk&");
        postData.append(QString("access_token=%1").arg(key));

        QNetworkReply *reply = d->manager->post(request, postData);

        connect(reply, SIGNAL(finished()), this, SLOT(onFeedPublished()));

    }

}

void FacebookSession::onFriendListReady()
{

    if (sender()) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender());

        if (reply) {
            QString data = reply->readAll();
            Json::Value root;
            Json::Reader jsonReader;
            bool parsingSuccessful = jsonReader.parse(data.toStdString(), root);

            if (parsingSuccessful) {
                if (root.isMember("error")) {
                    QVariantMap errorData;
                    errorData["command"] = QVariant("login");
                    errorData["token"] = QVariant("");
                    Q_EMIT sourceUpdated(errorData);
                    return;
                }
                const Json::Value data_list = root["data"];

                for ( int index = 0; index < data_list.size(); ++index) {
                    const char *friendID = data_list[index]["id"].asCString();
                    FacebookUserInfo *info = new FacebookUserInfo(d->manager, QString(friendID), d->mToken, this);
                    connect(info, SIGNAL(finished(FacebookUserInfo*)), this, SLOT(onUserInfoReady(FacebookUserInfo*)));
                }
            }
        }
    }
}

void FacebookSession::onUserInfoReady(FacebookUserInfo *job)
{
    FacebookUserInfo *user = job;

    if (user)
       Q_EMIT sourceUpdated(user->userInfo());
    else
        qDebug() << Q_FUNC_INFO << "Failed converating data";

    delete user;
}

void FacebookSession::onContactInfoReady()
{
    if (sender()) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender());

        if (reply) {
            QString data = reply->readAll();
            Json::Value root;
            Json::Reader jsonReader;

            bool parsingSuccessful = jsonReader.parse(data.toStdString(), root);

            if (parsingSuccessful) {
                QVariantMap response;
                response["command"] = QVariant("userdata");
                response["token"] = d->mToken;
                response["first_name"] = root["first_name"].asCString();
                response["last_name"] = root["last_name"].asCString();
                response["id"] = root["id"].asCString();
                response["hometown"] = root["hometown"]["name"].asCString();
                response["location"] = root["location"]["name"].asCString();
                response["picture"] = root["picture"]["data"]["url"].asCString();
                response["cover"] = root["cover"]["source"].asCString();

                if (root["cover"].isMember("offset_y")) {
                    response["cover_offset"] = root["cover"]["offset_y"].asInt();
                }

                Q_EMIT sourceUpdated(response);
            }
        }
        reply->deleteLater();
    }
}

void FacebookSession::onStatusReady()
{
    if (sender()) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender());

        if (reply) {
            QString data = reply->readAll() ;
            Json::Value root;
            Json::Reader jsonReader;
            bool parsingSuccessful = jsonReader.parse(data.toStdString(), root);

            if (parsingSuccessful && !root["data"].isNull()) {
                const Json::Value data_list = root["data"][0];
                QString message = data_list["message"].asCString();

                QVariantMap response;
                response["command"] = QVariant("status");
                response["message"] = message;
                response["id"] = data_list["from"]["id"].asCString();

                Q_EMIT sourceUpdated(response);
            }
        }

        reply->deleteLater();
    }
}

void FacebookSession::onFeedPublished()
{
    if (sender()) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender());

        if (reply) {
            //QString data = reply->readAll() ;

            reply->deleteLater();
        }
    }
}

