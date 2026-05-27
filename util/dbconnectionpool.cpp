#include "dbconnectionpool.h"
// 单例
DBConnectionPool *DBConnectionPool::instance = nullptr;
DBConnectionPool *DBConnectionPool::getInstance()
{
    static QMutex mutex;
    if(DBConnectionPool::instance == nullptr){
        mutex.lock();
        if(DBConnectionPool::instance == nullptr){
            DBConnectionPool::instance = new DBConnectionPool;
        }
        mutex.unlock();
    }
    return DBConnectionPool::instance;
}

// 获取链接
QSqlDatabase DBConnectionPool::getConnection()
{
    QMutexLocker locker(&mutex);
    // 获取当前线程ID
    Qt::HANDLE currentThreadId = QThread::currentThreadId();
    // 容器中是否有当前线程的连接
    if(this->threadCurrentConn.contains(currentThreadId)){
        QString connName = this->threadCurrentConn[currentThreadId];
        if(isConnectionValid(connName)){
            return QSqlDatabase::database(connName);
        }else{
            this->threadCurrentConn.remove(currentThreadId);
        }
    }
    // 若没有连接或连接无效，则创建新链接并保存到容器中
    QString connName = QUuid::createUuid().toString();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",connName);
    db.setHostName(this->hostName);
    db.setDatabaseName(this->databaseName);
    db.setUserName(this->userName);
    db.setPassword(this->password);
    db.setPort(this->port);
    if(db.open()){
        qDebug() << "New DB is created: " << connName;
        this->threadCurrentConn[currentThreadId] = connName;
        return db;
    }
    return QSqlDatabase();
}

// 归还连接,线程结束后移除连接
void DBConnectionPool::releaseConnection()
{
    QMutexLocker locker(&mutex);
    Qt::HANDLE tid = QThread::currentThreadId();
    if(!this->threadCurrentConn.contains(tid)){
        return;
    }
    QString connName = this->threadCurrentConn[tid];
    if(QSqlDatabase::contains(connName)){
        QSqlDatabase::removeDatabase(connName);
    }
    this->threadCurrentConn.remove(tid);
}

// 构造方法（加载配置文件）
DBConnectionPool::DBConnectionPool()
{
    QSettings settings("db_config.ini",QSettings::IniFormat);
    this->hostName = settings.value("Database/hostName").toString();
    this->databaseName = settings.value("Database/databaseName").toString();
    this->userName = settings.value("Database/userName").toString();
    this->password = settings.value("Database/password").toString();
    this->port = settings.value("Database/port").toInt();
    qDebug() << "dbconfig: " << this->hostName << this->databaseName << this->userName;
}

// 析构（释放所有连接）
DBConnectionPool::~DBConnectionPool()
{
    QMutexLocker locker(&mutex);
    for(Qt::HANDLE tid : this->threadCurrentConn.keys()){
        QString connName = this->threadCurrentConn[tid];
        if(QSqlDatabase::contains(connName)){
            QSqlDatabase::removeDatabase(connName);
        }
    }
    this->threadCurrentConn.clear();
}

// 判断连接有效性（连接名是否存在，是否已连接，执行最简sql语句）
bool DBConnectionPool::isConnectionValid(const QString &connName)
{
    if(!QSqlDatabase::contains(connName)){
        return false;
    }
    // 获取指定连接名的连接对象
    QSqlDatabase db = QSqlDatabase::database(connName);
    if(!db.isOpen()){
        return db.open(); // 再尝试一次
    }
    // 尝试执行一条最简单的SQL语句
    QSqlQuery query(db);
    return query.exec("select 1");
}

QString DBConnectionPool::getMd5(const QString &str)
{
    QByteArray byteArr = str.toUtf8();
    QByteArray md5Arr = QCryptographicHash::hash(byteArr, QCryptographicHash::Md5);
    return md5Arr.toHex();
}