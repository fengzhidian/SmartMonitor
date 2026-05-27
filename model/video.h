#ifndef VIDEO_H
#define VIDEO_H

#include <QString>

class Video
{
public:
    Video();
    QString getCreateTime() const;
    void setCreateTime(const QString &newCreateTime);

    int getChannel_id() const;
    void setChannel_id(int newChannel_id);

    QString getVideoPath() const;
    void setVideoPath(const QString &newVideoPath);

    QString getVideoName() const;
    void setVideoName(const QString &newVideoName);

    int getVideoId() const;
    void setVideoId(int newVideoId);

    double getFileSize() const;
    void setFileSize(double newFileSize);

    int getExceptionId() const;
    void setExceptionId(int newExceptionId);

private:
    int videoId;
    QString videoName;
    QString videoPath;
    int channel_id;
    QString createTime;
    double fileSize;
    int exceptionId;
};

#endif // VIDEO_H
