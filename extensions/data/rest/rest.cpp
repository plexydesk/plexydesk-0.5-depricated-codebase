/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : *
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
#include "rest.h"
#include <desktopwidget.h>
#include <plexyconfig.h>
#include <QNetworkAccessManager>

#define POST 0
#define GET 1

class RestData::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    QNetworkAccessManager *manager;
    QString user;
    QString pass;
    QVariantMap data;
};

RestData::RestData(QObject * /*object*/) : d(new Private)
{
    d->manager = new QNetworkAccessManager(this);
    connect(d->manager, SIGNAL(finished(QNetworkReply *)),
     this, SLOT(replyFinished(QNetworkReply *)));
    connect(d->manager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), this,
     SLOT(handleAuth(QNetworkReply *, QAuthenticator *)));
}

void RestData::init()
{
    qDebug() << Q_FUNC_INFO;
}

RestData::~RestData()
{
    delete d;
}

void RestData::setArguments(QVariant &arg)
{
    qDebug() << Q_FUNC_INFO << arg;
    QVariantMap param = arg.toMap();

    QUrl url = param["url"].toUrl();
    int type = param["type"].toInt();
    QString par = param["params"].toString();
    d->user = param["user"].toString();
    d->pass = param["pass"].toString();

    if (type == GET) {
        d->manager->get(QNetworkRequest(url));
    } else if (type == POST) {
        d->manager->post(QNetworkRequest(url), par.toAscii());
    }
}

void RestData::replyFinished(QNetworkReply *reply)
{
    QVariantMap response;
    response["data"] = QVariant(reply->readAll());
    d->data = response;
    Q_EMIT ready();
}

void RestData::handleAuth(QNetworkReply * /*r*/, QAuthenticator *auth)
{
    auth->setUser(d->user);
    auth->setPassword(d->pass);
}

QVariantMap RestData::readAll()
{
    return d->data;
}
