#include "usingTcpClient.h"

usingTcpClient :: usingTcpClient(QObject *parent) :
    QObject(parent)
{
    this->client = new tcpClientTest(this);
}
usingTcpClient:: ~usingTcpClient()
{

}

