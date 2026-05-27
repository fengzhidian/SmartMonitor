#include "winmanager.h"
WinManager *WinManager::instance = nullptr;
WinManager *WinManager::getInstance()
{
    if(WinManager::instance == nullptr){
        WinManager::instance = new WinManager;
    }
    return WinManager::instance;
}

WinManager::WinManager()
{

}

bool WinManager::showSettingsFromMain()
{
    if (!settingsWidget) {
        settingsWidget = new SettingsWidget;
    } else {
        settingsWidget->loadSettings();
    }
    return settingsWidget->exec() == QDialog::Accepted;
}
void WinManager::showLogin()
{
    if (!loginWidget) {
        loginWidget = new LoginWidget;
    }
    loginWidget->show();
}
