/****************************************************************************
** Meta object code from reading C++ file 'scope.h'
**
** Created: Fri Oct 30 01:46:19 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "scope.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scope.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Scope[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x0a,
      20,    6,    6,    6, 0x0a,
      36,   34,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Scope[] = {
    "Scope\0\0saveScreen()\0updateScope()\0s\0"
    "setTimeDiv(double)\0"
};

const QMetaObject Scope::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Scope,
      qt_meta_data_Scope, 0 }
};

const QMetaObject *Scope::metaObject() const
{
    return &staticMetaObject;
}

void *Scope::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Scope))
        return static_cast<void*>(const_cast< Scope*>(this));
    return QWidget::qt_metacast(_clname);
}

int Scope::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: saveScreen(); break;
        case 1: updateScope(); break;
        case 2: setTimeDiv((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
