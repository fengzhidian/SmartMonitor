#ifndef OPERATIONLOGMODEL_H
#define OPERATIONLOGMODEL_H

#include "../util/dbconnectionpool.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QMutexLocker>
#include <QList>
#include <QDateTime>

struct OperationLogInfo
{
    int id;
    int adminId;
    QString operationTime;
    QString openactionFunc;
    QString operationDesc;
    QString adminName;
};

class OperationLogModel
{
public:
    OperationLogModel();
    bool insertOperationLog(int adminId, const QString &func, const QString &desc);
    bool queryOperationLogs(int limit, int offset,
                            QList<OperationLogInfo> &logList, int &total);
private:
    static QMutex s_mutex;
};

#endif // OPERATIONLOGMODEL_H