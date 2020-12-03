#ifndef CAREMACOLLECTION_H
#define CAREMACOLLECTION_H
#include <QCamera>
#include <QCameraInfo>

class caremaCollection
{
public:
    caremaCollection();

private:
    QList<QCameraInfo> cameraInfoList;
};

#endif // CAREMACOLLECTION_H
