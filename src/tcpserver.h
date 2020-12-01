#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QNetworkInterface>
#include <QTcpSocket>

struct socket_info{
    quint16 port;
    QHostAddress addr;
    QString hostName;
    QByteArray* sendData;//发送数据buff
    QByteArray* revData; //接收数据buff
    QList<QTcpSocket*>* clientList;
};

class tcpServer : public QObject
{
    Q_OBJECT
public:
    explicit tcpServer(QObject* parent = Q_NULLPTR);
    ~tcpServer();
    QList<QTcpSocket*>* getInstance();
    void init_connect();//连接信号与槽
    void serverDisconnect();
    void init_clientSocketConnect(QTcpSocket* clientSocket);//初始化客户端socket信号与槽
    void clientSocketDisconnect(QTcpSocket* clientSocket);//断开客户端socket信号与槽

    bool creatServer(const QHostAddress &addr = QHostAddress::AnyIPv4, quint16 port = 8888);//创建服务器
    int creatServer(QString IP, quint16 port);
public:
    void setHostAddress(const QHostAddress& addr = QHostAddress::AnyIPv4);
    void setPort(quint16 port);
    quint16 getPort();
    QHostAddress& getHostAddress();
    QTcpServer* getQTcpServer();
    socket_info* getHostInfo(QTcpSocket* socket);
    void deleteHostInfo(socket_info* info);
    QMap<QTcpSocket*, socket_info*>* getSockInfoMap();

    int sendDataToClient(QTcpSocket* clientSocket, QArrayData* &data);

    qint64 sendData(QTcpSocket* clientSocket, QByteArray& data);//发送函数
public://静态函数
    static QList<QHostAddress> getLocalAddrList();
    static QList<QNetworkInterface> getAllLocalNetworkInterface();

public slots://槽函数
    void onNewConnection();//QTcpServer 的 newConnection() 信号
    //与socket连接
    void onSocketConnection();

    void onSocketDisconnection();

    void onSocketStatChanged(QAbstractSocket::SocketState);

    void onSocketReadyRead();

    void closeServerSlot();//关闭服务器,释放相关资源
public :
    //发送信号
    inline void sendSocketConnect(QTcpSocket* socket){emit socketConnect(socket);}
    inline void sendSocketDisconnect(QTcpSocket* socket){emit socketDisconnect(socket);}
    inline void sendsocketSendDataToClientSignal(QTcpSocket* socket, QByteArray* data){emit socketSendDataToClientSignal(socket, data);}
    inline void sendSocketRevDataToClientSignal(QTcpSocket* socket, QByteArray* data){emit socketRevDataToClientSignal(socket, data);}
signals:
    //发送客户端连接信号
    void socketConnect(QTcpSocket*);
    //发送客户端断开信号
    void socketDisconnect(QTcpSocket*);
    //发送数据信号
    void socketSendDataToClientSignal(QTcpSocket* socket, QByteArray* data);
    //收到数据信号
    void socketRevDataToClientSignal(QTcpSocket* socket, QByteArray* data);
private:
    QTcpServer *server;
    QTcpSocket *socket;
    QHostAddress addr;
    quint16 port;

    QList<QTcpSocket*> * ClientSocketList;//client socket列表
    QMap<QTcpSocket*, socket_info*> sockInfoMap;//socket与socket_info关联，socker_info 是对方主机信息
};

#endif // TCPSERVER_H
