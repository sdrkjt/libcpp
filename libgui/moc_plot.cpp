/****************************************************************************
** Meta object code from reading C++ file 'plot.hh'
**
** Created: Sun Sep 19 09:13:28 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "plot.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plot.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SdrPlot[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,   31,    8,    8, 0x0a,
      57,   53,    8,    8, 0x0a,
      87,   83,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SdrPlot[] = {
    "SdrPlot\0\0sig_showRunning(bool)\0ok\0"
    "setPlotState(bool)\0pos\0addMarker(QwtDoublePoint)\0"
    "p,v\0slo_legendChecked(QwtPlotItem*,bool)\0"
};

const QMetaObject SdrPlot::staticMetaObject = {
    { &QwtPlot::staticMetaObject, qt_meta_stringdata_SdrPlot,
      qt_meta_data_SdrPlot, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SdrPlot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SdrPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SdrPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SdrPlot))
        return static_cast<void*>(const_cast< SdrPlot*>(this));
    return QwtPlot::qt_metacast(_clname);
}

int SdrPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sig_showRunning((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: setPlotState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: addMarker((*reinterpret_cast< const QwtDoublePoint(*)>(_a[1]))); break;
        case 3: slo_legendChecked((*reinterpret_cast< QwtPlotItem*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SdrPlot::sig_showRunning(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_PlotConfig[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      20,   11,   11,   11, 0x0a,
      35,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PlotConfig[] = {
    "PlotConfig\0\0setup()\0setupClicked()\0"
    "colorClicked()\0"
};

const QMetaObject PlotConfig::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_PlotConfig,
      qt_meta_data_PlotConfig, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PlotConfig::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PlotConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PlotConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlotConfig))
        return static_cast<void*>(const_cast< PlotConfig*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int PlotConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setup(); break;
        case 1: setupClicked(); break;
        case 2: colorClicked(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PlotConfig::setup()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_SdrMainWin[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      32,   24,   11,   11, 0x0a,
      54,   11,   11,   11, 0x0a,
      69,   11,   11,   11, 0x0a,
      85,   11,   11,   11, 0x0a,
     101,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SdrMainWin[] = {
    "SdrMainWin\0\0printPlot()\0running\0"
    "slo_showRunning(bool)\0setupClicked()\0"
    "zoomerClicked()\0pannerClicked()\0"
    "markerClicked()\0"
};

const QMetaObject SdrMainWin::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SdrMainWin,
      qt_meta_data_SdrMainWin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SdrMainWin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SdrMainWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SdrMainWin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SdrMainWin))
        return static_cast<void*>(const_cast< SdrMainWin*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SdrMainWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: printPlot(); break;
        case 1: slo_showRunning((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: setupClicked(); break;
        case 3: zoomerClicked(); break;
        case 4: pannerClicked(); break;
        case 5: markerClicked(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
static const uint qt_meta_data_SdrGui[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,    8,    7,    7, 0x0a,
      70,   41,    7,    7, 0x0a,
     129,  110,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SdrGui[] = {
    "SdrGui\0\0Id,title\0slo_figure(int,QString)\0"
    "curveId,colStr,symStr,styStr\0"
    "slo_setpen(int,QString,QString,QString)\0"
    "curveId,data,title\0slo_plot(int,SdrData,QString)\0"
};

const QMetaObject SdrGui::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SdrGui,
      qt_meta_data_SdrGui, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SdrGui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SdrGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SdrGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SdrGui))
        return static_cast<void*>(const_cast< SdrGui*>(this));
    return QObject::qt_metacast(_clname);
}

int SdrGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slo_figure((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: slo_setpen((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 2: slo_plot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const SdrData(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_SdrThread[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   11,   10,   10, 0x05,
      69,   60,   10,   10, 0x05,
     121,   93,   10,   10, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SdrThread[] = {
    "SdrThread\0\0curveId,data,title\0"
    "sig_plot(int,SdrData,QString)\0Id,title\0"
    "sig_figure(int,QString)\0"
    "curveId,colStr,symStr,stySt\0"
    "sig_setpen(int,QString,QString,QString)\0"
};

const QMetaObject SdrThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SdrThread,
      qt_meta_data_SdrThread, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SdrThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SdrThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SdrThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SdrThread))
        return static_cast<void*>(const_cast< SdrThread*>(this));
    return QThread::qt_metacast(_clname);
}

int SdrThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sig_plot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const SdrData(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: sig_figure((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: sig_setpen((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SdrThread::sig_plot(int _t1, const SdrData & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SdrThread::sig_figure(int _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SdrThread::sig_setpen(int _t1, const QString & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_SdrMarker[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_SdrMarker[] = {
    "SdrMarker\0"
};

const QMetaObject SdrMarker::staticMetaObject = {
    { &QwtPlotPicker::staticMetaObject, qt_meta_stringdata_SdrMarker,
      qt_meta_data_SdrMarker, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SdrMarker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SdrMarker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SdrMarker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SdrMarker))
        return static_cast<void*>(const_cast< SdrMarker*>(this));
    return QwtPlotPicker::qt_metacast(_clname);
}

int SdrMarker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlotPicker::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
