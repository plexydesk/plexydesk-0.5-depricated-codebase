#include "jsonhandler.h"
#include <QScriptValueIterator>

JsonHandler::JsonHandler()
{
}

JsonData JsonHandler::parse(const QString &data)
{
    qDebug() << Q_FUNC_INFO <<  data;
    QScriptValue sc;
    QScriptEngine engine;
    sc = engine.evaluate("(" + QString(data) + ")");
    if (sc.property("error").isObject()) {
        JsonData result;
        result.setType(JsonData::Error);
        QScriptValue errorObj = sc.property("error").toObject();
        qDebug() << Q_FUNC_INFO << 
            "Error in Result" << 
            errorObj.property("type").toString() <<
            errorObj.property("message").toString();
        return result;
    }

    if (sc.isObject()) {
        QScriptValueIterator it(sc);
        JsonData result;
        result.setType(JsonData::Object);
        QScriptValue obj = sc;

        QVariantMap map = qscriptvalue_cast<QVariantMap>(sc);

        qDebug() << Q_FUNC_INFO << map;
        Q_FOREACH(QString var, map.keys()) {
                    QScriptValue value = sc.property(var);
                    QVariantMap innerdata = qscriptvalue_cast<QVariantMap>(value);
                    qDebug() << var << value.isArray() << value.isObject();
                    if (value.isArray()) {
                        QScriptValueIterator iter(value);
                        while (iter.hasNext()) {
                            iter.next();
                            if (iter.value().isObject()) {
                                QVariantMap list = qscriptvalue_cast<QVariantMap>(iter.value());
                                qDebug() << list;
                            }
                        }
                    }
                    if (value.isObject()) {
                        QVariantMap list = qscriptvalue_cast<QVariantMap>(value);
                        qDebug() << list;
                    }
            }
        return result;
    }
}

void JsonHandler::extractValue(QScriptValueIterator &it)
{
    if (it.hasNext()) {
        it.next();
        qDebug() << Q_FUNC_INFO << it.name();
        QScriptValueIterator rt(it.value());
        extractValue(rt);
    }


    return;
}
