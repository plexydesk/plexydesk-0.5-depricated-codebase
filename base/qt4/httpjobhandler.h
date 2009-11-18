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

#ifndef HTTPJOBHANDLER_H
#define HTTPJOBHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QList>

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
    void postFile(const QUrl &url, const QByteArray &data);
    QByteArray readData() const;

private:
    class HttpJobHandlerPrivate;
    HttpJobHandlerPrivate * const d;
    bool validateUrl(const QUrl &url);
    QList<QNetworkReply*> holder;
    QString msg, error;

private slots:
    void onFinish(QNetworkReply* buffer);
//    void authenticate(QNetworkReply*, QAuthenticator*); //when simple auth fails

};
} //namespace PlexyDesk
#endif // HTTPJOBHANDLER_H
