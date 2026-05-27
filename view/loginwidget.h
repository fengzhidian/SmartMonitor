#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "../util/dbconn.h"
#include "../service/userservice.h"
#include <QHeaderView>
#include <QDebug>
#include <QTime>
#include <QPainter>
#include <QMouseEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include <QVBoxLayout> //垂直布局
#include <QPushButton> //按钮
#include <QHBoxLayout> //水平布局
#include <QFormLayout> //表单布局
#include <QLineEdit>
#include <QLabel>
#include <QPixmap>
#include <QBrush>
#include <QPalette>
#include <QMessageBox>
#include <QFile>
#include <random>

class LoginWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton *btn_login;
    QPushButton *btn_toggle_pwd;
    QLineEdit *edit_user;
    QLineEdit *edit_pwd;
    QLineEdit *edit_vcode;
    QLabel *lab_vcode;
    QString vcode;
public:
    explicit LoginWidget(QWidget *parent = nullptr);
    void init_connect();
    QString generateVcode();
    void refreshVcode();
    void paintVCode(QPainter *painter, const QString &code);
    bool eventFilter(QObject *obj, QEvent *event) override;
    int randomInt();

signals://自定义信号区，只需要声明不需要定义
    void login_success(QString);
public slots:
    void login();
};

#endif // LOGINWIDGET_H
