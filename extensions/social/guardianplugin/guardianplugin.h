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



#ifndef WEATHERPLUGIN_H
#define WEATHERPLUGIN_H

#include <QObject>
#include <QString>
#include <QUrl>
//#include <QNetworkReply>
//#include "pendingjob.h"
#include <httpjobhandler.h>
#include <baseplugin.h>
#include <abstractplugininterface.h>
#include "guardianinterface.h"
#include <socialplugin.h>
//#include <QtXmlPatterns>


class GuardianPlugin : public PlexyDesk::SocialPlugin, GuardianInterface
{
    Q_OBJECT;
private:
    PlexyDesk::HttpJobHandler *job;
    QByteArray guardianXml;
    QString guardianError;
    void getXml(QUrl &url);
    QString getNewsPicture(const QString &url);
    QString checkPictureAvailability(const QString &url);
    static const QString cachePath;
public:
    GuardianPlugin(QObject *parent=0);

public slots:
    QVariantMap getnews(QVariantMap args);
    QStringList supportedMethods();
    QVariantMap data(const QString &methodName, QVariantMap args);

private slots:
    void listen();

};

#endif
