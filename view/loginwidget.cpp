#include "loginwidget.h"
#include "../util/winmanager.h"
LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("登录窗口");
    this->resize(800,600);
    //设置背景图
    QPixmap pm(":/image/bg1.jpeg");
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window,QBrush(pm.scaled(this->width(),this->height())));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QVBoxLayout *main_layout = new QVBoxLayout;
    this->setLayout(main_layout);

    QLabel *lab_title = new QLabel("智能监控系统");
    lab_title->setFont(QFont("宋体",20,QFont::Bold));
    lab_title->setAlignment(Qt::AlignCenter);//设置对齐方式为水平居中对齐

    //表单布局
    QHBoxLayout *h_layout = new QHBoxLayout;
    QFormLayout *form_layout = new QFormLayout;
    edit_user = new QLineEdit;
    edit_pwd = new QLineEdit;
    edit_pwd->setEchoMode(QLineEdit::Password);
    QHBoxLayout *pwd_layout = new QHBoxLayout;
    pwd_layout->setSpacing(4);
    pwd_layout->addWidget(edit_pwd);

    btn_toggle_pwd = new QPushButton("显示");
    btn_toggle_pwd->setFixedSize(40, edit_pwd->sizeHint().height());
    btn_toggle_pwd->setCursor(Qt::PointingHandCursor);
    btn_toggle_pwd->setCheckable(true);
    pwd_layout->addWidget(btn_toggle_pwd);

    form_layout->addRow("用户名：",edit_user);
    form_layout->addRow("密  码：", pwd_layout);
    //验证码布局容器
    QHBoxLayout *vcode_layout = new QHBoxLayout;
    QLabel *lab_layout = new QLabel("验证码");
    edit_vcode = new QLineEdit;
    edit_vcode->setMinimumWidth(40);
    edit_vcode->setMaximumWidth(60);
    edit_vcode->setMaxLength(4);
    lab_vcode = new QLabel;
    lab_vcode->setFixedSize(120,40);
    lab_vcode->setAlignment(Qt::AlignCenter);
    lab_vcode->setStyleSheet("background-color: #ffffff; border: 1px solid #cccccc;");
    lab_vcode->setCursor(Qt::PointingHandCursor);
    lab_vcode->installEventFilter(this);

    vcode_layout->addWidget(lab_layout);
    vcode_layout->addWidget(edit_vcode);
    vcode_layout->addWidget(lab_vcode);

    form_layout->addRow(vcode_layout);
    //按钮布局容器
    QHBoxLayout *btn_layout = new QHBoxLayout;
    btn_login = new QPushButton("登录");
    QPushButton *btn_cancel = new QPushButton("取消");
    btn_layout->addWidget(btn_login);
    btn_layout->addWidget(btn_cancel);
    connect(btn_cancel, &QPushButton::clicked, this, &QWidget::close);
    form_layout->addRow(btn_layout);

    form_layout->setSpacing(20);
    h_layout->addStretch();
    h_layout->addLayout(form_layout);
    h_layout->addStretch();

    main_layout->addStretch(2);
    main_layout->addWidget(lab_title);
    main_layout->addStretch(1);
    main_layout->addLayout(h_layout);
    main_layout->addStretch(2);
    this->refreshVcode();
    this->init_connect();

}

void LoginWidget::init_connect()
{
    connect(btn_login,SIGNAL(clicked()),this,SLOT(login()));
    connect(btn_toggle_pwd, &QPushButton::toggled, this, [this](bool checked) {
        if (checked) {
            edit_pwd->setEchoMode(QLineEdit::Normal);
            btn_toggle_pwd->setText("隐藏");
        } else {
            edit_pwd->setEchoMode(QLineEdit::Password);
            btn_toggle_pwd->setText("显示");
        }
    });
}

QString LoginWidget::generateVcode()
{
    QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString result;
    QTime time = QTime::currentTime();
    //qsrand(time.msec()+time.second()*1000);
    for(int i=0;i<4;i++){
        int index = randomInt() % chars.length();
        result += chars.at(index);
    }
    return result;
}

void LoginWidget::refreshVcode()
{
    this->vcode = this->generateVcode();
    QPixmap pix(lab_vcode->size());
    pix.fill(Qt::white);
    QPainter painter(&pix);
    paintVCode(&painter, this->vcode);
    lab_vcode->setPixmap(pix);
}

void LoginWidget::paintVCode(QPainter *painter, const QString &code)
{
    QFont font("Arial", 20, QFont::Bold);
    painter->setFont(font);

    QPen pen;
    QList<QColor> colors = {Qt::red, Qt::blue, Qt::green, Qt::black, Qt::darkMagenta};

    for(int i = 0; i < code.length(); i++){
        QColor color = colors[randomInt() % colors.length()];
        pen.setColor(color);
        painter->setPen(pen);

        int x = 15 + i * 25;
        int y = 28 + (randomInt() % 8);
        painter->drawText(x, y, QString(code[i]));
    }

    for(int i = 0; i < 100; i++){
        int x = randomInt() % 120;
        int y = randomInt() % 40;
        QColor color = colors[randomInt() % colors.length()];
        painter->setPen(color);
        painter->drawPoint(x, y);
    }

    for(int i = 0; i < 5; i++){
        int x1 = randomInt() % 120;
        int y1 = randomInt() % 40;
        int x2 = randomInt() % 120;
        int y2 = randomInt() % 40;
        QColor color = colors[randomInt() % colors.length()];
        pen.setColor(color);
        painter->setPen(pen);
        painter->drawLine(x1, y1, x2, y2);
    }
}

bool LoginWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == lab_vcode){
        if(event->type() == QEvent::MouseButtonPress){
            refreshVcode();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

int LoginWidget::randomInt()
{
    // 静态随机数引擎（仅初始化一次，避免重复种子）
    static std::mt19937 engine(std::random_device{}());
    // 0-32768之间
    std::uniform_int_distribution<int> dist(0,RAND_MAX);
    return dist(engine);
}
void LoginWidget::login()
{
    //发送login_success信号
    QString username = edit_user->text().trimmed();
    QString password = edit_pwd->text().trimmed();
    QString vcode = edit_vcode->text().trimmed();
    if(vcode.isEmpty()){
        QMessageBox::warning(this,"验证码错误","请输入验证码");
        return;
    }
    if(vcode.toLower() != this->vcode.toLower()){
        QMessageBox::warning(this,"验证码错误","验证码错误，请重新输入");
        this->refreshVcode();
        this->edit_vcode->setText("");
        return;
    }
    //查询数据库验证用户名和密码是否正确
    UserService userService;
    int userId = 0;
    bool result = userService.login(username, password, userId);
    if (result) {
        WinManager::getInstance()->mainWidget.setLoggedIn(username, userId);
        WinManager::getInstance()->mainWidget.show();
        this->hide();
    } else {
        QMessageBox::warning(this,"登录警告","用户名或密码错误，请重新输入");
        this->edit_pwd->setText("");
        this->edit_vcode->setText("");
        this->refreshVcode();
    }
}
