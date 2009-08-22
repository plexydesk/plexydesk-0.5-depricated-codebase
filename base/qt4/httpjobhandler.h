#ifndef HTTPJOBHANDLER_H
#define HTTPJOBHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>

#include "pendingjob.h"

class QNetworkAccessManager;
class QByteArray;

namespace PlexyDesk
{

class VISIBLE_SYM HttpJobHandler: public PendingJob
{
    Q_OBJECT
public:
    HttpJobHandler(QObject * parent);
    ~HttpJobHandler();
    void getFile(const QUrl &url);
    QByteArray readData() const;

private:
    class HttpJobHandlerPrivate;
    HttpJobHandlerPrivate * const d;

    QString msg, error;

private slots:
    void onFinish(QNetworkReply* buffer);
};
} //namespace PlexyDesk
#endif // HTTPJOBHANDLER_H
