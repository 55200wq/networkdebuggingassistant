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
    client = new tcpClientTest(this);
    clientSocketList = this->server->getClientList();
    /*********************** 连接信号与槽 **************************/
    connect(this,
            SIGNAL(closeServerSignal(int )),
            this->server,
            SLOT(closeServerSlot(int )));
    connect(this,
            SIGNAL(closeServerSignal(int )),
            this->client,
            SLOT(closeServerSlot(int )));

    showAllLocalAddressTo_cBx_hostAddr();
}

networkDebuggingAssistant::~networkDebuggingAssistant()
{
    delete ui;
    delete customContextMenu;
    delete server;
    delete client;
}

/**
 * @brief           使用 tcpServerTest::getLocalAddrList() 获取本地 AnyIPv4 类型 保存在成员变量中
 *
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void networkDebuggingAssistant::getLocalAddressList(QList<QHostAddress> &list)
{
    /*if(!this->localAddressList.isEmpty()){
        this->localAddressList.clear();
    }*/
    //this->localAddressList.append(QHostAddress::AnyIPv4);
    list = tcpServerTest::getLocalAddrList();
    for(auto addr : list)
    {
        if(addr.protocol() == QAbstractSocket::IPv6Protocol)
        {
            list.removeOne(addr);
        }
    }
    list.insert(0, QHostAddress::AnyIPv4);
}

/**
 * @brief           显示成员变量中保存的本地IP
 *
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void networkDebuggingAssistant::showAllLocalAddressTo_cBx_hostAddr()
{
    int curr_index = 0;
    QString curr = ui->cBx_hostAddr->currentText();
    ui->cBx_hostAddr->clear();//清除 cBx_hostAddr 所有节点
    //qDebug()<<curr;
    getLocalAddressList(this->localAddressList);//获取list
    if(this->localAddressList.value(0) == QHostAddress::AnyIPv4)
    {
        this->ui->cBx_hostAddr->addItem("AnyIPv4", QVariant::fromValue(6));
    }
    for(int i = 1; i< this->localAddressList.size(); i++)
    {
        //QVariant var = QVariant::fromValue(this->localAddressList.value(i));
        //this->ui->cBx_hostAddr->addItem(this->localAddressList.value(i).toString(), var);
        this->ui->cBx_hostAddr->addItem(this->localAddressList.value(i).toString());
        if(curr == this->localAddressList.value(i).toString()){
            curr_index = i;
        }
    }
    ui->cBx_hostAddr->setCurrentIndex(curr_index);
}



/***************** network 相关 ****************/
/////////////////////////////////////////////////////
/// \brief networkDebuggingAssistant::socketConnectSlot
///         连接成功进行相应的UI交互
/// \param client
///
/// TCPServer TCPClient 相关槽函数
///

/**
 * @brief           客户端连接槽函数，由将本类作为上层类的成员类发送
 *          对于conectTyPe = TCP_SERVER, 进行新连接客户端信息的显示
 *          对于conectTyPe = TCP_CLIENT, 显示相关信息
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void networkDebuggingAssistant::socketConnectSlot(QTcpSocket* client)
{
    qDebug()<<"parent: ";
    qDebug()<<"客户端连接成功";
    switch (this->connectType)
    {
    case TCP_SERVER:
        //添加客户端信息
        ui->cBx_clientList->addItem(client->peerAddress().toString()+":"+QString::number( client->peerPort()), QVariant::fromValue(client));
        break;
    case TCP_CLIENT:
        //添加server IP地址和 port
        ui->lEt_clientAddr->setText(this->client->peerAddress().toString());
        ui->sBx_clientPort->setValue(this->client->localPort());
        break;
    case UDP:
        break;
    }
}

/**
 * @brief           连接关闭，一般是对方主动关闭连接。
 *          此类主动关闭连接的处理代码在on_pBtn_connect_clicked()槽函数中。
 * @author
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void networkDebuggingAssistant::socketDisconnectSlot(QTcpSocket* client)
{
    //qDebug()<<"parent: ";
    //qDebug()<<"客户端断开";
    //在UI上删除相应信息
    int index;//server使用

    switch (this->connectType)
    {
    case TCP_SERVER:
        index = this->clientSocketList->indexOf(client);
        ui->cBx_clientList->removeItem(index);
        break;
    case TCP_CLIENT:
        ui->lEt_clientAddr->clear();
        ui->sBx_clientPort->clear();
        ui->pBtn_connect->setText("连接");
        ui->cBx_hostAddr->setEnabled(true);
        ui->cBx_connectType->setEnabled(true);
        break;
    case UDP:
        break;
    }

}

/**
 * @brief           接收来自客户端的数据 进行处理
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void networkDebuggingAssistant::socketRevDataToClientSlot(QTcpSocket* client,
                                                          QByteArray* data)
{
    qDebug()<<"接收到消息";
    switch (this->connectType)
    {
    case TCP_SERVER:
    {
        qDebug()<<"接收到消息";
        QMap<QTcpSocket*, socket_info*>* map = this->server->getSockInfoMap();
        auto socketInfo = map->value(client);
        ui->pTE_showData->appendPlainText(QString(*socketInfo->revData));
    }
        break;
    case TCP_CLIENT:
    {
        tcpClientTest *socket = (tcpClientTest *)client;
        ui->pTE_showData->appendPlainText(QString(socket->clientInfo->revData->data()));
    }
        break;
    case UDP:
        break;
    }

}


/////////////////////////////////////////////////////////////
/// \brief networkDebuggingAssistant::on_cBx_connectType_activated
/// \param index
///
/// UI 控件槽函数
///
///


/**
 * @brief           右键菜单设计相关
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
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
    if(color.isValid())
    {
        palet.setColor(QPalette::Background, color);
        ui->centralwidget->setPalette(palet);
    }
}

/**
 * @brief           连接失败的弹窗
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
#include <QMessageBox>
void networkDebuggingAssistant::on_action_setFont_triggered()
{
    QMessageBox box;
    box.setWindowTitle("ggg");
    box.setText("暂不支持");
    box.open();
    box.exec();
}

//暂未使用
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

/**
 * @brief           连接类型的切换事件处理
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void networkDebuggingAssistant::on_cBx_connectType_currentIndexChanged(int index)
{
    this->connectType = index;
    switch(this->connectType)
    {
    case TCP_SERVER:
        showAllLocalAddressTo_cBx_hostAddr();
        ui->cBx_hostAddr->setCurrentIndex(0);
        ui->cBx_hostAddr->setEditable(0);//锁定为不可编辑

        //在UI上显示相应客户端信息
        ui->lb_clientAddr->hide();
        ui->lEt_clientAddr->hide();
        ui->lb_clientPort->hide();
        ui->sBx_clientPort->hide();
        ui->lb_clientList->show();
        ui->cBx_clientList->show();
        ui->cBx_clientList->clear();

        break;
    case TCP_CLIENT:
        showAllLocalAddressTo_cBx_hostAddr();
        ui->cBx_hostAddr->removeItem(0);
        ui->cBx_hostAddr->setCurrentIndex(0);
        ui->cBx_hostAddr->setEditable(1);//设置为可编辑
        //
        ui->lb_clientAddr->show();
        ui->lEt_clientAddr->show();
        ui->lb_clientPort->show();
        ui->sBx_clientPort->show();
        ui->lb_clientList->hide();
        ui->cBx_clientList->hide();
        break;
    case UDP:
        break;
    }
}

/**
 * @brief           发送数据槽函数触发按键
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void networkDebuggingAssistant::on_pBt_textSend_clicked()//发送按键槽函数
{

    QTcpSocket* send_socktet;
    socket_info* info;
    QString sendString;
    QByteArray* array;
    QString QMessageBox_text;
    switch(this->connectType)
    {
    case TCP_SERVER:
        if(ui->cBx_clientList->count() == 0)
        {
            QMessageBox_text = "没有连接的客户端";
            goto END;
        }
        send_socktet = ui->cBx_clientList->currentData().value<QTcpSocket*>();
        if(send_socktet->isOpen())
        {
            info = this->server->getSockInfoMap()->value(send_socktet);
            sendString = ui->lEt_textInput->text();
            array = info->sendData;
            //向客户端发送信息
            *array = sendString.toLocal8Bit();
            this->server->ServerSendDataToClient(send_socktet, *info->sendData);
        }

        break;
    case TCP_CLIENT:
        send_socktet = this->client;
        if(send_socktet->isOpen())
        {
            info = this->client->clientInfo;
            sendString = ui->lEt_textInput->text();
            array = info->sendData;
            //向服务器发送信息
            *array = sendString.toLocal8Bit();
            this->client->ServerSendDataToClient(send_socktet, *info->sendData);
        }
        else
        {
            QMessageBox_text = "未连接服务器";
            goto END;
        }

        break;
    case UDP:
        break;
    }
    return;
END:
    QMessageBox box;
    box.setText(QMessageBox_text);
    box.exec();;


}

/**
 * @brief           连接按键事件处理槽函数
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void networkDebuggingAssistant::on_pBtn_connect_clicked()
{
    static bool pBtflag = false;//是否连接
    int type = 255;
    bool success = false;
    QString QMessageBox_text;//用于创建失败是设置弹窗文本
    if(!pBtflag)
    {
        switch (ui->cBx_connectType->currentIndex())
        {
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
    switch(this->connectType)
    {
    case TCP_SERVER://创建服务器
        if(pBtflag)
        {//关闭服务器
            sendCloseServer(TCP_SERVER);

            success = false;
        }
        else
        {//没有连接则连接，创建服务器
            QHostAddress curr_addr = ui->cBx_hostAddr->currentData().value<QHostAddress>();
            QMessageBox box;
            quint16 port = ui->sBx_port->value();
            if(this->server->createServer(curr_addr, port))
            {
                qDebug()<<"server 创建成功";
                success = true;
            }
            else
            {
                QMessageBox_text = "server 创建失败";
            }
        }
        break;
    case TCP_CLIENT://客户端连接服务器
        if(pBtflag)
        {//关闭连接
            sendCloseServer(TCP_CLIENT);
            success = false;
        }
        else
        {//创建连接
            qDebug()<<"连接服务器";
            QHostAddress serverAddr;
            serverAddr.setAddress(ui->cBx_hostAddr->currentText());
            quint16 serverPort = ui->sBx_port->value();
            QAbstractSocket::SocketState socketState =this->client->connectToTcpServer(serverAddr, serverPort) ;
            qDebug()<<"连接状态: "<< socketState;
            if(socketState == QAbstractSocket::ConnectedState )
            {
                success = true;
            }
            else
            {
                qDebug()<<"client 未连接到 TCP server";
                QMessageBox_text = "client 未连接到服务器";
            }

        }
        break;
    case UDP:
        if(pBtflag)
        {//关闭连接

        }
        else
        {//创建连接

            success = true;
        }
        break;
    default:
        break;
    }
    //处理
    if(pBtflag)//处理主动关闭连接
    {
        ui->pBtn_connect->setText("连接");
        ui->cBx_hostAddr->setEnabled(true);
        ui->cBx_connectType->setEnabled(true);
        switch(this->connectType)
        {
        case TCP_SERVER:
            ui->cBx_clientList->clear();
            break;
        case TCP_CLIENT:
            ui->lEt_clientAddr->clear();
            ui->sBx_clientPort->setValue(0);
            break;
        case UDP:
            break;
        }
        //showAllLocalAddressTo_cBx_hostAddr();
        pBtflag = false;
    }
    else
    {
        if(success)
        {
            if(TCP_SERVER == connectType)
            {

            }
            else if(TCP_CLIENT == connectType)
            {

            }
            ui->pBtn_connect->setText("断开连接");
            ui->cBx_hostAddr->setEnabled(false);
            ui->cBx_connectType->setEnabled(false);
            pBtflag = true;
        }
        else
        {
            QMessageBox box;
            box.setText(QMessageBox_text);
            box.exec();
        }
    }
}

/**
 * @brief           清除 pTE_showData 上显示的数据
 * @author          wq
 * @time            2020-12-02
 * @param
 * @return
 * @remarks
 */
void networkDebuggingAssistant::on_pBtn_clear_pTEShowData_clicked()
{
    ui->pTE_showData->clear();
}
