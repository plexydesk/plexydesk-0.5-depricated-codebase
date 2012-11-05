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
};

FacebookSession::FacebookSession(QObject *parent) :
    PlexyDesk::DataSource(parent),
    d (new Private)
{
    d->manager = new QNetworkAccessManager(this);
    connect(d->manager, SIGNAL(finished(QNetworkReply *)),
     this, SLOT(onFinished(QNetworkReply *)));
    connect(d->manager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), this,
     SLOT(handleAuth(QNetworkReply *, QAuthenticator *)));
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
            //qDebug() << data;
            Json::Value root;
            Json::Reader jsonReader;

            bool parsingSuccessful = jsonReader.parse(data.toStdString(), root);
            QVariantHash hash;

            if (parsingSuccessful) {
                qDebug() << Q_FUNC_INFO << "Parsing success";

                const Json::Value data_list = root["data"];

                for ( int index = 0; index < data_list.size(); ++index) {
                    const char *friendName = data_list[index]["name"].asCString();
                    const char *friendID = data_list[index]["id"].asCString();
                    hash[QString(friendName)] = QVariant(QString(friendID));
                }

                QVariantMap response;
                response["command"] = QVariant("friends");
                response["data"] = hash;

                Q_FOREACH(const QString &key, hash.keys()) {
                    response[key] = QVariant(hash[key]);
                    FacebookUserInfo *info = new FacebookUserInfo(d->manager, hash[key].toString(), d->mToken, this);
                }

                Q_EMIT sourceUpdated(response);
            }
        }
    }
}
