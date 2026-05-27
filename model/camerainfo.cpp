#include "camerainfo.h"

CameraInfo::CameraInfo() {}

int CameraInfo::getChannelId() const
{
    return channelId;
}

void CameraInfo::setChannelId(int newChannelId)
{
    channelId = newChannelId;
}

QString CameraInfo::getChannelName() const
{
    return channelName;
}

void CameraInfo::setChannelName(const QString &newChannelName)
{
    channelName = newChannelName;
}

QString CameraInfo::getCameraName() const
{
    return cameraName;
}

void CameraInfo::setCameraName(const QString &newCameraName)
{
    cameraName = newCameraName;
}
