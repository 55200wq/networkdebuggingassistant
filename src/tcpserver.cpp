#include "tcpserver.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QHostInfo>
tcpServer::tcpServer(QObject* parent):
    QObject(parent)
{
    qRegisterMetaType<QTcpSocket*>("QTcpSocket*");
    server = new QTcpServer;
    ClientSocketList = getInstance();

}

tcpServer::~tcpServer()
{
    delete server;
}

Q_GLOBAL_STATIC(QList<QTcpSocket*>, app)//定义全局变量保存连接上来的客户端的socket
QList<QTcpSocket*>* tcpServer::getInstance()
{
    return app;
}

//连接信号与槽
void tcpServer::init_connect()
{
    // 与父类连接通知有新客户端连接上来
    connect(this, SIGNAL(socketConnect(QTcpSocket* )), this->parent(), SLOT(socketConnectSlot(QTcpSocket*)));
    connect(this, SIGNAL(socketDisconnect(QTcpSocket* )), this->parent(), SLOT(socketDisconnectSlot(QTcpSocket*)));
    // 获取新客户端信号
    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    //客户端发送消息函数
    connect(this, SIGNAL(socketRevDataToClientSignal(QTcpSocket* , QByteArray* )), this->parent(), SLOT(socketRevDataToClientSlot(QTcpSocket*, QByteArray* )));
}

//关闭客户端时将所有信号与槽断开
void tcpServer::serverDisconnect()
{
    // 与父类连接通知有新客户端连接上来
    disconnect(this, SIGNAL(socketConnect(QTcpSocket* )), this->parent(), SLOT(socketConnectSlot(QTcpSocket*)));
    disconnect(this, SIGNAL(socketDisconnect(QTcpSocket* )), this->parent(), SLOT(socketDisconnectSlot(QTcpSocket*)));
    // 获取新客户端信号
    disconnect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    //客户端发送消息函数
    disconnect(this, SIGNAL(socketRevDataToClientSignal(QTcpSocket* , QByteArray* )), this->parent(), SLOT(socketRevDataToClientSlot(QTcpSocket*, QByteArray* )));
}

void tcpServer::init_clientSocketConnect(QTcpSocket* clientSocket)
{
    //connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnection()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnection()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

void tcpServer::clientSocketDisconnect(QTcpSocket* clientSocket)
{
    //connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnection()));
    disconnect(clientSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnection()));
    disconnect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    disconnect(clientSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

//创建客户端
bool tcpServer::creatServer(const QHostAddress &addr, quint16 port)
{
    init_connect();
    return server->listen(addr, port);//监听端口号
}

int tcpServer::creatServer(QString IP, quint16 port)
{
    QHostAddress addr(IP);
    if(QAbstractSocket::IPv4Protocol == addr.protocol())
        server->listen(QHostAddress(IP), port);
    else return -1;
    return 0;
}

/******************** 获取设置类的私有变量 ****************************/
QList<QHostAddress> tcpServer::getLocalAddrList()
{
    QList<QHostAddress> addrlist = QNetworkInterface::allAddresses();
    for(auto addr: addrlist){
        if(QAbstractSocket::IPv6Protocol == addr.protocol()){
            addrlist.removeOne(addr);
        }
    }
    return addrlist;
}
QList<QNetworkInterface> tcpServer::getAllLocalNetworkInterface()
{
    return QNetworkInterface::allInterfaces();
}

void tcpServer::setHostAddress(const QHostAddress& addr)
{
    this->addr = addr;
}
void tcpServer::setPort(quint16 port)
{
    this->port = port;
}
quint16 tcpServer::getPort()
{
    return this->port;
}
QHostAddress& tcpServer::getHostAddress()
{
    return this->addr;
}

QTcpServer* tcpServer::getQTcpServer()
{
    return this->server;
}

//连接成功后获取主机信息
socket_info* tcpServer::getHostInfo(QTcpSocket* socket)
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
void tcpServer::deleteHostInfo(socket_info* info)
{
    delete info->revData;
    delete info->sendData;
    delete info;
}

//返回sockInfoMap指针
QMap<QTcpSocket*, socket_info*>* tcpServer::getSockInfoMap()
{
    return &this->sockInfoMap;
}


/***************************** 槽函数 *********************************/
void tcpServer::onNewConnection()
{
    QList<QTcpSocket*>* clientSocketList = getInstance();
    this->socket = server->nextPendingConnection();//获取套接字
    if(! this->socket->isOpen()){
        qDebug()<<"套接字获取失败\n";
        this->socket = Q_NULLPTR;
        return;
    }
    //连接信号与槽
    init_clientSocketConnect(this->socket);
    clientSocketList->append(this->socket);
    //获取对方地址信息
    socket_info* info ;
    info = getHostInfo(this->socket);

    sockInfoMap.insert(this->socket, info);
    //qDebug()<<"name : " <<sockInfoMap[socket]->hostName;//
    //qDebug()<<sockInfoMap[socket]->addr.toString();
    //qDebug()<<sockInfoMap[socket]->port;
    sendSocketConnect(this->socket);//发送新客户端连接信号
}

void tcpServer::onSocketConnection()
{

}

void tcpServer::onSocketDisconnection()
{
    QTcpSocket* disconnectClientSocket = (QTcpSocket*)QObject::sender();
    qDebug()<<"tcpServer::onSocketDisconnection(): ";
    if(disconnectClientSocket->isOpen()){
        qDebug()<<"onSocketDisconnection断开";
        sendSocketDisconnect(disconnectClientSocket);
        deleteHostInfo(sockInfoMap[disconnectClientSocket]);//删除主机信息
        sockInfoMap.remove(disconnectClientSocket);//从QMap中删除
        this->ClientSocketList->removeOne(disconnectClientSocket);//从列表中删除
        disconnectClientSocket->close();//关闭套接字
    }
}

void tcpServer::onSocketStatChanged(QAbstractSocket::SocketState)
{

}
void tcpServer::onSocketReadyRead()//信息处理
{

    QTcpSocket* socket = (QTcpSocket*)QObject::sender();
    qDebug()<<"来自" << sockInfoMap[socket]->addr.toString()<<"  的消息： ";
    //qDebug()<<socker->readAll();
    auto info = this->sockInfoMap[socket];
    *info->revData = socket->readAll();
    qDebug()<<info->revData->data();
    sendSocketRevDataToClientSignal(socket, info->revData);

}

void tcpServer::closeServerSlot()//关闭服务器
{
    qDebug()<<"关闭服务器";
    serverDisconnect();
    for(QTcpSocket* clientSocket : *ClientSocketList)
    {
        // 将信号与槽断开.防止在关闭客户端时发送信号
        clientSocketDisconnect(clientSocket);

        //删除客户端相关信息，关闭客户端套接字
        deleteHostInfo(sockInfoMap[clientSocket]);
        clientSocket->close();//必须关闭客户端套接字，否则将socket释放后客户端再发消息过来会段错误
    }
    sockInfoMap.clear();
    ClientSocketList->clear();
    server->close();
}

qint64 tcpServer::sendData(QTcpSocket* clientSocket, QByteArray& data)
{
    return clientSocket->write(data);
}
