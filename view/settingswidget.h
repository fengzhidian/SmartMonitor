#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "../model/settinginfo.h"
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QMouseEvent>
#include <QPoint>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QMessageBox>
#include <QSet>
#include <QDir>

class SettingsWidget : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    void loadSettings();
    void saveSettings();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void setupUi();
    void applyStyles();
    QWidget* createTitleBar();
    bool validate();

    QLineEdit *edit_save_path;
    QPushButton *btn_browse;
    QComboBox *combo_interval;

    QLabel *lab_camera_names[4];
    QLineEdit *edit_channel_name[4];
    QComboBox *combo_channel[4];

    QWidget *title_bar;

    bool m_dragging = false;
    QPoint m_dragPosition;
};

#endif // SETTINGSWIDGET_H
