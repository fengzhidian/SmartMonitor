#ifndef DBCONNECTIONPOOL_H
#define DBCONNECTIONPOOL_H

#include <QSqlDatabase>
#include <QMutex>
#include <QString>
#include <QSettings>
#include <QMutexLocker>
#include <QHash>
#include <QThread>
#include <QSqlQuery>
#include <QUuid>
#include <QCryptographicHash>

class DBConnectionPool
{
public:
    static DBConnectionPool *getInstance(); // 单例获取接口
    QSqlDatabase getConnection(); // 获取当前线程可用连接
    void releaseConnection(); // 归还当前线程连接
    QString getMd5(const QString &str);
private:
    static DBConnectionPool *instance;
    DBConnectionPool();// 私有构造，防止外部创建
    // 禁用拷贝构造和赋值运算符
    DBConnectionPool(const DBConnectionPool&) = delete;
    DBConnectionPool& operator=(const DBConnectionPool&) = delete;
    ~DBConnectionPool(); // 私有析构，释放资源

    bool isConnectionValid(const QString& connName);

    QMutex mutex;// 互斥锁，保护连接队列
    QHash<Qt::HANDLE, QString> threadCurrentConn; // 保护链接连接的容器（线程ID = 连接名）
    QString hostName; // 数据库主机名
    QString databaseName; // 数据库名
    QString userName; // 用户名
    QString password; // 密码
    int port; // 端口号
};

#endif // DBCONNECTIONPOOL_H
