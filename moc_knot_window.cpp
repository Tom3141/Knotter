/****************************************************************************
** Meta object code from reading C++ file 'knot_window.hpp'
**
** Created: Thu Aug 16 22:16:27 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "knot_window.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'knot_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Knot_Window[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x09,
      41,   35,   12,   12, 0x09,
      60,   12,   12,   12, 0x0a,
      77,   12,   12,   12, 0x0a,
      89,   12,   12,   12, 0x0a,
     101,   12,   12,   12, 0x0a,
     122,   12,   12,   12, 0x0a,
     129,   12,   12,   12, 0x0a,
     135,   12,   12,   12, 0x0a,
     143,   12,   12,   12, 0x0a,
     151,   12,   12,   12, 0x0a,
     158,   12,   12,   12, 0x0a,
     167,   12,   12,   12, 0x0a,
     174,   12,   12,   12, 0x0a,
     186,   12,   12,   12, 0x0a,
     200,   12,   12,   12, 0x0a,
     210,   12,   12,   12, 0x0a,
     221,   12,   12,   12, 0x0a,
     246,  238,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Knot_Window[] = {
    "Knot_Window\0\0update_style_dialog()\0"
    "clean\0update_title(bool)\0mode_edge_list()\0"
    "mode_node()\0mode_edge()\0mouse_moved(QPointF)\0"
    "copy()\0cut()\0paste()\0clear()\0save()\0"
    "saveAs()\0open()\0exportSVG()\0apply_style()\0"
    "zoom_in()\0zoom_out()\0configure_grid()\0"
    "enabled\0enable_grid(bool)\0"
};

void Knot_Window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Knot_Window *_t = static_cast<Knot_Window *>(_o);
        switch (_id) {
        case 0: _t->update_style_dialog(); break;
        case 1: _t->update_title((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->mode_edge_list(); break;
        case 3: _t->mode_node(); break;
        case 4: _t->mode_edge(); break;
        case 5: _t->mouse_moved((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 6: _t->copy(); break;
        case 7: _t->cut(); break;
        case 8: _t->paste(); break;
        case 9: _t->clear(); break;
        case 10: _t->save(); break;
        case 11: _t->saveAs(); break;
        case 12: _t->open(); break;
        case 13: _t->exportSVG(); break;
        case 14: _t->apply_style(); break;
        case 15: _t->zoom_in(); break;
        case 16: _t->zoom_out(); break;
        case 17: _t->configure_grid(); break;
        case 18: _t->enable_grid((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Knot_Window::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Knot_Window::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Knot_Window,
      qt_meta_data_Knot_Window, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Knot_Window::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Knot_Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Knot_Window::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Knot_Window))
        return static_cast<void*>(const_cast< Knot_Window*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Knot_Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
