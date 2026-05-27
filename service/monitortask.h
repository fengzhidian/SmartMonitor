#ifndef MONITORTASK_H
#define MONITORTASK_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>
#include <opencv2/opencv.hpp>
using namespace cv;

class MonitorTask : public QObject
{
    Q_OBJECT
public:
    explicit MonitorTask(QString path = nullptr,int channel_id = 0,QObject *parent = nullptr);
    volatile bool isStop;// 是否停止
    int getWidth() const;

    int getHeight() const;

    int getFps() const;

    int getFrameCount() const;

private:
    int width;
    int height;
    int fps;
    int frameCount;
    QString filePath;
    int channelId;// 摄像头通道
    VideoCapture video;
public slots:
    void startMonitor();
    void stopMonitor();
signals:
    void sendFrame(cv::Mat mat,int channelId);
};

#endif // MONITORTASK_H
