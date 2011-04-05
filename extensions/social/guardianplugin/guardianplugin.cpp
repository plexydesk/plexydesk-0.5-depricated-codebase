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


#include <QXmlQuery>
#include "guardianplugin.h"
#include <QBuffer>
#include <QTest>
#include <QDebug>
#include <QDBusArgument>
#include <QtDBus/QtDBus>
#include <QDir>

using namespace PlexyDesk;

const QString GuardianPlugin::cachePath(QDir::homePath() + "/.config/plexydesk/social/daemon/image/cache/");

struct NewsData
{
    QString contentID;
    QString headLine;
    QString trailText;
    QString body;
    QString webUrl;
    QString docType;
    QString imageUrl;
};
Q_DECLARE_METATYPE(NewsData);

QDBusArgument &operator<<(QDBusArgument &argument, const NewsData &news)
{
        argument.beginStructure();
        argument << news.contentID << news.headLine << news.trailText<< news.body<<news.webUrl<<news.docType<<news.imageUrl;
        argument.endStructure();
        return argument;
}
const QDBusArgument &operator>>(const QDBusArgument &argument, NewsData &news)
{
        argument.beginStructure();
        argument >> news.contentID >> news.headLine >> news.trailText >> news.body >> news.webUrl >> news.docType >> news.imageUrl;
        argument.endStructure();
        return argument;
}

GuardianPlugin::GuardianPlugin(QObject * parent) : SocialPlugin(parent)
{
    job = new HttpJobHandler(this);
    connect(job,SIGNAL(finished()),this,SLOT(listen()));
    qDBusRegisterMetaType<NewsData>();
    QDir cacheDir;
    if (!QDir(cachePath).exists())
        cacheDir.mkpath(this->cachePath);
}
void GuardianPlugin::listen()
{
    if (job->isError())
    {
        guardianError = job->errorName();
    }
    else
    {
        guardianXml = job->readData();
    }
}
void GuardianPlugin::getXml(QUrl &url)
{
    job->getFile(url);
}
QVariantMap GuardianPlugin::getnews(QVariantMap args)
{
    QVariant date = args["needDay"];
    QVariant cont = args["searchCategory"];
    QString needDay,content;
    if (date.canConvert<QString>())
         needDay= date.value<QString>();
    if (cont.canConvert<QString>())
        content = cont.value<QString>();
    content = content.replace(" ","+",Qt::CaseSensitive);
    QUrl NEWS_URL;
    qDebug()<<content;
    if (content == "default")
    {
        QString url = "http://api.guardianapis.com/content/search?filter=/world&after"+needDay+"&api_key=gba692hup5jn84td66dpcpq5";
        NEWS_URL = QUrl(url);
        if (!NEWS_URL.isValid())
        {
            QUrl c_date_WN = "http://api.guardianapis.com/content/search?filter=/world&after"+QString::number(QDate::currentDate().year())+QString::number(QDate::currentDate().month())+QString::number(QDate::currentDate().day())+"&api_key=gba692hup5jn84td66dpcpq5";
            NEWS_URL = QUrl(c_date_WN);
        }
    }
    else
    {
        QString url = "http://api.guardianapis.com/content/search?q="+content+"&api_key=gba692hup5jn84td66dpcpq5";
        NEWS_URL = QUrl(url);
        qDebug()<<NEWS_URL.isValid();
        if (!NEWS_URL.isValid())
        {
            QString c_date_WN = "http://api.guardianapis.com/content/search?q=world&api_key=gba692hup5jn84td66dpcpq5";
            NEWS_URL = QUrl(c_date_WN);
        }
    }
    getXml(NEWS_URL);
    QBuffer buffer;
    while (guardianXml.size() == 0) {
        QTest::qWait(1000);
    }
    buffer.setBuffer(&guardianXml);
    buffer.open(QIODevice::ReadOnly);
    QXmlQuery query;
    query.setFocus(&buffer);

    QStringList contentID,headLine,body, trailText,web_url,doc_type,image_Url;

    query.setQuery("for $x in /search/results/content return string($x/@id)");
    query.evaluateTo(&contentID);

    query.setQuery("for $x in /search/results/content return string($x/headline)");
    query.evaluateTo(&headLine);

    query.setQuery("for $x in /search/results/content/type-specific return string($x/body)");
    query.evaluateTo(&body);

    query.setQuery("for $x in /search/results/content return string($x/trail-text)");
    query.evaluateTo(&trailText);

    query.setQuery("for $x in /search/results/content return string($x/@web-url)");
    query.evaluateTo(&web_url);

    query.setQuery("for $x in /search/results/content return string($x/@type)");
    query.evaluateTo(&doc_type);

    query.setQuery("for $x in /search/results/content return string($x/trail-image)");
    query.evaluateTo(&image_Url);


    guardianXml.clear();
    QVariantMap result;
    for(int i=0; i<headLine.length(); i++){

        NewsData nws;
        nws.contentID = contentID.at(i);
        nws.headLine = headLine.at(i);
        nws.trailText = trailText.at(i);
        nws.body = body.at(i);
        nws.webUrl = web_url.at(i);
        nws.docType = doc_type.at(i);
        if(image_Url.at(i) != "")
        {
            QString fileStat = checkPictureAvailability(image_Url.at(i));
            if (fileStat == "File Not Found")
                nws.imageUrl = getNewsPicture(image_Url.at(i));
            else
                nws.imageUrl = fileStat;
        }
        else
        {
            nws.imageUrl = "Invalid Content";
        }
        QVariant t;
        t.setValue(nws);
        result[contentID[i]] = t;
    }
    return result;
}
QString GuardianPlugin::getNewsPicture(const QString &url)
{
    QUrl tempUrl(url);
    guardianXml.clear();
    getXml(tempUrl);
    while(guardianXml.size() == 0){
        QTest::qWait(1000);
    }
    QImage img;
    img.loadFromData(guardianXml);
    QByteArray file;
    file.append(url);
    QString fileName = file.toBase64();
    fileName = this->cachePath + fileName + ".png";
    img.save(fileName);
    return fileName;
}
QString GuardianPlugin::checkPictureAvailability(const QString &url)
{
     QByteArray byteArrayFile;
     byteArrayFile.append(url);
     QString fileNameString = byteArrayFile.toBase64() + ".png";
     QDir cacheDir(this->cachePath);
     if(cacheDir.exists(fileNameString))
         return (this->cachePath + fileNameString);
     else
         return "File Not Found";
}
QVariantMap GuardianPlugin::data(const QString &methodName, QVariantMap args)
{
    if (methodName == "getnews")
        return this->getnews(args);
}
QStringList GuardianPlugin::supportedMethods()
{
    QStringList list;
    list <<"getnews";
    return list;
}
