#include "tcpclient.h"
#include <QHostInfo>

tcpClient::tcpClient(QObject* parent) :
    QObject(parent)
{
    client = new QTcpSocket(this);
    init_connect();
}

tcpClient::~tcpClient()
{
    delete client;
}
void tcpClient::init_connect()
{
    connect(client, SIGNAL(connected()), this, SLOT(onSocketConnection()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onSocketDisconnection()));
    connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    connect(client, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

void tcpClient::clientDeconnect()
{
    disconnect(client, SIGNAL(connected()), this, SLOT(onSocketConnection()));
    disconnect(client, SIGNAL(disconnected()), this, SLOT(onSocketDisconnection()));
    disconnect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStatChanged(QAbstractSocket::SocketState)));
    disconnect(client, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));

}

void tcpClient::connectServer(QHostAddress& addr, quint16 port)
{
    client->connectToHost(addr, port);
}

void tcpClient::setQHostAddress(const QHostAddress& addr)
{
    this->addr = addr;
}
void tcpClient::setPort(quint16 port)
{
    this->port = port;
}

quint16 tcpClient::getPort()
{
    return port;
}

QHostAddress& tcpClient::getHostAddress()
{
    return addr;
}

QTcpSocket* tcpClient::getClient()
{
    return this->client;
}

void tcpClient::onSocketConnection()
{
    qDebug()<<"连接server成功\n";
}

void tcpClient::onSocketDisconnection()
{
    qDebug()<<"断开连接";
    if(client->isOpen()){
        client->close();//关闭客户端套接字
    }
}
void tcpClient::onSocketStatChanged(QAbstractSocket::SocketState)
{
    //qDebug()<<"状态改变";
}
void tcpClient::onSocketReadyRead()//有消息读取
{
    qDebug()<<client->readAll();
}
