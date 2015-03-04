/****************************************************************************
** Meta object code from reading C++ file 'uidatacontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Prototype/uidatacontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uidatacontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UIDataController_t {
    QByteArrayData data[10];
    char stringdata[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UIDataController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UIDataController_t qt_meta_stringdata_UIDataController = {
    {
QT_MOC_LITERAL(0, 0, 16), // "UIDataController"
QT_MOC_LITERAL(1, 17, 9), // "sendToQml"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 20), // "sendToCrashDetection"
QT_MOC_LITERAL(4, 49, 10), // "SensorData"
QT_MOC_LITERAL(5, 60, 4), // "data"
QT_MOC_LITERAL(6, 65, 9), // "parseData"
QT_MOC_LITERAL(7, 75, 5), // "char*"
QT_MOC_LITERAL(8, 81, 6), // "buffer"
QT_MOC_LITERAL(9, 88, 7) // "getList"

    },
    "UIDataController\0sendToQml\0\0"
    "sendToCrashDetection\0SensorData\0data\0"
    "parseData\0char*\0buffer\0getList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UIDataController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   38,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       9,    0,   41,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,

 // methods: parameters
    QMetaType::QStringList,

       0        // eod
};

void UIDataController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UIDataController *_t = static_cast<UIDataController *>(_o);
        switch (_id) {
        case 0: _t->sendToQml(); break;
        case 1: _t->sendToCrashDetection((*reinterpret_cast< const SensorData(*)>(_a[1]))); break;
        case 2: _t->parseData((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 3: { QStringList _r = _t->getList();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UIDataController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UIDataController::sendToQml)) {
                *result = 0;
            }
        }
        {
            typedef void (UIDataController::*_t)(const SensorData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UIDataController::sendToCrashDetection)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject UIDataController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UIDataController.data,
      qt_meta_data_UIDataController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UIDataController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UIDataController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UIDataController.stringdata))
        return static_cast<void*>(const_cast< UIDataController*>(this));
    return QObject::qt_metacast(_clname);
}

int UIDataController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void UIDataController::sendToQml()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void UIDataController::sendToCrashDetection(const SensorData & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
