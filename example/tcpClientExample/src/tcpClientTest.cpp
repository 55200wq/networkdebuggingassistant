#include "tcpClientTest.h"

tcpClientTest::tcpClientTest(QObject* parent) :
    QTcpSocket(parent)
{
    qRegisterMetaType<QTcpSocket*>("QTcpSocket*");
    qRegisterMetaType<QTcpSocket*>("QAbstractSocket::SocketState");



    this->clientInfo = getHostInfo(this);
    socketConnectToUiSlot();
}

tcpClientTest::~tcpClientTest()
{
    deleteHostInfo(this->clientInfo);
    socketDisconnectToUiSlot();
}

/**
 * @brief           连接与上层类通信的信号槽，一般上层类是UI类 顾取名中有UI
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpClientTest::socketConnectToUiSlot()
{
    //与上层类交互相关信号
    if(this->parent() != Q_NULLPTR)
    {
        connect(this,
                SIGNAL(socketConnect(QTcpSocket* )),
                this->parent(),
                SLOT(socketConnectSlot(QTcpSocket*)));
        connect(this,
                SIGNAL(socketDisconnect(QTcpSocket* )),
                this->parent(),
                SLOT(socketDisconnectSlot(QTcpSocket*)));
        connect(this,
                SIGNAL(socketRevDataToClientSignal(QTcpSocket*, QByteArray*)),
                this->parent(),
                SLOT(socketRevDataToClientSlot(QTcpSocket*, QByteArray*)));
    }
    else
    {
        qDebug()<<"请在初始化时给本类添加上层类";
    }
}

/**
 * @brief           断开与上层类通信的信号槽，一般用在关闭socket时为了不让 QTcpSocket提供的信号槽触发
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpClientTest::socketDisconnectToUiSlot()
{
    if(this->parent() != Q_NULLPTR)
    {
        disconnect(this,
                   SIGNAL(socketConnect(QTcpSocket* )),
                   this->parent(),
                   SLOT(socketConnectSlot(QTcpSocket*)));
        disconnect(this,
                   SIGNAL(socketDisconnect(QTcpSocket* )),
                   this->parent(),
                   SLOT(socketDisconnectSlot(QTcpSocket*)));
        disconnect(this,
                SIGNAL(socketRevDataToClientSignal(QTcpSocket*, QByteArray*)),
                this->parent(),
                SLOT(socketRevDataToClientSlot(QTcpSocket*, QByteArray*)));
    }
}

/**
 * @brief           连接QTcpSocket 类实现的一套信号槽
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpClientTest::socketConnectSlot()
{
    // 在上层调用信号函数
    connect(this,
            SIGNAL(disconnectServerSignal(int )),
            this,
            SLOT(disconnectServerSlot(int )));
    //socket 相关信号
    connect(this,
            SIGNAL(connected()),
            this,
            SLOT(onSocketConnection()));
    connect(this,
            SIGNAL(disconnected()),
            this,
            SLOT(onSocketDisconnection()));
    connect(this,
            SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,
            SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    connect(this,
            SIGNAL(readyRead()),
            this,
            SLOT(onSocketReadyRead()));
    //connect(this, SIGNAL(errorOccurred( QAbstractSocket::SocketError)), this, SLOT(onSocketError( QAbstractSocket::SocketError)));
}//

/**
 * @brief           断开QTcpSocket 类实现的一套信号槽
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpClientTest::socketDisconnectSlot()
{
    // 在上层调用信号函数
    disconnect(this,
            SIGNAL(disconnectServerSignal(int )),
            this,
            SLOT(disconnectServerSlot(int )));
    // socket 相关
    disconnect(this,
               SIGNAL(connected()),
               this,
               SLOT(onSocketConnection()));
    disconnect(this,
               SIGNAL(disconnected()),
               this,
               SLOT(onSocketDisconnection()));
    disconnect(this,
               SIGNAL(stateChanged(QAbstractSocket::SocketState)),
               this,
               SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    disconnect(this,
               SIGNAL(readyRead()),
               this,
               SLOT(onSocketReadyRead()));
    //disconnect(this, SIGNAL( errorOccurred( QAbstractSocket::SocketError)), this, SLOT( onSocketError( QAbstractSocket::SocketError)));
}

using namespace TCP_ServerInfo;
//连接成功后获取主机信息
/**
 * @brief           通过 QTcpSocket 获取以连接的 QTcpSocket 的信息
 * @author          wq
 * @time            2020-12-02
 * @param           QTcpSocket* socket: 已经产生了连接的QTcpSocket
 * @return
 * @remarks
 */
socket_info* tcpClientTest::getHostInfo(QTcpSocket* socket)
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
void tcpClientTest::deleteHostInfo(socket_info* info)
{
    delete info->revData;
    delete info->sendData;
    delete info;
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
QAbstractSocket::SocketState tcpClientTest::connectToTcpServer(QHostAddress &hostAddr,
                                                               quint16 port)//连接失败在槽函数中处理
{
    qDebug()<<"server ip:"<< hostAddr.toString()+" pore: "<< port;
    socketConnectSlot();
    this->connectToHost(hostAddr, port);
    this->waitForConnected();//等待连接服务器
    QAbstractSocket::SocketState state = this->state();
    return state;
}

/**
 * @brief           向对方发送数据
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
int tcpClientTest::ServerSendDataToClient(QTcpSocket* clientSocket,
                                          QByteArray& data)
{
    return clientSocket->write(data.data(), data.size());//发送数据
}
/***************************** client 相关槽函数 ***********************************/
/**
 * @brief           连接成功触发槽
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpClientTest::onSocketConnection()
{
    qDebug()<<"client 连接服务器成功";
    sendSocketConnect(this);
}

/**
 * @brief           套接字断开信号，一般在此处理server主动断开的情况
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpClientTest::onSocketDisconnection()
{
    qDebug()<<"client 断开连接";
    sendSocketDisconnect(this);
    this->close();
}

void tcpClientTest::onSocketStatChanged(QAbstractSocket::SocketState)
{

}

/*
void tcpClientTest::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"连接失败";
}
*/

/**
 * @brief           接收来自对方套接字的数据
 * @author
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void tcpClientTest::onSocketReadyRead()
{
    if(this->isValid())
    {
        qDebug()<<"来自server  的数据： ";
        //qDebug()<<socker->readAll();
        auto info = this->clientInfo;
        *info->revData = this->readAll();
        qDebug()<<info->revData->data();
        sendSocketRevDataToClientSignal(this, info->revData);
    }
}

/**
 * @brief           接收来自上层类的关闭信号，一般是在上层UI按键槽函数中发送
 *          调用close()之前先将套接字的信号与槽断开，只保留本类与上层类的信号槽连接
 * @author
 * @time            2020-12-02
 * @param           int connectType : 连接类型，一般为TCP_SERVER TCP_CLIENT UDP
 * @return
 * @remarks
 */
void tcpClientTest::disconnectServerSlot(int connectType)//主动关闭客户端槽函数
{
    if(connectType == TCP_SocketPublicInfo::TCP_CLIENT)
    {
        qDebug()<<"关闭客户端套接字";
        socketDisconnectSlot();
        this->close();
    }
}
