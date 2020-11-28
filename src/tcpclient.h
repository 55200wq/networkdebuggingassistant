#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class tcpClient : QObject
{
    Q_OBJECT
public:
    explicit tcpClient(QObject* parent = Q_NULLPTR);
    ~tcpClient();

    void setQHostAddress(const QHostAddress& addr);
    void setPort(quint16 port);
    quint16 getPort();
    QHostAddress& getHostAddress();
    QTcpSocket* getClient();
    void init_connect();
    void clientDeconnect();
    void connectServer(QHostAddress& addr, quint16 port);
public slots:
    void onSocketConnection();
    void onSocketDisconnection();
    void onSocketStatChanged(QAbstractSocket::SocketState);
    void onSocketReadyRead();
private:
    QTcpSocket* client;
    QHostAddress addr;
    quint16 port;
};

#endif // TCPCLIENT_H
