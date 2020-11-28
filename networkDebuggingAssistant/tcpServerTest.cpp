#include "tcpServerTest.h"

tcpServerTest::tcpServerTest(QObject *parent) :
    QTcpServer(parent)
{
    init_connect();
}
tcpServerTest::~tcpServerTest()
{
    if(currentClient->isOpen())
        serverDisconnect();
}

QList<QTcpSocket*>& tcpServerTest::getClientList()
{
    return clientList;
}

//连接信号与槽
void tcpServerTest::init_connect()
{
    // 获取新客户端信号
    connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    // 与父类连接通知有新客户端连接上来
    connect(this, SIGNAL(socketConnect(QTcpSocket* )), this->parent(), SLOT(socketConnectSlot(QTcpSocket*)));
    connect(this, SIGNAL(socketDisconnect(QTcpSocket* )), this->parent(), SLOT(socketDisconnectSlot(QTcpSocket*)));
    //客户端发送消息函数
    connect(this, SIGNAL(socketRevDataToClientSignal(QTcpSocket* , QByteArray* )), this->parent(), SLOT(socketRevDataToClientSlot(QTcpSocket*, QByteArray* )));
}
//关闭客户端时将所有信号与槽断开
void tcpServerTest::serverDisconnect()
{
    // 与父类连接通知有新客户端连接上来
    disconnect(this, SIGNAL(socketConnect(QTcpSocket* )), this->parent(), SLOT(socketConnectSlot(QTcpSocket*)));
    disconnect(this, SIGNAL(socketDisconnect(QTcpSocket* )), this->parent(), SLOT(socketDisconnectSlot(QTcpSocket*)));
    // 获取新客户端信号
    disconnect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    //客户端发送消息函数
    disconnect(this, SIGNAL(socketRevDataToClientSignal(QTcpSocket* , QByteArray* )), this->parent(), SLOT(socketRevDataToClientSlot(QTcpSocket*, QByteArray* )));
}

void tcpServerTest::init_clientSocketConnect(QTcpSocket* clientSocket)
{
    //connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnection()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnection()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

void tcpServerTest::clientSocketDisconnect(QTcpSocket* clientSocket)
{
    //connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnection()));
    disconnect(clientSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnection()));
    disconnect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    disconnect(clientSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}
/************************** 类普通成员函数 *****************************/
//连接成功后获取主机信息
socket_info* tcpServerTest::getHostInfo(QTcpSocket* socket)
{
    socket_info* info = new socket_info;
    info->hostName   = socket->peerName();
    info->addr       = socket->peerAddress();
    info->port       = socket->peerPort();
    info->revData    = new QByteArray;
    info->sendData   = new QByteArray;
    return info;
}

//删除主机信息
void tcpServerTest::deleteHostInfo(socket_info* info)
{
    delete info->revData;
    delete info->sendData;
    delete info;
}
/***************************** 槽函数 *********************************/
void tcpServerTest::onNewConnection()
{
    this->currentClient = nextPendingConnection();//获取套接字
    if(! this->currentClient->isOpen()){
        qDebug()<<"套接字获取失败\n";
        this->currentClient = Q_NULLPTR;
        return;
    }
    //连接信号与槽
    init_clientSocketConnect(this->currentClient);
    clientList.append(this->currentClient);
    //获取对方地址信息
    socket_info* info ;
    info = getHostInfo(this->currentClient);

    sockInfoMap.insert(this->currentClient, info);
    //qDebug()<<"name : " <<sockInfoMap[socket]->hostName;//
    //qDebug()<<sockInfoMap[socket]->addr.toString();
    //qDebug()<<sockInfoMap[socket]->port;
    sendSocketConnect(this->currentClient);//发送新客户端连接信号
}

void tcpServerTest::onSocketConnection()
{

}

void tcpServerTest::onSocketDisconnection()
{
    QTcpSocket* disconnectClientSocket = (QTcpSocket*)QObject::sender();
    qDebug()<<"tcpServer::onSocketDisconnection(): ";
    if(disconnectClientSocket->isOpen()){
        qDebug()<<"onSocketDisconnection断开";
        sendSocketDisconnect(disconnectClientSocket);
        deleteHostInfo(sockInfoMap[disconnectClientSocket]);//删除主机信息
        sockInfoMap.remove(disconnectClientSocket);//从QMap中删除
        this->clientList.removeOne(disconnectClientSocket);//从列表中删除
        disconnectClientSocket->close();//关闭套接字
    }
}

void tcpServerTest::onSocketStatChanged(QAbstractSocket::SocketState)
{

}
void tcpServerTest::onSocketReadyRead()//信息处理
{

    QTcpSocket* socket = (QTcpSocket*)QObject::sender();
    qDebug()<<"来自" << sockInfoMap[socket]->addr.toString()<<"  的消息： ";
    //qDebug()<<socker->readAll();
    auto info = this->sockInfoMap[socket];
    *info->revData = socket->readAll();
    qDebug()<<info->revData->data();
    sendSocketRevDataToClientSignal(socket, info->revData);

}

void tcpServerTest::closeServerSlot()//关闭服务器
{
    qDebug()<<"关闭服务器";
    serverDisconnect();
    for(QTcpSocket* clientSocket : this->clientList)
    {
        // 将信号与槽断开.防止在关闭客户端时发送信号
        clientSocketDisconnect(clientSocket);

        //删除客户端相关信息，关闭客户端套接字
        deleteHostInfo(sockInfoMap[clientSocket]);
        clientSocket->close();//必须关闭客户端套接字，否则将socket释放后客户端再发消息过来会段错误
    }
    sockInfoMap.clear();
    this->clientList.clear();
    this->close();
}
