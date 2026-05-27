/****************************************************************************
** Meta object code from reading C++ file 'reviewwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../view/reviewwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'reviewwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12ReviewWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ReviewWidget::qt_create_metaobjectdata<qt_meta_tag_ZN12ReviewWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ReviewWidget",
        "backToMonitor",
        "",
        "updateVideoList",
        "prevPage",
        "nextPage",
        "onVideoItemDoubleClicked",
        "QListWidgetItem*",
        "item",
        "togglePause",
        "onSpeedChanged",
        "index",
        "captureScreenshot",
        "onPlaybackTick",
        "onSliderPressed",
        "onSliderReleased",
        "onSliderMoved",
        "position",
        "toggleImageQuery",
        "updateImageList",
        "showImagesOnGrid",
        "QList<ImageInfo>",
        "imageList",
        "imagePrevPage",
        "imageNextPage",
        "onImageItemClicked",
        "onFilterChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'backToMonitor'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateVideoList'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'prevPage'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'nextPage'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onVideoItemDoubleClicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'togglePause'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onSpeedChanged'
        QtMocHelpers::SlotData<void(int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Slot 'captureScreenshot'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onPlaybackTick'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onSliderPressed'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onSliderReleased'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onSliderMoved'
        QtMocHelpers::SlotData<void(int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Slot 'toggleImageQuery'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateImageList'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'showImagesOnGrid'
        QtMocHelpers::SlotData<void(const QList<ImageInfo> &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 21, 22 },
        }}),
        // Slot 'imagePrevPage'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'imageNextPage'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onImageItemClicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'onFilterChanged'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ReviewWidget, qt_meta_tag_ZN12ReviewWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ReviewWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ReviewWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ReviewWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ReviewWidgetE_t>.metaTypes,
    nullptr
} };

void ReviewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ReviewWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->backToMonitor(); break;
        case 1: _t->updateVideoList(); break;
        case 2: _t->prevPage(); break;
        case 3: _t->nextPage(); break;
        case 4: _t->onVideoItemDoubleClicked((*reinterpret_cast<std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 5: _t->togglePause(); break;
        case 6: _t->onSpeedChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->captureScreenshot(); break;
        case 8: _t->onPlaybackTick(); break;
        case 9: _t->onSliderPressed(); break;
        case 10: _t->onSliderReleased(); break;
        case 11: _t->onSliderMoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->toggleImageQuery(); break;
        case 13: _t->updateImageList(); break;
        case 14: _t->showImagesOnGrid((*reinterpret_cast<std::add_pointer_t<QList<ImageInfo>>>(_a[1]))); break;
        case 15: _t->imagePrevPage(); break;
        case 16: _t->imageNextPage(); break;
        case 17: _t->onImageItemClicked((*reinterpret_cast<std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 18: _t->onFilterChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ReviewWidget::*)()>(_a, &ReviewWidget::backToMonitor, 0))
            return;
    }
}

const QMetaObject *ReviewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReviewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ReviewWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ReviewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void ReviewWidget::backToMonitor()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
