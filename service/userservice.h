#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "../model/usermodel.h"

class UserService
{
public:
    UserService();
    bool login(QString username, QString password, int &userId);
private:
    UserModel usermodel;
};

#endif // USERSERVICE_H
