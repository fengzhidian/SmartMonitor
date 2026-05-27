#include "view/mainwidget.h"
#include "util/winmanager.h"
#include "model/settinginfo.h"
#include "view/settingswidget.h"
#include "yolo/Detect.h"
#include <QDebug>
#include <QApplication>
#include <iostream>

void test(){
    std::string modelPath = "data/model/yolov8n.onnx";
    Detect detect(modelPath);
    detect.loadModel();
    detect.model_input_size = cv::Size(640, 640);
    std::vector<YOLO_OUT> yoloOut;
    cv::Mat image = imread("data/images/bus.jpg");
    if (image.empty())
    {
        std::cerr << "No image loaded to run the test." << std::endl;
        return;
    }
    detect.org_Size = image.size();
    cv::Mat out_frame = detect.letterbox(image, detect.model_input_size.width, detect.model_input_size.height, cv::Scalar(114, 114, 114));
    detect.detect(out_frame, yoloOut);
    detect.draw(image, yoloOut);
    cv::imshow("Detected Image", image);
    cv::waitKey(0);

}
void test1(){
    std::string modelPath = "data/model/yolov8n.onnx";
    Detect detect(modelPath);
    detect.loadModel();
    detect.model_input_size = cv::Size(640, 640);
    VideoCapture video;
    video.open("../record_videos/jk1.mp4");
    if(!video.isOpened()){
        std::cerr << "No Video loaded to run the test. " << std::endl;
        return;
    }
    std::vector<YOLO_OUT> yoloOut;
    cv::Mat image;
    while(video.read(image)){
        detect.org_Size = image.size();
        cv::Mat out_frame = detect.letterbox(image, detect.model_input_size.width, detect.model_input_size.height, cv::Scalar(114, 114, 114));
        detect.detect(out_frame, yoloOut);
        detect.draw(image, yoloOut);
        cv::imshow("Detected image",image);
        cv::waitKey(40);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 加载配置文件
    SettingInfo::loadFromFile();

    SettingsWidget *sw = new SettingsWidget;
    if (sw->exec() != QDialog::Accepted) {
        delete sw;
        return 0;
    }
    delete sw;

    WinManager::getInstance()->mainWidget.show();
    // test1();
    return QCoreApplication::exec();
}
