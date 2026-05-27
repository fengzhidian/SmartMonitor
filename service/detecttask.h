#ifndef DETECTTASK_H
#define DETECTTASK_H

#include <QObject>
#include <QRect>
#include <QVector>
#include "../yolo/Detect.h"

class DetectTask : public QObject
{
    Q_OBJECT
private:
    Detect detect;
    std::vector<YOLO_OUT> yoloOut;
    int frameCount = 0; // 帧计数器
public:
    explicit DetectTask(QObject *parent = nullptr);

signals:
    void sendDetectBoxes(int channelId, QVector<QRect> rects);
public slots:
    void startDetect(cv::Mat frame, int channelId);
};

#endif // DETECTTASK_H
