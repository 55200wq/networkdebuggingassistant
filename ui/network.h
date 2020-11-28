#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../src/tcpserver.h"
#include "../src/tcpclient.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

typedef enum{
    TCP_SERVER,
    TCP_CLIENT,
    UDP,
    TCP_DISCONNECT = 255
}connectType;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showIP(QList<QHostAddress>& IPlist);

public://发送信号
    inline void sendCloseServer(){emit closeServerSignal();};

signals:
    void closeServerSignal();

public slots:
    void qtimer_out();
    void socketConnectSlot(QTcpSocket* socket);
    void socketDisconnectSlot(QTcpSocket* socket);//断开连接
    void socketRevDataToClientSlot(QTcpSocket* socket, QByteArray* data);//数据接收

private slots:
    void on_CbBtype_currentIndexChanged(int index);

    void on_CbB_IP_currentIndexChanged(int index);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_valueChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pBtnClearDisplayer_clicked();

    void on_pushButtonSend_clicked();

public:
    int connectType;
private://普通私有变量
    tcpServer* server = Q_NULLPTR;
    tcpClient* client = Q_NULLPTR;
    QList<QHostAddress> serverAddrList;
    QList<QHostAddress> clientAddrList;
    QList<QTcpSocket*>* clientSocketList;
//ui变量
private:
    QTimer *timer;
    Ui::MainWindow *ui;

};
Q_DECLARE_METATYPE(QHostAddress)
//Q_DECLARE_METATYPE(QTcpSocket)
#endif // MAINWINDOW_H
