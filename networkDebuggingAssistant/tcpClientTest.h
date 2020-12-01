#ifndef TCPCLIENTTEST_H
#define TCPCLIENTTEST_H

#include <QObject>
#include <QTcpSocket>

class tcpClientTest : QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpClientTest(QObject* parent = Q_NULLPTR);
    ~tcpClientTest();

    //连接释放信号与槽相关
    void socketConnectSlot();
    void socketDisconnectSlot();
    //成员函数
    void connectToTcpServer(QHostAddress &hostAddr, quint16 port);//连接服务器

    /*********************** 信号与槽 ************************/
public slots:
    //client 相关槽函数
    void onSocketConnection();

    void onSocketDisconnection();

    void onSocketStatChanged(QAbstractSocket::SocketState);

    void onSocketError(QAbstractSocket :: SocketError socketError);

    void onSocketReadyRead();

signals:
    //发送客户端连接信号，客户端进行连接
    void socketConnect(QTcpSocket*);
    //发送客户端断开信号
    void socketDisconnect(QTcpSocket*);
    //发送数据信号
    void socketSendDataToClientSignal(QTcpSocket* socket, QByteArray* data);
    //收到数据信号
    void socketRevDataToClientSignal(QTcpSocket* socket, QByteArray* data);

public:
    //信号函数封装一层
    inline void sendSocketConnect(QTcpSocket* socket){emit socketConnect(socket);}
    inline void sendSocketDisconnect(QTcpSocket* socket){emit socketDisconnect(socket);}
    inline void sendsocketSendDataToClientSignal(QTcpSocket* socket, QByteArray* data){emit socketSendDataToClientSignal(socket, data);}
    inline void sendSocketRevDataToClientSignal(QTcpSocket* socket, QByteArray* data){emit socketRevDataToClientSignal(socket, data);}

    /************************* 变量区 ********************************/
public:

private:


};

#endif // TCPCLIENTTEST_H
