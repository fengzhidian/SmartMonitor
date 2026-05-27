#ifndef USERMODEL_H
#define USERMODEL_H

#include "../util/dbconnectionpool.h"
#include <QString>
#include <QDateTime>

class UserModel
{
public:
    UserModel();
    bool queryUserAndPwd(QString &username, QString &password,int &userId);
    int updateLoginTime(int &userid);
};

#endif // USERMODEL_H
