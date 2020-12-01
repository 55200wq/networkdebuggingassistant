#include "tcpClientTest.h"

tcpClientTest::tcpClientTest(QObject* parent) :
    QTcpSocket(parent)
{
    qRegisterMetaType<QTcpSocket*>("QTcpSocket*");
    qRegisterMetaType<QTcpSocket*>("QAbstractSocket::SocketState");

    socketConnectSlot();
}

tcpClientTest::~tcpClientTest()
{
    socketDisconnectSlot();
}

void tcpClientTest::socketConnectSlot()
{
    //与上层类交互相关信号
    if(this->parent() != Q_NULLPTR)
    {
        connect(this, SIGNAL(socketConnect(QTcpSocket* )), this->parent(), SLOT(socketConnectSlot(QTcpSocket*)));
        connect(this, SIGNAL(socketDisconnect(QTcpSocket* )), this->parent(), SLOT(socketDisconnectSlot(QTcpSocket*)));
    }

    //socket 相关信号
    connect(this, SIGNAL(disconnected()), this, SLOT(onSocketDisconnection()));
    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    connect(this, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    //connect(this, SIGNAL(QAbstractSocket::errorOccurred(SocketError)), this, SLOT(onSocketError(SocketError)));
}
void tcpClientTest::socketDisconnectSlot()
{
    if(this->parent() != Q_NULLPTR)
    {
        disconnect(this, SIGNAL(socketConnect(QTcpSocket* )), this->parent(), SLOT(socketConnectSlot(QTcpSocket*)));
        disconnect(this, SIGNAL(socketDisconnect(QTcpSocket* )), this->parent(), SLOT(socketDisconnectSlot(QTcpSocket*)));
    }

    disconnect(this, SIGNAL(disconnected()), this, SLOT(onSocketDisconnection()));
    disconnect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    disconnect(this, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    //disconnect(this, SIGNAL(QAbstractSocket::errorOccurred(SocketError)), this, SLOT(onSocketError(SocketError)));
}

/**
 * @brief           连接到TCPServer
 * @author          wq
 * @date            2020-12-02
 * @param           HostAddress &hostAddr   服务器IP地址
 *                  quint16 port            服务器端口号
 * @return
 * @remarks
 */
void tcpClientTest::connectToTcpServer(QHostAddress &hostAddr, quint16 port)//连接失败在槽函数中处理
{
    this->connectToHost(hostAddr, port);
}


/***************************** client 相关槽函数 ***********************************/
void tcpClientTest::onSocketConnection()
{
    qDebug()<<"客户端连接成功";
}

void tcpClientTest::onSocketDisconnection()
{

}

void tcpClientTest::onSocketStatChanged(QAbstractSocket::SocketState)
{

}

void tcpClientTest::onSocketError(QAbstractSocket::SocketError socketError)
{

}

void tcpClientTest::onSocketReadyRead()
{

}

