#ifndef TCPCLIENTTEST_H
#define TCPCLIENTTEST_H

#include <QObject>
#include <QTcpSocket>

#include "../../common/network/tcpSocketHead.h"

class tcpClientTest : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpClientTest(QObject* parent = Q_NULLPTR);
    ~tcpClientTest();

    //连接释放信号与槽相关
    void socketConnectToUiSlot();
    void socketDisconnectToUiSlot();
    void socketConnectSlot();
    void socketDisconnectSlot();
    //成员函数
    TCP_ServerInfo::socket_info* getHostInfo(QTcpSocket* socket);
    void deleteHostInfo(TCP_ServerInfo::socket_info* info);
    QAbstractSocket::SocketState connectToTcpServer(QHostAddress &hostAddr, quint16 port);//连接服务器
    int ServerSendDataToClient(QTcpSocket* clientSocket, QByteArray& data);
    /*********************** 信号与槽 ************************/
public slots:
    //client 相关槽函数
    void onSocketConnection();

    void onSocketDisconnection();

    void onSocketStatChanged(QAbstractSocket::SocketState);

    //void onSocketError(QAbstractSocket :: SocketError socketError);

    void onSocketReadyRead();

    void disconnectServerSlot(int connectType);//断开客户端槽函数
signals:
    //发送客户端连接信号，客户端进行连接
    void socketConnect(QTcpSocket*);
    //发送客户端断开信号
    void socketDisconnect(QTcpSocket*);
    //收到数据信号
    void socketRevDataToClientSignal(QTcpSocket* socket, QByteArray* data);

    /**
     * @brief           主动与服务器断开连接，一般在上层类的UI响应中调用
     * @author          wq
     * @time            2020-12-03
     * @param
     * @return          int connectType: 连接类型
     * @remarks
     */
    //定义信号
    void disconnectServerSignal(int connectType);


public:
    //信号函数封装一层
    inline void sendSocketConnect(QTcpSocket* socket)
    {
        emit socketConnect(socket);
    }
    inline void sendSocketDisconnect(QTcpSocket* socket)
    {
        emit socketDisconnect(socket);
    }
    inline void sendSocketRevDataToClientSignal(QTcpSocket* socket,
                                                QByteArray* data)
    {
        emit socketRevDataToClientSignal(socket, data);
    }

     /**
     * @brief           封装的信号函数,看信号简介
     * @author          wq
     * @time            2020-12-03
     * @param
     * @return
     * @remarks
     */
    //定义send signals 函数区
    inline void sendCloseServer(int connectType)
    {
        emit disconnectServerSignal(connectType);
    };
    /************************* 变量区 ********************************/
public:
    TCP_ServerInfo::socket_info* clientInfo;
private:


};

#endif // TCPCLIENTTEST_H
