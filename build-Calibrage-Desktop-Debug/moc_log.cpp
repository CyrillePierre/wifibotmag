/****************************************************************************
** Meta object code from reading C++ file 'log.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Calibrage/log.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'log.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Log_t {
    QByteArrayData data[6];
    char stringdata[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Log_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Log_t qt_meta_stringdata_Log = {
    {
QT_MOC_LITERAL(0, 0, 3),
QT_MOC_LITERAL(1, 4, 6),
QT_MOC_LITERAL(2, 11, 0),
QT_MOC_LITERAL(3, 12, 3),
QT_MOC_LITERAL(4, 16, 11),
QT_MOC_LITERAL(5, 28, 10)
    },
    "Log\0append\0\0str\0appendError\0appendInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Log[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       4,    1,   32,    2, 0x0a /* Public */,
       5,    1,   35,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void Log::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Log *_t = static_cast<Log *>(_o);
        switch (_id) {
        case 0: _t->append((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->appendError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->appendInfo((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Log::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_Log.data,
      qt_meta_data_Log,  qt_static_metacall, 0, 0}
};


const QMetaObject *Log::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Log::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Log.stringdata))
        return static_cast<void*>(const_cast< Log*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int Log::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
