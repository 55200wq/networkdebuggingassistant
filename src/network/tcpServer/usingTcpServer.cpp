#include "usingTcpServer.h"

usingTcpServer :: usingTcpServer(QObject *parent) :
    QObject(parent)
{
    this->server = new tcpServerTest(this);
    localAddressListAll = server->getLocalAddrList();
    localAddressListIPv4 = server->getLocalAddrList();
    for(auto addr : this->localAddressListIPv4)
    {
        if(QAbstractSocket::IPv6Protocol == addr.protocol())
        {
            localAddressListIPv4.removeOne(addr);
        }
    }

}
usingTcpServer:: ~usingTcpServer()
{

}


