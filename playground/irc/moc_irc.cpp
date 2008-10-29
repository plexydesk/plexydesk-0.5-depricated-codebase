/****************************************************************************
** Meta object code from reading C++ file 'irc.h'
**
** Created: Wed Oct 29 16:56:22 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "irc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'irc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IrcData[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,    9,    8,    8, 0x05,
      66,   51,    8,    8, 0x05,
     111,   51,    8,    8, 0x05,
     186,  150,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
     251,    8,    8,    8, 0x0a,
     258,    8,    8,    8, 0x0a,
     301,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_IrcData[] = {
    "IrcData\0\0error\0error(QAbstractSocket::SocketError)\0"
    "response,error\0"
    "connectResponse(ConnectResponseType,QString)\0"
    "nickResponse(NickResponseType,QString)\0"
    "response,error,channelName,userList\0"
    "channelResponse(ChannelResponseType,QString,QString,QList<User>)\0"
    "init()\0errorHandler(QAbstractSocket::SocketError)\0"
    "parse()\0"
};

const QMetaObject IrcData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IrcData,
      qt_meta_data_IrcData, 0 }
};

const QMetaObject *IrcData::metaObject() const
{
    return &staticMetaObject;
}

void *IrcData::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IrcData))
        return static_cast<void*>(const_cast< IrcData*>(this));
    return QObject::qt_metacast(_clname);
}

int IrcData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: error((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 1: connectResponse((*reinterpret_cast< ConnectResponseType(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: nickResponse((*reinterpret_cast< NickResponseType(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: channelResponse((*reinterpret_cast< ChannelResponseType(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QList<User>(*)>(_a[4]))); break;
        case 4: init(); break;
        case 5: errorHandler((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 6: parse(); break;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void IrcData::error(QAbstractSocket::SocketError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IrcData::connectResponse(ConnectResponseType _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IrcData::nickResponse(NickResponseType _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void IrcData::channelResponse(ChannelResponseType _t1, QString _t2, QString _t3, QList<User> _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
