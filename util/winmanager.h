#ifndef WINMANAGER_H
#define WINMANAGER_H

#include "../view/mainwidget.h"
#include "../view/loginwidget.h"
#include "../view/settingswidget.h"

class WinManager
{
public:
    static WinManager *getInstance();
    // 禁用拷贝构造和=运算符

    MainWidget mainWidget;
    // LoginWidget loginWidget;
    SettingsWidget *settingsWidget = nullptr;
    LoginWidget *loginWidget = nullptr;
    bool showSettingsFromMain();
    void showLogin();

private:
    WinManager();
    static WinManager *instance;
};

#endif // WINMANAGER_H
