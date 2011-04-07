#ifndef SOCIALQDBUSPLUGINDATA_H
#define SOCIALQDBUSPLUGINDATA_H

//#include "plexy.h"
#include <QMap>
#include <QStringList>
#include <QPainter>
#include <QList>
#include <QVariantMap>
class SocialQDBusPluginData
{
public:
    SocialQDBusPluginData();
    virtual ~SocialQDBusPluginData();
    QVariantMap getData(QString pluginName, QString pluginMethod, QVariantMap args);
    bool getAuthentication(QString uName, QString pWord);

};

#endif

