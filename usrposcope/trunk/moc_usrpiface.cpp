/****************************************************************************
** Meta object code from reading C++ file 'usrpiface.h'
**
** Created: Fri Oct 30 01:46:20 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "usrpiface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usrpiface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_USRPiface[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,   26,   10,   10, 0x0a,
      45,   41,   10,   10, 0x0a,
      61,   59,   10,   10, 0x0a,
      80,   74,   10,   10, 0x0a,
      94,   74,   10,   10, 0x0a,
     108,   74,   10,   10, 0x0a,
     123,   74,   10,   10, 0x0a,
     137,   74,   10,   10, 0x0a,
     154,   74,   10,   10, 0x0a,
     179,  173,   10,   10, 0x0a,
     197,   10,   10,   10, 0x0a,
     213,  209,   10,   10, 0x0a,
     231,  209,   10,   10, 0x0a,
     249,  209,   10,   10, 0x0a,
     268,  209,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_USRPiface[] = {
    "USRPiface\0\0drawingReady()\0f\0setFreq(int)\0"
    "val\0setDecim(int)\0t\0setTrim(int)\0state\0"
    "showInph(int)\0showQuad(int)\0showMixed(int)\0"
    "showSpec(int)\0showFreqMod(int)\0"
    "showAntiAlias(int)\0pause\0pauseResume(bool)\0"
    "normalize()\0set\0setTriggerI(bool)\0"
    "setTriggerQ(bool)\0setTriggerIQ(bool)\0"
    "setTriggerNone(bool)\0"
};

const QMetaObject USRPiface::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_USRPiface,
      qt_meta_data_USRPiface, 0 }
};

const QMetaObject *USRPiface::metaObject() const
{
    return &staticMetaObject;
}

void *USRPiface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_USRPiface))
        return static_cast<void*>(const_cast< USRPiface*>(this));
    return QThread::qt_metacast(_clname);
}

int USRPiface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: drawingReady(); break;
        case 1: setFreq((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: setDecim((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: setTrim((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: showInph((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: showQuad((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: showMixed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: showSpec((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: showFreqMod((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: showAntiAlias((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: pauseResume((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: normalize(); break;
        case 12: setTriggerI((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: setTriggerQ((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: setTriggerIQ((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: setTriggerNone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void USRPiface::drawingReady()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
