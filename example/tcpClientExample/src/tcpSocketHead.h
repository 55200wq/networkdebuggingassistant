#ifndef TCPSOCKETHEAD_H
#define TCPSOCKETHEAD_H
#include <QObject>
#include <QHostAddress>

namespace TCP_SocketPublicInfo{
    typedef  enum
    {
        TCP_SERVER,
        TCP_CLIENT,
        UDP
    } ConnectType;
}

namespace TCP_ServerInfo
{
    struct server_info
    {
        quint16 port;
        QHostAddress addr;
    };//用于创建服务器

    struct socket_info
    {
        quint16 port;
        QHostAddress addr;
        QString hostName;
        QByteArray* sendData;//发送数据buff
        QByteArray* revData; //接收数据buff
    };
}

namespace TCP_ClientInfo
{

}

#endif // TCPSOCKETHEAD_H
