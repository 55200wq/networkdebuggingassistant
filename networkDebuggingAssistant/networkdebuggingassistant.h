#ifndef NETWORKDEBUGGINGASSISTANT_H
#define NETWORKDEBUGGINGASSISTANT_H

#include <QMainWindow>
#include <QMenu>
#include "tcpServerTest.h"

QT_BEGIN_NAMESPACE
namespace Ui { class networkDebuggingAssistant; }
QT_END_NAMESPACE

typedef  enum{
    TCP_SERVER,
    TCP_CLIENT,
    UDP
} ConnectType;

class networkDebuggingAssistant : public QMainWindow
{
    Q_OBJECT

public:
    networkDebuggingAssistant(QWidget *parent = nullptr);
    ~networkDebuggingAssistant();

public slots:
    //自定义槽函数
signals:
    //定义信号
    void closeServerSignal();
public:
    //定义send signals 函数区
    inline void sendCloseServer(){emit closeServerSignal();};

public slots:
    void socketConnectSlot(QTcpSocket* client);
    void socketDisconnectSlot(QTcpSocket* client);
    void socketRevDataToClientSlot(QTcpSocket* client, QByteArray* data);
private slots:
    void on_cBx_connectType_activated(int index);

    void on_centralwidget_customContextMenuRequested(const QPoint &pos);

    void on_action_SetWidthBgColor_triggered();

/**************************变量区************************/
    void on_action_setFont_triggered();

    void on_cBx_connectType_currentIndexChanged(int index);

    void on_pBt_textSend_clicked();

    void on_pBtn_connect_clicked();

public:
    tcpServerTest* server;
private:
    //ui变量
    Ui::networkDebuggingAssistant *ui;
    QMenu* customContextMenu;
    int connectType = 0;
};
#endif // NETWORKDEBUGGINGASSISTANT_H
