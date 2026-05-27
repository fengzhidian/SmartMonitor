#include "logviewwidget.h"

LogViewWidget::LogViewWidget(QWidget *parent)
    : QWidget{parent}
{
    current_page = 1;
    total_page = 1;
    current_log_type = 0;
    create_widget();
    updateTable();
}

void LogViewWidget::create_widget()
{
    QHBoxLayout *main_layout = new QHBoxLayout;
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    QWidget *left_panel = new QWidget;
    left_panel->setObjectName("left_panel");
    QWidget *right_panel = new QWidget;
    right_panel->setObjectName("right_panel");

    setup_left_panel(left_panel);
    setup_right_panel(right_panel);

    main_layout->addWidget(left_panel, 1);
    main_layout->addWidget(right_panel, 12);

    setLayout(main_layout);
}

void LogViewWidget::setup_left_panel(QWidget *panel)
{
    QVBoxLayout *left_layout = new QVBoxLayout;
    left_layout->setContentsMargins(12, 12, 12, 12);
    left_layout->setSpacing(12);
    panel->setLayout(left_layout);

    QLabel *lab_title = new QLabel("日志类型");
    lab_title->setObjectName("panel_title");
    left_layout->addWidget(lab_title);

    QFrame *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setObjectName("separator");
    left_layout->addWidget(separator);

    btn_operation_log = new QPushButton("操作日志");
    btn_operation_log->setObjectName("ctrl_btn");
    btn_operation_log->setCursor(Qt::PointingHandCursor);
    btn_operation_log->setCheckable(true);
    btn_operation_log->setChecked(true);
    btn_operation_log->setFixedHeight(40);

    btn_exception_log = new QPushButton("异常日志");
    btn_exception_log->setObjectName("ctrl_btn");
    btn_exception_log->setCursor(Qt::PointingHandCursor);
    btn_exception_log->setCheckable(true);
    btn_exception_log->setFixedHeight(40);

    left_layout->addWidget(btn_operation_log);
    left_layout->addWidget(btn_exception_log);
    left_layout->addStretch();

    connect(btn_operation_log, &QPushButton::clicked, this, [this]() {
        btn_operation_log->setChecked(true);
        btn_exception_log->setChecked(false);
        showOperationLog();
    });
    connect(btn_exception_log, &QPushButton::clicked, this, [this]() {
        btn_operation_log->setChecked(false);
        btn_exception_log->setChecked(true);
        showExceptionLog();
    });
}

void LogViewWidget::setup_right_panel(QWidget *panel)
{
    QVBoxLayout *right_layout = new QVBoxLayout;
    right_layout->setContentsMargins(12, 12, 12, 12);
    right_layout->setSpacing(8);
    panel->setLayout(right_layout);

    QLabel *lab_title = new QLabel("日志记录");
    lab_title->setObjectName("panel_title");
    right_layout->addWidget(lab_title);

    QFrame *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setObjectName("separator");
    right_layout->addWidget(separator);

    log_table = new QTableWidget(page_size, 4);
    log_table->setObjectName("log_table");
    log_table->setHorizontalHeaderLabels({"日志ID", "日志操作", "操作员", "操作时间"});
    log_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    log_table->verticalHeader()->setVisible(false);
    log_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    log_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    log_table->setSelectionMode(QAbstractItemView::SingleSelection);
    log_table->setAlternatingRowColors(true);
    right_layout->addWidget(log_table, 1);

    QWidget *page_bar = new QWidget;
    QHBoxLayout *page_layout = new QHBoxLayout;
    page_layout->setContentsMargins(0, 0, 0, 0);
    page_layout->setSpacing(8);
    page_bar->setLayout(page_layout);

    page_layout->addStretch();

    btn_prev = new QPushButton("上一页");
    btn_prev->setObjectName("ctrl_btn");
    btn_prev->setCursor(Qt::PointingHandCursor);

    lab_page = new QLabel("1/1");
    lab_page->setObjectName("channel_label");
    lab_page->setAlignment(Qt::AlignCenter);

    btn_next = new QPushButton("下一页");
    btn_next->setObjectName("ctrl_btn");
    btn_next->setCursor(Qt::PointingHandCursor);

    page_layout->addWidget(btn_prev);
    page_layout->addWidget(lab_page);
    page_layout->addWidget(btn_next);
    page_layout->addStretch();

    right_layout->addWidget(page_bar);

    connect(btn_prev, &QPushButton::clicked, this, &LogViewWidget::prevPage);
    connect(btn_next, &QPushButton::clicked, this, &LogViewWidget::nextPage);
}

void LogViewWidget::showOperationLog()
{
    current_log_type = 0;
    current_page = 1;
    updateTable();
}

void LogViewWidget::showExceptionLog()
{
    current_log_type = 1;
    current_page = 1;
    updateTable();
}

void LogViewWidget::updateTable()
{
    log_table->clearContents();
    log_table->setRowCount(0);

    if (current_log_type == 0) {
        OperationLogModel olm;
        QList<OperationLogInfo> logList;
        int total = 0;
        int offset = (current_page - 1) * page_size;
        olm.queryOperationLogs(page_size, offset, logList, total);

        total_page = total / page_size;
        if (total % page_size != 0) total_page++;
        if (total_page == 0) total_page = 1;

        log_table->setRowCount(logList.size());
        for (int i = 0; i < logList.size(); ++i) {
            QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(logList[i].id));
            QTableWidgetItem *item1 = new QTableWidgetItem(logList[i].openactionFunc);
            QTableWidgetItem *item2 = new QTableWidgetItem(logList[i].adminName);
            QTableWidgetItem *item3 = new QTableWidgetItem(logList[i].operationTime);
            item0->setTextAlignment(Qt::AlignCenter);
            item1->setTextAlignment(Qt::AlignCenter);
            item2->setTextAlignment(Qt::AlignCenter);
            item3->setTextAlignment(Qt::AlignCenter);
            item0->setToolTip(logList[i].operationDesc);
            item1->setToolTip(logList[i].operationDesc);
            log_table->setItem(i, 0, item0);
            log_table->setItem(i, 1, item1);
            log_table->setItem(i, 2, item2);
            log_table->setItem(i, 3, item3);
        }
    } else {
        ExceptionLogModel elm;
        QList<ExceptionLogInfo> logList;
        int total = 0;
        int offset = (current_page - 1) * page_size;
        elm.queryExceptionLogs(page_size, offset, logList, total);

        total_page = total / page_size;
        if (total % page_size != 0) total_page++;
        if (total_page == 0) total_page = 1;

        log_table->setRowCount(logList.size());
        for (int i = 0; i < logList.size(); ++i) {
            QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(logList[i].id));
            QTableWidgetItem *item1 = new QTableWidgetItem(logList[i].eventDesc);
            QTableWidgetItem *item2 = new QTableWidgetItem(QString("通道%1").arg(logList[i].channelId));
            QTableWidgetItem *item3 = new QTableWidgetItem(logList[i].eventTime);
            item0->setTextAlignment(Qt::AlignCenter);
            item1->setTextAlignment(Qt::AlignCenter);
            item2->setTextAlignment(Qt::AlignCenter);
            item3->setTextAlignment(Qt::AlignCenter);
            log_table->setItem(i, 0, item0);
            log_table->setItem(i, 1, item1);
            log_table->setItem(i, 2, item2);
            log_table->setItem(i, 3, item3);
        }
    }

    lab_page->setText(QString("%1/%2").arg(current_page).arg(total_page));
    btn_prev->setEnabled(current_page > 1);
    btn_next->setEnabled(current_page < total_page);
}

void LogViewWidget::prevPage()
{
    if (current_page <= 1) return;
    current_page--;
    updateTable();
}

void LogViewWidget::nextPage()
{
    if (current_page >= total_page) return;
    current_page++;
    updateTable();
}