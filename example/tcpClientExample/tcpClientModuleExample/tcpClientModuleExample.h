#ifndef TCPCLIENTMODULEEXAMPLE_H
#define TCPCLIENTMODULEEXAMPLE_H

#include <QMainWindow>
#include "../src/usingTcpClient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class tcpClientModuleExample; }
QT_END_NAMESPACE

class tcpClientModuleExample : public QMainWindow, public usingTcpClient
{
    Q_OBJECT

public:
    tcpClientModuleExample(QWidget *parent = nullptr);
    ~tcpClientModuleExample();

    void socketConnectSlot(QTcpSocket* client);
    void socketDisconnectSlot(QTcpSocket* client);
    void socketRevDataToClientSlot(QTcpSocket* client, QByteArray* rev_data);

signals:
    //定义信号
    void disconnectServerSignal(int connectType);
public:
    //定义send signals 函数区
    inline void sendCloseServer(int connectType)
    {
        emit disconnectServerSignal(connectType);
    };

private:
    Ui::tcpClientModuleExample *ui;

};
#endif // TCPCLIENTMODULEEXAMPLE_H
