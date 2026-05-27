#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H
#include "video.h"
#include "../util/dbconnectionpool.h"
#include <QMutex>
#include <QSqlError>
#include <QMutexLocker>
#include <QDate>

class VideoModel
{
public:
    VideoModel();
    bool insertVideo(Video &video);
    bool ensureChannel(int channelId);
    // 查询当前页数据
    bool queryVideoList(int channelId,QDate queryDate,int limit,int offset,QList<Video> &videoList);
    // 查询符合条件的总记录数
    int getVideosTotal(int channelId,QDate queryDate);
    // 查询条件包含异常事件Id
    bool queryVideoListByException(int exceptionId, int channelId, QDate queryDate, int limit, int offset, QList<Video> &videoList);
    int getVideosTotalByException(int exceptionId, int channelId, QDate queryDate);
private:
    static QMutex s_mutex;
};

#endif // VIDEOMODEL_H
