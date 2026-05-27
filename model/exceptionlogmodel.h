#ifndef EXCEPTIONLOGMODEL_H
#define EXCEPTIONLOGMODEL_H

#include "../util/dbconnectionpool.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QMutexLocker>
#include <QList>
#include <QDateTime>

struct ExceptionLogInfo
{
    int id;
    int channelId;
    QString eventTime;
    QString eventDesc;
    QString relatedVieroPath;
    QString createTime;
};

class ExceptionLogModel
{
public:
    ExceptionLogModel();
    bool insertExceptionLog(int channelId, const QString &eventDesc, const QString &relatedVieroPath);
    bool queryExceptionLogs(int limit, int offset,
                            QList<ExceptionLogInfo> &logList, int &total);
private:
    static QMutex s_mutex;
};

#endif // EXCEPTIONLOGMODEL_H