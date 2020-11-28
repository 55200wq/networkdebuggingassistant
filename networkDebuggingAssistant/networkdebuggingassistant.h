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
public:
    //定义send signals 函数区
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

public:
    tcpServerTest* server;
private:
    //ui变量
    Ui::networkDebuggingAssistant *ui;
    QMenu* customContextMenu;
};
#endif // NETWORKDEBUGGINGASSISTANT_H
