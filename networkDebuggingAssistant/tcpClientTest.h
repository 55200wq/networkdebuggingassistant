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
    void SocketConnect();
    void SocketDisconnect();
    void connectTcpServer();


};

#endif // TCPCLIENTTEST_H
