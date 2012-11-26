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
    qDebug() << Q_FUNC_INFO << args;
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
                    qDebug() << data;
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
