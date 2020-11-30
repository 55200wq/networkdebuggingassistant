#include "networkdebuggingassistant.h"
#include "ui_networkdebuggingassistant.h"

#include <QColorDialog>

networkDebuggingAssistant::networkDebuggingAssistant(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::networkDebuggingAssistant)
{
    qRegisterMetaType<QTcpSocket*>("QTcpSocket*");

    ui->setupUi(this);
    ui->lb_clientAddr->hide();
    ui->lEt_clientAddr->hide();
    ui->lb_clientPort->hide();
    ui->sBx_clientPort->hide();
    ui->lb_clientList->show();
    ui->cBx_clientList->show();

    customContextMenu = new QMenu;
    customContextMenu->addAction(ui->action_SetWidthBgColor);
    customContextMenu->addAction(ui->action_setFont);
    server = new tcpServerTest(this);
    clientSocketList = this->server->getClientList();
    /*********************** 连接信号与槽 **************************/
    connect(this, SIGNAL(closeServerSignal()), this->server, SLOT(closeServerSlot()));;


    showAllLocalAddressTo_cBx_hostAddr();
}

networkDebuggingAssistant::~networkDebuggingAssistant()
{
    delete ui;
    delete customContextMenu;
}

void networkDebuggingAssistant::getLocalAddressList(QList<QHostAddress> &list)
{
    /*if(!this->localAddressList.isEmpty()){
        this->localAddressList.clear();
    }*/
    //this->localAddressList.append(QHostAddress::AnyIPv4);
    list = tcpServerTest::getLocalAddrList();
    for(auto addr : list){
        if(addr.protocol() == QAbstractSocket::IPv6Protocol){
            list.removeOne(addr);
        }
    }
    list.insert(0, QHostAddress::AnyIPv4);
}

void networkDebuggingAssistant::showAllLocalAddressTo_cBx_hostAddr()
{
    getLocalAddressList(this->localAddressList);//获取list
    if(this->localAddressList.value(0) == QHostAddress::AnyIPv4){
        this->ui->cBx_hostAddr->addItem("AnyIPv4", QVariant::fromValue(6));
    }
    for(int i = 1; i< this->localAddressList.size(); i++){
        QVariant var = QVariant::fromValue(this->localAddressList.value(i));
        this->ui->cBx_hostAddr->addItem(this->localAddressList.value(i).toString(), var);
    }
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
    //qDebug()<<"parent: ";
    //qDebug()<<"客户端连接成功";

    //在UI上显示相应客户端信息
    ui->cBx_clientList->addItem(client->peerAddress().toString(), QVariant::fromValue(client));

}
void networkDebuggingAssistant::socketDisconnectSlot(QTcpSocket* client)
{
    //qDebug()<<"parent: ";
    //qDebug()<<"客户端断开";
    //在UI上删除相应信息
    int index = this->clientSocketList->indexOf(client);
    ui->cBx_clientList->removeItem(index);
}
void networkDebuggingAssistant::socketRevDataToClientSlot(QTcpSocket* client, QByteArray* data)
{

}

void networkDebuggingAssistant::on_cBx_connectType_currentIndexChanged(int index)
{
    switch(index){
    case TCP_SERVER:

        break;
    case TCP_CLIENT:
        break;
    case UDP:
        break;
    }
}

void networkDebuggingAssistant::on_pBt_textSend_clicked()
{

}

void networkDebuggingAssistant::on_pBtn_connect_clicked()
{
    static bool pBtflag = false;//是否连接
    int type = 255;
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
    switch(this->connectType){
    case TCP_SERVER://创建服务器
        if(pBtflag){//关闭服务器
            sendCloseServer();
            ui->cBx_hostAddr->clear();
            success = false;
        }
        else{//没有连接则连接，创建服务器
            QHostAddress curr_addr = ui->cBx_hostAddr->currentData().value<QHostAddress>();
            QMessageBox box;
            quint16 port = ui->sBx_port->value();
            if(this->server->createServer(curr_addr, port)){
                qDebug()<<"server 创建成功";
                ui->pBtn_connect->setText("断开");
                success = true;
            }
        }
        break;
    case TCP_CLIENT://客户端连接服务器
        if(pBtflag){//关闭连接

        }
        else{//创建连接

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
    //处理
    if(pBtflag){
        ui->pBtn_connect->setText("连接");
        ui->cBx_hostAddr->setEnabled(true);
        ui->cBx_connectType->setEnabled(true);
        showAllLocalAddressTo_cBx_hostAddr();
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
        else{
            QMessageBox box;
            box.setText("server 创建失败");
            box.exec();
        }
    }
}
