#include "tcpClientModuleExample.h"
#include "ui_tcpClientModuleExample.h"

tcpClientModuleExample::tcpClientModuleExample(QWidget *parent)
    : QMainWindow(parent)
    , usingTcpClient()
    , ui(new Ui::tcpClientModuleExample)
{
    ui->setupUi(this);
    this->client = new tcpClientTest(this);
    connect(this->client,
            SIGNAL(disconnectServerSignal(int )),
            this->client,
            SLOT(disconnectServerSlot(int )));
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
}
void tcpClientModuleExample::socketRevDataToClientSlot(QTcpSocket* client, QByteArray* rev_data)
{
    qDebug()<<"TcpServrExample: 来自client的数据:";
    qDebug()<<QString(rev_data->data());
    //qDebug()<<QString(*rev_data);
}
