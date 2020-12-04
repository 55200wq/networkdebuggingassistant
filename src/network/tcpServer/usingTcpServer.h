#ifndef USINGTCPSERVER_H
#define USINGTCPSERVER_H

#include "tcpServerTest.h"

/**
 * brief:           定义纯虚函数作为与tcpServerTest 的接口，
 *          使用此类必须在派生类的构造函数中申请server空间,在析构函数中释放，
 *          因为server构造必须将上层类作为 parent 进行构造，本类无法继承QObjece ,
 *          所以server 初始化必须放在本类的派生类中
 * author:          wq
 * time:            2020-12-03
 */
class usingTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit usingTcpServer(QObject *parent);
    ~usingTcpServer() ;
public slots:
    virtual void socketConnectSlot(QTcpSocket *clientSocket) = 0 ;
    virtual void socketDisconnectSlot(QTcpSocket *clientSocket) = 0 ;
    //客户端接收数据函数
    virtual void socketRevDataToClientSlot(QTcpSocket *clientSocket, QByteArray *rev_data) = 0 ;

   inline QList<QHostAddress>* localAddressList_All()
   {
       return &localAddressListAll;
   }

   inline QList<QHostAddress>* localAddressList_IPv4()
   {
       return &localAddressListIPv4;
   }
public:
    tcpServerTest *server;
    QList<QTcpSocket*>* clientSocketList;
private:
    QList<QHostAddress> localAddressListAll;
    QList<QHostAddress> localAddressListIPv4;


};

#endif // USINGTCPSERVER_H
