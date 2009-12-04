#include "storagelayer.h"
#include <QDir>
#include <QCryptographicHash>

#include <QString>
#include <QDebug>

namespace PlexyDesk
{
    SocialStorageLayer::SocialStorageLayer(QObject *parent) : StorageLayerInterface(parent)
    {

    }

   bool  SocialStorageLayer::writeAccount(const QString& id, const QVariantMap& data)
   {
       //extract properties
       QString serviceName, userName, displayName, password;
       bool valid, enabled;
       if(data.contains("SOCIAL_SERVICE"))
           serviceName = data.value("SOCIAL_SERVICE").toString();
       else
           serviceName = "unknown";

       if(data.contains("USERNAME"))
           userName = data.value("USERNAME").toString();
       else
           userName = "unknown";

       if(data.contains("DISPLAY_NAME"))
           displayName = data.value("DISPLAY_NAME").toString();
       else
           displayName = "unknown";

       if(data.contains("VALID"))
           valid = data.value("VALID").toBool();
       else
           valid = false;

       if(data.contains("ENABLED"))
           enabled = data.value("ENABLED").toBool();
       else
           enabled = false;

       if(data.contains("PASSWORD")){
           password = data.value("PASSWORD").toString(); //TODO: implement hashing
       }

       QSqlQuery query(QSqlDatabase::database());
       return query.exec("INSERT INTO Accounts VALUES('" + id + "', '" + serviceName + "', '" +userName + "', '"
                    + displayName + "', " + QString(valid?"1":"0") + ", " + QString(enabled?"1":"0") + ", '" + password + "')");
   }

   void SocialStorageLayer::setDatabasePath(const QString &path)
   {
        if(!QDir(path).exists()){
            QDir dbPath;
            dbPath.mkpath(path);
        }

   }

   void SocialStorageLayer::createTables()
   {
       QSqlQuery query(QSqlDatabase::database());
       qDebug() << "statuse of create tables:" << query.exec("CREATE TABLE Accounts(id varchar(100) PRIMARY KEY, "
                                                             "socialservice varchar(50), username varchar(30), "
                                                             "displayname varchar(50), "
                                                               "valid boolean, enabled boolean, password varchar(50))");
                                                                 //TODO: extend to accommodate parameter values
   }

   void SocialStorageLayer::open()
   {
       qDebug() << "SocialStorageLayer::open()";
        QSqlDatabase databaseConnection = QSqlDatabase::addDatabase("QSQLITE");
        QString dbPath(QDir::homePath() + "/.config/plexydesk/social/accountmanager/");
        setDatabasePath(dbPath);
        databaseConnection.setDatabaseName(dbPath + "socialdatabase.db");
        if(!databaseConnection.open()){
            qDebug() << "Unable to establish db connection";
            emit opened(false);
            return ;
        }

        createTables(); //Create the master table
        emit opened(true);
        qDebug() << "emitted true for database opened()";
   }

   void SocialStorageLayer::close()
   {
       QSqlDatabase::removeDatabase;
       emit closed();
   }

   QStringList SocialStorageLayer::listAccounts()
   {
       qDebug() << "SocialStorageLayer::listAccounts()";
       QStringList listOfAccounts;
       QSqlQuery query(QSqlDatabase::database());
       qDebug() << "SocialStorageLayer::listAccounts(): query status:" << query.exec("SELECT id FROM Accounts");

        while(query.next()){
            listOfAccounts << query.value(0).toString();
        }
        return listOfAccounts;
   }

   QVariantMap SocialStorageLayer::readAccount(const QString& id)
   {
       QVariantMap accountData;
       QSqlQuery query(QSqlDatabase::database());
       query.exec("SELECT * FROM Accounts WHERE id='" + id + "'");

       while(query.next()){
           accountData["SOCIAL_SERVICE"] = query.value(1).toString();
           accountData["USERNAME"] = query.value(2).toString();
           accountData["DISPLAY_NAME"] = query.value(3).toString();
           accountData["VALID"] = query.value(4).toBool();
           accountData["ENABLED"] = query.value(5).toBool();
           accountData["PASSWORD"] = query.value(6).toString();
       }
       return accountData;
   }

   bool SocialStorageLayer::removeAccount(const QString &id)
   {
       QString queryStr("DELETE FROM Accounts WHERE id='" + id + "'");
       QSqlQuery query(QSqlDatabase::database());
       return query.exec(queryStr);
   }

   bool SocialStorageLayer::updateAccount(const QString &id, const QString &propertyName, const QVariant &value)
   {
        QString queryStr;

        if(propertyName == "USERNAME")
            queryStr = "UPDATE Accounts SET username='" + value.toString() + "' WHERE id='" + id + "'";
        else if (propertyName == "PASSWORD")
            queryStr = "UPDATE Accounts SET password='" + value.toString() + "' WHERE id='" + id + "'";
        else if(propertyName == "DISPLAY_NAME")
            queryStr = "UPDATE Accounts SET displayname='" + value.toString() + "' WHERE id='" + id + "'";
        else if(propertyName == "VALID")
            queryStr = "UPDATE Accounts SET valid='" + QString(value.toBool()?"1":"0") + "' WHERE id='" + id + "'";
        else if(propertyName == "ENABLED")
            queryStr = "UPDATE Accounts SET enabled='" + QString(value.toBool()?"1":"0") + "' WHERE id='" + id + "'";
        else
            return false;

        QSqlQuery query(QSqlDatabase::database());
        return query.exec(queryStr);
   }
}
