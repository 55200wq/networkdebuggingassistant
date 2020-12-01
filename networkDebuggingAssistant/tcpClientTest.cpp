#include "tcpClientTest.h"

tcpClientTest::tcpClientTest(QObject* parent) :
    QTcpSocket(parent)
{
    SocketConnect();
}

tcpClientTest::~tcpClientTest()
{
    SocketDisconnect();
}

void tcpClientTest::SocketConnect()
{

}
void tcpClientTest::SocketDisconnect()
{

}

void tcpClientTest::connectTcpServer()//连接失败在槽函数中处理
{

}
