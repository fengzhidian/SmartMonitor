#include "userservice.h"

UserService::UserService()
{

}

bool UserService::login(QString username, QString password, int &userId)
{
    bool result = usermodel.queryUserAndPwd(username, password, userId);
    if (result) {
        int row = usermodel.updateLoginTime(userId);
        if (row > 0) {
            return true;
        }
    }
    return false;
}
