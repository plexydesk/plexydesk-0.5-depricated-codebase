#include "jsonhandler.h"
#include <QScriptValueIterator>

JsonHandler::JsonHandler()
{
}

JsonData JsonHandler::property(const QString &data, const QString &prop)
{
    QScriptValue sc;
    QScriptEngine engine;
    JsonData errorData;
    JsonData propData;;

    sc = engine.evaluate("(" + QString(data) + ")");

    if (sc.property("error").isObject()) {
        JsonData result;
        result.setType(JsonData::Error);
        QScriptValue errorObj = sc.property("error").toObject();
        qDebug() << Q_FUNC_INFO <<
        "Error in Result" <<
        errorObj.property("type").toString() <<
        errorObj.property("message").toString();
        errorData = result;
    }

    QScriptValue propValue = sc.property(prop);
    if (!propValue.isValid() || propValue.isUndefined()) {
        return errorData;
    }else {
        qDebug() << Q_FUNC_INFO << "Start parsing property :" << propValue.isObject();
        if (propValue.isObject() && !propValue.isArray()) {
            qDebug() << Q_FUNC_INFO << "IsObject" << propValue.isArray();
            QVariantMap list = qscriptvalue_cast<QVariantMap>(propValue);
            propData.setType(JsonData::Object);
            propData.addData(prop, QVariant(list));
            qDebug() << Q_FUNC_INFO << prop << ": "<<  propValue.toString() << endl;
            return propData;
        } else if (propValue.isString()) {
            qDebug() << Q_FUNC_INFO << "IsString";
            propData.setType(JsonData::String);
            propData.addData(prop, QVariant(propValue.toString()));
            qDebug() << Q_FUNC_INFO <<"RawData"<<  propValue.toString();
            return propData;
        } else if (propValue.isArray()) {
            qDebug() << Q_FUNC_INFO << "IsArray : " << prop;
            propData.setType(JsonData::Array);
            propData.addData(prop, QVariant(arrayToMap(propValue)));
            return propData;
        }
    }
    return propData;
}

QVariantMap JsonHandler::arrayToMap(const QScriptValue &value)
{
  QScriptValueIterator iter(value);
  QVariantMap rv;
  QVariantList vl;
  while (iter.hasNext()) {
        iter.next();
        if (iter.value().isObject()) {
            QVariantMap list = qscriptvalue_cast<QVariantMap>(iter.value());
            vl.append (list);
	    //return list;
        }
  }
  rv["data"] = vl;
  return rv;
}
