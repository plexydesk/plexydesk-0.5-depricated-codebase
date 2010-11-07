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

#include <QAuthenticator>


namespace PlexyDesk
{
class HttpJobHandler::HttpJobHandlerPrivate
{
public:
    HttpJobHandlerPrivate() {};
    QNetworkAccessManager *manager;
    QByteArray data;
    ~HttpJobHandlerPrivate() {};
};

HttpJobHandler::HttpJobHandler(QObject * parent) : PendingJob(parent), d(new HttpJobHandlerPrivate)
{
    qDebug() << "HttpJobHandler::HttpJobHandler()";
    d->manager = new QNetworkAccessManager(this);
    qDebug() << "I'm in httpjobhandler constructor";
    //connect the finished signal coming from QNetworkAccessManager with onFinish.
    connect(d->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinish(QNetworkReply*)));
}

void HttpJobHandler::getFile(const QUrl &url)
{
    qDebug() << "HttpJobHandler::getFile()";
    if (isValidUrl(url)) {
        QNetworkReply* reply = d->manager->get(QNetworkRequest(url));
        m_holder.append(reply);
    }
}

void HttpJobHandler::postFile(const QUrl &url, const QByteArray  &data)
{
    qDebug() << "HttpJobHandler::postFile()";
    if (isValidUrl(url)) {
        QNetworkReply* reply = d->manager->post(QNetworkRequest(url), data);
        m_holder.append(reply);
    }
}

bool HttpJobHandler::isValidUrl(const QUrl &url)
{
    qDebug() << "HttpJobHandler::isValidUrl()";
    if (url.path().isEmpty()) {
        m_msg = "Empty URL";
        m_error = "The URL was empty";
        setFinished(true, m_msg, m_error);
        return false;
    } else if (!url.isValid()) {
        m_msg = "Invalid URL";
        m_error = "The URL was not in valid format";
        setFinished(true, m_msg, m_error);
        return false;
    } else if (url.scheme() != "http" || url.scheme() != "https") {
        m_msg = "Unsupported Protocol";
        m_error = "The URL was not http/s.";
        setFinished(true, m_msg, m_error);
        return false;
    }
    return true;
}

void HttpJobHandler::onFinish(QNetworkReply* reply)
{
    qDebug() << "HttpJobHandler::onFinish()";
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCodeV.toInt() == 200) { //statuscode 200 means we are OK. Page not found is not taken as error.
        d->data = reply->readAll();
        m_msg = "200";
        m_error = "200";
        setFinished(false, m_msg, m_error);
    } else {
        d->data = reply->readAll(); //make server reply available to caller.
        m_msg = statusCodeV.toString();
        m_error = "Error fetching data";
        setFinished(true, m_msg, m_error);
    }
}

QByteArray HttpJobHandler::readData() const
{
    qDebug() << "HttpJobHandler::readData()";
    return d->data;
}

HttpJobHandler::~HttpJobHandler()
{
    delete d;
}

} //namespace PlexyDesk
