#ifndef VIDEOSERVICE_H
#define VIDEOSERVICE_H

#include "../model/videomodel.h"

class VideoService
{
private:
    VideoModel videoModel;
public:
    VideoService();
    void queryVideosByPage(int channelId,QDate queryDate,int limit,int offset,QList<Video> &videoList, int &total);
    void queryVideosByException(int exceptionId, int channelId, QDate queryDate, int limit, int offset, QList<Video> &videoList, int &total);
};

#endif // VIDEOSERVICE_H
