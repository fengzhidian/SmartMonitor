#include "settinginfo.h"

QList<CameraInfo> SettingInfo::cameras = QList<CameraInfo>();
QString SettingInfo::file_path = QString();
int SettingInfo::timeInterval = 0;

SettingInfo::SettingInfo() {}

void SettingInfo::saveToFile()
{
    QString configPath = "./config.json";
    QFile file(configPath);
    // 打开失败返回
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "file.open(QIODevice::WriteOnly) | QIODevice::Text) error";
        return ;
    }
    // 写入基础配置
    QJsonObject rootObj;
    rootObj["file_path"] = SettingInfo::file_path;
    rootObj["timeInterval"] = SettingInfo::timeInterval;

    // 写入摄像头列表
    QJsonArray cameArray;
    for(CameraInfo &cam : SettingInfo::cameras){
        QJsonObject camObj;
        camObj["cameraName"] = cam.getCameraName();
        camObj["channelName"] = cam.getChannelName();
        camObj["ChannelId"] = cam.getChannelId();
        cameArray.append(camObj);
    }
    rootObj["cameras"] = cameArray;

    // 写入文件
    QJsonDocument doc(rootObj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

// 从文件中读取配置信息
void SettingInfo::loadFromFile()
{
    QString configPath = "./config.json";
    QFile file(configPath);

    // 文件不存在直接返回
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        // 文件不存在则先从本机获取摄像头信息
        SettingInfo::initCamera();
        return;
    }

    // 读取文件并解析为json文档
    QByteArray jsonData = file.readAll();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData,&jsonError);

    // json格式解析失败处理
    if(jsonError.error != QJsonParseError::NoError || !doc.isObject()){
        file.close();
        return;
    }

    // 解析根对象
    QJsonObject rootObj = doc.object();

    // 读取基础配置信息，赋值给静态变量
    SettingInfo::file_path = rootObj["file_path"].toString();
    SettingInfo::timeInterval = rootObj["timeInterval"].toInt();

    // 读取摄像头列表，赋值到静态cameras列表
    // 清空原有数据，防止旧数据残留
    SettingInfo::cameras.clear();
    QJsonArray camArray = rootObj["cameras"].toArray();
    // 遍历摄像头数组，逐个解析赋值
    for(int i = 0; i < camArray.size(); i++){
        QJsonObject camObj = camArray[i].toObject();
        CameraInfo cam;
        cam.setCameraName(camObj["cameraName"].toString());
        cam.setChannelName(camObj["channelName"].toString());
        cam.setChannelId(camObj["ChannelId"].toInt());
        // 添加到静态列表中
        SettingInfo::cameras.append(cam);
    }
    file.close();
}

void SettingInfo::initCamera()
{
    SettingInfo::cameras.clear();
    // 获取本机可用摄像头列表
    QList<QCameraDevice> cameraList = QMediaDevices::videoInputs();

    // 判断是否有可用摄像头
    if(cameraList.isEmpty()){
        qDebug() << "未检测到可用摄像头";
        return;
    }

    // 遍历所有摄像头
    qDebug() << "本机检测到：" << cameraList.size() << "个摄像头设备";
    int i = 0;
    for(i = 0; i < cameraList.size(); i++){
        QCameraDevice camera = cameraList.at(i);
        QString cameraName = camera.description();
        QString cameraId = camera.id();
        bool isDefault = camera.isDefault();
        if(cameraName.contains("IR")) continue;

        // 打印所有信息
        qDebug() << "------------------";
        qDebug() << "序号：" << i + 1;
        qDebug() << "摄像头名称：" << cameraName;
        qDebug() << "摄像头ID：" << cameraId;
        qDebug() << "是否为默认摄像头：" << (isDefault ? "是" : "否");
        CameraInfo cameraInfo;
        cameraInfo.setChannelId(i+1);
        cameraInfo.setCameraName(cameraName);
        cameraInfo.setChannelName(cameraName);
        SettingInfo::cameras.append(cameraInfo);
    }
    for(i--; i < 4 ; i++){
        CameraInfo cameraInfo;
        cameraInfo.setChannelId(i+1);
        cameraInfo.setCameraName("virtual camera");
        cameraInfo.setChannelName("virtual camera");
        SettingInfo::cameras.append(cameraInfo);
    }
}
