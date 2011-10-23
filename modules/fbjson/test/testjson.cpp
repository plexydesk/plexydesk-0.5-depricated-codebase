#include "testjson.h"
#include <plexyconfig.h>
#include <jsonhandler.h>
#include <facebooksession.h>
#include <QFile>
#include <QDir>
#include <config.h>


QString readFile(QString filename)
{
    QFile file(filename);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         qDebug() << "Failed to open " << filename;
         return QString();
     }

     QString rv;
     while (!file.atEnd()) {
         QByteArray line = file.readLine();
         rv += line;
     }
     return rv;
}


void TestJson::loadSession()
{

   mLoop = new QEventLoop();
   
   FacebookSession *session = new FacebookSession();

   QSignalSpy spy(session, SIGNAL(ready())); 
   QVERIFY (session->hasToken());
   
   session->makeRequest("https://graph.facebook.com/me/feed"); 
 

   connect (session, SIGNAL(ready()), this, SLOT (onSessionReady()));
   
  mLoop->exec();
  QCOMPARE(spy.count(), 1);
    
}

void TestJson::onSessionReady()
{
   mLoop->exit();
}

void TestJson::loadJsons()
{
    mFetchComplete = false;
    JsonHandler json;
    QString fbinfo = readFile(QDir::toNativeSeparators(
                                PlexyDesk::Config::getInstance()->plexydeskBasePath() +
                                "/share/plexy/fbjson/data/fbstatus.json"));
    QVERIFY(!fbinfo.isEmpty());
    JsonData data = json.property(fbinfo, "comments");
    QCOMPARE(data.type() , JsonData::Object);

    QVariantList list = data.propertyValue("comments", "data").toList();

    QBENCHMARK {
    QVariantList speedlist = data.propertyValue("comments", "data").toList();
   }
    QCOMPARE(list.size(), 6);

    for (int i = 0 ; i < list.size(); i++) {
        qDebug() << list.at(i).toMap() << endl;
    }


    data = json.property(fbinfo, "likes");
    QCOMPARE(data.type(), JsonData::Object);

    list = data.propertyValue("likes", "data").toList();
    QCOMPARE(list.size(), 11);

    data = json.property(fbinfo, "message");
    QCOMPARE(data.type(), JsonData::String);

    data = json.property(fbinfo, "id");
    QCOMPARE(data.type(), JsonData::String);

    data = json.property(fbinfo, "updated_time");
    QCOMPARE(data.type(), JsonData::String);
    // qDebug() << data.data()["updated_time"].toString();
    data = json.property(fbinfo, "from");
    QCOMPARE(data.type(), JsonData::Object);

    QCOMPARE(data.propertyValue("from", "name").toString(),
            QString("Bret Taylor"));
}

void TestJson::loadInvalidJson()
{
}

void TestJson::onFinished()
{
}

void TestJson::onInvalidFinished()
{
}

QTEST_MAIN(TestJson)
