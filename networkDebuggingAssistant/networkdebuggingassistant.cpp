#include "networkdebuggingassistant.h"
#include "ui_networkdebuggingassistant.h"

#include <QColorDialog>

networkDebuggingAssistant::networkDebuggingAssistant(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::networkDebuggingAssistant)
{
    qRegisterMetaType<QTcpSocket*>("QTcpSocket*");


    ui->setupUi(this);
    customContextMenu = new QMenu;
    customContextMenu->addAction(ui->action_SetWidthBgColor);
    customContextMenu->addAction(ui->action_setFont);
    server = new tcpServerTest(this);

    /*********************** 连接信号与槽 **************************/
    connect(this, SIGNAL(closeServerSignal()), this->server, SLOT(closeServerSlot()));;
}

networkDebuggingAssistant::~networkDebuggingAssistant()
{
    delete ui;
    delete customContextMenu;
}



void networkDebuggingAssistant::on_cBx_connectType_activated(int index)
{
    switch (index) {
    case TCP_SERVER:
        break;
    case TCP_CLIENT:
        break;
    case UDP:
        break;
    default:
        break;
    }
}

#include <QDebug>
void networkDebuggingAssistant::on_centralwidget_customContextMenuRequested(const QPoint &pos)
{

    customContextMenu->show();
    customContextMenu->move(cursor().pos());//移动到width当前位置
}


void networkDebuggingAssistant::on_action_SetWidthBgColor_triggered()
{
    QPalette palet = ui->centralwidget ->palette();
    QColor inicolor; //= palet.color(QPalette::Background);
    QColor color = QColorDialog::getColor(inicolor, this, "选择颜色");
    if(color.isValid()){
        palet.setColor(QPalette::Background, color);
        ui->centralwidget->setPalette(palet);
    }
}

#include <QMessageBox>
void networkDebuggingAssistant::on_action_setFont_triggered()
{
    QMessageBox box;
    box.setWindowTitle("ggg");
    box.setText("暂不支持");
    box.open();
    box.exec();
}

/***************** network 相关 ****************/
void networkDebuggingAssistant::socketConnectSlot(QTcpSocket* client)
{

}
void networkDebuggingAssistant::socketDisconnectSlot(QTcpSocket* client)
{

}
void networkDebuggingAssistant::socketRevDataToClientSlot(QTcpSocket* client, QByteArray* data)
{

}

void networkDebuggingAssistant::on_cBx_connectType_currentIndexChanged(int index)
{

}

void networkDebuggingAssistant::on_pBt_textSend_clicked()
{

}

void networkDebuggingAssistant::on_pBtn_connect_clicked()
{
    static bool pBtflag = false;//是否连接
    int type;
    bool success = false;
    if(!pBtflag){

        switch (ui->cBx_connectType->currentIndex()){
        case TCP_SERVER:
            type = TCP_SERVER;
            qDebug()<<ui->cBx_connectType->currentText();
            break;
        case TCP_CLIENT:
            type = TCP_CLIENT;
            qDebug()<<ui->cBx_connectType->currentText();
            break;
        case UDP:
            type = UDP;
            qDebug()<<ui->cBx_connectType->currentText();
            break;
        }
        this->connectType = type;
    }
    switch(type){
    case TCP_SERVER://创建服务器
        if(pBtflag){//关闭服务器
            sendCloseServer();
            ui->cBx_hostAddr->clear();
        }
        else{//没有连接则连接，创建服务器
            getAddrAndPort();//获取 addr 和 port
            success = this->server->creatServer();
            if(success)
            {
                qDebug()<<"服务器创建成功";
            }
            else{
                qDebug("创建服务器失败");
            }
            clientSocketList = this->server->getInstance();//获取客户端列表
        }
        break;
    case TCP_CLIENT://客户端连接服务器
        if(pBtflag){//关闭连接
            this->client->getClient()->close();
        }
        else{//创建连接
            this->client->setPort(ui->spinBox->value());
            this->client->setQHostAddress(ui->CbB_IP->currentData().value<QHostAddress>());
            this->client->connectServer(this->client->getHostAddress(), this->client->getPort());
            success = true;
        }
        break;
    case UDP:
        if(pBtflag){//关闭连接

        }
        else{//创建连接
            success = true;
        }
        break;
    default:
        break;
    }
    if(pBtflag){
        ui->pBtn_connect->setText("连接");
        ui->cBx_hostAddr->setEnabled(true);
        ui->cBx_connectType->setEnabled(true);
        pBtflag = false;
    }
    else {
        if(success){
            ui->pBtn_connect->setText("断开连接");
            ui->cBx_hostAddr->setEnabled(false);
            ui->cBx_connectType->setEnabled(false);
            ui->cBx_clientList->clear();
            pBtflag = true;
        }
    }
}
