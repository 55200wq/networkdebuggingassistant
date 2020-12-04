#ifndef USINGTCPCLIENT_H
#define USINGTCPCLIENT_H

#include "tcpClientTest.h"

/**
 * brief:           定义纯虚函数作为与 usingTcpClient 的接口，
 *          使用此类必须在派生类的构造函数中申请 client 空间,在析构函数中释放，
 *          因为 client 构造必须将上层类作为 parent 进行构造，本类无法继承QObjece ,
 *          所以 client 初始化必须放在本类的派生类中
 * author:          wq
 * time:            2020-12-03
 */
class usingTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit usingTcpClient(QObject *parent);
    ~usingTcpClient() ;
public slots:
    virtual void socketConnectSlot(QTcpSocket *clientSocket) = 0 ;
    virtual void socketDisconnectSlot(QTcpSocket *clientSocket) = 0 ;
    //客户端接收数据函数
    virtual void socketRevDataToClientSlot(QTcpSocket *clientSocket, QByteArray *rev_data) = 0 ;
public:
    tcpClientTest *client;

};

#endif // USINGTCPSERVER_H
