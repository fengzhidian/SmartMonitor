#ifndef CAMERAINFO_H
#define CAMERAINFO_H

#include <QString>

class CameraInfo
{
public:
    CameraInfo();
    int getChannelId() const;
    void setChannelId(int newChannelId);

    QString getChannelName() const;
    void setChannelName(const QString &newChannelName);

    QString getCameraName() const;
    void setCameraName(const QString &newCameraName);

private:
    int channelId;
    QString channelName;
    QString cameraName;
};

#endif // CAMERAINFO_H
