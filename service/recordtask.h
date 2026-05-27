#ifndef RECORDTASK_H
#define RECORDTASK_H

#include "../model/videomodel.h"
#include <QObject>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>
#include <opencv2/opencv.hpp>
using namespace cv;

class RecordTask : public QObject
{
    Q_OBJECT
public:
    explicit RecordTask(int intervalSec = 10, int exceptionId = 0, QObject *parent = nullptr);
private:
    VideoWriter videoWriter;
    int frameCount;
    QString fileName;
    QDateTime createTime;
    int lastChannelId;
    int timeInterval;
    int m_exceptionId;
    QString getRecordDir() const;
public slots:
    void saveVideo(cv::Mat mat,int channelId);
    void flushVideo();
signals:
};

#endif // RECORDTASK_H
