/****************************************************************************
** Meta object code from reading C++ file 'detecttask.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../service/detecttask.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'detecttask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10DetectTaskE_t {};
} // unnamed namespace

template <> constexpr inline auto DetectTask::qt_create_metaobjectdata<qt_meta_tag_ZN10DetectTaskE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DetectTask",
        "sendDetectBoxes",
        "",
        "channelId",
        "QList<QRect>",
        "rects",
        "startDetect",
        "cv::Mat",
        "frame"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sendDetectBoxes'
        QtMocHelpers::SignalData<void(int, QVector<QRect>)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { 0x80000000 | 4, 5 },
        }}),
        // Slot 'startDetect'
        QtMocHelpers::SlotData<void(cv::Mat, int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 }, { QMetaType::Int, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DetectTask, qt_meta_tag_ZN10DetectTaskE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DetectTask::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10DetectTaskE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10DetectTaskE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10DetectTaskE_t>.metaTypes,
    nullptr
} };

void DetectTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DetectTask *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sendDetectBoxes((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QList<QRect>>>(_a[2]))); break;
        case 1: _t->startDetect((*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DetectTask::*)(int , QVector<QRect> )>(_a, &DetectTask::sendDetectBoxes, 0))
            return;
    }
}

const QMetaObject *DetectTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DetectTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10DetectTaskE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DetectTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DetectTask::sendDetectBoxes(int _t1, QVector<QRect> _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
QT_WARNING_POP
