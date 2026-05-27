#include "settingswidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>


SettingsWidget::SettingsWidget(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowTitle("系统设置");
    setMinimumSize(580, 480);
    setFixedSize(620, 520);

    setupUi();
    applyStyles();
    loadSettings();
}

QWidget* SettingsWidget::createTitleBar()
{
    title_bar = new QWidget;
    title_bar->setObjectName("settings_title_bar");
    title_bar->setFixedHeight(40);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(16, 0, 8, 0);
    layout->setSpacing(0);
    title_bar->setLayout(layout);

    QLabel *lab_title = new QLabel("系统设置");
    lab_title->setObjectName("settings_title");
    lab_title->setAttribute(Qt::WA_TransparentForMouseEvents);
    layout->addWidget(lab_title);

    layout->addStretch();

    QPushButton *btn_close = new QPushButton(QString::fromUtf8("\xC3\x97"));
    btn_close->setObjectName("settings_close_btn");
    btn_close->setFixedSize(32, 32);
    btn_close->setCursor(Qt::PointingHandCursor);

    layout->addWidget(btn_close);

    connect(btn_close, &QPushButton::clicked, this, [this]() {
        reject();
    });

    return title_bar;
}

void SettingsWidget::setupUi()
{
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    QWidget *title_bar = createTitleBar();
    main_layout->addWidget(title_bar);

    QWidget *content = new QWidget;
    content->setObjectName("settings_content");
    QVBoxLayout *content_layout = new QVBoxLayout;
    content_layout->setContentsMargins(30, 20, 30, 25);
    content_layout->setSpacing(16);
    content->setLayout(content_layout);

    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setObjectName("settings_separator");
    content_layout->addWidget(line);

    QHBoxLayout *path_layout = new QHBoxLayout;
    path_layout->setSpacing(10);

    QLabel *lab_path = new QLabel("保存路径:");
    lab_path->setObjectName("settings_label");
    lab_path->setFixedWidth(70);

    edit_save_path = new QLineEdit;
    edit_save_path->setObjectName("settings_edit");
    edit_save_path->setPlaceholderText("请选择保存路径...");

    btn_browse = new QPushButton("浏览...");
    btn_browse->setObjectName("settings_btn");
    btn_browse->setCursor(Qt::PointingHandCursor);
    btn_browse->setFixedWidth(80);

    path_layout->addWidget(lab_path);
    path_layout->addWidget(edit_save_path, 1);
    path_layout->addWidget(btn_browse);

    content_layout->addLayout(path_layout);

    QHBoxLayout *interval_layout = new QHBoxLayout;
    interval_layout->setSpacing(10);

    QLabel *lab_interval = new QLabel("间隔时间:");
    lab_interval->setObjectName("settings_label");
    lab_interval->setFixedWidth(70);

    combo_interval = new QComboBox;
    combo_interval->setObjectName("settings_combo");
    combo_interval->addItem("10秒", 10);
    combo_interval->addItem("20秒", 20);
    combo_interval->addItem("30秒", 30);
    combo_interval->addItem("40秒", 40);
    combo_interval->addItem("50秒", 50);
    combo_interval->addItem("60秒", 60);
    combo_interval->addItem("120秒", 120);

    interval_layout->addWidget(lab_interval);
    interval_layout->addWidget(combo_interval, 1);
    interval_layout->addStretch();

    content_layout->addLayout(interval_layout);

    QFrame *line2 = new QFrame;
    line2->setFrameShape(QFrame::HLine);
    line2->setObjectName("settings_separator");
    content_layout->addWidget(line2);

    for (int i = 0; i < 4; ++i) {
        QHBoxLayout *channel_layout = new QHBoxLayout;
        channel_layout->setSpacing(10);

        lab_camera_names[i] = new QLabel("virtual camera");
        lab_camera_names[i]->setObjectName("settings_label");
        lab_camera_names[i]->setFixedWidth(130);

        edit_channel_name[i] = new QLineEdit;
        edit_channel_name[i]->setObjectName("settings_edit");

        combo_channel[i] = new QComboBox;
        combo_channel[i]->setObjectName("settings_combo");
        combo_channel[i]->addItem("通道1", 1);
        combo_channel[i]->addItem("通道2", 2);
        combo_channel[i]->addItem("通道3", 3);
        combo_channel[i]->addItem("通道4", 4);

        channel_layout->addWidget(lab_camera_names[i]);
        channel_layout->addWidget(edit_channel_name[i], 1);
        channel_layout->addWidget(combo_channel[i]);

        content_layout->addLayout(channel_layout);
    }

    content_layout->addStretch();

    QHBoxLayout *btn_layout = new QHBoxLayout;
    btn_layout->addStretch();

    QPushButton *btn_confirm = new QPushButton("确认");
    btn_confirm->setObjectName("settings_confirm_btn");
    btn_confirm->setCursor(Qt::PointingHandCursor);
    btn_confirm->setFixedSize(100, 36);

    QPushButton *btn_cancel = new QPushButton("取消");
    btn_cancel->setObjectName("settings_cancel_btn");
    btn_cancel->setCursor(Qt::PointingHandCursor);
    btn_cancel->setFixedSize(100, 36);

    btn_layout->addWidget(btn_confirm);
    btn_layout->addWidget(btn_cancel);

    content_layout->addLayout(btn_layout);

    main_layout->addWidget(content);

    setLayout(main_layout);

    connect(btn_browse, &QPushButton::clicked, this, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this, "选择保存路径", edit_save_path->text());
        if (!dir.isEmpty()) {
            edit_save_path->setText(dir);
        }
    });

    connect(btn_confirm, &QPushButton::clicked, this, [this]() {
        if (!validate()) {
            return;
        }
        saveSettings();
        accept();
    });

    connect(btn_cancel, &QPushButton::clicked, this, [this]() {
        reject();
    });
}

void SettingsWidget::applyStyles()
{
    setObjectName("settings_dialog");
    QFile file(":/qss/settings.qss");
    if (file.open(QFile::ReadOnly)) {
        setStyleSheet(file.readAll());
        file.close();
    }
}

void SettingsWidget::loadSettings()
{
    edit_save_path->setText(SettingInfo::file_path);

    int idx = combo_interval->findData(SettingInfo::timeInterval);
    if (idx >= 0) {
        combo_interval->setCurrentIndex(idx);
    }

    for (int i = 0; i < 4 && i < SettingInfo::cameras.size(); ++i) {
        CameraInfo &cam = SettingInfo::cameras[i];
        lab_camera_names[i]->setText(cam.getCameraName());
        edit_channel_name[i]->setText(cam.getChannelName());

        int chIdx = combo_channel[i]->findData(cam.getChannelId());
        if (chIdx >= 0) {
            combo_channel[i]->setCurrentIndex(chIdx);
        }
    }
}

void SettingsWidget::saveSettings()
{
    QMessageBox::StandardButton confirm = QMessageBox::question(this,"配置确认","是否确认保存");
    if(confirm == QMessageBox::Yes){
        SettingInfo::file_path = edit_save_path->text();
        SettingInfo::timeInterval = combo_interval->currentData().toInt();

        for (int i = 0; i < 4 && i < SettingInfo::cameras.size(); ++i) {
            CameraInfo &cam = SettingInfo::cameras[i];
            cam.setChannelName(edit_channel_name[i]->text());
            cam.setChannelId(combo_channel[i]->currentData().toInt());
        }

        SettingInfo::saveToFile();
    }
}

bool SettingsWidget::validate()
{
    if (edit_save_path->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "错误提示", "保存路径不能为空！");
        edit_save_path->setFocus();
        return false;
    }

    QDir dir(edit_save_path->text().trimmed());
    if (!dir.exists()) {
        QMessageBox::warning(this, "错误提示", "保存路径不存在，请重新选择！");
        edit_save_path->setFocus();
        return false;
    }

    QSet<int> channelIds;
    for (int i = 0; i < 4; ++i) {
        int chId = combo_channel[i]->currentData().toInt();
        if (channelIds.contains(chId)) {
            QMessageBox::warning(this, "错误提示",
                                 QString("通道%1 的通道号(%2)与前面的通道重复！").arg(i + 1).arg(chId));
            combo_channel[i]->setFocus();
            return false;
        }
        channelIds.insert(chId);
    }

    QSet<QString> channelNames;
    for (int i = 0; i < 4; ++i) {
        QString name = edit_channel_name[i]->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "错误提示",
                                 QString("第%1行通道名称不能为空！").arg(i + 1));
            edit_channel_name[i]->setFocus();
            return false;
        }
        if (channelNames.contains(name)) {
            QMessageBox::warning(this, "错误提示",
                                 QString("第%1行通道名称 \"%2\" 与前面的通道重名！").arg(i + 1).arg(name));
            edit_channel_name[i]->setFocus();
            return false;
        }
        channelNames.insert(name);
    }

    return true;
}

void SettingsWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint posInTitle = title_bar->mapFromGlobal(event->globalPos());
        if (title_bar->rect().contains(posInTitle)) {
            QWidget *child = title_bar->childAt(posInTitle);
            if (!qobject_cast<QPushButton *>(child)) {
                m_dragging = true;
                m_dragPosition = event->globalPos() - frameGeometry().topLeft();
                return;
            }
        }
    }
    QDialog::mousePressEvent(event);
}

void SettingsWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        move(event->globalPos() - m_dragPosition);
        return;
    }
    QDialog::mouseMoveEvent(event);
}

void SettingsWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging) {
        m_dragging = false;
        return;
    }
    QDialog::mouseReleaseEvent(event);
}