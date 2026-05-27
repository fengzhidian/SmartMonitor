#ifndef DBCONN_H
#define DBCONN_H
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>

class DBConn
{
public:
    static DBConn *getInstence();
    static void releaseInstence();
    DBConn(const DBConn&) = delete;
    DBConn& operator=(const DBConn&) = delete;
    QSqlDatabase getDb() const;
    QString getMd5(const QString &str);
private:
    DBConn();
    ~DBConn();
    static DBConn *s_instence;
    QSqlDatabase db;
};

#endif // DBCONN_H
