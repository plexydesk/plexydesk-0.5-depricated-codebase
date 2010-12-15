#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include "json_global.h"
#include <QtCore>
#include <QScriptEngine>
#include <QScriptValueIterator>


class JSONSHARED_EXPORT JsonData
{
public:
    typedef enum { Error = 1 << 0,
        Object = 1 << 1
    } Type;
    JsonData(){}
    void setType(Type type) { mType = type;}
    Type type() const{ return mType;}
    void addData(const QString &key, const QVariant &value)
    {
        mData[key] = value;
    }
    QHash<QString, QVariant> data() { return mData;}
private:
    Type mType;
    QHash<QString, QVariant> mData;
};

class JSONSHARED_EXPORT JsonHandler {
public:
    JsonHandler();
    JsonData parse(const QString& data);
    void extractValue(QScriptValueIterator &value);
};

#endif // JSONHANDLER_H
