#include "featureimagemodel.h"

QMutex FeatureImageModel::s_mutex;

FeatureImageModel::FeatureImageModel() {}

bool FeatureImageModel::insertImage(const QString &imageName, const QString &imagePath,
                                    int channelId, const QString &captureTime,
                                    int exceptionId)
{
    QMutexLocker locker(&s_mutex);
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();

    if (!db.isValid()) {
        qDebug() << "FeatureImageModel 连接数据库失败：" << db.lastError().text();
        return false;
    }

    QString sql = "INSERT INTO feature_image(image_name, image_path, channel_id, "
                  "capture_time, exception_id, create_time) "
                  "VALUES(:imageName, :imagePath, :channelId, "
                  ":captureTime, :exceptionId, :createTime)";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":imageName", imageName);
    query.bindValue(":imagePath", imagePath);
    query.bindValue(":channelId", channelId);
    query.bindValue(":captureTime", captureTime);
    query.bindValue(":exceptionId", exceptionId);
    query.bindValue(":createTime", captureTime);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            qDebug() << "截图入库成功：" << imageName;
            return true;
        }
    }
    qDebug() << "截图入库失败：" << query.lastError();
    return false;
}

bool FeatureImageModel::queryImages(int channelId, QDate queryDate, int exceptionId,
                                    int limit, int offset,
                                    QList<ImageInfo> &imageList, int &total)
{
    QMutexLocker locker(&s_mutex);
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();
    if (!db.isValid()) {
        qDebug() << "FeatureImageModel 查询连接失败";
        return false;
    }

    QString beginTime = queryDate.toString("yyyy-MM-dd") + " 00:00:00";
    QString endTime = queryDate.toString("yyyy-MM-dd") + " 23:59:59";
    QString conditionSql = "WHERE capture_time >= :beginTime AND capture_time <= :endTime "
                           "AND exception_id = :exceptionId ";
    if (channelId > 0) conditionSql += "AND channel_id = :channelId ";

    QString countSql = "SELECT count(*) as count FROM feature_image " + conditionSql;
    QSqlQuery countQuery(db);
    countQuery.prepare(countSql);
    countQuery.bindValue(":beginTime", beginTime);
    countQuery.bindValue(":endTime", endTime);
    countQuery.bindValue(":exceptionId", exceptionId);
    if (channelId > 0) countQuery.bindValue(":channelId", channelId);
    if (countQuery.exec() && countQuery.next()) {
        total = countQuery.value("count").toInt();
    }

    QString sql = "SELECT * FROM feature_image " + conditionSql
                  + "ORDER BY capture_time DESC LIMIT :limit OFFSET :offset";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":beginTime", beginTime);
    query.bindValue(":endTime", endTime);
    query.bindValue(":exceptionId", exceptionId);
    if (channelId > 0) query.bindValue(":channelId", channelId);
    query.bindValue(":limit", limit);
    query.bindValue(":offset", offset);

    if (query.exec()) {
        while (query.next()) {
            ImageInfo info;
            info.id = query.value("id").toInt();
            info.imageName = query.value("image_name").toString();
            info.imagePath = query.value("image_path").toString();
            info.channelId = query.value("channel_id").toInt();
            info.captureTime = query.value("capture_time").toString();
            info.exceptionId = query.value("exception_id").toInt();
            imageList.append(info);
        }
        return true;
    }
    qDebug() << "查询图片失败：" << query.lastError();
    return false;
}
