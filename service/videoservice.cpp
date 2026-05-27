#include "videoservice.h"

VideoService::VideoService() {}

void VideoService::queryVideosByPage(int channelId, QDate queryDate, int limit, int offset, QList<Video> &videoList, int &total)
{
    videoModel.queryVideoList(channelId,queryDate,limit,offset,videoList);
    total = videoModel.getVideosTotal(channelId,queryDate);
}

void VideoService::queryVideosByException(int exceptionId, int channelId, QDate queryDate, int limit, int offset, QList<Video> &videoList, int &total)
{
    videoModel.queryVideoListByException(exceptionId, channelId, queryDate, limit, offset, videoList);
    total = videoModel.getVideosTotalByException(exceptionId, channelId, queryDate);
}
