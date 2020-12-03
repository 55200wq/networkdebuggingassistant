#include "TcpServrExample.h"
#include "ui_TcpServrExample.h"


TcpServrExample::TcpServrExample(QWidget *parent)
    : QMainWindow(parent)
    , usingTcpServer()
    , ui(new Ui::TcpServrExample)
{
    ui->setupUi(this);
    this->server = new tcpServerTest(this);

}

TcpServrExample::~TcpServrExample()
{
    delete ui;
    delete this->server;
}

void TcpServrExample::socketConnectSlot(QTcpSocket *clientSocket)
{
    qDebug()<<"TcpServrExample: 连接";
}
void TcpServrExample::socketDisconnectSlot(QTcpSocket *clientSocket)
{
    qDebug()<<"TcpServrExample: 断开连接";
}
//客户端接收数据函数
void TcpServrExample::socketRevDataToClientSlot(QTcpSocket *clientSocket, QByteArray *rev_data)
{
    qDebug()<<"TcpServrExample: 来自client的数据:";
    qDebug()<<QString(rev_data->data());
    //qDebug()<<QString(*rev_data);
}

void TcpServrExample::on_pBtn_connect_clicked()
{
    static int buttonFlag = false;
    if(!buttonFlag)
    {
        if(this->server->createServer(QHostAddress::AnyIPv4, 8080))
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
        this->server->sendCloseServer(TCP_SERVER);
        this->ui->pBtn_connect->setText("连接");
    }

}

