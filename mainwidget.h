#ifndef MAINWIDGET_H
#define MAINWIDGET_H

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

class MainWidget : public QWidget
{
    Q_OBJECT

private:
    QPushButton *btn_monitor;
    QPushButton *btn_review;
    QPushButton *btn_setting;
    QPushButton *btn_user;
    QPushButton *btn_logout;

    QLabel *lab_title;
    QLabel *lab_time;
    QLabel *lab_status;

    QListWidget *device_list;

    QLabel *video_placeholder;

    QWidget* create_top_bar();
    QWidget* create_main_area();
    QWidget* create_bottom_bar();
    void setup_left_panel(QWidget *panel);
    void setup_right_panel(QWidget *panel);

public:
    explicit MainWidget(QWidget *parent = nullptr);
    void init_qss();
    void init_time();

signals:

public slots:
    void update_time();
};

#endif // MAINWIDGET_H
