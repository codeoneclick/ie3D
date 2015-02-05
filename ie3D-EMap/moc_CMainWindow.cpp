/****************************************************************************
** Meta object code from reading C++ file 'CMainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Sources/CMainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CMainWindow_t {
    QByteArrayData data[16];
    char stringdata[416];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CMainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CMainWindow_t qt_meta_stringdata_CMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 33),
QT_MOC_LITERAL(2, 46, 0),
QT_MOC_LITERAL(3, 47, 5),
QT_MOC_LITERAL(4, 53, 37),
QT_MOC_LITERAL(5, 91, 31),
QT_MOC_LITERAL(6, 123, 30),
QT_MOC_LITERAL(7, 154, 25),
QT_MOC_LITERAL(8, 180, 25),
QT_MOC_LITERAL(9, 206, 25),
QT_MOC_LITERAL(10, 232, 25),
QT_MOC_LITERAL(11, 258, 41),
QT_MOC_LITERAL(12, 300, 4),
QT_MOC_LITERAL(13, 305, 41),
QT_MOC_LITERAL(14, 347, 41),
QT_MOC_LITERAL(15, 389, 25)
    },
    "CMainWindow\0on_m_brushSizeSlider_valueChanged\0"
    "\0value\0on_m_brushStrengthSlider_valueChanged\0"
    "on_m_falloffSlider_valueChanged\0"
    "on_m_smoothSlider_valueChanged\0"
    "on_m_texture01Btn_pressed\0"
    "on_m_texture01Btn_clicked\0"
    "on_m_texture02Btn_clicked\0"
    "on_m_texture03Btn_clicked\0"
    "on_m_textureTilling01SpinBox_valueChanged\0"
    "arg1\0on_m_textureTilling02SpinBox_valueChanged\0"
    "on_m_textureTilling03SpinBox_valueChanged\0"
    "on_generateButton_clicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x08,
       4,    1,   77,    2, 0x08,
       5,    1,   80,    2, 0x08,
       6,    1,   83,    2, 0x08,
       7,    0,   86,    2, 0x08,
       8,    0,   87,    2, 0x08,
       9,    0,   88,    2, 0x08,
      10,    0,   89,    2, 0x08,
      11,    1,   90,    2, 0x08,
      13,    1,   93,    2, 0x08,
      14,    1,   96,    2, 0x08,
      15,    0,   99,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,

       0        // eod
};

void CMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CMainWindow *_t = static_cast<CMainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_m_brushSizeSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_m_brushStrengthSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_m_falloffSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_m_smoothSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_m_texture01Btn_pressed(); break;
        case 5: _t->on_m_texture01Btn_clicked(); break;
        case 6: _t->on_m_texture02Btn_clicked(); break;
        case 7: _t->on_m_texture03Btn_clicked(); break;
        case 8: _t->on_m_textureTilling01SpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_m_textureTilling02SpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_m_textureTilling03SpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_generateButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject CMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CMainWindow.data,
      qt_meta_data_CMainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *CMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMainWindow.stringdata))
        return static_cast<void*>(const_cast< CMainWindow*>(this));
    if (!strcmp(_clname, "std::enable_shared_from_this<CMainWindow>"))
        return static_cast< std::enable_shared_from_this<CMainWindow>*>(const_cast< CMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
