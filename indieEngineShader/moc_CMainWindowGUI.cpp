/****************************************************************************
** Meta object code from reading C++ file 'CMainWindowGUI.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CMainWindowGUI.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CMainWindowGUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CMainWindowGUI_t {
    QByteArrayData data[15];
    char stringdata[325];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CMainWindowGUI_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CMainWindowGUI_t qt_meta_stringdata_CMainWindowGUI = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 23),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 23),
QT_MOC_LITERAL(4, 64, 37),
QT_MOC_LITERAL(5, 102, 4),
QT_MOC_LITERAL(6, 107, 21),
QT_MOC_LITERAL(7, 129, 21),
QT_MOC_LITERAL(8, 151, 21),
QT_MOC_LITERAL(9, 173, 29),
QT_MOC_LITERAL(10, 203, 27),
QT_MOC_LITERAL(11, 231, 19),
QT_MOC_LITERAL(12, 251, 19),
QT_MOC_LITERAL(13, 271, 21),
QT_MOC_LITERAL(14, 293, 30)
    },
    "CMainWindowGUI\0on_pushButton_2_clicked\0"
    "\0on_pushButton_3_clicked\0"
    "on_materials_list_currentIndexChanged\0"
    "arg1\0on_buttonOpen_clicked\0"
    "on_buttonSave_clicked\0on_buttonExit_clicked\0"
    "on_btn_shader_compile_clicked\0"
    "on_btn_content_path_clicked\0"
    "on_btn_open_clicked\0on_btn_save_clicked\0"
    "on_btn_saveas_clicked\0"
    "on_btn_material_editor_clicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMainWindowGUI[] = {

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
       1,    0,   74,    2, 0x08,
       3,    0,   75,    2, 0x08,
       4,    1,   76,    2, 0x08,
       6,    0,   79,    2, 0x08,
       7,    0,   80,    2, 0x08,
       8,    0,   81,    2, 0x08,
       9,    0,   82,    2, 0x08,
      10,    0,   83,    2, 0x08,
      11,    0,   84,    2, 0x08,
      12,    0,   85,    2, 0x08,
      13,    0,   86,    2, 0x08,
      14,    0,   87,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CMainWindowGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CMainWindowGUI *_t = static_cast<CMainWindowGUI *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_2_clicked(); break;
        case 1: _t->on_pushButton_3_clicked(); break;
        case 2: _t->on_materials_list_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_buttonOpen_clicked(); break;
        case 4: _t->on_buttonSave_clicked(); break;
        case 5: _t->on_buttonExit_clicked(); break;
        case 6: _t->on_btn_shader_compile_clicked(); break;
        case 7: _t->on_btn_content_path_clicked(); break;
        case 8: _t->on_btn_open_clicked(); break;
        case 9: _t->on_btn_save_clicked(); break;
        case 10: _t->on_btn_saveas_clicked(); break;
        case 11: _t->on_btn_material_editor_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject CMainWindowGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CMainWindowGUI.data,
      qt_meta_data_CMainWindowGUI,  qt_static_metacall, 0, 0}
};


const QMetaObject *CMainWindowGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMainWindowGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMainWindowGUI.stringdata))
        return static_cast<void*>(const_cast< CMainWindowGUI*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CMainWindowGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
