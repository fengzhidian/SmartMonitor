#include "usermodel.h"

UserModel::UserModel()
{

}

bool UserModel::queryUserAndPwd(QString &username, QString &password, int &userId)
{
    //查询数据库验证用户名和密码是否正确
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();
    if(!db.isValid()){
        qDebug() << "数据库连接失败";
        return false;
    }
    QString sql = "select * from admin_user where username = :username and password = :password";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":username",username);
    query.bindValue(":password",DBConnectionPool::getInstance()->getMd5(password));
    if(query.exec()){
        if(query.next()){
            userId = query.value("id").toInt();
            return true;
        }
    }
    return false;
}

int UserModel::updateLoginTime(int &userid)
{
    // 注册MySQL驱动
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();

    // 连接数据库
    if(!db.isValid()){// 打开失败
        qDebug() << "连接数据库失败：";
        return false;
    }
    qDebug() << "数据库连接成功";
    QString sql = "update admin_user set last_login_time = :logintime where id = :id";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":logintime",QDateTime::currentDateTime());
    query.bindValue(":id", userid);
    if(query.exec())
        return query.numRowsAffected();
    return -1;
}
