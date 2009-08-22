<<<<<<< HEAD
/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Sri Lanka Institute of Information Technology
*
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

#include "httpjobhandler.h"

#include <plexyconfig.h>

namespace PlexyDesk
{
class HttpJobHandler::HttpJobHandlerPrivate
{

public:
    HttpJobHandlerPrivate(){};
    ~HttpJobHandlerPrivate(){};
    QNetworkAccessManager *manager;
    QByteArray data;
};

HttpJobHandler::HttpJobHandler(QObject * parent) : PendingJob(parent), d(new HttpJobHandlerPrivate)
{
    d->manager = new QNetworkAccessManager(this);

    //catch the  finished signal coming from QNetworkAccessManager and trigger onFinish.
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinish(QNetworkReply*)));
}

void HttpJobHandler::getFile(const QUrl &url)
{
    if (!url.isValid()) {
        msg = "Invalid URL";
        error = "The URL was not in valid format";
        setFinished(true, msg, error);
    }

    if (url.scheme() != "http") {
        msg = "Unsupported Protocol";
        error = "The URL was not http, the currently supported protocol.";
        setFinished(true, msg, error);
    }

    if (url.path().isEmpty()) {
        msg = "Empty URL";
        error = "The URL was empty";
        setFinished(true, msg, error);
    }

    //fetch required file using QNetworkAccessManager
    d->manager->get(QNetworkRequest(url));
}


void HttpJobHandler::onFinish(QNetworkReply* reply)
{
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (statusCodeV.toInt()==200) { //statuscode 200 means we are OK
        d->data = reply->readAll();
        setFinished(false, msg, error);
    }

    else {
        msg = "200";
        error = "Error fetching data";

        setFinished(true, msg, error);
    }
}

QByteArray HttpJobHandler::readData() const
{
    return d->data;
}

HttpJobHandler::~HttpJobHandler()
{
    delete d;
}

} //namespace PlexyDesk
