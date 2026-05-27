#include "motionreviewwidget.h"
#include <QFileInfo>

MotionReviewWidget::MotionReviewWidget(QWidget *parent)
    : QWidget{parent}
{
    current_page = 1;
    total_page = 1;
    current_image_page = 1;
    total_image_page = 1;
    m_isPlaying = false;
    m_isPaused = false;
    m_playbackSpeed = 1.0;
    m_totalFrames = 0;
    m_currentFrame = 0;
    m_currentVideoChannelId = 0;

    m_playbackTimer = new QTimer(this);

    create_widget();
    updateVideoList();
}

void MotionReviewWidget::create_widget()
{
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(1);
    splitter->setObjectName("main_splitter");

    QWidget *left_panel = new QWidget;
    left_panel->setObjectName("left_panel");
    left_panel->setFixedWidth(300);

    QWidget *right_panel = new QWidget;
    right_panel->setObjectName("right_panel");

    setup_left_panel(left_panel);
    setup_right_panel(right_panel);

    splitter->addWidget(left_panel);
    splitter->addWidget(right_panel);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    QVBoxLayout *outer_layout = new QVBoxLayout(this);
    outer_layout->setContentsMargins(0, 0, 0, 0);
    outer_layout->setSpacing(0);
    outer_layout->addWidget(splitter);
    setLayout(outer_layout);

    connect(m_playbackTimer, &QTimer::timeout, this, &MotionReviewWidget::onPlaybackTick);
    connect(dateEdit, &QDateEdit::dateChanged, this, &MotionReviewWidget::onFilterChanged);
    connect(channel_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MotionReviewWidget::onFilterChanged);
    connect(btn_prev, &QPushButton::clicked, this, &MotionReviewWidget::prevPage);
    connect(btn_next, &QPushButton::clicked, this, &MotionReviewWidget::nextPage);
    connect(videos_list, &QListWidget::itemDoubleClicked,
            this, &MotionReviewWidget::onVideoItemDoubleClicked);
    connect(videos_list, &QListWidget::itemClicked,
            this, &MotionReviewWidget::onImageItemClicked);
    connect(btn_pause, &QPushButton::clicked, this, &MotionReviewWidget::togglePause);
    connect(speed_combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MotionReviewWidget::onSpeedChanged);
    connect(btn_screenshot, &QPushButton::clicked, this, &MotionReviewWidget::captureScreenshot);
    connect(progress_slider, &QSlider::sliderPressed, this, &MotionReviewWidget::onSliderPressed);
    connect(progress_slider, &QSlider::sliderReleased, this, &MotionReviewWidget::onSliderReleased);
    connect(progress_slider, &QSlider::sliderMoved, this, &MotionReviewWidget::onSliderMoved);
    connect(btn_image_query, &QPushButton::clicked, this, &MotionReviewWidget::toggleImageQuery);
    connect(btn_image_prev, &QPushButton::clicked, this, &MotionReviewWidget::imagePrevPage);
    connect(btn_image_next, &QPushButton::clicked, this, &MotionReviewWidget::imageNextPage);
}

void MotionReviewWidget::setup_left_panel(QWidget *panel)
{
    QVBoxLayout *left_layout = new QVBoxLayout;
    left_layout->setContentsMargins(12, 12, 12, 12);
    left_layout->setSpacing(8);
    panel->setLayout(left_layout);

    QLabel *lab_title = new QLabel("移动侦测查询");
    lab_title->setObjectName("panel_title");
    left_layout->addWidget(lab_title);

    QFrame *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setObjectName("separator");
    left_layout->addWidget(separator);

    QHBoxLayout *selector_layout = new QHBoxLayout;
    selector_layout->setSpacing(8);

    dateEdit = new QDateEdit;
    dateEdit->setDisplayFormat("yyyy年MM月dd日");
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumDate(QDate(1900,1,1));
    dateEdit->setMaximumDate(QDate(2100,12,31));

    dateEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    QLocale locale(QLocale::Chinese);
    dateEdit->setLocale(locale);
    QCalendarWidget *cal = dateEdit->calendarWidget();
    cal->setGridVisible(true);
    cal->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    cal->setNavigationBarVisible(true);
    selector_layout->addWidget(dateEdit);

    channel_box = new QComboBox;
    channel_box->addItem("所有通道",0);
    channel_box->addItem("通道1",1);
    channel_box->addItem("通道2",2);
    channel_box->addItem("通道3",3);
    channel_box->addItem("通道4",4);
    channel_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    selector_layout->addWidget(channel_box);

    left_layout->addLayout(selector_layout);
    videos_list = new QListWidget;
    videos_list->setObjectName("device_list");
    videos_list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    left_layout->addWidget(videos_list, 1);

    QWidget *page_widget = new QWidget;
    QHBoxLayout *page_layout = new QHBoxLayout;
    page_layout->setContentsMargins(0, 0, 0, 0);
    page_layout->setSpacing(8);
    page_widget->setLayout(page_layout);
    btn_prev = new QPushButton("上一页");
    btn_prev->setObjectName("ctrl_btn");
    btn_prev->setCursor(Qt::PointingHandCursor);

    lab_page = new QLabel(QString("%1/%2").arg(current_page).arg(total_page));
    lab_page->setObjectName("channel_label");
    lab_page->setAlignment(Qt::AlignCenter);

    btn_next = new QPushButton("下一页");
    btn_next->setObjectName("ctrl_btn");
    btn_next->setCursor(Qt::PointingHandCursor);

    page_layout->addWidget(btn_prev);
    page_layout->addWidget(lab_page, 1);
    page_layout->addWidget(btn_next);
    left_layout->addWidget(page_widget);

}

void MotionReviewWidget::setup_right_panel(QWidget *panel)
{
    QVBoxLayout *right_layout = new QVBoxLayout;
    right_layout->setContentsMargins(12, 12, 12, 12);
    panel->setLayout(right_layout);

    QWidget *right_header = new QWidget;
    QHBoxLayout *header_layout = new QHBoxLayout;
    header_layout->setContentsMargins(0, 0, 0, 0);
    right_header->setLayout(header_layout);

    QLabel *lab_title = new QLabel("视频播放");
    lab_title->setObjectName("panel_title");
    header_layout->addWidget(lab_title);

    header_layout->addStretch();

    btn_image_query = new QPushButton("特征图片查询");
    btn_image_query->setObjectName("ctrl_btn");
    btn_image_query->setCursor(Qt::PointingHandCursor);
    btn_image_query->setCheckable(true);
    btn_image_query->setFixedWidth(120);
    header_layout->addWidget(btn_image_query);

    right_layout->addWidget(right_header);

    QFrame *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setObjectName("separator");
    right_layout->addWidget(separator);

    right_stack = new QStackedWidget;

    video_page = new QWidget;
    QVBoxLayout *video_layout = new QVBoxLayout;
    video_layout->setContentsMargins(0, 0, 0, 0);
    video_page->setLayout(video_layout);

    video_display = new QLabel;
    video_display->setObjectName("video_placeholder");
    video_display->setAlignment(Qt::AlignCenter);
    video_display->setScaledContents(true);
    video_display->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    video_display->setText("移动侦测回放区域\nMOTION REVIEW AREA");
    video_layout->addWidget(video_display, 1);

    progress_slider = new QSlider(Qt::Horizontal);
    progress_slider->setObjectName("progress_slider");
    progress_slider->setEnabled(false);
    video_layout->addWidget(progress_slider);

    QWidget *control_bar = new QWidget;
    control_bar->setObjectName("control_bar");
    QHBoxLayout *control_layout = new QHBoxLayout;
    control_layout->setContentsMargins(8, 6, 8, 6);
    control_layout->setSpacing(12);
    control_bar->setLayout(control_layout);

    btn_pause = new QPushButton("暂停");
    btn_pause->setObjectName("ctrl_btn");
    btn_pause->setCursor(Qt::PointingHandCursor);
    btn_pause->setFixedWidth(80);

    speed_combo = new QComboBox;
    speed_combo->setObjectName("settings_combo");
    speed_combo->addItem("0.5x", 50);
    speed_combo->addItem("0.75x", 75);
    speed_combo->addItem("1x", 100);
    speed_combo->addItem("1.5x", 150);
    speed_combo->addItem("2x", 200);
    speed_combo->addItem("3x", 300);
    speed_combo->setCurrentIndex(2);
    speed_combo->setFixedWidth(90);
    speed_combo->setCursor(Qt::PointingHandCursor);

    btn_screenshot = new QPushButton("截图");
    btn_screenshot->setObjectName("ctrl_btn");
    btn_screenshot->setCursor(Qt::PointingHandCursor);
    btn_screenshot->setFixedWidth(80);

    control_layout->addStretch();
    control_layout->addWidget(btn_pause);
    control_layout->addWidget(speed_combo);
    control_layout->addWidget(btn_screenshot);
    control_layout->addStretch();

    video_layout->addWidget(control_bar);

    right_stack->addWidget(video_page);
    right_stack->addWidget(create_image_page());
    right_stack->setCurrentIndex(0);

    right_layout->addWidget(right_stack, 1);
}

QWidget* MotionReviewWidget::create_image_page()
{
    QWidget *img_page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);
    img_page->setLayout(layout);

    image_stack = new QStackedWidget;

    grid_page = new QWidget;
    QVBoxLayout *grid_layout = new QVBoxLayout;
    grid_layout->setContentsMargins(0, 0, 0, 0);
    grid_layout->setSpacing(4);
    grid_page->setLayout(grid_layout);

    QGridLayout *grid = new QGridLayout;
    grid->setSpacing(4);
    for (int row = 0; row < 3; ++row) {
        grid->setRowStretch(row, 1);
        for (int col = 0; col < 3; ++col) {
            if (row == 0) grid->setColumnStretch(col, 1);
            image_grid[row][col] = new QLabel;
            image_grid[row][col]->setAlignment(Qt::AlignCenter);
            image_grid[row][col]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            image_grid[row][col]->setMinimumSize(80, 80);
            grid->addWidget(image_grid[row][col], row, col);
        }
    }
    grid_layout->addLayout(grid, 1);

    image_single_display = new QLabel;
    image_single_display->setObjectName("video_placeholder");
    image_single_display->setAlignment(Qt::AlignCenter);
    image_single_display->setScaledContents(true);
    image_single_display->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    image_stack->addWidget(grid_page);
    image_stack->addWidget(image_single_display);
    image_stack->setCurrentIndex(0);

    layout->addWidget(image_stack, 1);

    QWidget *image_page_bar = new QWidget;
    QHBoxLayout *image_page_layout = new QHBoxLayout;
    image_page_layout->setContentsMargins(0, 0, 0, 0);
    image_page_layout->setSpacing(8);
    image_page_bar->setLayout(image_page_layout);

    btn_image_prev = new QPushButton("上一页");
    btn_image_prev->setObjectName("ctrl_btn");
    btn_image_prev->setCursor(Qt::PointingHandCursor);

    lab_image_page = new QLabel(QString("1/1"));
    lab_image_page->setObjectName("channel_label");
    lab_image_page->setAlignment(Qt::AlignCenter);

    btn_image_next = new QPushButton("下一页");
    btn_image_next->setObjectName("ctrl_btn");
    btn_image_next->setCursor(Qt::PointingHandCursor);

    image_page_layout->addStretch();
    image_page_layout->addWidget(btn_image_prev);
    image_page_layout->addWidget(lab_image_page);
    image_page_layout->addWidget(btn_image_next);
    image_page_layout->addStretch();

    layout->addWidget(image_page_bar);

    return img_page;
}

void MotionReviewWidget::toggleImageQuery()
{
    if (btn_image_query->isChecked()) {
        stopPlayback();
        btn_image_query->setText("返回录像");
        right_stack->setCurrentIndex(1);
        current_image_page = 1;
        image_stack->setCurrentIndex(0);
        m_selectedImagePath.clear();
        videos_list->clearSelection();
        updateImageList();
    } else {
        btn_image_query->setText("特征图片查询");
        right_stack->setCurrentIndex(0);
        updateVideoList();
    }
}

void MotionReviewWidget::onFilterChanged()
{
    if (right_stack->currentIndex() == 1) {
        current_image_page = 1;
        updateImageList();
    } else {
        updateVideoList();
    }
}

void MotionReviewWidget::updateImageList()
{
    current_image_page = 1;
    image_stack->setCurrentIndex(0);
    m_selectedImagePath.clear();
    videos_list->clearSelection();
    videos_list->clear();

    FeatureImageModel fim;
    QDate queryDate = dateEdit->date();
    int channelId = channel_box->currentData().toInt();
    QList<ImageInfo> imageList;
    int total = 0;
    fim.queryImages(channelId, queryDate, 1, image_page_size, 0, imageList, total);

    for (auto &img : imageList) {
        QString displayText = QString("[通道%1] %2")
                                  .arg(img.channelId)
                                  .arg(img.imageName);
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, img.imagePath);
        item->setToolTip(img.imagePath);
        videos_list->addItem(item);
    }

    total_image_page = total / image_page_size;
    if (total % image_page_size != 0) total_image_page++;
    if (total_image_page == 0) total_image_page = 1;
    lab_image_page->setText(QString("%1/%2").arg(current_image_page).arg(total_image_page));
    btn_image_prev->setEnabled(current_image_page > 1);
    btn_image_next->setEnabled(current_image_page < total_image_page);

    showImagesOnGrid(imageList);
}

void MotionReviewWidget::imagePrevPage()
{
    if (current_image_page <= 1) return;
    current_image_page--;
    videos_list->clear();

    FeatureImageModel fim;
    QDate queryDate = dateEdit->date();
    int channelId = channel_box->currentData().toInt();
    QList<ImageInfo> imageList;
    int total = 0;
    int offset = (current_image_page - 1) * image_page_size;
    fim.queryImages(channelId, queryDate, 1, image_page_size, offset, imageList, total);

    for (auto &img : imageList) {
        QString displayText = QString("[通道%1] %2")
                                  .arg(img.channelId)
                                  .arg(img.imageName);
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, img.imagePath);
        item->setToolTip(img.imagePath);
        videos_list->addItem(item);
    }

    total_image_page = total / image_page_size;
    if (total % image_page_size != 0) total_image_page++;
    if (total_image_page == 0) total_image_page = 1;
    lab_image_page->setText(QString("%1/%2").arg(current_image_page).arg(total_image_page));
    btn_image_prev->setEnabled(current_image_page > 1);
    btn_image_next->setEnabled(current_image_page < total_image_page);

    showImagesOnGrid(imageList);
}

void MotionReviewWidget::imageNextPage()
{
    if (current_image_page >= total_image_page) return;
    current_image_page++;
    videos_list->clear();

    FeatureImageModel fim;
    QDate queryDate = dateEdit->date();
    int channelId = channel_box->currentData().toInt();
    QList<ImageInfo> imageList;
    int total = 0;
    int offset = (current_image_page - 1) * image_page_size;
    fim.queryImages(channelId, queryDate, 1, image_page_size, offset, imageList, total);

    for (auto &img : imageList) {
        QString displayText = QString("[通道%1] %2")
                                  .arg(img.channelId)
                                  .arg(img.imageName);
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, img.imagePath);
        item->setToolTip(img.imagePath);
        videos_list->addItem(item);
    }

    total_image_page = total / image_page_size;
    if (total % image_page_size != 0) total_image_page++;
    if (total_image_page == 0) total_image_page = 1;
    lab_image_page->setText(QString("%1/%2").arg(current_image_page).arg(total_image_page));
    btn_image_prev->setEnabled(current_image_page > 1);
    btn_image_next->setEnabled(current_image_page < total_image_page);

    showImagesOnGrid(imageList);
}

void MotionReviewWidget::showImagesOnGrid(const QList<ImageInfo> &imageList)
{
    int cellW = image_stack->width() / 3 - 4;
    int cellH = (image_stack->height() - 50) / 3 - 4;
    if (cellW < 80) cellW = 200;
    if (cellH < 80) cellH = 150;
    for (int i = 0; i < 9; ++i) {
        int row = i / 3;
        int col = i % 3;
        if (i < imageList.size()) {
            QPixmap pixmap(imageList[i].imagePath);
            if (!pixmap.isNull()) {
                image_grid[row][col]->setPixmap(pixmap.scaled(cellW, cellH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            } else {
                image_grid[row][col]->clear();
                image_grid[row][col]->setText("加载失败");
            }
        } else {
            image_grid[row][col]->clear();
        }
    }
}

void MotionReviewWidget::onImageItemClicked(QListWidgetItem *item)
{
    if (right_stack->currentIndex() != 1) return;
    QString imagePath = item->data(Qt::UserRole).toString();
    if (imagePath.isEmpty()) return;
    if (image_stack->currentIndex() == 1) {
        if (imagePath == m_selectedImagePath) {
            videos_list->clearSelection();
            image_stack->setCurrentIndex(0);
            m_selectedImagePath.clear();
            return;
        }
        m_selectedImagePath = imagePath;
        image_single_display->clear();
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            image_single_display->setPixmap(pixmap);
        } else {
            image_single_display->setText("图片文件不存在");
        }
        return;
    }

    m_selectedImagePath = imagePath;
    image_stack->setCurrentIndex(1);
    image_single_display->clear();
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        image_single_display->setPixmap(pixmap);
    } else {
        image_single_display->setText("图片文件不存在");
    }
}

void MotionReviewWidget::updateVideoList()
{
    current_page = 1;
    videos_list->clear();

    VideoService videoService;
    QDate queryDate = dateEdit->date();
    int channelId = channel_box->currentData().toInt();
    QList<Video> videoList;
    int total = 0;
    videoService.queryVideosByException(1, channelId, queryDate, page_size, 0, videoList, total);

    for (auto &video : videoList) {
        QString displayText = QString("[通道%1] %2")
                                  .arg(video.getChannel_id())
                                  .arg(video.getVideoName());
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, video.getVideoPath());
        item->setData(Qt::UserRole + 1, video.getChannel_id());
        item->setToolTip(video.getVideoPath());
        videos_list->addItem(item);
    }

    total_page = total / page_size;
    if (total % page_size != 0) total_page++;
    if (total_page == 0) total_page = 1;
    lab_page->setText(QString("%1/%2").arg(current_page).arg(total_page));
    btn_prev->setEnabled(current_page > 1);
    btn_next->setEnabled(current_page < total_page);
}

void MotionReviewWidget::prevPage()
{
    if (right_stack->currentIndex() == 1) {
        imagePrevPage();
        return;
    }
    if (current_page <= 1) return;
    current_page--;
    videos_list->clear();

    VideoService videoService;
    QDate queryDate = dateEdit->date();
    int channelId = channel_box->currentData().toInt();
    QList<Video> videoList;
    int total = 0;
    int offset = (current_page - 1) * page_size;
    videoService.queryVideosByException(1, channelId, queryDate, page_size, offset, videoList, total);

    for (auto &video : videoList) {
        QString displayText = QString("[通道%1] %2")
                                  .arg(video.getChannel_id())
                                  .arg(video.getVideoName());
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, video.getVideoPath());
        item->setData(Qt::UserRole + 1, video.getChannel_id());
        item->setToolTip(video.getVideoPath());
        videos_list->addItem(item);
    }

    total_page = total / page_size;
    if (total % page_size != 0) total_page++;
    if (total_page == 0) total_page = 1;
    lab_page->setText(QString("%1/%2").arg(current_page).arg(total_page));
    btn_prev->setEnabled(current_page > 1);
    btn_next->setEnabled(current_page < total_page);
}

void MotionReviewWidget::nextPage()
{
    if (right_stack->currentIndex() == 1) {
        imageNextPage();
        return;
    }
    if (current_page >= total_page) return;
    current_page++;
    videos_list->clear();

    VideoService videoService;
    QDate queryDate = dateEdit->date();
    int channelId = channel_box->currentData().toInt();
    QList<Video> videoList;
    int total = 0;
    int offset = (current_page - 1) * page_size;
    videoService.queryVideosByException(1, channelId, queryDate, page_size, offset, videoList, total);

    for (auto &video : videoList) {
        QString displayText = QString("[通道%1] %2")
                                  .arg(video.getChannel_id())
                                  .arg(video.getVideoName());
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, video.getVideoPath());
        item->setData(Qt::UserRole + 1, video.getChannel_id());
        item->setToolTip(video.getVideoPath());
        videos_list->addItem(item);
    }

    total_page = total / page_size;
    if (total % page_size != 0) total_page++;
    if (total_page == 0) total_page = 1;
    lab_page->setText(QString("%1/%2").arg(current_page).arg(total_page));
    btn_prev->setEnabled(current_page > 1);
    btn_next->setEnabled(current_page < total_page);
}

void MotionReviewWidget::onVideoItemDoubleClicked(QListWidgetItem *item)
{
     if (right_stack->currentIndex() == 1) return;
    stopPlayback();

    QString videoPath = item->data(Qt::UserRole).toString();
    if (videoPath.isEmpty()) return;

    QFileInfo fileInfo(videoPath);
    QString baseName = fileInfo.completeBaseName();
    QStringList parts = baseName.split('_');
    m_currentVideoChannelId = parts.isEmpty() ? 0 : parts.last().toInt();

    if (!m_videoCapture.open(videoPath.toStdString())) {
        QMessageBox::warning(this, "播放失败", "无法打开视频文件：" + videoPath);
        return;
    }

    m_totalFrames = static_cast<int>(m_videoCapture.get(cv::CAP_PROP_FRAME_COUNT));
    m_currentFrame = 0;
    m_isPlaying = true;
    m_isPaused = false;
    m_playbackSpeed = 1.0;

    progress_slider->setEnabled(true);
    progress_slider->setRange(0, m_totalFrames > 0 ? m_totalFrames : 1000);
    progress_slider->setValue(0);

    btn_pause->setText("暂停");
    speed_combo->setCurrentIndex(2);

    int interval = static_cast<int>(33.33 / m_playbackSpeed);
    m_playbackTimer->start(interval);
}

void MotionReviewWidget::stopPlayback()
{
    m_playbackTimer->stop();
    m_isPlaying = false;
    m_isPaused = false;
    if (m_videoCapture.isOpened()) {
        m_videoCapture.release();
    }
    video_display->setText("移动侦测回放区域\nMOTION REVIEW AREA");
    progress_slider->setEnabled(false);
    progress_slider->setValue(0);
    btn_pause->setText("暂停");
}

void MotionReviewWidget::onPlaybackTick()
{
    if (!m_isPlaying || m_isPaused) return;
    if (!m_videoCapture.isOpened()) return;

    cv::Mat frame;
    bool readOk = m_videoCapture.read(frame);
    if (!readOk || frame.empty()) {
        stopPlayback();
        return;
    }

    m_currentFrame = static_cast<int>(m_videoCapture.get(cv::CAP_PROP_POS_FRAMES));
    if (!progress_slider->isSliderDown()) {
        progress_slider->setValue(m_currentFrame);
    }

    QImage image = MatToImage(frame);
    if (!image.isNull()) {
        video_display->setPixmap(QPixmap::fromImage(image));
    }
}

void MotionReviewWidget::togglePause()
{
    if (!m_isPlaying) return;
    m_isPaused = !m_isPaused;
    btn_pause->setText(m_isPaused ? "播放" : "暂停");
    if (!m_isPaused) {
        int interval = static_cast<int>(33.33 / m_playbackSpeed);
        if (interval < 1) interval = 1;
        m_playbackTimer->start(interval);
    }
}

void MotionReviewWidget::onSpeedChanged(int index)
{
    if (index < 0) return;
    int speedPercent = speed_combo->itemData(index).toInt();
    m_playbackSpeed = speedPercent / 100.0;

    if (m_isPlaying && !m_isPaused) {
        int interval = static_cast<int>(33.33 / m_playbackSpeed);
        if (interval < 1) interval = 1;
        m_playbackTimer->start(interval);
    }
}

void MotionReviewWidget::captureScreenshot()
{
    if (!m_isPlaying) return;
    if (!m_videoCapture.isOpened()) return;

    QPixmap screenshot = video_display->pixmap();
    if (screenshot.isNull()) return;

    QDateTime now = QDateTime::currentDateTime();
    QString filename = QString("motion_screenshot_%1.png")
                           .arg(now.toString("yyyyMMdd_HHmmss"));
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString dirPath = appDir.absoluteFilePath("screenshots");
    QDir dir;
    if (!dir.exists(dirPath)) {
        dir.mkpath(dirPath);
    }

    QString fullPath = dirPath + "/" + filename;
    if (screenshot.save(fullPath, "PNG")) {
        FeatureImageModel fim;
        fim.insertImage(filename, fullPath, m_currentVideoChannelId,
                        now.toString("yyyy-MM-dd hh:mm:ss"), 1);
        QMessageBox::information(this, "截图成功", "截图已保存到：" + fullPath);
    } else {
        QMessageBox::warning(this, "截图失败", "截图保存失败");
    }
}

void MotionReviewWidget::onSliderPressed()
{
    if (!m_isPlaying) return;
    m_playbackTimer->stop();
}

void MotionReviewWidget::onSliderReleased()
{
    if (!m_isPlaying) return;
    int pos = progress_slider->value();
    m_videoCapture.set(cv::CAP_PROP_POS_FRAMES, pos);
    m_currentFrame = pos;
    if (!m_isPaused) {
        int interval = static_cast<int>(33.33 / m_playbackSpeed);
        if (interval < 1) interval = 1;
        m_playbackTimer->start(interval);
    }
}

void MotionReviewWidget::onSliderMoved(int position)
{
    if (!m_isPlaying) return;
    m_currentFrame = position;
    if (!m_isPaused) {
        m_videoCapture.set(cv::CAP_PROP_POS_FRAMES, position);
        cv::Mat frame;
        if (m_videoCapture.read(frame) && !frame.empty()) {
            QImage image = MatToImage(frame);
            if (!image.isNull()) {
                video_display->setPixmap(QPixmap::fromImage(image));
            }
        }
    }
}

QImage MotionReviewWidget::MatToImage(const cv::Mat &frame)
{
    if (frame.empty()) {
        return QImage();
    }
    cv::Mat temp;
    switch (frame.type()) {
    case CV_8UC1:
        return QImage(frame.data, frame.cols, frame.rows, frame.step,
                      QImage::Format_Grayscale8).copy();
    case CV_8UC3:
        cv::cvtColor(frame, temp, cv::COLOR_BGR2RGB);
        return QImage(temp.data, temp.cols, temp.rows, temp.step,
                      QImage::Format_RGB888).copy();
    default:
        return QImage();
    }
}