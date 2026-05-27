#ifndef LOGVIEWWIDGET_H
#define LOGVIEWWIDGET_H

#include "../model/operationlogmodel.h"
#include "../model/exceptionlogmodel.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QSplitter>
#include <QFrame>

class LogViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogViewWidget(QWidget *parent = nullptr);
    void create_widget();

private slots:
    void showOperationLog();
    void showExceptionLog();
    void prevPage();
    void nextPage();

private:
    void setup_left_panel(QWidget *panel);
    void setup_right_panel(QWidget *panel);
    void updateTable();

    QPushButton *btn_operation_log;
    QPushButton *btn_exception_log;
    QTableWidget *log_table;
    QLabel *lab_page;
    QPushButton *btn_prev;
    QPushButton *btn_next;

    int current_page;
    int total_page;
    int current_log_type;
    static const int page_size = 6;
};

#endif // LOGVIEWWIDGET_H