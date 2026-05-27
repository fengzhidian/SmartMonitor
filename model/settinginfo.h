#ifndef SETTINGINFO_H
#define SETTINGINFO_H

#include "camerainfo.h"
#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QCameraDevice>
#include <QMediaDevices>

class SettingInfo
{
public:
    SettingInfo();
    static QString file_path; // 录像保存路径
    static QList<CameraInfo> cameras; // 摄像头
    static int timeInterval; // 采集时间间隔
    static void saveToFile(); // 保存配置到文件中
    static void loadFromFile(); // 从配置文件加载配置信息
    static void initCamera();
};

#endif // SETTINGINFO_H
