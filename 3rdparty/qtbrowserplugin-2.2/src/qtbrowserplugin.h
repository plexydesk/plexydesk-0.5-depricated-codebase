/****************************************************************************
**
** Copyright (C) 2003-2007 Trolltech ASA. All rights reserved.
**
** This file is part of a Qt Solutions component.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** Trolltech sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QTNETSCAPE_H
#define QTNETSCAPE_H

#include <QtCore/QMetaObject>
#include <QtCore/QFile>


struct QtNPInstance;
class QtNPBindable;
class QPainter;
class QDateTime;
class QtNPStreamPrivate;

struct NPP_t;
typedef NPP_t* NPP;

class QtNPBindable 
{
    friend class QtNPStream;
public:
    enum Reason {
        ReasonDone = 0,
        ReasonBreak = 1,
        ReasonError = 2,
        ReasonUnknown = -1
    };
    enum DisplayMode 
    { 
        Embedded = 1, 
        Fullpage = 2
    };

    QMap<QByteArray, QVariant> parameters() const;
    DisplayMode displayMode() const;
    QString mimeType() const;

    QString userAgent() const;
    void getNppVersion(int *major, int *minor) const;
    void getBrowserVersion(int *major, int *minor) const;

    // incoming streams (SRC=... tag)
    virtual bool readData(QIODevice *source, const QString &format);

    // URL stuff
    int openUrl(const QString &url, const QString &window = QString());
    int uploadData(const QString &url, const QString &window, const QByteArray &data);
    int uploadFile(const QString &url, const QString &window, const QString &filename);
    virtual void transferComplete(const QString &url, int id, Reason r);

    NPP instance() const;

protected:
    QtNPBindable();
    virtual ~QtNPBindable();

private:
    QtNPInstance* pi;
};

class QtNPFactory {
public:
    QtNPFactory();
    virtual ~QtNPFactory();

    virtual QStringList mimeTypes() const = 0;
    virtual QObject* createObject(const QString &type) = 0;

    virtual QString pluginName() const = 0;
    virtual QString pluginDescription() const = 0;
};

extern QtNPFactory *qtNPFactory();

template<class T>
class QtNPClass : public QtNPFactory
{
public:
    QtNPClass() {}

    QObject *createObject(const QString &key)
    {
        if (!mimeTypes().contains(key))
            return 0;
        return new T;
    }

    QStringList mimeTypes() const
    {
        const QMetaObject &mo = T::staticMetaObject;
        QStringList types = QString::fromLatin1(mo.classInfo(mo.indexOfClassInfo("MIME")).value()).split(';');
        QStringList mimeTypes;
        for (int m = 0; m < types.count(); ++m) {
            QString type = types.at(m);
            type = type.left(type.indexOf(':'));
            if (!type.isEmpty())
                mimeTypes.append(type);
        }

        Q_ASSERT(mimeTypes.count());
        return mimeTypes;
    }

    QString pluginName() const { return QString(); }
    QString pluginDescription() const { return QString(); }
};

#define QTNPFACTORY_BEGIN(Name, Description) \
class QtNPClassList : public QtNPFactory \
{ \
    QHash<QString, QtNPFactory*> factories; \
    QString m_name, m_description; \
public: \
    QtNPClassList() \
    : m_name(Name), m_description(Description) \
    { \
        QtNPFactory *factory = 0; \
        QStringList keys; \
        QStringList::Iterator it; \

#define QTNPCLASS(Class) \
        factory = new QtNPClass<Class>; \
        keys = factory->mimeTypes(); \
        for (it = keys.begin(); it != keys.end(); ++it) \
            factories.insert(*it, factory); \

#define QTNPFACTORY_END() \
    } \
    ~QtNPClassList() { /*crashes? qDeleteAll(factories);*/ } \
    QObject *createObject(const QString &mime) { \
        QtNPFactory *factory = factories.value(mime); \
        return factory ? factory->createObject(mime) : 0; \
    } \
    QStringList mimeTypes() const { return factories.keys(); } \
    QString pluginName() const { return m_name; } \
    QString pluginDescription() const { return m_description; } \
}; \
QtNPFactory *qtns_instantiate() { return new QtNPClassList; } \

#define QTNPFACTORY_EXPORT(Class) \
QtNPFactory *qtns_instantiate() { return new Class; }

#endif  // QTNETSCAPE_H
