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
                   Object = 1 << 1,
                   String = 1 << 2,
                   Array = 1 << 3
    } Type;
    JsonData(){
    }
    void setType(Type type) {
        mType = type;
    }
    Type type() const {
        return mType;
    }
    void addData(const QString &key, const QVariant &value)
    {
        mData[key] = value;
    }
    QHash<QString, QVariant> data() {
        return mData;
    }

    QVariant propertyValue(const QString &prop, const QString &key) const{
        return mData[prop].toMap()[key];
    }

private:
    Type mType;
    QHash<QString, QVariant> mData;
};

class JSONSHARED_EXPORT JsonHandler {
public:
    JsonHandler();
    JsonData property(const QString &data, const QString &prop);
    QHash<QString, QVariant> arrayToMap(const QScriptValue &value);
};

#endif // JSONHANDLER_H
