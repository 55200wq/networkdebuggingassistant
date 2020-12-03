#ifndef TCPCLIENTMODULEEXAMPLE_H
#define TCPCLIENTMODULEEXAMPLE_H

#include <QMainWindow>
#include "../src/usingTcpClient.h"
#include "../src/tcpSocketHead.h"
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class tcpClientModuleExample; }
QT_END_NAMESPACE

class tcpClientModuleExample : public QMainWindow, public usingTcpClient
{
    Q_OBJECT

public:
    tcpClientModuleExample(QWidget *parent = nullptr);
    ~tcpClientModuleExample();

public slots:
    void socketConnectSlot(QTcpSocket* client);
    void socketDisconnectSlot(QTcpSocket* client);
    void socketRevDataToClientSlot(QTcpSocket* client, QByteArray* rev_data);



private slots:
    void on_pBtn_connect_clicked();

private:
    Ui::tcpClientModuleExample *ui;

};
#endif // TCPCLIENTMODULEEXAMPLE_H
