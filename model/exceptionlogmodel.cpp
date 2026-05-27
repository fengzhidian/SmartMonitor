#include "exceptionlogmodel.h"

QMutex ExceptionLogModel::s_mutex;

ExceptionLogModel::ExceptionLogModel() {}

bool ExceptionLogModel::insertExceptionLog(int channelId, const QString &eventDesc,
                                           const QString &relatedVieroPath)
{
    QMutexLocker locker(&s_mutex);
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();
    if (!db.isValid()) {
        qDebug() << "ExceptionLogModel 连接数据库失败";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO exception_log(channel_id, event_time, event_desc, "
                  "related_viero_path, create_time) "
                  "VALUES(:channelId, :eventTime, :eventDesc, :relatedVieroPath, :createTime)");
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    query.bindValue(":channelId", channelId);
    query.bindValue(":eventTime", now);
    query.bindValue(":eventDesc", eventDesc);
    query.bindValue(":relatedVieroPath", relatedVieroPath);
    query.bindValue(":createTime", now);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            qDebug() << "异常日志入库成功：" << eventDesc;
            return true;
        }
    }
    qDebug() << "异常日志入库失败：" << query.lastError();
    return false;
}

bool ExceptionLogModel::queryExceptionLogs(int limit, int offset,
                                           QList<ExceptionLogInfo> &logList, int &total)
{
    QMutexLocker locker(&s_mutex);
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();
    if (!db.isValid()) {
        qDebug() << "ExceptionLogModel 查询连接失败";
        return false;
    }

    QSqlQuery countQuery(db);
    countQuery.prepare("SELECT count(*) as count FROM exception_log");
    if (countQuery.exec() && countQuery.next()) {
        total = countQuery.value("count").toInt();
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, channel_id, event_time, event_desc, "
                  "related_viero_path, create_time "
                  "FROM exception_log "
                  "ORDER BY event_time DESC LIMIT :limit OFFSET :offset");
    query.bindValue(":limit", limit);
    query.bindValue(":offset", offset);

    if (query.exec()) {
        while (query.next()) {
            ExceptionLogInfo info;
            info.id = query.value("id").toInt();
            info.channelId = query.value("channel_id").toInt();
            info.eventTime = query.value("event_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            info.eventDesc = query.value("event_desc").toString();
            info.relatedVieroPath = query.value("related_viero_path").toString();
            info.createTime = query.value("create_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            logList.append(info);
        }
        return true;
    }
    qDebug() << "查询异常日志失败：" << query.lastError();
    return false;
}