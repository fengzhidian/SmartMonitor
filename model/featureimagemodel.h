#ifndef FEATUREIMAGEMODEL_H
#define FEATUREIMAGEMODEL_H

#include "../util/dbconnectionpool.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QMutexLocker>
#include <QDate>
#include <QList>

struct ImageInfo
{
    int id;
    QString imageName;
    QString imagePath;
    int channelId;
    QString captureTime;
    int exceptionId;
};

class FeatureImageModel
{
public:
    FeatureImageModel();
    bool insertImage(const QString &imageName, const QString &imagePath,
                     int channelId, const QString &captureTime,
                     int exceptionId = 0);
    bool queryImages(int channelId, QDate queryDate, int exceptionId,
                     int limit, int offset,
                     QList<ImageInfo> &imageList, int &total);
private:
    static QMutex s_mutex;
};

#endif // FEATUREIMAGEMODEL_H
