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
