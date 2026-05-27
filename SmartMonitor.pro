QT += widgets sql core multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    model/camerainfo.cpp \
    model/exceptionlogmodel.cpp \
    model/featureimagemodel.cpp \
    model/operationlogmodel.cpp \
    model/settinginfo.cpp \
    model/usermodel.cpp \
    model/video.cpp \
    model/videomodel.cpp \
    service/detecttask.cpp \
    service/monitortask.cpp \
    service/recordtask.cpp \
    service/userservice.cpp \
    service/videoservice.cpp \
    util/dbconn.cpp \
    util/dbconnectionpool.cpp \
    util/winmanager.cpp \
    view/loginwidget.cpp \
    view/logviewwidget.cpp \
    view/mainwidget.cpp \
    view/motionreviewwidget.cpp \
    view/reviewwidget.cpp \
    view/settingswidget.cpp \
    yolo/Detect.cpp \
    yolo/YOLO.cpp

HEADERS += \
    model/camerainfo.h \
    model/exceptionlogmodel.h \
    model/featureimagemodel.h \
    model/operationlogmodel.h \
    model/settinginfo.h \
    model/usermodel.h \
    model/video.h \
    model/videomodel.h \
    service/detecttask.h \
    service/monitortask.h \
    service/recordtask.h \
    service/userservice.h \
    service/videoservice.h \
    util/dbconn.h \
    util/dbconnectionpool.h \
    util/winmanager.h \
    view/loginwidget.h \
    view/logviewwidget.h \
    view/mainwidget.h \
    view/motionreviewwidget.h \
    view/reviewwidget.h \
    view/settingswidget.h \
    yolo/Detect.h \
    yolo/YOLO.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 配置OpenCV路径
OPENCV_ROOT = D:/opencv/opencv
# 引入OpenCV头文件
INCLUDEPATH += $${OPENCV_ROOT}/include
# OpenCV库文件
LIBS += -L$${OPENCV_ROOT}/x64/mingw/lib
# 静态引入OpenCV库文件
# 必选核心库（基础功能+视频读写，依赖 FFmpeg）
LIBS += -llibopencv_core4100        # 核心模块（内存管理、数据结构）
LIBS += -llibopencv_imgproc4100     # 图像处理（滤波、缩放等）
LIBS += -llibopencv_videoio4100     # 视频 I/O（调用 FFmpeg 读写视频）
LIBS += -llibopencv_highgui4100     # 简单 GUI 显示（如 imshow）
LIBS += -llibopencv_imgcodecs4100   # 图像编解码（读写图片）
LIBS += -llibopencv_dnn4100         # OpenCV访问Onnx模型的接口模块
# 可选扩展库（根据项目需求添加）
LIBS += -llibopencv_features2d4100  # 特征检测（如 SIFT/SURF）
LIBS += -llibopencv_calib3d4100    # 相机标定、3D 重建
LIBS += -llibopencv_objdetect4100   # 目标检测（如 Haar 级联）

DESTDIR = $$PWD/bin

RESOURCES += \
    resource.qrc

TARGET = SmartMonitor