#ifndef TCPSERVERTEST_H
#define TCPSERVERTEST_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

struct server_info{
    quint16 port;
    QHostAddress addr;
};//用于创建服务器

struct socket_info{
    quint16 port;
    QHostAddress addr;
    QString hostName;
    QByteArray* sendData;//发送数据buff
    QByteArray* revData; //接收数据buff
};

class tcpServerTest : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcpServerTest(QObject *parent = Q_NULLPTR);
    ~tcpServerTest();

    //连接释放信号与槽相关
    void init_connect();
    void serverDisconnect();
    void init_clientSocketConnect(QTcpSocket* clientSocket);
    void clientSocketDisconnect(QTcpSocket* clientSocket);

    //成员函数
    QList<QTcpSocket*> *getClientList();
    socket_info* getHostInfo(QTcpSocket* socket);
    void deleteHostInfo(socket_info* info);
    QMap<QTcpSocket*, socket_info*>* getSockInfoMap();

    bool createServer(const QHostAddress& hostAddr, quint16 port);
    int ServerSendDataToClient(QTcpSocket* clientSocket, QByteArray& data);

    static QList<QHostAddress> getLocalAddrList();
    static QList<QNetworkInterface> getAllLocalNetworkInterface();
/*************************信号与槽*************************/
public slots:

    void onNewConnection();//QTcpServer 的 newConnection() 信号
    //与socket连接
    void onSocketConnection();

    void onSocketDisconnection();

    void onSocketStatChanged(QAbstractSocket::SocketState);

    void onSocketReadyRead();

    void closeServerSlot();//关闭服务器,释放相关资源

signals:
    //发送客户端连接信号
    void socketConnect(QTcpSocket*);
    //发送客户端断开信号
    void socketDisconnect(QTcpSocket*);
    //发送数据信号
    void socketSendDataToClientSignal(QTcpSocket* socket, QByteArray* data);
    //收到数据信号
    void socketRevDataToClientSignal(QTcpSocket* socket, QByteArray* data);

public://发送信号内联函数
    inline void sendSocketConnect(QTcpSocket* socket){emit socketConnect(socket);}
    inline void sendSocketDisconnect(QTcpSocket* socket){emit socketDisconnect(socket);}
    inline void sendsocketSendDataToClientSignal(QTcpSocket* socket, QByteArray* data){emit socketSendDataToClientSignal(socket, data);}
    inline void sendSocketRevDataToClientSignal(QTcpSocket* socket, QByteArray* data){emit socketRevDataToClientSignal(socket, data);}

/*************************变量区***************************/
public:
    server_info* serverInfo = nullptr;//用于创建服务器
    QTcpSocket* currentClient;
    //QList<QTcpSocket*> clientList;//连接上来的客户端列表
private:
    //私有变量
    QList<QTcpSocket*> clientList;//连接上来的客户端列表
    QMap<QTcpSocket*, socket_info*> sockInfoMap;//socket与socket_info关联，socker_info 是对方主机信息
};

#endif // TCPSERVERTEST_H
