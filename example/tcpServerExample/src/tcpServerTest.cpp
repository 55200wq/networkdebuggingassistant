/**
 * brief:           tcpServer
 * author:          wq
 * date:            2020-12-01
 */
#include "tcpServerTest.h"

tcpServerTest::tcpServerTest(QObject *parent) :
    QTcpServer(parent)
{
    qRegisterMetaType<QTcpSocket*>("QTcpSocket*");
    qRegisterMetaType<QTcpSocket*>("QAbstractSocket::SocketState");
    connect(this,
            SIGNAL(closeServerSignal(int )),
            this,
            SLOT(closeServerSlot(int )));
}
tcpServerTest::~tcpServerTest()
{
    if(currentClient->isOpen())
    {
        serverDisconnect();
    }
    disconnect(this,
            SIGNAL(closeServerSignal(int )),
            this,
            SLOT(closeServerSlot(int )));
}

QList<QTcpSocket*>* tcpServerTest::getClientList()
{
    return &clientList;
}

/**
 * @brief           连接与上层类通信的信号槽，一般上层类是UI类
 *          注意 newConnection() 是与本类连接，在由新客户端连接时触发
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpServerTest::init_connect()
{
    // 获取新客户端信号
    connect(this,
            SIGNAL(newConnection()),
            this,
            SLOT(onNewConnection()));

    // 与父类连接通知有新客户端连接上来
    connect(this,
            SIGNAL(socketConnect(QTcpSocket* )),
            this->parent(),
            SLOT(socketConnectSlot(QTcpSocket*)));
    connect(this,
            SIGNAL(socketDisconnect(QTcpSocket* )),
            this->parent(),
            SLOT(socketDisconnectSlot(QTcpSocket*)));
    //客户端接收数据函数
    connect(this,
            SIGNAL(socketRevDataToClientSignal(QTcpSocket* , QByteArray* )),
            this->parent(),
            SLOT(socketRevDataToClientSlot(QTcpSocket*, QByteArray* )));
}
/**
 * @brief           关闭与上层类通信的信号槽，一般在Socket 关闭时调用
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpServerTest::serverDisconnect()
{
    // 与父类连接通知有新客户端连接上来
    disconnect(this,
               SIGNAL(socketConnect(QTcpSocket* )),
               this->parent(),
               SLOT(socketConnectSlot(QTcpSocket*)));
    disconnect(this,
               SIGNAL(socketDisconnect(QTcpSocket* )),
               this->parent(),
               SLOT(socketDisconnectSlot(QTcpSocket*)));
    // 获取新客户端信号
    disconnect(this,
               SIGNAL(newConnection()),
               this,
               SLOT(onNewConnection()));
    //客户端发送消息函数
    disconnect(this,
               SIGNAL(socketRevDataToClientSignal(QTcpSocket* , QByteArray* )),
               this->parent(),
               SLOT(socketRevDataToClientSlot(QTcpSocket*, QByteArray* )));
}

/**
 * @brief           QTcpSocket 提供的信号 与 此类中实现的槽的连接
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpServerTest::init_clientSocketConnect(QTcpSocket* clientSocket)
{
    //connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnection()));
    connect(clientSocket,
            SIGNAL(disconnected()),
            this,
            SLOT(onSocketDisconnection()));
    connect(clientSocket,
            SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,
            SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    connect(clientSocket,
            SIGNAL(readyRead()),
            this,
            SLOT(onSocketReadyRead()));
    //connect(clientSocket, SIGNAL(QAbstractSocket::errorOccurred(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

void tcpServerTest::clientSocketDisconnect(QTcpSocket* clientSocket)
{
    //connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnection()));
    disconnect(clientSocket,
               SIGNAL(disconnected()),
               this,
               SLOT(onSocketDisconnection()));
    disconnect(clientSocket,
               SIGNAL(stateChanged(QAbstractSocket::SocketState)),
               this,
               SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    disconnect(clientSocket,
               SIGNAL(readyRead()),
               this,
               SLOT(onSocketReadyRead()));
    //disconnect(clientSocket, SIGNAL(QAbstractSocket::errorOccurred(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}
/************************** 类普通成员函数 *****************************/
//连接成功后获取主机信息
/**
 * @brief           通过 QTcpSocket 获取以连接的 QTcpSocket 的信息
 * @author          wq
 * @time            2020-12-02
 * @param           QTcpSocket* socket: 已经产生了连接的QTcpSocket
 * @return
 * @remarks
 */
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

/**
 * @brief           与 getHostInfo() 相对
 * @author          wq
 * @time            2020-12-02
 * @param           socket_info* info: 由 getHostInfo() 获取的socket_info
 * @return
 * @remarks
 */
void tcpServerTest::deleteHostInfo(socket_info* info)
{
    delete info->revData;
    delete info->sendData;
    delete info;
}

/**
 * @brief           sockInfoMap 是由 QTcpSocket*作为键值 socket_info*做为值的QMap
 *          该变量是私有变量，在其他类中访问需要用此函数
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
QMap<QTcpSocket*, socket_info*>* tcpServerTest::getSockInfoMap()
{
    return &(this->sockInfoMap);
}

/**
 * @brief           向对方发送数据
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
int tcpServerTest::ServerSendDataToClient(QTcpSocket* clientSocket,
                                          QByteArray& data)
{
    return clientSocket->write(data.data(), data.size());//发送数据
}

// 静态函数
/**
 * @brief           获取本机所有地址信息
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
QList<QHostAddress> tcpServerTest::getLocalAddrList()
{
    QList<QHostAddress> addrlist = QNetworkInterface::allAddresses();
    for(auto addr: addrlist){
        if(QAbstractSocket::IPv6Protocol == addr.protocol()){
            addrlist.removeOne(addr);
        }
    }
    return addrlist;
}

/**
 * @brief           获取本机所有地址信息
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
QList<QNetworkInterface> tcpServerTest::getAllLocalNetworkInterface()
{
    return QNetworkInterface::allInterfaces();
}



/***************************** 槽函数 *********************************/

/**
 * @brief           由新连接时触发，在此槽函数中获取 QTcpSocket 与目标进行通信，
 *          将获取到的 QTcpSocket 添加进QList<QTcpSocket*> clientList 中。
 *          获取 Socket_info ,初始化 sockInfoMap，此变量的每一个项都是一个连接上的客户端的信息，
 *          成功获取后调用 sendSocketConnect()向上层类发送新客户端连接成功信号。
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpServerTest::onNewConnection()
{
    this->currentClient = nextPendingConnection();//获取套接字
    if(! this->currentClient->isOpen())
    {
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
    qDebug()<<"Host name : " <<sockInfoMap[this->currentClient]->hostName;//
    qDebug()<<sockInfoMap[this->currentClient]->addr.toString();
    qDebug()<<sockInfoMap[this->currentClient]->port;
    sendSocketConnect(this->currentClient);//发送新客户端连接信号
}

void tcpServerTest::onSocketConnection()
{

}

/**
 * @brief           客户端断开处理槽函数，此函数负责删除断开客户端信息
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpServerTest::onSocketDisconnection()//客户端断开处理槽函数
{
    QTcpSocket* disconnectClientSocket = (QTcpSocket*)QObject::sender();
    qDebug()<<"tcpServer::onSocketDisconnection(): ";
    if(disconnectClientSocket->isOpen())
    {
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
void tcpServerTest::onSocketError(QAbstractSocket :: SocketError socketError)//出错槽函数
{

}

/**
 * @brief           有数据可读则触发此槽函数进行数据的读取
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpServerTest::onSocketReadyRead()//读取数据进行处理
{
    QTcpSocket* socket = (QTcpSocket*)QObject::sender();
    if(socket->isValid())
    {
        qDebug()<<"来自" << sockInfoMap[socket]->addr.toString()<<"  的消息： ";
        //qDebug()<<socker->readAll();
        auto info = this->sockInfoMap[socket];
        *info->revData = socket->readAll();
        qDebug()<<info->revData->data();
        sendSocketRevDataToClientSignal(socket, info->revData);
    }


}

/**
 * @brief           关闭服务器，由上层类发送信号触发，一般是上层UI类的按键触发
 *          循环断开所有QTcpSocket信号与槽，关闭所有客户端，删除所有客户端信息
 * @author          wq
 * @time            2020-12-02
 * @param           int connectType:    连接类型
 * @return
 * @remarks
 */
void tcpServerTest::closeServerSlot(int connectType)//关闭服务器
{
    if(connectType == TCP_SERVER)
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
}

/**
 * @brief           创建服务器，绑定IP地址，监听端口号
 * @author          wq
 * @time            2020-12-02
 * @param           const QHostAddress& hostAddr:   要绑定的本地IP
 *                  quint16 port:               :   端口号
 * @return          成功:     true
 *                  失败:     false
 * @remarks
 */
bool tcpServerTest::createServer(const QHostAddress& hostAddr,
                                 quint16 port)
{
    bool createServerFlag;
    init_connect();

    createServerFlag = this->listen(hostAddr, port);
    if(createServerFlag)
    {
        qDebug()<<"创建服务器成功";
    }
    else
    {
        qDebug()<<"创建服务器失败";
    }
    return createServerFlag;
}
