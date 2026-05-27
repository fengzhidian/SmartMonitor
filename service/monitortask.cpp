#include "monitortask.h"

MonitorTask::MonitorTask(QString path ,int channel_id ,QObject *parent)
    : QObject{parent}
{
    this->filePath = path;
    this->channelId = channel_id;
    this->isStop = false;
    if(!path.isNull()){// 这是一个视频文件
        qDebug() << "file_path:" << filePath;
        video.open(path.toStdString());
    } else{
        video.open(channel_id);// 打开摄像头
    }
    if(!video.isOpened()){
        qDebug() << "open error";
        return;
    }
    // 获取视频原始属性
    width = video.get(CAP_PROP_FRAME_WIDTH);
    height = video.get(CAP_PROP_FRAME_HEIGHT);
    fps = video.get(CAP_PROP_FPS);// 摄像头获取的fps为0，实际为30
    frameCount = video.get(CAP_PROP_FRAME_COUNT);
}

int MonitorTask::getWidth() const
{
    return width;
}

int MonitorTask::getHeight() const
{
    return height;
}

int MonitorTask::getFps() const
{
    return fps;
}

int MonitorTask::getFrameCount() const
{
    return frameCount;
}

void MonitorTask::startMonitor()
{
    // 开启视频解析
    Mat frame;
    int count = 0;
    while(!this->isStop){
        bool flag = video.read(frame);
        if(flag){
            emit sendFrame(frame, this->channelId);
        } else{// 视频播放完成
            bool reset_video = video.set(CAP_PROP_POS_FRAMES,0);// 从头开始播放
            if(!reset_video) break;
        }
        QThread::msleep(40);
    }
    // 释放资源
    qDebug() << "释放资源";
    video.release();
}

void MonitorTask::stopMonitor()
{
    this->isStop = true;
}
