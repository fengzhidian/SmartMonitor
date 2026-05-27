#include "dbconn.h"
DBConn *DBConn::s_instence = nullptr;
DBConn *DBConn::getInstence()
{
    if(DBConn::s_instence == nullptr){
        DBConn::s_instence = new DBConn;
    }
    return DBConn::s_instence;
}

void DBConn::releaseInstence()
{
    if(DBConn::s_instence != nullptr){
        delete DBConn::s_instence;
        DBConn::s_instence = nullptr;
    }
}

DBConn::DBConn()
{
    // 注册mysql驱动
    db =QSqlDatabase::addDatabase("QMYSQL");
    // 设置数据库链接参数
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("1234");
    db.setDatabaseName("monitor");
    // 连接数据库
    if(!db.open()){// 打开失败
        qDebug() << "连接数据库失败：" << db.lastError().text();
        return;
    }
    qDebug() << "数据库连接成功";
}

DBConn::~DBConn()
{
    //释放数据库链接
    qDebug() << "释放数据库链接";
    if(db.isOpen()){
        db.close();// 断开数据库链接
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);// 移除默认连接
    }
}

QSqlDatabase DBConn::getDb() const
{
    return db;
}
// MD5加密
QString DBConn::getMd5(const QString &str){
    // 将字符串转为utf-8编码的字节数组（避免中文乱码）
    QByteArray byteArr = str.toUtf8();
    // 计算md5的哈希值
    QByteArray md5Arr = QCryptographicHash::hash(byteArr,QCryptographicHash::Md5);
    // 将字节数组转为16进制的字符串
    QString md5Str = md5Arr.toHex();
    return md5Str;
}
