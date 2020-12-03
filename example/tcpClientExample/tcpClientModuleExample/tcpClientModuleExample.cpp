#include "tcpClientModuleExample.h"
#include "ui_tcpClientModuleExample.h"

tcpClientModuleExample::tcpClientModuleExample(QWidget *parent)
    : QMainWindow(parent)
    , usingTcpClient()
    , ui(new Ui::tcpClientModuleExample)
{
    ui->setupUi(this);
    this->client = new tcpClientTest(this);
}

tcpClientModuleExample::~tcpClientModuleExample()
{
    delete ui;
    delete this->client;
}

void tcpClientModuleExample::socketConnectSlot(QTcpSocket* client)
{
    qDebug()<<"TcpServrExample: 连接";
}
void tcpClientModuleExample::socketDisconnectSlot(QTcpSocket* client)
{
    qDebug()<<"TcpServrExample: 断开连接";
    ui->pBtn_connect->click();
}
void tcpClientModuleExample::socketRevDataToClientSlot(QTcpSocket* client, QByteArray* rev_data)
{
    qDebug()<<"TcpServrExample: 来自 server 的数据:";
    qDebug()<<QString(rev_data->data());
    //qDebug()<<QString(*rev_data);
}

void tcpClientModuleExample::on_pBtn_connect_clicked()
{
    static int buttonFlag = false;
    if(!buttonFlag)
    {
        QHostAddress addr(QHostAddress::LocalHost);
        if(this->client->connectToTcpServer(addr, 8080) == QAbstractSocket::ConnectedState)
        {
            buttonFlag = true;
            this->ui->pBtn_connect->setText("断开");
        }
        else
        {
            buttonFlag = false;
        }
    }
    else
    {
        buttonFlag = false;
        this->client->sendCloseServer(TCP_SocketPublicInfo::TCP_CLIENT);
        this->ui->pBtn_connect->setText("连接");
    }
}
