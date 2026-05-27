#include "recordtask.h"

// 启动线程后默认执行事件循环
RecordTask::RecordTask(int intervalSec, int exceptionId, QObject *parent)
    : QObject{parent}
{
    this->frameCount = 0;
    this->lastChannelId = -1;
    this->timeInterval = intervalSec;
    this->m_exceptionId = exceptionId;
}

QString RecordTask::getRecordDir() const
{
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString dirPath = appDir.absoluteFilePath("record_videos");
    QDir dir;
    if (!dir.exists(dirPath)) {
        dir.mkpath(dirPath);
    }
    return dirPath;
}

void RecordTask::saveVideo(Mat mat, int channelId)
{
    this->lastChannelId = channelId;
    // 保存视频帧到文件中
    // 首帧，创建VideoWriter，写入首帧
    if(!this->videoWriter.isOpened()){// 未创建VideoWriter
        // 视频文件名定义: Record_yyyymmdd-hhmmss_1.mp4
        createTime = QDateTime::currentDateTime();// 读取当前时间
        // createTime = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");
        fileName = "Record_" + createTime.toString("yyyyMMdd-hhmmss")
                   + "_" + QString::number(channelId + 1) + ".mp4";
        qDebug() << "创建文件：" << fileName;
        // 定义视频编码
        int fourcc = videoWriter.fourcc('H','2','6','4');
        Size frameSize(mat.cols,mat.rows);
        videoWriter = VideoWriter("../record_videos/" + fileName.toStdString(),fourcc,30,frameSize,true);
        if(!videoWriter.isOpened()){
            qDebug() << "videoWriter创建失败";
            return;
        }
    }
    // 写入每帧数据
    videoWriter.write(mat);
    this->frameCount++;
    // 尾帧，关闭VideoWriter，并初始化，为下一个视频写入做准备
     if(this->frameCount >= 30 * this->timeInterval){// 10秒保存一个视频
        videoWriter.release();
        qDebug() << "文件:" << fileName << "写入结束";
        int totalFrames = this->frameCount;
        this->frameCount = 0;
        // 写入数据库
        VideoModel vm;
        Video v;
        v.setFileSize(totalFrames);
        v.setVideoName(fileName);
        v.setVideoPath("../record_videos/" + fileName);
        v.setChannel_id(this->lastChannelId + 1);
        v.setCreateTime(createTime.toString("yyyy-MM-dd hh:mm:ss"));
        v.setExceptionId(this->m_exceptionId);
        bool result = vm.insertVideo(v);
        if(result){
            qDebug() << fileName << "视频插入成功";
        }
    }
}

void RecordTask::flushVideo()
{
    if(videoWriter.isOpened()){
        videoWriter.release();
        qDebug() << "文件:" << fileName << "写入结束(手动停止)";
        VideoModel vm;
        Video v;
        v.setFileSize(this->frameCount);
        v.setVideoName(fileName);
        v.setVideoPath("../record_videos/" + fileName);
        v.setChannel_id(this->lastChannelId + 1);
        v.setCreateTime(createTime.toString("yyyy-MM-dd hh:mm:ss"));
        v.setExceptionId(this->m_exceptionId);
        bool result = vm.insertVideo(v);
        if(result){
            qDebug() << fileName << "视频插入成功";
        }
        this->frameCount = 0;
    }
}
