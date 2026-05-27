#include "video.h"

Video::Video() {}

QString Video::getCreateTime() const
{
    return createTime;
}

void Video::setCreateTime(const QString &newCreateTime)
{
    createTime = newCreateTime;
}

int Video::getChannel_id() const
{
    return channel_id;
}

void Video::setChannel_id(int newChannel_id)
{
    channel_id = newChannel_id;
}

QString Video::getVideoPath() const
{
    return videoPath;
}

void Video::setVideoPath(const QString &newVideoPath)
{
    videoPath = newVideoPath;
}

QString Video::getVideoName() const
{
    return videoName;
}

void Video::setVideoName(const QString &newVideoName)
{
    videoName = newVideoName;
}

int Video::getVideoId() const
{
    return videoId;
}

void Video::setVideoId(int newVideoId)
{
    videoId = newVideoId;
}

double Video::getFileSize() const
{
    return fileSize;
}

void Video::setFileSize(double newFileSize)
{
    fileSize = newFileSize;
}

int Video::getExceptionId() const
{
    return exceptionId;
}

void Video::setExceptionId(int newExceptionId)
{
    exceptionId = newExceptionId;
}
