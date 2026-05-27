#include "mainwidget.h"
#include "../util/winmanager.h"
#include <QCoreApplication>
#include <QSettings>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("智能监控系统");
    setMinimumSize(1000, 700);
    resize(1400, 900);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    this->setLayout(main_layout);

    main_layout->addWidget(create_top_bar());
    content_stack = new QStackedWidget;
    content_stack->addWidget(create_main_area());
    reviewWidget = new ReviewWidget(this);
    content_stack->addWidget(reviewWidget);
    motionReviewWidget = new MotionReviewWidget(this);
    content_stack->addWidget(motionReviewWidget);
    logViewWidget = new LogViewWidget(this);
    content_stack->addWidget(logViewWidget);
    content_stack->setCurrentIndex(0);
    main_layout->addWidget(content_stack, 1);
    main_layout->addWidget(create_bottom_bar());

    this->init_qss();
    this->init_time();
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<QVector<QRect>>("QVector<QRect>");

    updateAuthUI();
    start_cameras();
}

void MainWidget::show_monitor_view()
{
    content_stack->setCurrentIndex(0);
}

void MainWidget::show_review_view()
{
    content_stack->setCurrentIndex(1);
    reviewWidget->updateVideoList();
    insertOperationLog("进入录像回看", "管理员进入录像回看页面");
}

void MainWidget::show_motion_review_view()
{
    content_stack->setCurrentIndex(2);
    motionReviewWidget->updateVideoList();
    insertOperationLog("进入移动侦测回看", "管理员进入移动侦测回看页面");
}

void MainWidget::show_log_view()
{
    content_stack->setCurrentIndex(3);
}

QWidget* MainWidget::create_top_bar()
{
    QWidget *top_bar = new QWidget;
    top_bar->setObjectName("top_bar");
    top_bar->setFixedHeight(70);

    QHBoxLayout *top_layout = new QHBoxLayout;
    top_layout->setContentsMargins(20, 0, 20, 0);
    top_layout->setSpacing(0);
    top_bar->setLayout(top_layout);

    lab_title = new QLabel("智能监控系统管理平台");
    lab_title->setObjectName("title");
    top_layout->addWidget(lab_title);

    QLabel *lab_subtitle = new QLabel("SMART MONITOR SYSTEM");
    lab_subtitle->setObjectName("subtitle");
    top_layout->addWidget(lab_subtitle);

    top_layout->addStretch();

    QWidget *btn_group = new QWidget;
    btn_group->setObjectName("btn_group");
    QHBoxLayout *btn_layout = new QHBoxLayout;
    btn_layout->setContentsMargins(0, 0, 0, 0);
    btn_layout->setSpacing(8);
    btn_group->setLayout(btn_layout);

    btn_monitor = new QPushButton("实时监控");
    btn_monitor->setObjectName("nav_btn");
    btn_monitor->setCursor(Qt::PointingHandCursor);

    btn_review = new QPushButton("录像回放");
    btn_review->setObjectName("nav_btn");
    btn_review->setCursor(Qt::PointingHandCursor);

    btn_motion_review = new QPushButton("移动侦测回看");
    btn_motion_review->setObjectName("nav_btn");
    btn_motion_review->setCursor(Qt::PointingHandCursor);

    btn_setting = new QPushButton("系统设置");
    btn_setting->setObjectName("nav_btn");
    btn_setting->setCursor(Qt::PointingHandCursor);

    btn_user = new QPushButton("日志查看");
    btn_user->setObjectName("nav_btn");
    btn_user->setCursor(Qt::PointingHandCursor);

    btn_layout->addWidget(btn_monitor);
    btn_layout->addWidget(btn_review);
    btn_layout->addWidget(btn_motion_review);
    btn_layout->addWidget(btn_setting);
    btn_layout->addWidget(btn_user);

    top_layout->addWidget(btn_group);

    QWidget *user_box = new QWidget;
    user_box->setObjectName("user_box");
    QHBoxLayout *user_layout = new QHBoxLayout;
    user_layout->setContentsMargins(0, 0, 0, 0);
    user_layout->setSpacing(8);
    user_box->setLayout(user_layout);

    QLabel *lab_avatar = new QLabel;
    lab_avatar->setObjectName("avatar");
    lab_avatar->setFixedSize(36, 36);
    lab_avatar->setAlignment(Qt::AlignCenter);
    lab_avatar->setText(QString::fromUtf8("\xF0\x9F\x91\xA4"));

    lab_username = new QLabel(QString::fromUtf8("\xE6\xB8\xB8\xE5\xAE\xA2"));
    lab_username->setObjectName("username");

    btn_auth = new QPushButton(QString::fromUtf8("\xE7\x99\xBB\xE5\xBD\x95"));
    btn_auth->setObjectName("btn_logout");
    btn_auth->setCursor(Qt::PointingHandCursor);

    user_layout->addStretch();
    user_layout->addWidget(lab_avatar);
    user_layout->addWidget(lab_username);
    user_layout->addWidget(btn_auth);

    top_layout->addWidget(user_box);

    connect(btn_monitor, &QPushButton::clicked, this, &MainWidget::show_monitor_view);
    connect(btn_review, &QPushButton::clicked, this, &MainWidget::show_review_view);
    connect(btn_motion_review, &QPushButton::clicked, this, &MainWidget::show_motion_review_view);
    connect(btn_user, &QPushButton::clicked, this, &MainWidget::show_log_view);
    connect(btn_setting, &QPushButton::clicked, this, &MainWidget::open_settings);
    connect(btn_auth, &QPushButton::clicked, this, &MainWidget::on_auth_btn_clicked);

    return top_bar;
}

QWidget* MainWidget::create_main_area()
{
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(1);
    splitter->setObjectName("main_splitter");

    QWidget *left_panel = new QWidget;
    left_panel->setObjectName("left_panel");
    left_panel->setFixedWidth(260);
    setup_left_panel(left_panel);

    QWidget *right_panel = new QWidget;
    right_panel->setObjectName("right_panel");
    setup_right_panel(right_panel);

    splitter->addWidget(left_panel);
    splitter->addWidget(right_panel);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    return splitter;
}

void MainWidget::setup_left_panel(QWidget *panel)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);
    panel->setLayout(layout);

    QLabel *lab_device_title = new QLabel("设备列表");
    lab_device_title->setObjectName("panel_title");
    layout->addWidget(lab_device_title);

    QFrame *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setObjectName("separator");
    layout->addWidget(separator);

    device_list = new QListWidget;
    device_list->setObjectName("device_list");
    device_list->setIconSize(QSize(24, 24));

    layout->addWidget(device_list);
    layout->addStretch();

    QWidget *device_info = new QWidget;
    device_info->setObjectName("device_info");
    QVBoxLayout *info_layout = new QVBoxLayout;
    info_layout->setContentsMargins(8, 8, 8, 8);
    info_layout->setSpacing(4);
    device_info->setLayout(info_layout);

    info_online = new QLabel;
    info_online->setObjectName("info_label");
    info_layout->addWidget(info_online);

    info_offline = new QLabel;
    info_offline->setObjectName("info_label_warn");
    info_layout->addWidget(info_offline);

    layout->addWidget(device_info);

    connect(device_list, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        int channelIdx = item->data(Qt::UserRole).toInt();
        if (channelIdx >= 0 && channelIdx < 4) {
            m_currentChannel = channelIdx;
            btn_channel[channelIdx]->setChecked(true);
            if (view_stack->currentIndex() != 0) {
                view_stack->setCurrentIndex(0);
                btn_single_view->setChecked(true);
            }
            update();
        }
    });

    refresh_device_panel();
}

QWidget* MainWidget::create_single_view()
{
    QWidget *single_widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    single_widget->setLayout(layout);

    video_placeholder = new QLabel;
    video_placeholder->setObjectName("video_placeholder");
    video_placeholder->setAlignment(Qt::AlignCenter);
    video_placeholder->setScaledContents(true);
    video_placeholder->setText("视频监控画面区域\nVIDEO MONITOR AREA");
    layout->addWidget(video_placeholder, 1);

    return single_widget;
}

QWidget* MainWidget::create_quad_view()
{
    QWidget *quad_widget = new QWidget;
    QGridLayout *grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(4);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(1, 1);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);
    quad_widget->setLayout(grid);

    for (int i = 0; i < 4; ++i) {
        quad_placeholders[i] = new QLabel;
        quad_placeholders[i]->setObjectName("video_placeholder");
        quad_placeholders[i]->setAlignment(Qt::AlignCenter);
        quad_placeholders[i]->setScaledContents(true);
        quad_placeholders[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        quad_placeholders[i]->setMinimumSize(1, 1);
        quad_placeholders[i]->setText(QString("通道%1\nCHANNEL %1").arg(i + 1));

        int row = i / 2;
        int col = i % 2;
        grid->addWidget(quad_placeholders[i], row, col);
    }

    return quad_widget;
}

void MainWidget::setup_right_panel(QWidget *panel)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);
    panel->setLayout(layout);

    QWidget *display_header = new QWidget;
    QHBoxLayout *header_layout = new QHBoxLayout;
    header_layout->setContentsMargins(0, 0, 0, 0);
    display_header->setLayout(header_layout);

    QLabel *lab_view_title = new QLabel("监控画面");
    lab_view_title->setObjectName("panel_title");
    header_layout->addWidget(lab_view_title);

    btn_single_view = new QPushButton("单通道");
    btn_single_view->setObjectName("view_btn");
    btn_single_view->setCursor(Qt::PointingHandCursor);
    btn_single_view->setCheckable(true);
    btn_single_view->setChecked(true);

    btn_quad_view = new QPushButton("四通道");
    btn_quad_view->setObjectName("view_btn");
    btn_quad_view->setCursor(Qt::PointingHandCursor);
    btn_quad_view->setCheckable(true);

    QButtonGroup *view_group = new QButtonGroup(this);
    view_group->setExclusive(true);
    view_group->addButton(btn_single_view);
    view_group->addButton(btn_quad_view);

    header_layout->addWidget(btn_single_view);
    header_layout->addWidget(btn_quad_view);

    header_layout->addStretch();

    layout->addWidget(display_header);

    QFrame *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setObjectName("separator");
    layout->addWidget(separator);

    view_stack = new QStackedWidget;
    view_stack->addWidget(create_single_view());
    view_stack->addWidget(create_quad_view());
    view_stack->setCurrentIndex(0);
    layout->addWidget(view_stack, 1);

    QWidget *control_bar = new QWidget;
    control_bar->setObjectName("control_bar");
    QHBoxLayout *control_layout = new QHBoxLayout;
    control_layout->setContentsMargins(0, 0, 0, 0);
    control_layout->setSpacing(8);
    control_bar->setLayout(control_layout);

    btn_screenshot = new QPushButton("截图");
    btn_screenshot->setObjectName("ctrl_btn");
    btn_screenshot->setCursor(Qt::PointingHandCursor);

    btn_motion_detect = new QPushButton("开启移动侦测");
    btn_motion_detect->setObjectName("ctrl_btn");
    btn_motion_detect->setCursor(Qt::PointingHandCursor);
    btn_motion_detect->setCheckable(true);

    control_layout->addWidget(btn_screenshot);
    control_layout->addWidget(btn_motion_detect);
    control_layout->addStretch();

    layout->addWidget(control_bar);

    QWidget *channel_bar = new QWidget;
    channel_bar->setObjectName("channel_bar");
    QHBoxLayout *channel_layout = new QHBoxLayout;
    channel_layout->setContentsMargins(0, 0, 0, 0);
    channel_layout->setSpacing(8);
    channel_bar->setLayout(channel_layout);

    QLabel *lab_channel = new QLabel("通道切换:");
    lab_channel->setObjectName("channel_label");

    channel_group = new QButtonGroup(this);
    channel_group->setExclusive(true);

    channel_layout->addWidget(lab_channel);

    for (int i = 0; i < 4; ++i) {
        btn_channel[i] = new QPushButton(QString("通道%1").arg(i + 1));
        btn_channel[i]->setObjectName("channel_btn");
        btn_channel[i]->setCursor(Qt::PointingHandCursor);
        btn_channel[i]->setCheckable(true);
        channel_group->addButton(btn_channel[i], i);

        if (i < SettingInfo::cameras.size()) {
            btn_channel[i]->setToolTip(SettingInfo::cameras[i].getChannelName());
        }

        channel_layout->addWidget(btn_channel[i]);
    }

    btn_channel[0]->setChecked(true);

    channel_layout->addStretch();
    layout->addWidget(channel_bar);

    connect(channel_group, &QButtonGroup::idClicked, this, &MainWidget::switch_channel);
    connect(btn_single_view, &QPushButton::clicked, this, &MainWidget::show_single_view);
    connect(btn_quad_view, &QPushButton::clicked, this, &MainWidget::show_quad_view);
    connect(btn_screenshot, &QPushButton::clicked, this, &MainWidget::capture_screenshot);
    connect(btn_motion_detect, &QPushButton::clicked, this, &MainWidget::on_motion_detect_clicked);
}

QWidget* MainWidget::create_bottom_bar()
{
    QWidget *bottom_bar = new QWidget;
    bottom_bar->setObjectName("bottom_bar");
    bottom_bar->setFixedHeight(36);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(20, 0, 20, 0);
    layout->setSpacing(20);
    bottom_bar->setLayout(layout);

    lab_status = new QLabel("系统运行正常");
    lab_status->setObjectName("status_label");
    layout->addWidget(lab_status);

    layout->addStretch();

    lab_time = new QLabel;
    lab_time->setObjectName("time_label");
    layout->addWidget(lab_time);

    return bottom_bar;
}

void MainWidget::init_qss()
{
    QFile file(":/qss/main.qss");
    if (file.open(QFile::ReadOnly)) {
        setStyleSheet(file.readAll());
        file.close();
    }
}

void MainWidget::init_time()
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &MainWidget::update_time);
    timer->start();
    update_time();
}

void MainWidget::update_time()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("yyyy-MM-dd HH:mm:ss");
    lab_time->setText(timeString);
}

void MainWidget::updateAuthUI()
{
    if (m_loggedIn) {
        lab_username->setText(m_username);
        btn_auth->setText(QString::fromUtf8("\xE9\x80\x80\xE5\x87\xBA"));
        btn_auth->setToolTip("点击退出登录");
        btn_review->setEnabled(true);
        btn_setting->setEnabled(true);
        btn_motion_review->setEnabled(true);
        btn_user->setEnabled(true);
        btn_screenshot->setEnabled(true);
        btn_motion_detect->setEnabled(true);
    } else {
        lab_username->setText(QString::fromUtf8("\xE6\xB8\xB8\xE5\xAE\xA2"));
        btn_auth->setText(QString::fromUtf8("\xE7\x99\xBB\xE5\xBD\x95"));
        btn_auth->setToolTip("点击登录");
        btn_review->setEnabled(false);
        btn_motion_review->setEnabled(false);
        btn_setting->setEnabled(false);
        btn_user->setEnabled(false);
        btn_screenshot->setEnabled(false);
        btn_motion_detect->setEnabled(false);
    }
}

void MainWidget::refresh_device_panel()
{
    device_list->clear();

    int onlineCount = 0;
    int offlineCount = 0;

    for (int i = 0; i < SettingInfo::cameras.size() && i < 4; ++i) {
        const CameraInfo &cam = SettingInfo::cameras[i];
        int channelIdx = cam.getChannelId() - 1;
        if (channelIdx < 0 || channelIdx >= 4) {
            channelIdx = i;
        }

        bool isReal = cam.getCameraName() != "virtual camera";
        QString status = isReal ? "在线" : "离线";
        if (isReal) onlineCount++; else offlineCount++;

        QString displayText = QString("%1 - %2")
                                  .arg(cam.getCameraName())
                                  .arg(cam.getChannelName());
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setToolTip(status);
        item->setData(Qt::UserRole, channelIdx);
        if (!isReal) {
            item->setForeground(QColor("#ff9100"));
        }
        device_list->addItem(item);
    }

    info_online->setText(QString("在线设备: %1 台").arg(onlineCount));
    info_offline->setText(QString("离线设备: %1 台").arg(offlineCount));
}

void MainWidget::setLoggedIn(const QString &username, int userId)
{
    m_loggedIn = true;
    m_username = username;
    m_userId = userId;
    updateAuthUI();
    start_recording();
    insertOperationLog("用户登录", "管理员登录系统");
}

void MainWidget::on_auth_btn_clicked()
{
    if (m_loggedIn) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("确认退出");
        msgBox.setText("是否确认退出登录？");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, QString::fromUtf8("\xE7\xA1\xAE\xE8\xAE\xA4"));
        msgBox.setButtonText(QMessageBox::No, QString::fromUtf8("\xE5\x8F\x96\xE6\xB6\x88"));
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes) {
            insertOperationLog("用户退出", "管理员退出登录");
            m_loggedIn = false;
            m_username.clear();
            stop_recording();
            show_monitor_view();
            reviewWidget->stopPlayback();
            motionReviewWidget->stopPlayback();
            updateAuthUI();
        }
    } else {
        this->hide();
        WinManager::getInstance()->showLogin();
    }
}

void MainWidget::open_settings()
{
    if (!m_loggedIn) {
        return;
    }
    bool accepted = WinManager::getInstance()->showSettingsFromMain();

    if (accepted) {
        stop_cameras();
        for (int i = 0; i < 4; ++i) {
            btn_channel[i]->setToolTip(QString("通道%1").arg(i + 1));
        }
        m_currentChannel = 0;
        btn_channel[0]->setChecked(true);
        btn_single_view->setChecked(true);
        view_stack->setCurrentIndex(0);
        video_placeholder->setPixmap(QPixmap());
        video_placeholder->setText("视频监控画面区域\nVIDEO MONITOR AREA");
        for (int i = 0; i < 4; ++i) {
            quad_placeholders[i]->setPixmap(QPixmap());
            quad_placeholders[i]->setText(QString("通道%1\nCHANNEL %1").arg(i + 1));
        }
        start_cameras();
        if (m_loggedIn) {
            start_recording();
        }
        refresh_device_panel();
        insertOperationLog("修改设置", "管理员修改了系统设置信息");
    }
}

void MainWidget::switch_channel(int channel)
{
    if (channel >= 0 && channel < 4) {
        m_currentChannel = channel;
        if (view_stack->currentIndex() != 0) {
            view_stack->setCurrentIndex(0);
            btn_single_view->setChecked(true);
        }
        update();
    }
}

void MainWidget::show_single_view()
{
    view_stack->setCurrentIndex(0);
    update();
}

void MainWidget::show_quad_view()
{
    view_stack->setCurrentIndex(1);
    update();
}

void MainWidget::start_cameras()
{
    SettingInfo::loadFromFile();

    m_realCameraCount = 0;
    int virtualCount = 0;
    for (int i = 0; i < SettingInfo::cameras.size() && i < 4; ++i) {
        if (SettingInfo::cameras[i].getCameraName() != "virtual camera") {
            m_realCameraCount++;
        } else {
            virtualCount++;
        }
    }

    if (m_realCameraCount == 0 && virtualCount == 0) {
        lab_status->setText("未检测到可用摄像头");
        return;
    }

    QString statusText;
    if (m_realCameraCount > 0) {
        statusText = QString("已连接 %1 个摄像头").arg(m_realCameraCount);
    }
    if (virtualCount > 0) {
        if (!statusText.isEmpty()) statusText += "，";
        statusText += QString("%1 个视频通道").arg(virtualCount);
    }
    lab_status->setText(statusText);

    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString videoDir = appDir.absoluteFilePath("record_videos");

    QStringList videoFiles = {"jk1.mp4", "jk2.mp4", "jk3.mp4"};

    int camIdx = 0;
    int videoIdx = 0;
    for (int i = 0; i < 4 && i < SettingInfo::cameras.size(); ++i) {
        int configuredChannel = SettingInfo::cameras[i].getChannelId() - 1;
        if (configuredChannel < 0 || configuredChannel >= 4) {
            configuredChannel = i;
        }

        bool isRealCamera = (SettingInfo::cameras[i].getCameraName() != "virtual camera");
        if (isRealCamera) {
            mTasks[i] = new MonitorTask(nullptr, camIdx);
            camIdx++;
        } else {
            QString videoPath = videoDir + "/" + videoFiles[videoIdx % videoFiles.size()];
            mTasks[i] = new MonitorTask(videoPath, configuredChannel);
            videoIdx++;
        }
        mThreads[i] = new QThread;
        mTasks[i]->moveToThread(mThreads[i]);

        dTasks[i] = new DetectTask;
        dThreads[i] = new QThread;
        dTasks[i]->moveToThread(dThreads[i]);
        dThreads[i]->start();

        int channel = configuredChannel;
        connect(mTasks[i], &MonitorTask::sendFrame, this, [this, channel, i, isRealCamera](cv::Mat mat, int) {
            if (m_motionDetectEnabled && dTasks[i]) {
                bool isSingleView = (view_stack->currentIndex() == 0);
                bool shouldDetect = (isSingleView && channel == m_currentChannel)
                                    || (!isSingleView && isRealCamera);
                if (shouldDetect && m_detectSkipCount[channel]++ % 3 == 0) {
                    QMetaObject::invokeMethod(dTasks[i], "startDetect", Qt::QueuedConnection,
                                              Q_ARG(cv::Mat, mat.clone()), Q_ARG(int, channel));
                }
            } else {
                m_cachedRects[channel].clear();
                m_detectSkipCount[channel] = 0;
            }
            for (const QRect &r : m_cachedRects[channel]) {
                cv::rectangle(mat, cv::Rect(r.x(), r.y(), r.width(), r.height()),
                              cv::Scalar(0, 255, 0), 2);
            }
            m_frames[channel] = MatToImage(mat);
            update();
            if (m_isRecording && rTasks[i]) {
                QMetaObject::invokeMethod(rTasks[i], "saveVideo", Qt::QueuedConnection,
                                          Q_ARG(cv::Mat, mat), Q_ARG(int, channel));
            }
        });

        connect(dTasks[i], &DetectTask::sendDetectBoxes, this, [this, channel](int, QVector<QRect> rects) {
            m_cachedRects[channel] = rects;
        });

        connect(mThreads[i], &QThread::started, mTasks[i], &MonitorTask::startMonitor);

        mThreads[i]->start();

        btn_channel[channel]->setToolTip(SettingInfo::cameras[i].getChannelName());
    }
}

void MainWidget::stop_cameras()
{
    stop_recording();
    for (int i = 0; i < 4; ++i) {
        if (mTasks[i]) {
            mTasks[i]->stopMonitor();
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (mThreads[i]) {
            mThreads[i]->quit();
            mThreads[i]->wait(3000);
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (dThreads[i]) {
            dThreads[i]->quit();
            dThreads[i]->wait(3000);
        }
    }

    for (int i = 0; i < 4; ++i) {
        delete mTasks[i];
        mTasks[i] = nullptr;
        delete mThreads[i];
        mThreads[i] = nullptr;
        delete dTasks[i];
        dTasks[i] = nullptr;
        delete dThreads[i];
        dThreads[i] = nullptr;
        m_frames[i] = QImage();
        m_cachedRects[i].clear();
        m_detectSkipCount[i] = 0;
    }

    m_realCameraCount = 0;
    lab_status->setText("系统运行正常");

    video_placeholder->setText("视频监控画面区域\nVIDEO MONITOR AREA");
    for (int i = 0; i < 4; ++i) {
        quad_placeholders[i]->setText(QString("通道%1\nCHANNEL %1").arg(i + 1));
    }
}

void MainWidget::start_recording()
{
    if (m_isRecording) return;

    bool hasActiveChannel = false;
    for (int i = 0; i < 4; ++i) {
        if (mTasks[i]) {
            hasActiveChannel = true;
            break;
        }
    }
    if (!hasActiveChannel) return;

    for (int i = 0; i < 4 && i < SettingInfo::cameras.size(); ++i) {
        if (!mTasks[i]) continue;

        int interval = SettingInfo::timeInterval;
        if (interval <= 0) interval = 10;

        rTasks[i] = new RecordTask(interval, m_motionDetectEnabled ? 1 : 0);
        rThreads[i] = new QThread;
        rTasks[i]->moveToThread(rThreads[i]);

        rThreads[i]->start();
    }

    m_isRecording = true;
    int activeCount = 0;
    for (int i = 0; i < 4; ++i) {
        if (rTasks[i]) activeCount++;
    }
    lab_status->setText(QString("已连接 %1 个通道，正在录制...").arg(activeCount));
}

void MainWidget::stop_recording()
{
    if (!m_isRecording) return;

    for (int i = 0; i < 4; ++i) {
        if (rTasks[i]) {
            QMetaObject::invokeMethod(rTasks[i], "flushVideo", Qt::BlockingQueuedConnection);
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (rThreads[i]) {
            rThreads[i]->quit();
            rThreads[i]->wait(3000);
        }
    }

    for (int i = 0; i < 4; ++i) {
        delete rTasks[i];
        rTasks[i] = nullptr;
        delete rThreads[i];
        rThreads[i] = nullptr;
    }

    m_isRecording = false;
    lab_status->setText("系统运行正常");
}

QImage MainWidget::MatToImage(const cv::Mat &frame)
{
    if (frame.empty()) {
        return QImage();
    }
    cv::Mat temp;
    switch (frame.type()) {
    case CV_8UC1:
        return QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_Grayscale8).copy();
    case CV_8UC3:
        cv::cvtColor(frame, temp, cv::COLOR_BGR2RGB);
        return QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888).copy();
    default:
        return QImage();
    }
}

void MainWidget::capture_screenshot()
{
    if (m_currentChannel < 0 || m_currentChannel >= 4) return;
    if (m_frames[m_currentChannel].isNull()) return;

    QDateTime now = QDateTime::currentDateTime();
    QString filename = QString("monitor_screenshot_%1.png")
                           .arg(now.toString("yyyyMMdd_HHmmss"));
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString dirPath = appDir.absoluteFilePath("screenshots");
    QDir dir;
    if (!dir.exists(dirPath)) {
        dir.mkpath(dirPath);
    }

    QString fullPath = dirPath + "/" + filename;
    QPixmap pixmap = QPixmap::fromImage(m_frames[m_currentChannel]);
    if (pixmap.save(fullPath, "PNG")) {
        FeatureImageModel fim;
        int channelId = m_currentChannel + 1;
        fim.insertImage(filename, fullPath, channelId,
                        now.toString("yyyy-MM-dd hh:mm:ss"),
                        m_motionDetectEnabled ? 1 : 0);
        QMessageBox::information(this, "截图成功", "截图已保存到：" + fullPath);
    } else {
        QMessageBox::warning(this, "截图失败", "截图保存失败");
    }
}

void MainWidget::on_motion_detect_clicked()
{
    m_motionDetectEnabled = btn_motion_detect->isChecked();
    if (m_motionDetectEnabled) {
        btn_motion_detect->setText("关闭移动侦测");
        for (int i = 0; i < 4; ++i) {
            m_detectSkipCount[i] = 0;
        }
        lab_status->setText("移动侦测已开启");
         insertOperationLog("开启移动侦测", "管理员开启了移动侦测功能");
    } else {
        btn_motion_detect->setText("开启移动侦测");
        lab_status->setText("移动侦测已关闭");
        insertOperationLog("关闭移动侦测", "管理员关闭了移动侦测功能");
    }

    if (m_isRecording) {
        stop_recording();
        start_recording();
    }
}

void MainWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (view_stack->currentIndex() == 0) {
        if (!m_frames[m_currentChannel].isNull()) {
            video_placeholder->setPixmap(QPixmap::fromImage(m_frames[m_currentChannel]));
        } else {
            video_placeholder->setPixmap(QPixmap());
            video_placeholder->setText(QString("通道%1\nCHANNEL %1").arg(m_currentChannel + 1));
        }
    } else {
        for (int i = 0; i < 4 && i < SettingInfo::cameras.size(); ++i) {
            int channelIdx = SettingInfo::cameras[i].getChannelId() - 1;
            if (channelIdx >= 0 && channelIdx < 4 && !m_frames[channelIdx].isNull()) {
                quad_placeholders[channelIdx]->setPixmap(QPixmap::fromImage(m_frames[channelIdx]));
            }
        }
    }
}

void MainWidget::insertOperationLog(const QString &func, const QString &desc)
{
    if (!m_loggedIn) return;
    OperationLogModel olm;
    olm.insertOperationLog(m_userId, func, desc);
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    stop_recording();
    stop_cameras();
    reviewWidget->stopPlayback();
    motionReviewWidget->stopPlayback();
    event->accept();
}