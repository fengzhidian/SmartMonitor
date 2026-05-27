#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "../model/settinginfo.h"
#include "../service/monitortask.h"
#include "../service/recordtask.h"
#include "reviewwidget.h"
#include "../model/featureimagemodel.h"
#include "../service/detecttask.h"
#include "motionreviewwidget.h"
#include "logviewwidget.h"
#include "../model/operationlogmodel.h"
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
#include <QStackedWidget>
#include <QMessageBox>
#include <QImage>
#include <QMetaObject>
#include <QPainter>
#include <QDir>
#include <QCloseEvent>
#include <QRect>
#include <QVector>
#include <QCloseEvent>


class MainWidget : public QWidget
{
    Q_OBJECT

private:
    QPushButton *btn_monitor;
    QPushButton *btn_review;
    QPushButton *btn_motion_review;
    QPushButton *btn_setting;
    QPushButton *btn_user;
    QPushButton *btn_auth;
    QLabel *lab_username;

    QPushButton *btn_screenshot;
    QPushButton *btn_motion_detect;

    QLabel *lab_title;
    QLabel *lab_time;
    QLabel *lab_status;

    QListWidget *device_list;
    QLabel *info_online;
    QLabel *info_offline;

    QButtonGroup *channel_group;

    QPushButton *btn_channel[4];
    QPushButton *btn_single_view;
    QPushButton *btn_quad_view;

    QStackedWidget *view_stack;
    QLabel *video_placeholder;
    QLabel *quad_placeholders[4];

    QStackedWidget *content_stack;
    ReviewWidget *reviewWidget;
    MotionReviewWidget *motionReviewWidget;
    LogViewWidget *logViewWidget;

    bool m_loggedIn = false;
    QString m_username;
    int m_userId = 0;

    MonitorTask *mTasks[4] = {};
    QThread *mThreads[4] = {};
    RecordTask *rTasks[4] = {};
    QThread *rThreads[4] = {};
    DetectTask *dTasks[4] = {};
    QThread *dThreads[4] = {};
    QImage m_frames[4];
    QVector<QRect> m_cachedRects[4];
    int m_detectSkipCount[4] = {};
    int m_realCameraCount = 0;
    int m_currentChannel = 0;
    bool m_isRecording = false;
    bool m_motionDetectEnabled = false;

    QWidget* create_top_bar();
    QWidget* create_main_area();
    QWidget* create_bottom_bar();
    void setup_left_panel(QWidget *panel);
    void setup_right_panel(QWidget *panel);
    QWidget* create_single_view();
    QWidget* create_quad_view();
    void updateAuthUI();
    void refresh_device_panel();
    void start_cameras();
    void stop_cameras();
    void start_recording();
    void stop_recording();
    QImage MatToImage(const cv::Mat &frame);
    void insertOperationLog(const QString &func, const QString &desc);

protected:
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public:
    explicit MainWidget(QWidget *parent = nullptr);
    void init_qss();
    void init_time();
    void setLoggedIn(const QString &username, int userId = 0);

signals:

public slots:
    void update_time();
    void open_settings();
    void switch_channel(int channel);
    void show_single_view();
    void show_quad_view();
    void on_auth_btn_clicked();
    void show_monitor_view();
    void show_review_view();
    void show_log_view();
    void show_motion_review_view();
    void capture_screenshot();
    void on_motion_detect_clicked();
};

#endif // MAINWIDGET_H
