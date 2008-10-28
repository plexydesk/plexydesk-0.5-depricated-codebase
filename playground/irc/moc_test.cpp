/****************************************************************************
** Meta object code from reading C++ file 'test.h'
**
** Created: Tue Oct 28 20:26:05 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "test.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Tester[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      32,   28,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Tester[] = {
    "Tester\0\0connectionHandler()\0str\0"
    "sampleHandler(QString)\0"
};

const QMetaObject Tester::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Tester,
      qt_meta_data_Tester, 0 }
};

const QMetaObject *Tester::metaObject() const
{
    return &staticMetaObject;
}

void *Tester::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Tester))
        return static_cast<void*>(const_cast< Tester*>(this));
    return QObject::qt_metacast(_clname);
}

int Tester::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectionHandler(); break;
        case 1: sampleHandler((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
