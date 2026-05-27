#ifndef REVIEWWIDGET_H
#define REVIEWWIDGET_H

#include "../service/videoservice.h"
#include "../model/featureimagemodel.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <QSplitter>
#include <QFrame>
#include <QButtonGroup>
#include <QMessageBox>
#include <QImage>
#include <QMetaObject>
#include <QPainter>
#include <QDateEdit>
#include <QComboBox>
#include <QCalendarWidget>
#include <QSlider>
#include <QDir>
#include <QStackedWidget>
#include <QScrollArea>
#include <opencv2/opencv.hpp>
#include <QCoreApplication>

class ReviewWidget : public QWidget
{
    Q_OBJECT
private:
    QDateEdit *dateEdit;
    QComboBox *channel_box;
    QListWidget *videos_list;
    QLabel *lab_page;
    QPushButton *btn_prev, *btn_next;

    QStackedWidget *right_stack;
    QWidget *video_page;
    QWidget *image_page;
    QLabel *video_display;
    QPushButton *btn_pause;
    QComboBox *speed_combo;
    QPushButton *btn_screenshot;
    QSlider *progress_slider;
    QPushButton *btn_image_query;

    QLabel *image_grid[3][3];
    QLabel *lab_image_page;
    QPushButton *btn_image_prev, *btn_image_next;
    QStackedWidget *image_stack;
    QWidget *grid_page;
    QLabel *image_single_display;
    QString m_selectedImagePath;

    cv::VideoCapture m_videoCapture;
    QTimer *m_playbackTimer;
    bool m_isPlaying;
    bool m_isPaused;
    double m_playbackSpeed;
    int m_totalFrames;
    int m_currentFrame;
    int m_currentVideoChannelId;

    int current_page;
    int total_page;
    int current_image_page;
    int total_image_page;
    static const int page_size = 10;
    static const int image_page_size = 9;

    QImage MatToImage(const cv::Mat &frame);
public:
    explicit ReviewWidget(QWidget *parent = nullptr);
    void create_widget();
    void setup_left_panel(QWidget *panel);
    void setup_right_panel(QWidget *panel);
    QWidget* create_image_page();
    void stopPlayback();

signals:
    void backToMonitor();
public slots:
    void updateVideoList();
    void prevPage();
    void nextPage();
    void onVideoItemDoubleClicked(QListWidgetItem *item);
    void togglePause();
    void onSpeedChanged(int index);
    void captureScreenshot();
    void onPlaybackTick();
    void onSliderPressed();
    void onSliderReleased();
    void onSliderMoved(int position);
    void toggleImageQuery();
    void updateImageList();
    void showImagesOnGrid(const QList<ImageInfo> &imageList);
    void imagePrevPage();
    void imageNextPage();
    void onImageItemClicked(QListWidgetItem *item);
    void onFilterChanged();
};

#endif // REVIEWWIDGET_H
