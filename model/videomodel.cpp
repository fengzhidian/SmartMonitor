#include "videomodel.h"

QMutex VideoModel::s_mutex;
VideoModel::VideoModel() {}

bool VideoModel::insertVideo(Video &video)
{
    // 添加锁，防止四通道录制结束后抢占资源造成死锁
    QMutexLocker locker(&s_mutex);
    // 注册MySQL驱动
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();

    // 连接数据库
    if(!db.isValid()){// 打开失败
        qDebug() << "连接数据库失败：" << db.lastError().text();
        return false;
    }
    qDebug() << "数据库连接成功";
    QString sql = "insert into video(video_name,video_path,channel_id,file_size,create_time,exception_id) "
                  "values(:videoName,:videoPath,:channelId,:fileSize,:createTime,:exceptionId)";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":videoName",video.getVideoName());
    query.bindValue(":videoPath",video.getVideoPath());
    query.bindValue(":channelId",video.getChannel_id());
    query.bindValue(":createTime",video.getCreateTime());
    query.bindValue(":fileSize",video.getFileSize());
    query.bindValue(":exceptionId",video.getExceptionId());
    if(query.exec())
        if(query.numRowsAffected() > 0)
            return true;
    qDebug() << query.lastError();
    return false;
}

bool VideoModel::ensureChannel(int channelId)
{
    // 添加锁，防止四通道录制结束后抢占资源造成死锁
    QMutexLocker locker(&s_mutex);
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();
    if(!db.isValid()){
        qDebug() << "连接数据库失败：" << db.lastError().text();
        return false;
    }
    QSqlQuery query(db);
    query.prepare("INSERT IGNORE INTO camera_channel(id, camera_name) VALUES(:id, :name)");
    query.bindValue(":id", channelId);
    if(!query.exec() || !query.next()){
        qDebug() << "ensureChannel 查询失败：" << query.lastError();
        return false;
    }
    if(query.value(0).toInt() == 0){
        query.prepare("INSERT IGNORE INTO camera_channel(id, channel_name) VALUES(:id, :name)");
        query.bindValue(":id", channelId);
        query.bindValue(":name", QString("通道%1").arg(channelId));
        if(query.exec()){
            qDebug() << "camera_channel 插入通道" << channelId << "成功";
            return true;
        }
        qDebug() << "camera_channel 插入失败：" << query.lastError();
        return false;
    }
    return true;
}

bool VideoModel::queryVideoList(int channelId, QDate queryDate, int limit, int offset, QList<Video> &videoList)
{
    // 添加锁，防止四通道录制结束后抢占资源造成死锁
    QMutexLocker locker(&s_mutex);
    // 注册MySQL驱动
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();

    // 连接数据库
    if(!db.isValid()){// 打开失败
        qDebug() << "连接数据库失败：" << db.lastError().text();
        return false;
    }

    QString beginTime = queryDate.toString("yyyy-MM-dd") + " 00:00:00";
    QString endTime = queryDate.toString("yyyy-MM-dd") + " 23:59:59";
    QString sql = "SELECT * FROM video WHERE create_time >= :beginTime AND create_time <= :endTime ";
    if(channelId > 0) sql += "AND channel_id = :channelId ";
    sql += "ORDER BY create_time DESC LIMIT :limit OFFSET :offset";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":beginTime",beginTime);
    query.bindValue(":endTime",endTime);
    if(channelId > 0) query.bindValue(":channelId",channelId);
    query.bindValue(":limit",limit);
    query.bindValue(":offset",offset);
    if(query.exec()){
        while(query.next()){
            Video video;
            video.setVideoId(query.value("id").toInt());
            video.setVideoName(query.value("video_name").toString());
            video.setVideoPath(query.value("video_path").toString());
            video.setChannel_id(query.value("channel_id").toInt());
            videoList.append(video);
        }
        return true;
    }
    qDebug() << query.lastError();
    return false;
}

int VideoModel::getVideosTotal(int channelId, QDate queryDate)
{
    int total = 0;
    // 添加锁，防止四通道录制结束后抢占资源造成死锁
    QMutexLocker locker(&s_mutex);
    // 注册MySQL驱动
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();

    // 连接数据库
    if(!db.isValid()){// 打开失败
        qDebug() << "连接数据库失败：" << db.lastError().text();
        return false;
    }

    QString beginTime = queryDate.toString("yyyy-MM-dd") + " 00:00:00";
    QString endTime = queryDate.toString("yyyy-MM-dd") + " 23:59:59";
    QString sql = "SELECT count(*) as count FROM video WHERE create_time >= :beginTime AND create_time <= :endTime ";
    if(channelId > 0) sql += "AND channel_id = :channelId ";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":beginTime",beginTime);
    query.bindValue(":endTime",endTime);
    if(channelId > 0) query.bindValue(":channelId",channelId);
    if(query.exec()){
        while(query.next()){
            total = query.value("count").toInt();
        }
    }else {
        qDebug() << query.lastError();
    }
    return total;
}

bool VideoModel::queryVideoListByException(int exceptionId, int channelId, QDate queryDate, int limit, int offset, QList<Video> &videoList)
{
    QMutexLocker locker(&s_mutex);
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();

    if(!db.isValid()){
        qDebug() << "连接数据库失败：" << db.lastError().text();
        return false;
    }

    QString beginTime = queryDate.toString("yyyy-MM-dd") + " 00:00:00";
    QString endTime = queryDate.toString("yyyy-MM-dd") + " 23:59:59";
    QString sql = "SELECT * FROM video WHERE exception_id = :exceptionId "
                  "AND create_time >= :beginTime AND create_time <= :endTime ";
    if(channelId > 0) sql += "AND channel_id = :channelId ";
    sql += "ORDER BY create_time DESC LIMIT :limit OFFSET :offset";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":exceptionId", exceptionId);
    query.bindValue(":beginTime", beginTime);
    query.bindValue(":endTime", endTime);
    if(channelId > 0) query.bindValue(":channelId", channelId);
    query.bindValue(":limit", limit);
    query.bindValue(":offset", offset);
    if(query.exec()){
        while(query.next()){
            Video video;
            video.setVideoId(query.value("id").toInt());
            video.setVideoName(query.value("video_name").toString());
            video.setVideoPath(query.value("video_path").toString());
            video.setChannel_id(query.value("channel_id").toInt());
            videoList.append(video);
        }
        return true;
    }
    qDebug() << query.lastError();
    return false;
}

int VideoModel::getVideosTotalByException(int exceptionId, int channelId, QDate queryDate)
{
    int total = 0;
    QMutexLocker locker(&s_mutex);
    QSqlDatabase db = DBConnectionPool::getInstance()->getConnection();

    if(!db.isValid()){
        qDebug() << "连接数据库失败：" << db.lastError().text();
        return 0;
    }

    QString beginTime = queryDate.toString("yyyy-MM-dd") + " 00:00:00";
    QString endTime = queryDate.toString("yyyy-MM-dd") + " 23:59:59";
    QString sql = "SELECT count(*) as count FROM video WHERE exception_id = :exceptionId "
                  "AND create_time >= :beginTime AND create_time <= :endTime ";
    if(channelId > 0) sql += "AND channel_id = :channelId ";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":exceptionId", exceptionId);
    query.bindValue(":beginTime", beginTime);
    query.bindValue(":endTime", endTime);
    if(channelId > 0) query.bindValue(":channelId", channelId);
    if(query.exec()){
        while(query.next()){
            total = query.value("count").toInt();
        }
    } else {
        qDebug() << query.lastError();
    }
    return total;
}