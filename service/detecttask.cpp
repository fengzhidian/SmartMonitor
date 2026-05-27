#include "detecttask.h"

DetectTask::DetectTask(QObject *parent)
    : QObject{parent},detect("data/model/yolov8n.onnx")
{
    // 加载模型
    detect.loadModel();
    // 获取模型尺寸
    detect.model_input_size = cv::Size(320, 320);
}

void DetectTask::startDetect(cv::Mat frame, int channelId)
{
    // 获取原始图片大小
    detect.org_Size = frame.size();
    // 模型推理
    if(this->frameCount % 3 == 0){ // 抽帧处理
        // 图像预处理
        cv::Mat out_frame = detect.letterbox(frame, detect.model_input_size.width, detect.model_input_size.height, cv::Scalar(114, 114, 114));
        yoloOut.clear();
        detect.detect(out_frame,yoloOut);
        QVector<QRect> rects;
        for (const auto &out : yoloOut) {
            const cv::Rect &r = out.outRect;
            rects.append(QRect(r.x, r.y, r.width, r.height));
        }
        // 发送信号给UI线程,等待主线程绘制图像
        emit sendDetectBoxes(channelId, rects);
    }
    this->frameCount ++;
}
