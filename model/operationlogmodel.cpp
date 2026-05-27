#include "operationlogmodel.h"

QMutex OperationLogModel::s_mutex;

OperationLogModel::OperationLogModel() {}

bool OperationLogModel::insertOperationLog(int adminId, const QString &func, const QString &desc)
{
    QMutexLocker locker(&s_mutex);
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();
    if (!db.isValid()) {
        qDebug() << "OperationLogModel 连接数据库失败";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO operation_log(admin_id, operation_time, openaction_func, operation_desc) "
                  "VALUES(:adminId, :operationTime, :func, :desc)");
    query.bindValue(":adminId", adminId);
    query.bindValue(":operationTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":func", func);
    query.bindValue(":desc", desc);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            qDebug() << "操作日志入库成功：" << func;
            return true;
        }
    }
    qDebug() << "操作日志入库失败：" << query.lastError();
    return false;
}

bool OperationLogModel::queryOperationLogs(int limit, int offset,
                                           QList<OperationLogInfo> &logList, int &total)
{
    QMutexLocker locker(&s_mutex);
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();
    if (!db.isValid()) {
        qDebug() << "OperationLogModel 查询连接失败";
        return false;
    }

    QSqlQuery countQuery(db);
    countQuery.prepare("SELECT count(*) as count FROM operation_log");
    if (countQuery.exec() && countQuery.next()) {
        total = countQuery.value("count").toInt();
    }

    QSqlQuery query(db);
    query.prepare("SELECT ol.id, ol.admin_id, ol.operation_time, ol.openaction_func, "
                  "ol.operation_desc, au.username "
                  "FROM operation_log ol "
                  "LEFT JOIN admin_user au ON ol.admin_id = au.id "
                  "ORDER BY ol.operation_time DESC LIMIT :limit OFFSET :offset");
    query.bindValue(":limit", limit);
    query.bindValue(":offset", offset);

    if (query.exec()) {
        while (query.next()) {
            OperationLogInfo info;
            info.id = query.value("id").toInt();
            info.adminId = query.value("admin_id").toInt();
            info.operationTime = query.value("operation_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            info.openactionFunc = query.value("openaction_func").toString();
            info.operationDesc = query.value("operation_desc").toString();
            info.adminName = query.value("username").toString();
            logList.append(info);
        }
        return true;
    }
    qDebug() << "查询操作日志失败：" << query.lastError();
    return false;
}