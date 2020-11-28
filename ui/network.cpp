#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVariant>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    server = new tcpServer(this);
    client = new tcpClient(this);

    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(qtimer_out()));
    /****************连接client相关信号与槽********************/
    connect(this, SIGNAL(closeServerSignal()), this->server, SLOT(closeServerSlot()));
    /*********************end*******************************/
    ui->setupUi(this);
    QIcon icon("../../../icon/tip.png");//加载图标
    /* 初始化 ui->CbBtype*/
    ui->CbBtype->addItem(icon, "TCP server");
    ui->CbBtype->addItem(icon, "TCP client");
    ui->CbBtype->addItem(icon, "UDP");

    /**获取 serverAddrList  初始化 ui->CbB_IP**/
    QHostAddress addr(QHostAddress::AnyIPv4);
    serverAddrList = tcpServer::getLocalAddrList();
    clientAddrList = tcpServer::getLocalAddrList();

    serverAddrList .insert(0, addr);
    for(auto addr : serverAddrList){
        //ui->comboBox->addItem(addr.toString());//不加载图标
        //ui->comboBox->addItem(icon ,addr.toString());//加载图标

        if(addr.toString().contains("0.0.0.0")){
            ui->CbB_IP ->addItem(icon , "AnyIPv4", QVariant::fromValue(addr));//添加具有私有数据的列表项
        }
        else
            ui->CbB_IP ->addItem(icon , addr.toString(), QVariant::fromValue(addr));//添加具有私有数据的列表项
    }
    //初始化 clientAddrList
    //clientAddrList = serverAddrList;
    //clientAddrList.removeAt(0);
    /*int itemcount = ui->CbB_IP->count();
    for(int i=0; i < itemcount; i++){
        qDebug()<<ui->CbB_IP->itemText(i);
    }
    int itemcount = clientAddrList.count();
        for(int i=0; i < itemcount; i++){
            qDebug()<<clientAddrList.at(i).toString();
        }
    qDebug()<<" ";
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showIP(QList<QHostAddress>& IPlist)
{
    ui->CbB_IP->clear();
    QIcon icon("../../../icon/tip.png");//加载图标
    for(auto addr : IPlist){
        if(addr.toString().contains("0.0.0.0")){
            ui->CbB_IP ->addItem(icon , "AnyIPv4", QVariant::fromValue(addr));//添加具有私有数据的列表项
        }
        else
            ui->CbB_IP ->addItem(icon , addr.toString(), QVariant::fromValue(addr));//添加具有私有数据的列表项
    }
}

void MainWindow::on_CbBtype_currentIndexChanged(int index)
{
    //qDebug()<<"修改label_IP";
    if(ui->CbBtype->itemText(index).contains("server")){
        ui->label_IP->setText("本地IP地址");
        //显示本地IP
        showIP(serverAddrList);
    }
    else if(ui->CbBtype->itemText(index).contains("client")){
        ui->label_IP->setText("服务器IP地址");
        //显示client 要连接的IP
        showIP(clientAddrList);

    }
    else if(ui->CbBtype->itemText(index).contains("UDP")){
        ui->label_IP->setText("UDP");
    }
}

void MainWindow::on_CbB_IP_currentIndexChanged(int index)
{
    QHostAddress addr;
    addr = ui->CbB_IP->itemData(index).value<QHostAddress>();
    if(addr.isNull()){
        qDebug("用户输入:");
        QIcon icon("../../../icon/tip.png");//加载图标
        qDebug()<<"set ip:"<<ui->CbB_IP->itemText(index);
        addr.setAddress(ui->CbB_IP->itemText(index));//设置IP地址
        if(!(addr.isNull())){
            qDebug()<<"add";
            if(addr.toString() == ui->CbB_IP->itemText(index)){
                ui->CbB_IP ->addItem(icon ,addr.toString(), QVariant::fromValue(addr));//添加具有私有数据的列表项
                clientAddrList .append( addr);
                qDebug()<<"add success";
            }
            else {
                qDebug()<<"输入IP错误\n";
            }
        }
        ui->CbB_IP->removeItem(index);//删除本节点
    }
    qDebug()<<"CbB_IPIndexChanged: "<<addr.toString();
    qDebug()<<" ";
}

void MainWindow::qtimer_out()
{
    qDebug()<<"超时";
    timer->stop();
}

void MainWindow::on_spinBox_valueChanged(int arg1)//数值改变
{
    //timer.
    static int changeNum = 0;
    static int step = 1;
    if( changeNum == 0 && step == 1) timer->start(500);
    if(timer->isActive()){
        changeNum++;
        timer->start(500);
        step += changeNum/5;
        ui->spinBox->setSingleStep(step);
    }
    else {
        qDebug()<< "val = "<< arg1;
        changeNum = 0;
        step = 1;
    }

}

void MainWindow::on_spinBox_valueChanged(const QString &arg1)
{
    qDebug()<< "val = "<<arg1;
}

void MainWindow::on_pushButton_clicked()
{
    static bool pBtflag = false;//是否连接
    static int type;
    bool success = false;
    if(!pBtflag){
        if(ui->CbBtype->currentText().contains("server") )
        {
            type = TCP_SERVER;
            qDebug()<<ui->CbBtype->currentText();
        }
        else if(ui->CbBtype->currentText().contains("client")){
            type = TCP_CLIENT;
            qDebug()<<ui->CbBtype->currentText();
        }
        else if(ui->CbBtype->currentText().contains("UDP")){
            type = UDP;
            qDebug()<<ui->CbBtype->currentText();
        }
        this->connectType = type;
    }
    switch(type){
    case 0://创建服务器
        if(pBtflag){//关闭服务器

            sendCloseServer();
            ui->CbB_Client->clear();
        }
        else{//没有连接则连接，创建服务器
            if(ui->CbB_IP->currentText() == ui->CbB_IP->itemText(0)){
                this->server->setHostAddress(ui->CbB_IP->currentData().value<QHostAddress>());
            }
            else{
                this->server->setHostAddress(ui->CbB_IP->currentData().value<QHostAddress>());
            }
            this->server->setPort(ui->spinBox->value());
            qDebug()<<this->server->getPort();
            qDebug()<<this->server->getHostAddress().toString();
            success = this->server->creatServer(this->server->getHostAddress(), this->server->getPort());
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
    case 1://客户端连接服务器
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
    case 2:
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
        ui->pushButton->setText("连接");
        ui->CbB_IP->setEnabled(true);
        ui->CbBtype->setEnabled(true);
        pBtflag = false;
    }
    else {
        if(success){
            ui->pushButton->setText("断开连接");
            ui->CbB_IP->setEnabled(false);
            ui->CbBtype->setEnabled(false);
            ui->CbB_Client->clear();
            pBtflag = true;
        }
    }
}

/*** 客户端连接上的槽函数 ***/
void MainWindow::socketConnectSlot(QTcpSocket* socket)
{

    auto map_info = server->getSockInfoMap();

    /*qDebug()<<"MainWindow:: 新连接上的客户端:"
            <<"IP地址: "<< map_info->value(socket)->addr.toString()
            <<"port:  "<< map_info->value(socket)->port;*/
    QIcon icon("../../../icon/tip.png");
    ui->CbB_Client->addItem(icon, map_info->value(socket)->addr.toString()+" : "+QString::number(map_info->value(socket)->port),QVariant::fromValue(socket));
}

void MainWindow::socketDisconnectSlot(QTcpSocket* socket)//客户端断开消息处理
{
    qDebug()<<"MainWindow::socketDisconnectSlot():";
    //int num = ui->CbB_Client->count();
    //qDebug()<<"num"<< num;
    //qDebug()<<"this->clientSocketList->indexOf(socket) = "<< this->clientSocketList->indexOf(socket);
    //qDebug()<<"客户端数量"<<this->clientSocketList->size();
    //qDebug()<<"客户端列表节点号:"<<clientSocketList->indexOf(socket);
    ui->CbB_Client->removeItem(this->clientSocketList->indexOf(socket));
}
//数据处理槽函数
void MainWindow::socketRevDataToClientSlot(QTcpSocket* socket, QByteArray* data)
{
    qDebug()<<"MainWindow：";
    //qDebug()<<data->data();
    auto map_info = this->server->getSockInfoMap();

    //this->ui->plainTextEdit->appendPlainText(tr("MainWindow:"));
    this->ui->plainTextEdit->appendPlainText(tr(map_info->value(socket)->addr.toString().toLocal8Bit()));
    this->ui->plainTextEdit->appendPlainText(tr(data->data()));
}


void MainWindow::on_pBtnClearDisplayer_clicked()
{
    this->ui->plainTextEdit->clear();
}

// 发送消息按钮
void MainWindow::on_pushButtonSend_clicked()
{
    switch(this->connectType){
    case TCP_DISCONNECT:
        break;
    case TCP_SERVER:
        break;
    case TCP_CLIENT:
        break;
    case UDP:
        break;
    }
}
