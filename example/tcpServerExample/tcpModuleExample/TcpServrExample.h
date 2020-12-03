#ifndef TCPSERVREXAMPLE_H
#define TCPSERVREXAMPLE_H

#include <QMainWindow>
#include "../src/tcpSocketHead.h"
#include "../src/tcpServerTest.h"
#include "../src/usingTcpServer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TcpServrExample; }
QT_END_NAMESPACE

class TcpServrExample : public QMainWindow, public usingTcpServer
{
    Q_OBJECT

public:
    TcpServrExample(QWidget *parent = nullptr);
    ~TcpServrExample();

public slots:
    void socketConnectSlot(QTcpSocket *clientSocket) ;
    void socketDisconnectSlot(QTcpSocket *clientSocket);
    //客户端接收数据函数
    void socketRevDataToClientSlot(QTcpSocket *clientSocket, QByteArray *rev_data);

private slots:
    void on_pBtn_connect_clicked();


private:
    Ui::TcpServrExample *ui;
    //tcpServerTest* server;
};
#endif // TCPSERVREXAMPLE_H
