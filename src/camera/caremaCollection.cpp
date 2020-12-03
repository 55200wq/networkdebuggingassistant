#include "caremaCollection.h"

caremaCollection::caremaCollection()
{
    cameraInfoList = QCameraInfo::availableCameras();
    qDebug()<<"camList"<<cameraInfoList.size();
    for(auto i : cameraInfoList)
    {
        qDebug()<<i.description();
    }
}
